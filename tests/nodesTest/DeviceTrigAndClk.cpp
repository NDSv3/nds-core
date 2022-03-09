#include <mutex>
#include <functional>

#include <nds3/nds.h>

#include "DeviceTrigAndClk.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceTrigAndClk*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceTrigAndClk::DeviceTrigAndClk(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t & parameters):
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

    routingNode = nds::Routing<std::string>(
            "Route",
            std::bind(&DeviceTrigAndClk::switchOn_Routing, this),
            std::bind(&DeviceTrigAndClk::switchOff_Routing, this),
            std::bind(&DeviceTrigAndClk::start_Routing, this),
            std::bind(&DeviceTrigAndClk::stop_Routing, this),
            std::bind(&DeviceTrigAndClk::recover_Routing, this),
            std::bind(&DeviceTrigAndClk::allow_Routing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&DeviceTrigAndClk::PV_Routing_ClkSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_Routing_ClkDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_Routing_TermSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_Routing_TermDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2
    ));

    /**
      * Add a Routing node:
      */
    if (findParam != parameters.end() && findParam->second=="YES") {
        //Set compulsory methods
        nds::TriggerAndClkArgs_t handlerTrig = nds::TriggerAndClkArgs_t(
            std::bind(&DeviceTrigAndClk::switchOn_Routing, this),
            std::bind(&DeviceTrigAndClk::switchOff_Routing, this),
            std::bind(&DeviceTrigAndClk::start_Routing, this),
            std::bind(&DeviceTrigAndClk::stop_Routing, this),
            std::bind(&DeviceTrigAndClk::recover_Routing, this),
            std::bind(&DeviceTrigAndClk::allow_Routing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&DeviceTrigAndClk::PV_SetSW_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_LoadTrigConf_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_ResetTrigConf_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_PLLSyncSET_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_EnableDisablePLL_Writer,this, std::placeholders::_1, std::placeholders::_2),
            routingNode);
        //Set optional methods
        handlerTrig.PV_SetSW_Initializer = std::bind(&DeviceTrigAndClk::PV_SetSW_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerTrig.PV_LoadTrigConf_Initializer = std::bind(&DeviceTrigAndClk::PV_LoadTrigConf_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerTrig.PV_ResetTrigConf_Initializer = std::bind(&DeviceTrigAndClk::PV_ResetTrigConf_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerTrig.PV_PLLSyncSet_Initializer = std::bind(&DeviceTrigAndClk::PV_PLLSyncSET_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerTrig.PV_EnableDisablePLL_Initializer = std::bind(&DeviceTrigAndClk::PV_EnableDisablePLL_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        //Set init values: Note that these value have no actual sense and they are fixed only for testing purposes.
        handlerTrig.m_HWBlock_Init = 0;
        handlerTrig.m_DAQStartTimeDelay_Init = 0;
        handlerTrig.m_TriggPeriod_Init = 0;
        handlerTrig.m_Level_Init = 0;
        handlerTrig.m_Edge_Init = 0;
        handlerTrig.m_Change_Init = 0;
        handlerTrig.m_Mode_Init = 0;
        handlerTrig.m_ClkDivider_Init = 0;
        handlerTrig.m_PreTrigSamples_Init = 1;
        handlerTrig.m_PostTrigSamples_Init = 1;
        handlerTrig.m_SyncMode_Init = 0;
        handlerTrig.m_PLLRefFreq_Init = 0;
        handlerTrig.m_PLLRefDiv_Init = 0;
        handlerTrig.m_PLLRefMult_Init = 0;
        handlerTrig.m_PLLRefDivAll_Init = 1;

      m_TriggerAndClk = rootNode.addChild(nds::TriggerAndClk<std::vector<timespec>>("TrigAndClk",handlerTrig));


    } else {
        m_TriggerAndClk = rootNode.addChild(nds::TriggerAndClk<std::vector<timespec>>(
            "TrigAndClk",
            std::bind(&DeviceTrigAndClk::switchOn_Routing, this),
            std::bind(&DeviceTrigAndClk::switchOff_Routing, this),
            std::bind(&DeviceTrigAndClk::start_Routing, this),
            std::bind(&DeviceTrigAndClk::stop_Routing, this),
            std::bind(&DeviceTrigAndClk::recover_Routing, this),
            std::bind(&DeviceTrigAndClk::allow_Routing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&DeviceTrigAndClk::PV_SetSW_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_LoadTrigConf_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_ResetTrigConf_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_PLLSyncSET_Writer,this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTrigAndClk::PV_EnableDisablePLL_Writer,this, std::placeholders::_1, std::placeholders::_2),
            routingNode
        ));
    }

    m_TriggerAndClk.setStartTimestampDelegate(std::bind(&DeviceTrigAndClk::getCurrentTime,this));
    m_TriggerAndClk.setLogLevel(nds::logLevel_t::debug);

    timespec timestamp={0,0};
    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
    m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

    //  We have declared all the nodes with several types of PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
    rootNode.setTimestampDelegate(std::bind(&DeviceTrigAndClk::getCurrentTime,this));

}

DeviceTrigAndClk::~DeviceTrigAndClk()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

DeviceTrigAndClk* DeviceTrigAndClk::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceTrigAndClk*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

timespec DeviceTrigAndClk::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Trig & Clk
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control TrigAndClk state machine
*/
// Called when the TrigAndClk node has to be switched on.
void DeviceTrigAndClk::switchOn_TrigAndClk(){

}

// Called when the TrigAndClk node has to be switched off.
void DeviceTrigAndClk::switchOff_TrigAndClk(){

}

// Called when the TrigAndClk node has to start working. We start the FTE thread.
void DeviceTrigAndClk::start_TrigAndClk(){

}

// Stop the TrigAndClk node thread
void DeviceTrigAndClk::stop_TrigAndClk(){

}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.

void DeviceTrigAndClk::recover_TrigAndClk(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.

bool DeviceTrigAndClk::allow_TrigAndClk_Change(const nds::state_t, const nds::state_t, const nds::state_t){

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Routing
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control Routing state machine
*/
// Called when the Routing node has to be switched on.
void DeviceTrigAndClk::switchOn_Routing(){

}

// Called when the Routing node has to be switched off.
void DeviceTrigAndClk::switchOff_Routing(){

}

// Called when the Routing node has to start working.
void DeviceTrigAndClk::start_Routing(){

}

// Stop the Routing node thread
void DeviceTrigAndClk::stop_Routing(){

}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.

void DeviceTrigAndClk::recover_Routing(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.

bool DeviceTrigAndClk::allow_Routing_Change(const nds::state_t, const nds::state_t, const nds::state_t){

    return true;
}


/**
 * Trig & Clk setters
 */
void DeviceTrigAndClk::PV_SetSW_Writer(const timespec& timestamp, const std::int32_t& value){

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value>0){//Value==1 simulates a software trigger set, value==0 ignores the action.
        std::int32_t hwBlock = m_TriggerAndClk.getHWBlock();
        m_TriggerAndClk.setHWBlockRBV(timestamp, hwBlock);
    }

}

void DeviceTrigAndClk::PV_LoadTrigConf_Writer(const timespec& timestamp, const std::int32_t& value){

    if(value>0){

        int32_t daqStartTimeDelay = 0;
        int32_t trigPeriod = 0;
        int32_t level = 0;
        int32_t edge = 0;
        int32_t change = 0;
        int32_t mode = 0;
        int32_t ClkDivider = 0;
        int32_t postTrigSamples = 0;
        int32_t preTrigSamples = 0;

        //Getting all the PV
        daqStartTimeDelay = m_TriggerAndClk.getDAQStartTimeDelay();
        trigPeriod = m_TriggerAndClk.getTriggPeriod();
        level = m_TriggerAndClk.getLevel();
        edge = m_TriggerAndClk.getEdge();
        change = m_TriggerAndClk.getChange();
        mode = m_TriggerAndClk.getMode();
        ClkDivider = m_TriggerAndClk.getClkDivider();
        postTrigSamples = m_TriggerAndClk.getPostTrigSamples();
        preTrigSamples = m_TriggerAndClk.getPreTrigSamples();

        switch(mode){

        case 0: //This mode simulates the FINITE configuration

            postTrigSamples = m_TriggerAndClk.getPostTrigSamples();

            m_TriggerAndClk.setDAQStartTimeDelayRBV(timestamp, daqStartTimeDelay);  //RBV PV setting
            m_TriggerAndClk.setTriggPeriodRBV(timestamp, trigPeriod);               //RBV PV setting
            m_TriggerAndClk.setLevelRBV(timestamp, level);                          //RBV PV setting
            m_TriggerAndClk.setEdgeRBV(timestamp, edge);                            //RBV PV setting
            m_TriggerAndClk.setChangeRBV(timestamp, change);                        //RBV PV setting
            m_TriggerAndClk.setModeRBV(timestamp, mode);                            //RBV PV setting
            m_TriggerAndClk.setClkDividerRBV(timestamp, ClkDivider);                    //RBV PV setting
            m_TriggerAndClk.setPostTrigSamplesRBV(timestamp, postTrigSamples);      //RBV PV setting

            m_TriggerAndClk.setTrigLoadStatus(timestamp,"OK");
            m_TriggerAndClk.setTrigLoadCode(timestamp, (std::int32_t)1);

            break;

        case 1: //This mode simulates the RETRIGERABLE configuration

            postTrigSamples = m_TriggerAndClk.getPostTrigSamples();
            preTrigSamples = m_TriggerAndClk.getPreTrigSamples();

            m_TriggerAndClk.setDAQStartTimeDelayRBV(timestamp, daqStartTimeDelay);  //RBV PV setting
            m_TriggerAndClk.setTriggPeriodRBV(timestamp, trigPeriod);               //RBV PV setting
            m_TriggerAndClk.setLevelRBV(timestamp, level);                          //RBV PV setting
            m_TriggerAndClk.setEdgeRBV(timestamp, edge);                            //RBV PV setting
            m_TriggerAndClk.setChangeRBV(timestamp, change);                        //RBV PV setting
            m_TriggerAndClk.setModeRBV(timestamp, mode);
            m_TriggerAndClk.setClkDividerRBV(timestamp, ClkDivider);  //RBV PV setting
            m_TriggerAndClk.setPreTrigSamplesRBV(timestamp, preTrigSamples);        //RBV PV setting
            m_TriggerAndClk.setPostTrigSamplesRBV(timestamp, postTrigSamples);      //RBV PV setting

            m_TriggerAndClk.setTrigLoadStatus(timestamp,"OK");
            m_TriggerAndClk.setTrigLoadCode(timestamp, (std::int32_t)1);

            break;

        default: //Wrong mode configuration

            m_TriggerAndClk.setDAQStartTimeDelayRBV(timestamp, 0);  //RBV PV setting
            m_TriggerAndClk.setTriggPeriodRBV(timestamp, 0);        //RBV PV setting
            m_TriggerAndClk.setLevelRBV(timestamp, 0);              //RBV PV setting
            m_TriggerAndClk.setEdgeRBV(timestamp, 0);               //RBV PV setting
            m_TriggerAndClk.setChangeRBV(timestamp, 0);             //RBV PV setting
            m_TriggerAndClk.setPostTrigSamplesRBV(timestamp, 0);    //RBV PV setting
            m_TriggerAndClk.setPreTrigSamplesRBV(timestamp, 0);     //RBV PV setting

            m_TriggerAndClk.setModeRBV(timestamp, mode);
            m_TriggerAndClk.setClkDividerRBV(timestamp, 0);
            m_TriggerAndClk.setTrigLoadStatus(timestamp,"WRONG");
            m_TriggerAndClk.setTrigLoadCode(timestamp, (std::int32_t)0);
            break;
        }

    }else{
        m_TriggerAndClk.setPostTrigSamplesRBV(timestamp, 0);    //RBV PV setting
        m_TriggerAndClk.setDAQStartTimeDelayRBV(timestamp, 0);  //RBV PV setting
        m_TriggerAndClk.setTriggPeriodRBV(timestamp, 0);        //RBV PV setting
        m_TriggerAndClk.setLevelRBV(timestamp, 0);              //RBV PV setting
        m_TriggerAndClk.setEdgeRBV(timestamp, 0);               //RBV PV setting
        m_TriggerAndClk.setChangeRBV(timestamp, 0);             //RBV PV setting
        m_TriggerAndClk.setModeRBV(timestamp, 0);               //RBV PV setting
        m_TriggerAndClk.setClkDividerRBV(timestamp, 0);
        m_TriggerAndClk.setPreTrigSamplesRBV(timestamp, 0);     //RBV PV setting

        m_TriggerAndClk.setTrigLoadStatus(timestamp,"OK");
        m_TriggerAndClk.setTrigLoadCode(timestamp, (std::int32_t)0);
    }


}

void DeviceTrigAndClk::PV_ResetTrigConf_Writer(const timespec& timestamp, const std::int32_t& value){

    std::int32_t resetCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value>0){//Value==1 simulates a reset in the hardware configuration.
        resetCode=1;
        m_TriggerAndClk.setTrigLoadStatus(timestamp,"OK");
        m_TriggerAndClk.setTrigLoadCode(timestamp, (std::int32_t)resetCode);

    }else{ //Value<=0 simulates no reset in the hardware configuration, but a PV call.
        m_TriggerAndClk.setTrigLoadStatus(timestamp,"OK");
        m_TriggerAndClk.setTrigLoadCode(timestamp, (std::int32_t)resetCode);
    }

}

void DeviceTrigAndClk::PV_PLLSyncSET_Writer(const timespec& timestamp, const std::int32_t& value){

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value>0){//Value==1 simulates the PLL configuration.

        std::int32_t syncMode = m_TriggerAndClk.getSyncMode();
        std::int32_t PLLRefFreq = m_TriggerAndClk.getPLLRefFreq();
        std::int32_t PLLRefDiv = m_TriggerAndClk.getPLLRefDiv();
        std::int32_t PLLRefMult = m_TriggerAndClk.getPLLRefMult();
        std::int32_t PLLRefDivAll = m_TriggerAndClk.getPLLRefDivALL();

        m_TriggerAndClk.setSyncModeRBV(timestamp, syncMode);
        m_TriggerAndClk.setPLLRefFreqRBV(timestamp, PLLRefFreq);
        m_TriggerAndClk.setPLLRefDivRBV(timestamp, PLLRefDiv);
        m_TriggerAndClk.setPLLRefMultRBV(timestamp, PLLRefMult);
        m_TriggerAndClk.setPLLRefDivALLRBV(timestamp, PLLRefDivAll);

        m_TriggerAndClk.setPLLLoadStatus(timestamp, "OK");
        m_TriggerAndClk.setPLLLoadCode(timestamp, 1);

    }else{//Value<=0 simulates no PLL configuration, but a PV call.
        m_TriggerAndClk.setSyncModeRBV(timestamp, 0);
        m_TriggerAndClk.setPLLRefFreqRBV(timestamp, 0);
        m_TriggerAndClk.setPLLRefDivRBV(timestamp, 0);
        m_TriggerAndClk.setPLLRefMultRBV(timestamp, 0);
        m_TriggerAndClk.setPLLRefDivALLRBV(timestamp, 0);

        m_TriggerAndClk.setPLLLoadStatus(timestamp, "OK");
        m_TriggerAndClk.setPLLLoadCode(timestamp, 0);
    }


}

void DeviceTrigAndClk::PV_EnableDisablePLL_Writer(const timespec& timestamp, const std::int32_t& value){

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value>0){//Value > 0 simulates enabling the PLL .
        m_TriggerAndClk.setEnableDisablePLLRBV(timestamp, 1);
    }else{//Value <= 0 simulates PLL disabling
        m_TriggerAndClk.setEnableDisablePLLRBV(timestamp, 0);
    }
}



/**
 * Routing node setters
 */
void DeviceTrigAndClk::PV_Routing_ClkSet_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t RoutingSetCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){//Value==1 simulates a clock routing set.

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t clockSrc = m_TriggerAndClk.m_Routing.getClkSrc();
        std::int32_t clockDst = m_TriggerAndClk.m_Routing.getClkDst();

        //Just check PVs have been written
        if(clockSrc!=0 && clockDst!=0){
            RoutingSetCode=1;
        }
        else{
            RoutingSetCode=-1;
        }

        //Fill the Status and Code PVs with some information
        if(RoutingSetCode==1){
            m_TriggerAndClk.m_Routing.setClkSetStatus(timestamp,"OK");
            m_TriggerAndClk.m_Routing.setClkSetCode(timestamp,(std::int32_t)RoutingSetCode);

        }else{
            m_TriggerAndClk.m_Routing.setClkSetStatus(timestamp,"WRONG");
            m_TriggerAndClk.m_Routing.setClkSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }
    }else{
        m_TriggerAndClk.m_Routing.setClkSetStatus(timestamp,"OK");
        m_TriggerAndClk.m_Routing.setClkSetCode(timestamp,(std::int32_t)RoutingSetCode);

    }

}

void DeviceTrigAndClk::PV_Routing_ClkDstRead_Writer(const timespec& timestamp, const std::int32_t& value){

    //This code has been developed just for testing purposes. Should be replaced with HW API.

    //Fill the readable PV with the connection information for this destination clock
    // As an example any given ClkDst is connected to ClkDst+1 as source
    m_TriggerAndClk.m_Routing.setClkSrcRead(timestamp,value + 1);
}

void DeviceTrigAndClk::PV_Routing_TermSet_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t RoutingSetCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){//Value==1 simulates a terminal routing set.

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t terminalSrc = m_TriggerAndClk.m_Routing.getTermSrc();
        std::int32_t terminalDst = m_TriggerAndClk.m_Routing.getTermDst();
        std::int32_t terminalSyncSet = m_TriggerAndClk.m_Routing.getTermSyncSet();
        std::int32_t terminalInvertSet = m_TriggerAndClk.m_Routing.getTermInvertSet();

        //Just check PVs have been written
        if(terminalSrc!=0 && terminalDst!=0 && terminalSyncSet!=0 && terminalInvertSet!=0){
            RoutingSetCode=1;
        }
        else{
            RoutingSetCode=-1;
        }

        //Fill the Status and Code PVs with some information
        if(RoutingSetCode==1){
            m_TriggerAndClk.m_Routing.setTermSetStatus(timestamp,"OK");
            m_TriggerAndClk.m_Routing.setTermSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }else{
            m_TriggerAndClk.m_Routing.setTermSetStatus(timestamp,"WRONG");
            m_TriggerAndClk.m_Routing.setTermSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }
    }else{
        m_TriggerAndClk.m_Routing.setTermSetStatus(timestamp,"OK");
        m_TriggerAndClk.m_Routing.setTermSetCode(timestamp,(std::int32_t)RoutingSetCode);
    }

}

