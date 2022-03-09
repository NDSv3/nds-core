#include <mutex>
#include <functional>

#include <nds3/nds.h>

#include "DeviceFTE.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceFTE*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceFTE::DeviceFTE(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t & parameters):
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
    if (findParam != parameters.end() && findParam->second=="YES") {
        //Set compulsory methods
        nds::FTEArgs_t handlerFTE = nds::FTEArgs_t(
                std::bind(&DeviceFTE::switchOn_FTE, this),
                std::bind(&DeviceFTE::switchOff_FTE, this),
                std::bind(&DeviceFTE::start_FTE, this),
                std::bind(&DeviceFTE::stop_FTE, this),
                std::bind(&DeviceFTE::recover_FTE, this),
                std::bind(&DeviceFTE::allow_FTE_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceFTE::PV_FTE_Set_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceFTE::PV_FTE_Suppress_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceFTE::PV_FTE_ChgPeriod_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceFTE::PV_FTE_PendingValue_Writer,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerFTE.PV_Set_Initializer = std::bind(&DeviceFTE::PV_FTE_Set_Initializer, this, std::placeholders::_1, std::placeholders::_2),
        handlerFTE.PV_Suppress_Initializer = std::bind(&DeviceFTE::PV_FTE_Suppress_Initializer, this, std::placeholders::_1, std::placeholders::_2),
        handlerFTE.PV_ChgPeriod_Initializer = std::bind(&DeviceFTE::PV_FTE_ChgPeriod_Initializer, this, std::placeholders::_1, std::placeholders::_2),
        handlerFTE.PV_PendingValue_Initializer = std::bind(&DeviceFTE::PV_FTE_PendingValue_Initializer, this, std::placeholders::_1, std::placeholders::_2),
        //Set init values: Note that these value have no actual sense and they are fixed only for testing purposes.
        handlerFTE.m_TerminalSet_Init = -1;
        handlerFTE.m_ModeSet_Init = 0;
        handlerFTE.m_StartTimeSet_Init = {10,20};
        handlerFTE.m_StopTimeSet_Init = {20,10};
        handlerFTE.m_LevelSet_Init = 0;
        handlerFTE.m_PeriodNsecSet_Init = 1000;
        handlerFTE.m_DutyCycleSet_Init = 80;
        handlerFTE.m_TerminalSuppress_Init = -2;
        handlerFTE.m_ModeSuppress_Init = 0;
        handlerFTE.m_AllSuppress_Init = 0;
        handlerFTE.m_StartTimeSuppress_Init = {20,20};
        handlerFTE.m_TerminalChgPeriod_Init = -3;
        handlerFTE.m_PeriodChgPeriod_Init = 500;
        m_FTE = rootNode.addChild(nds::FTE<std::string>("FTENode", handlerFTE));
    } else {
        m_FTE = rootNode.addChild(nds::FTE<std::string>(
                "FTENode",
                std::bind(&DeviceFTE::switchOn_FTE, this),
                std::bind(&DeviceFTE::switchOff_FTE, this),
                std::bind(&DeviceFTE::start_FTE, this),
                std::bind(&DeviceFTE::stop_FTE, this),
                std::bind(&DeviceFTE::recover_FTE, this),
                std::bind(&DeviceFTE::allow_FTE_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceFTE::PV_FTE_Set_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceFTE::PV_FTE_Suppress_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceFTE::PV_FTE_ChgPeriod_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceFTE::PV_FTE_PendingValue_Writer,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }

    /**
      * Add a FTE node:
      */

    //m_FTE.setStartTimestampDelegate(std::bind(&DeviceFTE::getCurrentTime,this));
    m_FTE.setLogLevel(nds::logLevel_t::debug);

    timespec timestamp={0,0};
    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
    m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

    //  We have declared all the nodes with several types of PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
    rootNode.setTimestampDelegate(std::bind(&DeviceFTE::getCurrentTime,this));

}

DeviceFTE::~DeviceFTE()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

DeviceFTE* DeviceFTE::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceFTE*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

timespec DeviceFTE::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  FTE
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control FTE state machine
*/
// Called when the FTE node has to be switched on.
void DeviceFTE::switchOn_FTE(){
    // Call API HW to retrieve Maximum -> (ex: Maximum FTE that can be scheduled 20)
    m_FTE.setMaximum(getCurrentTime(),20);
}

// Called when the FTE node has to be switched off.
void DeviceFTE::switchOff_FTE(){
}

// Called when the FTE node has to start working. We start the FTE thread.
void DeviceFTE::start_FTE(){
}

// Stop the FTE node thread
void DeviceFTE::stop_FTE(){
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.

void DeviceFTE::recover_FTE(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.


bool DeviceFTE::allow_FTE_Change(const nds::state_t, const nds::state_t, const nds::state_t){

    return true;
}

/**
* FTE setters
*/
void DeviceFTE::PV_FTE_Set_Writer(const timespec& timestamp, const std::int32_t& value){

    std::int32_t FTESetCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){//Value==1 simulates on FTE set.

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t terminalSet = m_FTE.getTerminalSet();
        std::int32_t modeSet = m_FTE.getModeSet();
        timespec startTimeSet = m_FTE.getStartTimeSet();
        timespec stopTimeSet = m_FTE.getStopTimeSet();
        std::int32_t levelSet = m_FTE.getLevelSet();
        std::int32_t periodNsecSet = m_FTE.getPeriodNsecSet();
        std::int32_t dutyCycleSet = m_FTE.getDutyCycleSet();


        //Just check if there some data in previous PVs
        if(terminalSet>0 && modeSet>0 && levelSet>0 && periodNsecSet>0 && dutyCycleSet>0 && startTimeSet.tv_sec!=0 && stopTimeSet.tv_sec!=0){
            FTESetCode=1;
        }
        else{
            FTESetCode=-1;
        }

        //Fill the Status and Code PVs with some information
        if(FTESetCode==1){
            m_FTE.setSetStatus(timestamp,"OK");
            m_FTE.setSetCode(timestamp,(std::int32_t)FTESetCode);
        }else{
            m_FTE.setSetStatus(timestamp,"WRONG");
            m_FTE.setSetCode(timestamp,(std::int32_t)FTESetCode);
        }
    }else{
        m_FTE.setSetStatus(timestamp,"OK");
        m_FTE.setSetCode(timestamp,(std::int32_t)FTESetCode);
    }

}
void DeviceFTE::PV_FTE_Suppress_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t FTESuppressCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t terminalSuppress = m_FTE.getTerminalSuppress();
        std::int32_t modeSuppress = m_FTE.getModeSuppress();
        timespec startTimeSuppress = m_FTE.getStartTimeSuppress();
        std::int32_t allSuppress = m_FTE.getAllSuppress();

        //Just check if there some data in previous PVs
        if(terminalSuppress>0 && modeSuppress>0 && allSuppress>0 && startTimeSuppress.tv_sec!=0 && startTimeSuppress.tv_nsec!=0){
            FTESuppressCode=1;
        }
        else{
            FTESuppressCode=-1;
        }

        //Fill the Status and Code PVs with some information
        if(FTESuppressCode==1){
            m_FTE.setSuppressStatus(timestamp,"OK");
            m_FTE.setSuppressCode(timestamp,(std::int32_t)FTESuppressCode);
        }else{
            m_FTE.setSuppressStatus(timestamp,"WRONG");
            m_FTE.setSuppressCode(timestamp,(std::int32_t)FTESuppressCode);
        }
    }else{
        m_FTE.setSuppressStatus(timestamp,"OK");
        m_FTE.setSuppressCode(timestamp,(std::int32_t)FTESuppressCode);
    }

}
void DeviceFTE::PV_FTE_ChgPeriod_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t FTEChgPeriodCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t terminalChgPeriod = m_FTE.getTerminalChgPeriod();
        std::int32_t periodChgPeriod = m_FTE.getPeriodChgPeriod();

        //Just check if there some data in previous PVs
        if(terminalChgPeriod>0 && periodChgPeriod>0){
            FTEChgPeriodCode=1;
        }
        else{
            FTEChgPeriodCode=-1;
        }
        //Fill the Status and Code PVs with some information
        if(FTEChgPeriodCode==1){
            m_FTE.setChgPeriodStatus(timestamp,"OK");
            m_FTE.setChgPeriodCode(timestamp,(std::int32_t)FTEChgPeriodCode);
        }else{
            m_FTE.setChgPeriodStatus(timestamp,"WRONG");
            m_FTE.setChgPeriodCode(timestamp,(std::int32_t)FTEChgPeriodCode);
        }
    }else{
        m_FTE.setChgPeriodStatus(timestamp,"OK");
        m_FTE.setChgPeriodCode(timestamp,(std::int32_t)FTEChgPeriodCode);
    }
}

void DeviceFTE::PV_FTE_PendingValue_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t FTEPendingValue=0;
    std::int32_t FTEPendingCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value>=0){ //If some terminal has been chosen
        FTEPendingValue=1; //HWValue of pending FTEs for the terminal in value
        FTEPendingCode=1;
        m_FTE.setPendingValue(timestamp,FTEPendingValue);
        m_FTE.setPendingStatus(timestamp,"OK");
        m_FTE.setPendingCode(timestamp,FTEPendingCode);

    }
    else{
        FTEPendingValue=0; //if terminal<0 not pending values
        FTEPendingCode=-1;
        m_FTE.setPendingValue(timestamp,FTEPendingValue);
        m_FTE.setPendingStatus(timestamp,"WRONG");
        m_FTE.setPendingCode(timestamp,FTEPendingCode);
    }
}

void DeviceFTE::PV_FTE_Set_Initializer(timespec* timestamp, int32_t* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = 0;  //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceFTE::PV_FTE_Suppress_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 1;  //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceFTE::PV_FTE_ChgPeriod_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 30};
    *value = 0;  //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceFTE::PV_FTE_PendingValue_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 40};
    *value = 0;  //Note that this value has no sense and it is fixed only for testing purposes.
}

// NDS_DEFINE_DRIVER(DeviceFTE, DeviceFTE)
/**
 * Allocation function
 *********************/
void* DeviceFTE::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
{
    return new DeviceFTE(factory, deviceName, parameters);
}

/**
 * Deallocation function
 ***********************/
void DeviceFTE::deallocateDevice(void* deviceName)
{
    delete (DeviceFTE*)deviceName;
}
