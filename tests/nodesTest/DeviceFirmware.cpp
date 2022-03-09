#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <functional>

#include <nds3/nds.h>

#include "DeviceFirmware.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */


static std::map<std::string, DeviceFirmware*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DeviceFirmware::DeviceFirmware(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t & parameters):
                        m_Name(DeviceName),
                        m_bStop_Firmware(true)
    {
    //Verify that there is no devices of this type with the same name
    {
        std::lock_guard<std::mutex> lock(m_lockDevicesMap);
        if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end())
        {
            throw std::logic_error("Device with the same name already allocated. This should not happen");
        }
        m_DevicesMap[DeviceName] = this;
    }

    /**
     * Here we declare the root node.
     * It is a good practice to name it with the Device name.
     *
     * Also, for simplicity we declare it as a "Port": this means that
     * the root node will be responsible for the communication with
     * the underlying control system.
     *
     * It is possible to have the root node as a simple Node and promote one or
     * more of its children to "Port": each port will interface with a different
     * control system thread.
     */
    nds::Port rootNode(DeviceName);

    //Add a PV to set the current time
    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
    m_setCurrentTime.setDescription("Set timestamp (in secodns)");
    // For testing purposes the current time is set to a constant bigger
    // than 1 of January of 1990 which is the EPICS epoch.
    timespec timestamp = {0, 0};
    m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);


    nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");
    // Add Firmware node
    if (findParam != parameters.end() && findParam->second=="YES") {
        nds::FirmwareArgs_t handlers = nds::FirmwareArgs_t(
                    std::bind(&DeviceFirmware::switchOn_Firmware, this),
                    std::bind(&DeviceFirmware::switchOff_Firmware, this),
                    std::bind(&DeviceFirmware::start_Firmware, this),
                    std::bind(&DeviceFirmware::stop_Firmware, this),
                    std::bind(&DeviceFirmware::recover_Firmware, this),
                    std::bind(&DeviceFirmware::allow_Firmware_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                    std::bind(&DeviceFirmware::PV_Firmware_Path_Writer, this, std::placeholders::_1, std::placeholders::_2));
        handlers.PV_FirmwarePath_Initializer = std::bind(&DeviceFirmware::PV_Firmware_Path_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        m_Firmware = rootNode.addChild(nds::Firmware("Firm",
                                                     256,
                                                     handlers));
    } else {
        m_Firmware = rootNode.addChild(nds::Firmware("Firm",
                                 256, // Maximum string length.
                    std::bind(&DeviceFirmware::switchOn_Firmware, this),
                    std::bind(&DeviceFirmware::switchOff_Firmware, this),
                    std::bind(&DeviceFirmware::start_Firmware, this),
                    std::bind(&DeviceFirmware::stop_Firmware, this),
                    std::bind(&DeviceFirmware::recover_Firmware, this),
                    std::bind(&DeviceFirmware::allow_Firmware_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                    std::bind(&DeviceFirmware::PV_Firmware_Path_Writer, this, std::placeholders::_1, std::placeholders::_2)));
    }
    m_Firmware.setTimestampDelegate(std::bind(&DeviceFirmware::getCurrentTime,this));

    // We have declared all the nodes and PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
    rootNode.setTimestampDelegate(std::bind(&DeviceFirmware::getCurrentTime,this));

    //Stream information for debugging purposes
    rootNode.setLogLevel(nds::logLevel_t::debug);
    rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger:The DeviceFirmware is created" << std::endl;
    ndsDebugStream(rootNode) << "This is the ndsDebugStream: The DeviceFirmware named " << rootNode.getFullName() << " is created" << std::endl;

}



DeviceFirmware::~DeviceFirmware()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_Name);

}

DeviceFirmware* DeviceFirmware::getInstance(const std::string& DeviceName)
{

    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceFirmware*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }

    return findDevice->second;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// TEST FIRMWARE STATE MACHINE
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* Methods to control Firmware state machine
*/

// Called when the Firmware node has to be switched on.
void DeviceFirmware::switchOn_Firmware(){
    // Call API HW to retrieve FirmwareVersion
    m_Firmware.setFirmwareVersion(getCurrentTime(),"Firmware test version");
    // Call API HW to retrieve FirmwareStatus
    m_Firmware.setFirmwareStatus(getCurrentTime(),0);
    // Call API HW to retrieve HardwareRevision
    m_Firmware.setHardwareRevision(getCurrentTime(),"Firmware test hardware revision");
    // Call API HW to retrieve SerialNumber
    m_Firmware.setSerialNumber(getCurrentTime(),"Firmware test serial number");
    // Call API HW to retrieve DeviceModel
    m_Firmware.setDeviceModel(getCurrentTime(),"Firmware test device model");
    // Call API HW to retrieve DeviceType
    m_Firmware.setDeviceType(getCurrentTime(),"Firmware test device type");
        // Call API HW to retrieve DriverVersion
    m_Firmware.setDriverVersion(getCurrentTime(),"<major_id>.<minor_id>.<maintenance_id>");
        // Call API HW to retrieve ChassisNumber
    m_Firmware.setChassisNumber(getCurrentTime(),42);
        // Call API HW to retrieve SlotNumber
    m_Firmware.setSlotNumber(getCurrentTime(),42);
    // Call API HW to retrieve FirmwarePath
    m_Firmware.setFirmwarePath(getCurrentTime(),"Firmware path to be uploaded");
}

