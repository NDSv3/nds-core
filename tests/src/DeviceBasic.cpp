
#include "DeviceBasic.h"

#include <nds3/nds.h>
#include <mutex>
#ifdef _WIN32
static const double M_PI = 3.14159265358979323846;
#else
#include <unistd.h>
#endif
#include <functional>


static std::map<std::string, DeviceBasic*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DeviceBasic::DeviceBasic(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t &/*parameters*/):
    m_name(DeviceName),

    PVVariable_value_I32(0),PVDelegate_value_I32(0),PVVariable_value_I64(0),PVDelegate_value_I64(0),PVVariable_value_DBL(0),PVDelegate_value_DBL(0),PVVariable_vector_I8(2,0),PVDelegate_vector_I8(2,0),
    PVVariable_vector_UI8(2,0), PVDelegate_vector_UI8(2,0),PVVariable_vector_I32(2,0),PVDelegate_vector_I32(2,0),PVVariable_vector_I64(2,0),PVDelegate_vector_I64(2,0),PVVariable_vector_DBL(2,0),PVDelegate_vector_DBL(2,0),
    PVVariable_value_string{""},PVDelegate_value_string{""},

    timestamp_device{0,0},readtimeStamp{0,0},

    m_int32_DelegateIn("int32_DelegateIn",std::bind(&DeviceBasic::read_I32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_int32_DelegateOut("int32_DelegateOut",std::bind(&DeviceBasic::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_int32_DelegateOut_init("int32_DelegateOut_init",std::bind(&DeviceBasic::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_int64_DelegateIn("int64_DelegateIn",std::bind(&DeviceBasic::read_I64_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_int64_DelegateOut("int64_DelegateOut",std::bind(&DeviceBasic::write_I64_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_int64_DelegateOut_init("int64_DelegateOut_init",std::bind(&DeviceBasic::write_I64_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_I64_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_double_DelegateIn("double_DelegateIn",std::bind(&DeviceBasic::read_DBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_double_DelegateOut("double_DelegateOut",std::bind(&DeviceBasic::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_double_DelegateOut_init("double_DelegateOut_init",std::bind(&DeviceBasic::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_vectorI8_DelegateIn("vectorI8_DelegateIn",std::bind(&DeviceBasic::read_vectorI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorI8_DelegateOut("vectorI8_DelegateOut",std::bind(&DeviceBasic::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorI8_DelegateOut_init("vectorI8_DelegateOut_init",std::bind(&DeviceBasic::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_vectorUI8_DelegateIn("vectorUI8_DelegateIn",std::bind(&DeviceBasic::read_vectorUI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorUI8_DelegateOut("vectorUI8_DelegateOut",std::bind(&DeviceBasic::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorUI8_DelegateOut_init("vectorUI8_DelegateOut_init",std::bind(&DeviceBasic::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_vectorI32_DelegateIn("vectorI32_DelegateIn",std::bind(&DeviceBasic::read_vectorI32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorI32_DelegateOut("vectorI32_DelegateOut",std::bind(&DeviceBasic::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorI32_DelegateOut_init("vectorI32_DelegateOut_init",std::bind(&DeviceBasic::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_vectorI64_DelegateIn("vectorI64_DelegateIn",std::bind(&DeviceBasic::read_vectorI64_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorI64_DelegateOut("vectorI64_DelegateOut",std::bind(&DeviceBasic::write_vectorI64_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorI64_DelegateOut_init("vectorI64_DelegateOut_init",std::bind(&DeviceBasic::write_vectorI64_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_vectorI64_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_vectorDBL_DelegateIn("vectorDBL_DelegateIn",std::bind(&DeviceBasic::read_vectorDBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorDBL_DelegateOut("vectorDBL_DelegateOut",std::bind(&DeviceBasic::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_vectorDBL_DelegateOut_init("vectorDBL_DelegateOut_init",std::bind(&DeviceBasic::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_string_DelegateIn("string_DelegateIn",std::bind(&DeviceBasic::read_string_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
    m_string_DelegateOut("string_DelegateOut",std::bind(&DeviceBasic::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
    m_string_DelegateOut_init("string_DelegateOut_init",std::bind(&DeviceBasic::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceBasic::init_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

    m_delegateIn("delegateIn", std::bind(&DeviceBasic::readDelegate, this, std::placeholders::_1, std::placeholders::_2)),
    m_delegateOut("delegateOut", std::bind(&DeviceBasic::writeDelegate, this, std::placeholders::_1, std::placeholders::_2)),
    m_writeTestVariableIn("writeTestVariableIn", std::bind(&DeviceBasic::writeTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)),
    m_pushTestVariableIn("pushTestVariableIn", std::bind(&DeviceBasic::pushTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)),
    m_readTestVariableOut("readTestVariableOut", std::bind(&DeviceBasic::readTestVariableOut, this, std::placeholders::_1, std::placeholders::_2))
    {
    //TODO:Study this.
    {
        std::lock_guard<std::mutex> lock(m_lockDevicesMap);
        if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end())
        {
            throw std::logic_error("Device with the same name already allocated. This should not happen");
        }
        m_DevicesMap[DeviceName] = this;
    }

    /**
     * @verbatim
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
     * @endverbatim
     */
    nds::Port rootNode(DeviceName);

    // Add state machine
    m_Device_stateMachine = rootNode.addChild(nds::StateMachine(true,
            std::bind(&DeviceBasic::switchOn_Device, this),
            std::bind(&DeviceBasic::switchOff_Device, this),
            std::bind(&DeviceBasic::start_Device, this),
            std::bind(&DeviceBasic::stop_Device, this),
            std::bind(&DeviceBasic::recover_Device, this),
            std::bind(&DeviceBasic::allow__Device_Change,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    // Add Firmware node
    m_Firmware = rootNode.addChild(nds::Firmware("Firm",
            256, // Maximum string length.
            std::bind(&DeviceBasic::switchOn_Firmware, this),
            std::bind(&DeviceBasic::switchOff_Firmware, this),
            std::bind(&DeviceBasic::start_Firmware, this),
            std::bind(&DeviceBasic::stop_Firmware, this),
            std::bind(&DeviceBasic::recover_Firmware, this),
            std::bind(&DeviceBasic::allow_Firmware_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&DeviceBasic::PV_Firmware_Path_Writer, this, std::placeholders::_1, std::placeholders::_2)));

    m_Firmware.setTimestampDelegate(std::bind(&DeviceBasic::getCurrentTime,this));
    m_Firmware.setLogLevel(nds::logLevel_t::debug);

    /**
     * @verbatim
     * Test PV variables:
     *      - PVVariableIn: input variable to CS. The Device support sets its value using setValue, and the CS reads this value.
     *      - PVVariableOut: output variable from CS. The Device support uses getValue()to retrieve the PV's value, and the control system uses read() and write() to read and set the value.
     *
     * For testing purposes, we use:
     *      - setValue(const T& value) and setValue(const timespec& timestamp, const T& value), over PVVariableIn but we do not modify the initial value.
     *      - getValue() and getValue(timespec* pTime, T* pValue) to retrieve the initial value and timestamp.
     * @endverbatim
     */

    m_int32_VariableIn = rootNode.addChild(nds::PVVariableIn<std::int32_t>("int32_VariableIn"));
    m_int32_VariableIn.setValue((std::int32_t)0);
    m_int32_VariableIn.setValue(timestamp_device,(std::int32_t)0);
    m_int32_VariableOut = rootNode.addChild(nds::PVVariableOut<std::int32_t>("int32_VariableOut"));
    m_int32_VariableOut.getValue();
    m_int32_VariableOut.getValue(&readtimeStamp,&PVVariable_value_I32);

    m_int64_VariableIn = rootNode.addChild(nds::PVVariableIn<std::int64_t>("int64_VariableIn"));
    m_int64_VariableIn.setValue((std::int64_t)0);
    m_int64_VariableIn.setValue(timestamp_device,(std::int64_t)0);
    m_int64_VariableOut = rootNode.addChild(nds::PVVariableOut<std::int64_t>("int64_VariableOut"));
    m_int64_VariableOut.getValue();
    m_int64_VariableOut.getValue(&readtimeStamp,&PVVariable_value_I64);

    m_double_VariableIn = rootNode.addChild(nds::PVVariableIn<double>("double_VariableIn"));
    m_double_VariableIn.setValue((double)0);
    m_double_VariableIn.setValue(timestamp_device,(double)0);
    m_double_VariableOut = rootNode.addChild(nds::PVVariableOut<double>("double_VariableOut"));
    m_double_VariableOut.getValue();
    m_double_VariableOut.getValue(&readtimeStamp,&PVVariable_value_DBL);

    m_vectorI8_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::int8_t> >("vectorI8_VariableIn"));
    m_vectorI8_VariableIn.setMaxElements(2);
    m_vectorI8_VariableIn.setValue(std::vector<int8_t>(2,0));
    m_vectorI8_VariableIn.setValue(timestamp_device,std::vector<int8_t>(2,0));
    m_vectorI8_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::int8_t> >("vectorI8_VariableOut"));
    m_vectorI8_VariableOut.getValue();
    m_vectorI8_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_I8);

    m_vectorUI8_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::uint8_t> >("vectorUI8_VariableIn"));
    m_vectorUI8_VariableIn.setMaxElements(2);
    m_vectorUI8_VariableIn.setValue(std::vector<uint8_t>(2,0));
    m_vectorUI8_VariableIn.setValue(timestamp_device,std::vector<uint8_t>(2,0));
    m_vectorUI8_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::uint8_t> >("vectorUI8_VariableOut"));
    m_vectorUI8_VariableOut.getValue();
    m_vectorUI8_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_UI8);

    m_vectorI32_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::int32_t> >("vectorI32_VariableIn"));
    m_vectorI32_VariableIn.setMaxElements(2);
    m_vectorI32_VariableIn.setValue(std::vector<int32_t>(2,0));
    m_vectorI32_VariableIn.setValue(timestamp_device,std::vector<int32_t>(2,0));
    m_vectorI32_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::int32_t> >("vectorI32_VariableOut"));
    m_vectorI32_VariableOut.getValue();
    m_vectorI32_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_I32);

    m_vectorI64_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::int64_t> >("vectorI64_VariableIn"));
    m_vectorI64_VariableIn.setMaxElements(2);
    m_vectorI64_VariableIn.setValue(std::vector<int64_t>(2,0));
    m_vectorI64_VariableIn.setValue(timestamp_device,std::vector<int64_t>(2,0));
    m_vectorI64_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::int64_t> >("vectorI64_VariableOut"));
    m_vectorI64_VariableOut.getValue();
    m_vectorI64_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_I64);

    m_vectorDBL_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<double> >("vectorDBL_VariableIn"));
    m_vectorDBL_VariableIn.setMaxElements(2);
    m_vectorDBL_VariableIn.setValue(std::vector<double>(2,0));
    m_vectorDBL_VariableIn.setValue(timestamp_device,std::vector<double>(2,0));
    m_vectorDBL_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<double> >("vectorDBL_VariableOut"));
    m_vectorDBL_VariableOut.getValue();
    m_vectorDBL_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_DBL);

    m_string_VariableIn= rootNode.addChild(nds::PVVariableIn<std::string>("string_VariableIn"));
    m_string_VariableIn.setValue("");
    m_string_VariableIn.setValue(timestamp_device,"");
    m_string_VariableOut= rootNode.addChild(nds::PVVariableOut<std::string>("string_VariableOut"));
    m_string_VariableOut.getValue();
    m_string_VariableOut.getValue(&readtimeStamp,&PVVariable_value_string);

    m_testVariableIn= rootNode.addChild(nds::PVVariableIn<std::string>("testVariableIn"));
    m_testVariableOut= rootNode.addChild(nds::PVVariableOut<std::string>("testVariableOut"));

    /**
     * Test PV Delegates: input Delegate to CS. The Device support sets its value, and the CS reads this value
     * The Device support uses getValue()to retrieve the PV's value and the control system use read() and write() to read and set the value.
     */

    rootNode.addChild(m_int32_DelegateIn);
    rootNode.addChild(m_int32_DelegateOut);
    rootNode.addChild(m_int32_DelegateOut_init);

    rootNode.addChild(m_int64_DelegateIn);
    rootNode.addChild(m_int64_DelegateOut);
    rootNode.addChild(m_int64_DelegateOut_init);

    rootNode.addChild(m_double_DelegateIn);
    rootNode.addChild(m_double_DelegateOut);
    rootNode.addChild(m_double_DelegateOut_init);

    m_vectorI8_DelegateIn.setMaxElements(2);
    rootNode.addChild(m_vectorI8_DelegateIn);
    rootNode.addChild(m_vectorI8_DelegateOut);
    rootNode.addChild(m_vectorI8_DelegateOut_init);

    m_vectorUI8_DelegateIn.setMaxElements(2);
    rootNode.addChild(m_vectorUI8_DelegateIn);
    rootNode.addChild(m_vectorUI8_DelegateOut);
    rootNode.addChild(m_vectorUI8_DelegateOut_init);

    m_vectorI32_DelegateIn.setMaxElements(2);
    rootNode.addChild(m_vectorI32_DelegateIn);
    rootNode.addChild(m_vectorI32_DelegateOut);
    rootNode.addChild(m_vectorI32_DelegateOut_init);

    m_vectorI64_DelegateIn.setMaxElements(2);
    rootNode.addChild(m_vectorI64_DelegateIn);
    rootNode.addChild(m_vectorI64_DelegateOut);
    rootNode.addChild(m_vectorI64_DelegateOut_init);

    m_vectorDBL_DelegateIn.setMaxElements(2);
    rootNode.addChild(m_vectorDBL_DelegateIn);
    rootNode.addChild(m_vectorDBL_DelegateOut);
    rootNode.addChild(m_vectorDBL_DelegateOut_init);

    rootNode.addChild(m_string_DelegateIn);
    rootNode.addChild(m_string_DelegateOut);
    rootNode.addChild(m_string_DelegateOut_init);

    rootNode.addChild(m_delegateIn);
    rootNode.addChild(m_delegateOut);
    rootNode.addChild(m_writeTestVariableIn);
    rootNode.addChild(m_pushTestVariableIn);
    rootNode.addChild(m_readTestVariableOut);

    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));

    // We have declared all the nodes and PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);

    std::string rootNodeComponentName = rootNode.getComponentName();
    std::string rootNodeFullExternalName = rootNode.getFullExternalName();
    std::string rootNodeFullName = rootNode.getFullName();
    std::string rootNodeFullNameFromPort = rootNode.getFullNameFromPort();
    //timespec rootNodetime = rootNode.getTimestamp();
    //    bool isLogLevelEnabled = rootNode.isLogLevelEnabled(nds::logLevel_t::debug);
    //
    //    std::cout<<"\trootNodeComponentName = " <<rootNodeComponentName<<std::endl;
    //    std::cout<<"\trootNodeFullExternalName = " <<rootNodeFullExternalName<<std::endl;
    //    std::cout<<"\trootNodeFullName = " <<rootNodeFullName<<std::endl;
    //    std::cout<<"\trootNodeFullNameFromPort = " <<rootNodeFullNameFromPort<<std::endl;
    //    std::cout<<"\tisLogLevelEnabled = " <<isLogLevelEnabled<<std::endl;
    //    std::cout<<"\trootNodetime.tv_sec = " <<rootNodetime.tv_sec<<"\t;\trootNodetime.tv_nsec = " <<rootNodetime.tv_nsec<<std::endl;

    rootNode.setTimestampDelegate(std::bind(&DeviceBasic::getCurrentTime,this));
    rootNode.setLogLevel(nds::logLevel_t::debug);

    rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger:The device is created" << std::endl;
    ndsDebugStream(rootNode) << "This is the ndsDebugStream: The device is created" << std::endl;

}


DeviceBasic::~DeviceBasic()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_name);

}

DeviceBasic* DeviceBasic::getInstance(const std::string& DeviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceBasic*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

/*
 * Allocation function
 *********************/
void* DeviceBasic::allocateDevice(nds::Factory& factory, const std::string& DeviceName, const nds::namedParameters_t& parameters)
{
    return new DeviceBasic(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceBasic::deallocateDevice(void* DeviceName)
{
    delete (DeviceBasic*)DeviceName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEST Device STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control Device state machine
 */
void DeviceBasic::switchOn_Device(){
    // Call HW initialization function here,
        //HW_CALL_INIT_FUNCTION

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
        // Call API HW to retrieve FirmwarePath
        m_Firmware.setFirmwarePath(getCurrentTime(),"Firmware path to be uploaded");

}
void DeviceBasic::switchOff_Device(){

}
void DeviceBasic::start_Device(){

}
void DeviceBasic::stop_Device(){

}
void DeviceBasic::recover_Device(){

}

bool DeviceBasic::allow__Device_Change(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// EXTRA PVDELEGATE IN/OUT FOR TESTING PURPOSES
///////////////////////////////////////////////////////////////////////////////////////////////////

void DeviceBasic::read_I32_DelegateIn(timespec* timestamp, std::int32_t* value){
    *value=PVDelegate_value_I32;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_I64_DelegateIn(timespec* timestamp, std::int64_t* value){
    *value=PVDelegate_value_I64;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_DBL_DelegateIn(timespec* timestamp, double* value){
    *value=PVDelegate_value_DBL;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_vectorI8_DelegateIn(timespec* timestamp, std::vector<std::int8_t>* value){
    *value=PVDelegate_vector_I8;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_vectorUI8_DelegateIn(timespec* timestamp, std::vector<std::uint8_t>* value){
    *value=PVDelegate_vector_UI8;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_vectorI32_DelegateIn(timespec* timestamp, std::vector<std::int32_t>* value){
    *value=PVDelegate_vector_I32;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_vectorI64_DelegateIn(timespec* timestamp, std::vector<std::int64_t>* value){
    *value=PVDelegate_vector_I64;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_vectorDBL_DelegateIn(timespec* timestamp, std::vector<double>* value){
    *value=PVDelegate_vector_DBL;
    *timestamp=timestamp_device;
}
void DeviceBasic::read_string_DelegateIn(timespec* timestamp, std::string* value){
    *value=PVDelegate_value_string;
    *timestamp=timestamp_device;
}

void DeviceBasic::write_I32_DelegateOut(const timespec& timestamp, const std::int32_t& value){
    PVDelegate_value_I32=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_I64_DelegateOut(const timespec& timestamp, const std::int64_t& value){
    PVDelegate_value_I64=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_DBL_DelegateOut(const timespec& timestamp,const double& value){
    PVDelegate_value_DBL=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_vectorI8_DelegateOut(const timespec& timestamp,const std::vector<std::int8_t>& value){
    PVDelegate_vector_I8=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_vectorUI8_DelegateOut(const timespec& timestamp,const std::vector<std::uint8_t>& value){
    PVDelegate_vector_UI8=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_vectorI32_DelegateOut(const timespec& timestamp,const std::vector<std::int32_t>& value){
    PVDelegate_vector_I32=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_vectorI64_DelegateOut(const timespec& timestamp,const std::vector<std::int64_t>& value){
    PVDelegate_vector_I64=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_vectorDBL_DelegateOut(const timespec& timestamp,const std::vector<double>& value){
    PVDelegate_vector_DBL=value;
    timestamp_device=timestamp;
}
void DeviceBasic::write_string_DelegateOut(const timespec& timestamp,const std::string& value){
    PVDelegate_value_string=value;
    timestamp_device=timestamp;
}

void DeviceBasic::init_I32_DelegateOut(timespec* timestamp,  std::int32_t* value){
    *value=PVDelegate_value_I32;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_I64_DelegateOut(timespec* timestamp,  std::int64_t* value){
    *value=PVDelegate_value_I64;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_DBL_DelegateOut(timespec* timestamp, double* value){
    *value=PVDelegate_value_DBL;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_vectorI8_DelegateOut(timespec* timestamp, std::vector<std::int8_t>* value){
    *value=PVDelegate_vector_I8;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_vectorUI8_DelegateOut(timespec* timestamp, std::vector<std::uint8_t>* value){
    *value=PVDelegate_vector_UI8;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_vectorI32_DelegateOut(timespec* timestamp, std::vector<std::int32_t>* value){
    *value=PVDelegate_vector_I32;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_vectorI64_DelegateOut(timespec* timestamp, std::vector<std::int64_t>* value){
    *value=PVDelegate_vector_I64;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_vectorDBL_DelegateOut(timespec* timestamp, std::vector<double>* value){
    *value=PVDelegate_vector_DBL;
    *timestamp=timestamp_device;
}
void DeviceBasic::init_string_DelegateOut(timespec* timestamp, std::string* value){
    *value=PVDelegate_value_string;
    *timestamp=timestamp_device;
}


void DeviceBasic::readDelegate(timespec* pTimestamp, std::string* pValue)
{
    *pTimestamp = timestamp_device;
    *pValue = m_writtenByDelegate;
}

void DeviceBasic::writeDelegate(const timespec& timestamp, const std::string& value)
{
    timestamp_device = timestamp;
    m_writtenByDelegate = value;
}

void DeviceBasic::writeTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.setValue(timestamp, value);
}

void DeviceBasic::pushTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.push(timestamp, value);
}

void DeviceBasic::readTestVariableOut(timespec* pTimestamp, std::string* pValue)
{
    m_testVariableOut.getValue(pTimestamp, pValue);
}

timespec DeviceBasic::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// FIRMWARE SUPPORT NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* Firmware State Machine
*/

// Called when the Firmware node has to be switched on.
void DeviceBasic::switchOn_Firmware(){

}

// Called when the Firmware node has to be switched off.
void DeviceBasic::switchOff_Firmware(){

}

// Called when the Firmware node has to start acquiring. We start the Firmware thread.
void DeviceBasic::start_Firmware(){

    m_bStop_Firmware = false; //< We will set to true to stop the Firmware thread
    /**
     *   Start the Firmware thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_Firmware_Thread = std::thread(std::bind(&DeviceBasic::Firmware_thread_body, this));
}

// Stop the DataAcquisition node thread
void DeviceBasic::stop_Firmware(){
    m_bStop_Firmware = true;
    m_Firmware_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void DeviceBasic::recover_Firmware(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool DeviceBasic::allow_Firmware_Change(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

/*
* Firmware support setters
*/
void DeviceBasic::PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value){
    std::string firmwarePath;
    //firmwarePath has the firmware path to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real firmware path programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  firmwarePath are equal.
    firmwarePath=value;
    m_Firmware.setFirmwarePath(timestamp,firmwarePath);
}

void DeviceBasic::Firmware_thread_body(){

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
    // Get FirmwarePath
    std::string FirmwarePath = m_Firmware.getFirmwarePath();
    std::string FirmwarePathOld=m_Firmware.getFirmwarePath();


    std::cout<<"Firmware support information:"<<std::endl;
    std::cout<<"\tFirmwareVersion = "<<FirmwareVersion<<std::endl;
    std::cout<<"\tFirmwareStatus = "<<FirmwareStatus<<std::endl;
    std::cout<<"\tHardwareRevision = "<<HardwareRevision<<std::endl;
    std::cout<<"\tSerialNumber = "<<SerialNumber<<std::endl;
    std::cout<<"\tDeviceModel = "<<DeviceModel<<std::endl;
    std::cout<<"\tDeviceType = "<<DeviceType<<std::endl;
    std::cout<<"\tFirmwarePath = "<<FirmwarePath<<std::endl;

    // Run until the state machine stops us
    while(!m_bStop_Firmware){


        // Get FirmwarePath
        std::string FirmwarePath = m_Firmware.getFirmwarePath();
        if(FirmwarePath.compare(FirmwarePathOld)!=0){
            // Push the FirmwarePath to the control system
            m_Firmware.push(m_Firmware.getTimestamp(), FirmwarePath);
            FirmwarePathOld=FirmwarePath;
        }
        // Rest for a while
        ::usleep(1000000);
    }
}