void DeviceTrigAndClk::PV_Routing_TermDstRead_Writer(const timespec& timestamp, const std::int32_t& value){
    //This code has been developed just for testing purposes. Should be replaced with HW API.

    //Fill the readable PV with the connection information for this destination terminal
    // As an example any given TermDst is connected to TermDst+1 as source
    // and set Sync and Invert PVs with 0 when Dst is 0 and with 1 in any other case
    m_TriggerAndClk.m_Routing.setTermSrcRead(timestamp,value + 1);

    if (value == 0){
        m_TriggerAndClk.m_Routing.setTermSyncRead(timestamp,0);
        m_TriggerAndClk.m_Routing.setTermInvertRead(timestamp,0);
    } else {
        m_TriggerAndClk.m_Routing.setTermSyncRead(timestamp,1);
        m_TriggerAndClk.m_Routing.setTermInvertRead(timestamp,1);
    }
}



void
DeviceTrigAndClk::PV_SetSW_Initializer (timespec* timestamp,
                                        std::int32_t* value)
{
  *timestamp = m_TriggerAndClk.getTimestamp();
  *value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void
DeviceTrigAndClk::PV_LoadTrigConf_Initializer (timespec* timestamp,
                                               std::int32_t* value)
{
  *timestamp = m_TriggerAndClk.getTimestamp();
  *value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void
DeviceTrigAndClk::PV_ResetTrigConf_Initializer (timespec* timestamp,
                                                std::int32_t* value)
{
  *timestamp = m_TriggerAndClk.getTimestamp();
  *value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void
DeviceTrigAndClk::PV_PLLSyncSET_Initializer (timespec* timestamp,
                                             std::int32_t* value)
{
  *timestamp = m_TriggerAndClk.getTimestamp();
  *value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void
DeviceTrigAndClk::PV_EnableDisablePLL_Initializer (timespec* timestamp,
                                                   std::int32_t* value)
{
  *timestamp = m_TriggerAndClk.getTimestamp();
  *value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

// NDS_DEFINE_DRIVER(DeviceTrigAndClk, DeviceTrigAndClk)

 /**
 * Allocation function
 *********************/
void* DeviceTrigAndClk::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
{
    return new DeviceTrigAndClk(factory, deviceName, parameters);
}

/**
 * Deallocation function
 ***********************/
void DeviceTrigAndClk::deallocateDevice(void* deviceName)
{
    delete (DeviceTrigAndClk*)deviceName;
}