// Called when the Firmware node has to be switched off.
void DeviceFirmware::switchOff_Firmware(){

}

// Called when the Firmware node has to start working. We start the Firmware thread.
void DeviceFirmware::start_Firmware(){

    m_bStop_Firmware = false; //< We will set to true to stop the Firmware thread
    /**
     *   Start the Firmware thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_Firmware_Thread =
      m_Firmware.runInThread("Firmware",
                    std::bind(&DeviceFirmware::Firmware_thread_body, this));
}

// Stop the Firmware node thread
void DeviceFirmware::stop_Firmware(){
    m_bStop_Firmware = true;
    m_Firmware_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void DeviceFirmware::recover_Firmware(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool DeviceFirmware::allow_Firmware_Change(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

/*
* Firmware support setters
*/
void DeviceFirmware::PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value){
    std::string firmwarePath;
    //firmwarePath has the firmware path to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real firmware path programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  firmwarePath are equal.
    firmwarePath=value;
    m_Firmware.setFirmwarePath(timestamp,firmwarePath);
}

/*
 * Firmware support initializers
 */
void DeviceFirmware::PV_Firmware_Path_Initializer(timespec* time, std::string* path){
    *time = {0, 0};
    *path = "not available";
}

void DeviceFirmware::Firmware_thread_body(){

    // Get FirmwareVersion
    std::string FirmwareVersion = m_Firmware.getFirmwareVersion();
    // Get FirmwareStatus
    std::int32_t FirmwareStatus = m_Firmware.getFirmwareStatus();
    // Get HardwareRevision
    std::string HardwareRevision = m_Firmware.getHardwareRevision();
    // Get SerialNumber
    std::string SerialNumber = m_Firmware.getSerialNumber();
    // Get DeviceModel
    std::string DeviceModel = m_Firmware.getDeviceModel();
    // Get DeviceType
    std::string DeviceType = m_Firmware.getDeviceType();
    // Get DriverVersion
    std::string DriverVersion = m_Firmware.getDriverVersion();
    // Get ChassisNumber
        std::int32_t ChassisNumber = m_Firmware.getChassisNumber();
    // Get SlotNumber
        std::int32_t SlotNumber = m_Firmware.getSlotNumber();
    // Get FirmwarePath
    std::string FirmwarePath = m_Firmware.getFirmwarePath();
    std::string FirmwarePathOld=m_Firmware.getFirmwarePath();


    std::cout << "Firmware support information:" << std::endl;
    std::cout << "\tFirmwareVersion = " << FirmwareVersion<<std::endl;
    std::cout << "\tFirmwareStatus = " << FirmwareStatus << std::endl;
    std::cout << "\tHardwareRevision = " << HardwareRevision << std::endl;
    std::cout << "\tSerialNumber = " << SerialNumber << std::endl;
    std::cout << "\tDeviceModel = " << DeviceModel << std::endl;
    std::cout << "\tDeviceType = " << DeviceType << std::endl;
    std::cout << "\tDriverVersion = " << DriverVersion << std::endl;
    std::cout << "\tChassisNumber = " << ChassisNumber << std::endl;
    std::cout << "\tSlotNumber = " << SlotNumber << std::endl;
    std::cout << "\tFirmwarePath = " << FirmwarePath << std::endl;

    // Run until the state machine stops us
    while(!m_bStop_Firmware){


        // Get FirmwarePath
        std::string FirmwarePath = m_Firmware.getFirmwarePath();
        if(FirmwarePath.compare(FirmwarePathOld)!=0){
            // Push the Firmware data to the control system
            m_Firmware.push(m_Firmware.getTimestamp(), FirmwarePath);
            FirmwarePathOld=FirmwarePath;
        }
        // Rest for a while
        ::usleep(1000000);
    }
}

timespec DeviceFirmware::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}


//NDS_DEFINE_DRIVER(DeviceFirmware, DeviceFirmware)

/*
 * Allocation function
 *********************/
void* DeviceFirmware::allocateDevice(nds::Factory& factory,
                     const std::string& DeviceName,
                     const nds::namedParameters_t& parameters){
    return new DeviceFirmware(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceFirmware::deallocateDevice(void* DeviceName)
{
    delete (DeviceFirmware*)DeviceName;
}

