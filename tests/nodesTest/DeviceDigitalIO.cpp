#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <functional>

#include <nds3/nds.h>

#include "DeviceDigitalIO.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */


static std::map<std::string, DeviceDigitalIO*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceDigitalIO::DeviceDigitalIO(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t & parameters):
    m_name(deviceName), timestamp_device{0,0},readtimeStamp{0,0}
{
    //TODO:Study this.
    {
        std::lock_guard<std::mutex> lock(m_lockDevicesMap);
        if(m_devicesMap.find(deviceName) != m_devicesMap.end())
        {
            throw std::logic_error("Device with the same name already allocated. This should not happen");
        }
        m_devicesMap[deviceName] = this;
    }

    /**
     * Here we declare the root node.
     * It is a good practice to name it with the device name.
     *
     * Also, for simplicity we declare it as a "Port": this means that
     * the root node will be responsible for the communication with
     * the underlying control system.
     *
     * It is possible to have the root node as a simple Node and promote one or
     * more of its children to "Port": each port will interface with a different
     * control system thread.
     */
    nds::Port rootNode(deviceName);

    nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");

    /**
     * Add a Digital I/O node for boolean PV:
     */
    if (findParam != parameters.end() && findParam->second=="YES") {
         //Set compulsory methods
        nds::DigitalIOArgs_t<std::vector<bool>> handlerDIO = nds::DigitalIOArgs_t<std::vector<bool>>(
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerDIO.PV_dataOutMask_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelHigh_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelLow_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_ChannelDir_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        //Set init values
        handlerDIO.m_Decimation_Init = 1;
        handlerDIO.m_DataOut_Init = std::vector<bool> (128, true); //128 booleans with value true

        m_DigitalIO_Bool = rootNode.addChild(nds::DigitalIO<std::vector<bool> >(
                "DigitalIOBoolNode",
                128,
                handlerDIO));
    } else {
        m_DigitalIO_Bool = rootNode.addChild(nds::DigitalIO<std::vector<bool> >(
                "DigitalIOBoolNode",
                128,
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }
    m_DigitalIO_Bool.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
    m_DigitalIO_Bool.getStartTimestamp();

    /**
     * Add a Digital I/O node for uint8_t PV:
     */
    if (findParam != parameters.end() && findParam->second=="YES") {
         //Set compulsory methods
        nds::DigitalIOArgs_t<std::vector<std::int8_t>> handlerDIO = nds::DigitalIOArgs_t<std::vector<std::int8_t>>(
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I8, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I8,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I8,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I8,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I8,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerDIO.PV_dataOutMask_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I8, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelHigh_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelLow_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_ChannelDir_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I8, this, std::placeholders::_1, std::placeholders::_2);
        //Set init values
        handlerDIO.m_Decimation_Init = 1;
        handlerDIO.m_DataOut_Init = std::vector<std::int8_t> (128, 100); //128 integers(8) with value 100

        m_DigitalIO_I8 = rootNode.addChild(nds::DigitalIO<std::vector<std::int8_t>>(
                "DigitalIOI8Node",
                128,
                handlerDIO));

    } else {
        m_DigitalIO_I8 = rootNode.addChild(nds::DigitalIO<std::vector<std::int8_t> >(
                "DigitalIOI8Node",
                128,
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I8, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I8, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I8,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I8,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I8,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I8,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }
    m_DigitalIO_I8.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
    m_DigitalIO_I8.getStartTimestamp();

    /**
     * Add a Digital I/O node for uint16_t PV:
     */
    if (findParam != parameters.end() && findParam->second=="YES") {
         //Set compulsory methods
        nds::DigitalIOArgs_t<std::vector<std::int16_t>> handlerDIO = nds::DigitalIOArgs_t<std::vector<std::int16_t>>(
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I16, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I16,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I16,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I16,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I16,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerDIO.PV_dataOutMask_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I16, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelHigh_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelLow_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_ChannelDir_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I16, this, std::placeholders::_1, std::placeholders::_2);
        //Set init values
        handlerDIO.m_Decimation_Init = 1;
        handlerDIO.m_DataOut_Init = std::vector<std::int16_t> (128, 200); //128 integers(16) with value 200

        m_DigitalIO_I16 = rootNode.addChild(nds::DigitalIO<std::vector<std::int16_t>>(
                "DigitalIOI16Node",
                128,
                handlerDIO));

    } else {
        m_DigitalIO_I16 = rootNode.addChild(nds::DigitalIO<std::vector<std::int16_t> >(
                "DigitalIOI16Node",
                128,
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I16, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I16, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I16,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I16,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I16,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I16,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }
    m_DigitalIO_I16.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
    m_DigitalIO_I16.getStartTimestamp();

    /**
     * Add a Digital I/O node for uint32_t PV:
     */
    if (findParam != parameters.end() && findParam->second=="YES") {
         //Set compulsory methods
        nds::DigitalIOArgs_t<std::vector<std::int32_t>> handlerDIO = nds::DigitalIOArgs_t<std::vector<std::int32_t>>(
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I32, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I32,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I32,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I32,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I32,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerDIO.PV_dataOutMask_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I32, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelHigh_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelLow_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_ChannelDir_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I32, this, std::placeholders::_1, std::placeholders::_2);
        //Set init values
        handlerDIO.m_Decimation_Init = 1;
        handlerDIO.m_DataOut_Init = std::vector<std::int32_t> (128, 300); //128 integers(32) with value 300

        m_DigitalIO_I32 = rootNode.addChild(nds::DigitalIO<std::vector<std::int32_t>>(
                "DigitalIOI32Node",
                128,
                handlerDIO));

    } else {
        m_DigitalIO_I32 = rootNode.addChild(nds::DigitalIO<std::vector<std::int32_t> >(
                "DigitalIOI32Node",
                128,
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I32, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I32, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I32,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I32,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I32,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I32,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }
    m_DigitalIO_I32.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
    m_DigitalIO_I32.getStartTimestamp();

    /**
     * Add a Digital I/O node for int64_t PV:
     */
    if (findParam != parameters.end() && findParam->second=="YES") {
         //Set compulsory methods
        nds::DigitalIOArgs_t<std::vector<std::int64_t>> handlerDIO = nds::DigitalIOArgs_t<std::vector<std::int64_t>>(
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I64, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I64,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I64,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I64,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I64,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerDIO.PV_dataOutMask_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I64, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelHigh_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_voltLevelLow_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Initializer, this, std::placeholders::_1, std::placeholders::_2);
        handlerDIO.PV_ChannelDir_Initializer = std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I64, this, std::placeholders::_1, std::placeholders::_2);
        //Set init values
        handlerDIO.m_Decimation_Init = 1;
        handlerDIO.m_DataOut_Init = std::vector<std::int64_t> (128, 300); //128 integers(64) with value 300

        m_DigitalIO_I64 = rootNode.addChild(nds::DigitalIO<std::vector<std::int64_t>>(
                "DigitalIOI64Node",
                128,
                handlerDIO));

    } else {
        m_DigitalIO_I64 = rootNode.addChild(nds::DigitalIO<std::vector<std::int64_t> >(
                "DigitalIOI64Node",
                128,
                std::bind(&DeviceDigitalIO::switchOn_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::switchOff_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::start_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::stop_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::recover_DigitalIO_I64, this),
                std::bind(&DeviceDigitalIO::allow_DigitalIO_Change_I64, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I64,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I64,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I64,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I64,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }
    m_DigitalIO_I64.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
    m_DigitalIO_I64.getStartTimestamp();

    timespec timestamp={0,0};
    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
    m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

    //  We have declared all the nodes with several types of PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
    rootNode.setTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));

}

DeviceDigitalIO::~DeviceDigitalIO()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

DeviceDigitalIO* DeviceDigitalIO::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceDigitalIO*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

timespec DeviceDigitalIO::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
// Bool Node STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Methods to control DigitalIO state machine
*/
void DeviceDigitalIO::switchOn_DigitalIO(){

}
void DeviceDigitalIO::switchOff_DigitalIO(){

}
void DeviceDigitalIO::start_DigitalIO(){
    m_bStop_DigitalIO_Bool = false; //< We will set to true to stop the acquisition thread

    /**
     *   Start the acquisition thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_DigitalIO_Thread_Bool =
      m_DigitalIO_Bool.runInThread("DIOB",
                       std::bind(&DeviceDigitalIO::DigitalIO_thread_body_Bool, this));


}
void DeviceDigitalIO::stop_DigitalIO(){
    m_bStop_DigitalIO_Bool = true;
    m_DigitalIO_Thread_Bool.join();

}
void DeviceDigitalIO::recover_DigitalIO(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceDigitalIO::allow_DigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

/**
* DigitalIO setters
*/
void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the dataOutMask to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DigitalIO_Bool.setDataOutMask(timestamp,value);

}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelHigh to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_Bool.setVoltLevelHigh(timestamp,HW_value);

}
void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelLow to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelLow programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_Bool.setVoltLevelLow(timestamp,HW_value);

}
void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the ChannelDir to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real ChannelDir programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_Bool.setChannelDir(timestamp,HW_value);

}

/**
* Body of function DigitalIO thread for vector Bool PV type
*/
void DeviceDigitalIO::DigitalIO_thread_body_Bool(){

    // Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
    std::vector<bool> outputData(m_DigitalIO_Bool.getMaxElements(),0);

    //Counter for number of pushed data blocks
    std::int32_t NumberOfPushedDataBlocks(0);

    bool value = false;

    // Get DataOutMask
    std::vector<bool> DataOutMask = m_DigitalIO_Bool.getDataOutMask();
    // Get VoltLevelHigh
    double VoltLevelHigh = m_DigitalIO_Bool.getVoltLevelHigh();
    // Get VoltLevelLow
    double VoltLevelLow = m_DigitalIO_Bool.getVoltLevelLow();
    // Get ChannelDir
    std::vector<bool> ChannelDir = m_DigitalIO_Bool.getChannelDir();

    std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
    std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
    //std::cout<<"\DataOutMask = "<<DataOutMask<<std::endl;
    //std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


    // Run until the state machine stops us
    while(!m_bStop_DigitalIO_Bool){

        size_t scanVector(0);

        for(scanVector=0; scanVector != outputData.size(); ++scanVector){
            outputData[scanVector] = value;
            value = (scanVector%2 ? true:false);
        }
        //++value;

        // Push the vector to the control system
        m_DigitalIO_Bool.push(m_DigitalIO_Bool.getTimestamp(), outputData);
        ++NumberOfPushedDataBlocks;
        //TODO: Send values to data acquisition node.

        // Rest for a while
        ::usleep(100000);
    }
    m_DigitalIO_Bool.setNumberOfPushedDataBlocks(m_DigitalIO_Bool.getTimestamp(),NumberOfPushedDataBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// int8_t Node STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Methods to control DigitalIO state machine
*/
void DeviceDigitalIO::switchOn_DigitalIO_I8(){

}
void DeviceDigitalIO::switchOff_DigitalIO_I8(){

}
void DeviceDigitalIO::start_DigitalIO_I8(){
    m_bStop_DigitalIO_I8   = false;

    /**
     *   Start the acquisition thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_DigitalIO_Thread_I8 =
      m_DigitalIO_I8.runInThread("DIOI8",
                     std::bind(&DeviceDigitalIO::DigitalIO_thread_body_I8, this));

}
void DeviceDigitalIO::stop_DigitalIO_I8(){
    m_bStop_DigitalIO_I8 = true;
    m_DigitalIO_Thread_I8.join();

}
void DeviceDigitalIO::recover_DigitalIO_I8(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceDigitalIO::allow_DigitalIO_Change_I8(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

/**
* DigitalIO setters
*/
void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I8(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the dataOutMask to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DigitalIO_I8.setDataOutMask(timestamp,value);
}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I8(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelHigh to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I8.setVoltLevelHigh(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I8(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelLow to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelLow programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I8.setVoltLevelLow(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I8(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the ChannelDir to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real ChannelDir programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I8.setChannelDir(timestamp,HW_value);
}


/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_I8(){

    // Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
    std::vector<std::int8_t> outputData(m_DigitalIO_I8.getMaxElements(),0);

        //Counter for number of pushed data blocks
    std::int32_t NumberOfPushedDataBlocks(0);

    std::int8_t value(0);

    // Get DataOutMask
    std::vector<bool> DataOutMask = m_DigitalIO_I8.getDataOutMask();
    // Get VoltLevelHigh
    double VoltLevelHigh = m_DigitalIO_I8.getVoltLevelHigh();
    // Get VoltLevelLow
    double VoltLevelLow = m_DigitalIO_I8.getVoltLevelLow();
    // Get ChannelDir
    std::vector<bool> ChannelDir = m_DigitalIO_I8.getChannelDir();

    std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
    std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
    //std::cout<<"\DataOutMask = "<<DataOutMask<<std::endl;
    //std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


    // Run until the state machine stops us
    while(!m_bStop_DigitalIO_I8){

        size_t scanVector(0);

        for(scanVector=0; scanVector != outputData.size(); ++scanVector){
            outputData[scanVector] = value;
        }
        ++value;

        // Push the vector to the control system
        m_DigitalIO_I8.push(m_DigitalIO_I8.getTimestamp(), outputData);
        ++NumberOfPushedDataBlocks;
        //TODO: Send values to data acquisition node.

        // Rest for a while
        ::usleep(100000);
    }
    m_DigitalIO_I8.setNumberOfPushedDataBlocks(m_DigitalIO_I8.getTimestamp(),NumberOfPushedDataBlocks);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Uint16_t Node STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Methods to control DigitalIO state machine
*/
void DeviceDigitalIO::switchOn_DigitalIO_I16(){

}
void DeviceDigitalIO::switchOff_DigitalIO_I16(){

}
void DeviceDigitalIO::start_DigitalIO_I16(){
    m_bStop_DigitalIO_I16  = false;

    /**
     *   Start the acquisition thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_DigitalIO_Thread_I16 =
      m_DigitalIO_I16.runInThread("DIOI16",
                      std::bind(&DeviceDigitalIO::DigitalIO_thread_body_I16, this));

}
void DeviceDigitalIO::stop_DigitalIO_I16(){
    m_bStop_DigitalIO_I16 = true;
    m_DigitalIO_Thread_I16.join();
}
void DeviceDigitalIO::recover_DigitalIO_I16(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceDigitalIO::allow_DigitalIO_Change_I16(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

/**
* DigitalIO setters
*/
void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I16(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the dataOutMask to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DigitalIO_I16.setDataOutMask(timestamp,value);
}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I16(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelHigh to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I16.setVoltLevelHigh(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I16(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelLow to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelLow programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I16.setVoltLevelLow(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I16(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the ChannelDir to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real ChannelDir programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I16.setChannelDir(timestamp,HW_value);
}

/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_I16(){

    // Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
    std::vector<std::int16_t> outputData(m_DigitalIO_I16.getMaxElements(),0);

    //Counter for number of pushed data blocks
    std::int32_t NumberOfPushedDataBlocks(0);

    std::int16_t value(0);

    // Get DataOutMask
    std::vector<bool> DataOutMask = m_DigitalIO_I16.getDataOutMask();
    // Get VoltLevelHigh
    double VoltLevelHigh = m_DigitalIO_I16.getVoltLevelHigh();
    // Get VoltLevelLow
    double VoltLevelLow = m_DigitalIO_I16.getVoltLevelLow();
    // Get ChannelDir
    std::vector<bool> ChannelDir = m_DigitalIO_I16.getChannelDir();

    std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
    std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
    //std::cout<<"\DataOutMask = "<<DataOutMask<<std::endl;
    //std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


    // Run until the state machine stops us
    while(!m_bStop_DigitalIO_I16){

        size_t scanVector(0);

        for(scanVector=0; scanVector != outputData.size(); ++scanVector){
            outputData[scanVector] = value;
        }
        ++value;
        // Push the vector to the control system
        m_DigitalIO_I16.push(m_DigitalIO_I16.getTimestamp(), outputData);
        ++NumberOfPushedDataBlocks;
        //TODO: Send values to data acquisition node.

        // Rest for a while
        ::usleep(100000);
    }
    m_DigitalIO_I16.setNumberOfPushedDataBlocks(m_DigitalIO_I16.getTimestamp(),NumberOfPushedDataBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Int64_t Node STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Methods to control DigitalIO state machine
*/
void DeviceDigitalIO::switchOn_DigitalIO_I64(){

}
void DeviceDigitalIO::switchOff_DigitalIO_I64(){

}
void DeviceDigitalIO::start_DigitalIO_I64(){
    m_bStop_DigitalIO_I64  = false;

    /**
     *   Start the acquisition thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_DigitalIO_Thread_I64 =
      m_DigitalIO_I64.runInThread("DIOI64",
                      std::bind(&DeviceDigitalIO::DigitalIO_thread_body_I64, this));

}
void DeviceDigitalIO::stop_DigitalIO_I64(){
    m_bStop_DigitalIO_I64 = true;
    m_DigitalIO_Thread_I64.join();
}
void DeviceDigitalIO::recover_DigitalIO_I64(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceDigitalIO::allow_DigitalIO_Change_I64(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

/**
* DigitalIO setters
*/
void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I64(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the dataOutMask to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DigitalIO_I64.setDataOutMask(timestamp,value);
}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I64(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelHigh to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I64.setVoltLevelHigh(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I64(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelLow to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelLow programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I64.setVoltLevelLow(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I64(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the ChannelDir to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real ChannelDir programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I64.setChannelDir(timestamp,HW_value);
}

/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_I64(){

    // Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
    std::vector<std::int64_t> outputData(m_DigitalIO_I64.getMaxElements(),0);

    //Counter for number of pushed data blocks
    std::int32_t NumberOfPushedDataBlocks(0);

    std::int64_t value(0);

    // Get DataOutMask
    std::vector<bool> DataOutMask = m_DigitalIO_I64.getDataOutMask();
    // Get VoltLevelHigh
    double VoltLevelHigh = m_DigitalIO_I64.getVoltLevelHigh();
    // Get VoltLevelLow
    double VoltLevelLow = m_DigitalIO_I64.getVoltLevelLow();
    // Get ChannelDir
    std::vector<bool> ChannelDir = m_DigitalIO_I64.getChannelDir();

    std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
    std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
    //std::cout<<"\DataOutMask = "<<DataOutMask<<std::endl;
    //std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


    // Run until the state machine stops us
    while(!m_bStop_DigitalIO_I64){

        size_t scanVector(0);

        for(scanVector=0; scanVector != outputData.size(); ++scanVector){
            outputData[scanVector] = value;
        }
        ++value;
        // Push the vector to the control system
        m_DigitalIO_I64.push(m_DigitalIO_I64.getTimestamp(), outputData);
        ++NumberOfPushedDataBlocks;
        //TODO: Send values to data acquisition node.

        // Rest for a while
        ::usleep(100000);
    }
    m_DigitalIO_I64.setNumberOfPushedDataBlocks(m_DigitalIO_I64.getTimestamp(),NumberOfPushedDataBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Uint32_t Node STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Methods to control DigitalIO state machine
*/
void DeviceDigitalIO::switchOn_DigitalIO_I32(){

}
void DeviceDigitalIO::switchOff_DigitalIO_I32(){

}
void DeviceDigitalIO::start_DigitalIO_I32(){
    m_bStop_DigitalIO_I32  = false;

    /**
     *   Start the acquisition thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_DigitalIO_Thread_I32 =
      m_DigitalIO_I32.runInThread("DIOI32",
                      std::bind(&DeviceDigitalIO::DigitalIO_thread_body_I32, this));

}
void DeviceDigitalIO::stop_DigitalIO_I32(){
    m_bStop_DigitalIO_I32 = true;
    m_DigitalIO_Thread_I32.join();
}
void DeviceDigitalIO::recover_DigitalIO_I32(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceDigitalIO::allow_DigitalIO_Change_I32(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}



/**
* DigitalIO setters
*/
void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer_I32(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the dataOutMask to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DigitalIO_I32.setDataOutMask(timestamp,value);
}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer_I32(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelHigh to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I32.setVoltLevelHigh(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer_I32(const timespec& timestamp, const double& value){
    double HW_value;
    //Value has the voltLevelLow to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real voltLevelLow programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I32.setVoltLevelLow(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer_I32(const timespec& timestamp, const std::vector<bool>& value){
    std::vector<bool> HW_value;
    //Value has the ChannelDir to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real ChannelDir programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value=value;
    m_DigitalIO_I32.setChannelDir(timestamp,HW_value);
}

void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer(timespec* timestamp,
        std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = std::vector<bool>(1, true); //Vector with one data to true.

}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Initializer(timespec* timestamp,
        double* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 3.3;
}

void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Initializer(timespec* timestamp,
        double* value) {
    *timestamp = {NDS_EPOCH, 30};
    *value = 1.1;
}

void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer(timespec* timestamp,
        std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 40};
    *value = std::vector<bool>(1, false); //Vector with one data to false.
}

void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I8(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 50};
    *value = std::vector<bool>(8, false); //Vector with 8 data to false.
}

void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I8(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 60};
    *value = std::vector<bool>(8, true); //Vector with 8 data to true.
}

void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I16(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 70};
    *value = std::vector<bool>(16, true); //Vector with 16 data to true.
}

void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I16(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 80};
    *value = std::vector<bool>(16, false); //Vector with 16 data to false.
}

void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I32(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 90};
    *value = std::vector<bool>(32, false); //Vector with 32 data to false.
}

void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I32(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 100};
    *value = std::vector<bool>(32, true); //Vector with 32 data to true.
}

void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Initializer_I64(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 110};
    *value = std::vector<bool>(64, false); //Vector with 64 data to false.
}

void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Initializer_I64(
        timespec* timestamp, std::vector<bool>* value) {
    *timestamp = {NDS_EPOCH, 120};
    *value = std::vector<bool>(64, true); //Vector with 64 data to true.
}

/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_I32(){

    // Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
    std::vector<std::int32_t> outputData(m_DigitalIO_I32.getMaxElements(),0);

    //Counter for number of pushed data blocks
    std::int32_t NumberOfPushedDataBlocks(0);

    std::int32_t value(0);

    // Get DataOutMask
    std::vector<bool> DataOutMask = m_DigitalIO_I32.getDataOutMask();
    // Get VoltLevelHigh
    double VoltLevelHigh = m_DigitalIO_I32.getVoltLevelHigh();
    // Get VoltLevelLow
    double VoltLevelLow = m_DigitalIO_I32.getVoltLevelLow();
    // Get ChannelDir
    std::vector<bool> ChannelDir = m_DigitalIO_I32.getChannelDir();

    std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
    std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
    //std::cout<<"\DataOutMask = "<<DataOutMask<<std::endl;
    //std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


    // Run until the state machine stops us
    while(!m_bStop_DigitalIO_I32){

        size_t scanVector(0);

        for(scanVector=0; scanVector != outputData.size(); ++scanVector){
            outputData[scanVector] = value;
        }
        ++value;
        // Push the vector to the control system
        m_DigitalIO_I32.push(m_DigitalIO_I32.getTimestamp(), outputData);
        ++NumberOfPushedDataBlocks;
        //TODO: Send values to data acquisition node.

        // Rest for a while
        ::usleep(100000);
    }
    m_DigitalIO_I32.setNumberOfPushedDataBlocks(m_DigitalIO_I32.getTimestamp(),NumberOfPushedDataBlocks);
}

// NDS_DEFINE_DRIVER(DeviceDigitalIO, DeviceDigitalIO)
 /**
  * Allocation function
  *********************/
 void* DeviceDigitalIO::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
 {
     return new DeviceDigitalIO(factory, deviceName, parameters);
 }

 /**
  * Deallocation function
  ***********************/
 void DeviceDigitalIO::deallocateDevice(void* deviceName)
 {
     delete (DeviceDigitalIO*)deviceName;
 }

