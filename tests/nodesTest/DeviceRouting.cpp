#include <mutex>
#include <functional>

#include <nds3/nds.h>

#include "DeviceRouting.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceRouting*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceRouting::DeviceRouting(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t & parameters):
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

    /**
      * Add a Routing node:
      */

    nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");
    if (findParam != parameters.end() && findParam->second=="YES") {
        //Set compulsory methods
        nds::RoutingArgs_t handlerRTN = nds::RoutingArgs_t(
                std::bind(&DeviceRouting::switchOn_Routing, this),
                std::bind(&DeviceRouting::switchOff_Routing, this),
                std::bind(&DeviceRouting::start_Routing, this),
                std::bind(&DeviceRouting::stop_Routing, this),
                std::bind(&DeviceRouting::recover_Routing, this),
                std::bind(&DeviceRouting::allow_Routing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceRouting::PV_Routing_ClkSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceRouting::PV_Routing_ClkDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceRouting::PV_Routing_TermSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceRouting::PV_Routing_TermDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2));
        //Set optional methods
        handlerRTN.PV_ClkSet_Initializer = std::bind(&DeviceRouting::PV_Routing_ClkSet_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerRTN.PV_ClkDstRead_Initializer = std::bind(&DeviceRouting::PV_Routing_ClkDstRead_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerRTN.PV_TermSet_Initializer = std::bind(&DeviceRouting::PV_Routing_TermSet_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        handlerRTN.PV_TermDstRead_Initializer = std::bind(&DeviceRouting::PV_Routing_TermDstRead_Initializer,this, std::placeholders::_1, std::placeholders::_2);
        //Set init values: Note that these value have no actual sense and they are fixed only for testing purposes.
        handlerRTN.m_ClkSrc_Init = -1;
        handlerRTN.m_ClkDst_Init = -2;
        handlerRTN.m_TermSrc_Init = -3;
        handlerRTN.m_TermDst_Init = -4;
        handlerRTN.m_TermSyncSet_Init = 0;
        handlerRTN.m_TermInvertSet_Init = 0;
        //Add the routing node
        m_Routing = rootNode.addChild(nds::Routing<std::string>(
                "RoutingNode", handlerRTN));
    } else {
        m_Routing = rootNode.addChild(nds::Routing<std::string>(
                "RoutingNode",
                std::bind(&DeviceRouting::switchOn_Routing, this),
                std::bind(&DeviceRouting::switchOff_Routing, this),
                std::bind(&DeviceRouting::start_Routing, this),
                std::bind(&DeviceRouting::stop_Routing, this),
                std::bind(&DeviceRouting::recover_Routing, this),
                std::bind(&DeviceRouting::allow_Routing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceRouting::PV_Routing_ClkSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceRouting::PV_Routing_ClkDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceRouting::PV_Routing_TermSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceRouting::PV_Routing_TermDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2)
        ));
    }

    m_Routing.setStartTimestampDelegate(std::bind(&DeviceRouting::getCurrentTime,this));
    m_Routing.setLogLevel(nds::logLevel_t::debug);

    timespec timestamp={0,0};
    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
    m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

    //  We have declared all the nodes with several types of PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
    rootNode.setTimestampDelegate(std::bind(&DeviceRouting::getCurrentTime,this));

}

DeviceRouting::~DeviceRouting()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

DeviceRouting* DeviceRouting::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceRouting*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

timespec DeviceRouting::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Routing
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control Routing state machine
*/
// Called when the Routing node has to be switched on.
void DeviceRouting::switchOn_Routing(){

}

// Called when the Routing node has to be switched off.
void DeviceRouting::switchOff_Routing(){

}

// Called when the Routing node has to start working. We start the FTE thread.
void DeviceRouting::start_Routing(){

}

// Stop the Routing node thread
void DeviceRouting::stop_Routing(){

}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.

void DeviceRouting::recover_Routing(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.

bool DeviceRouting::allow_Routing_Change(const nds::state_t, const nds::state_t, const nds::state_t){

    return true;
}



/**
 * Routing setters
 */
void DeviceRouting::PV_Routing_ClkSet_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t RoutingSetCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){//Value==1 simulates a clock routing set.

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t clockSrc = m_Routing.getClkSrc();
        std::int32_t clockDst = m_Routing.getClkDst();

        //Just check PVs have been written
        if(clockSrc!=0 && clockDst!=0){
            RoutingSetCode=1;
        }
        else{
            RoutingSetCode=-1;
        }

        //Fill the Status and Code PVs with some information
        if(RoutingSetCode==1){
            m_Routing.setClkSetStatus(timestamp,"OK");
            m_Routing.setClkSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }else{
            m_Routing.setClkSetStatus(timestamp,"WRONG");
            m_Routing.setClkSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }
    }else{
        m_Routing.setClkSetStatus(timestamp,"OK");
        m_Routing.setClkSetCode(timestamp,(std::int32_t)RoutingSetCode);
    }

}

void DeviceRouting::PV_Routing_ClkDstRead_Writer(const timespec& timestamp, const std::int32_t& value){

    //This code has been developed just for testing purposes. Should be replaced with HW API.

    //Fill the readable PV with the connection information for this destination clock
    // As an example any given ClkDst is connected to ClkDst+1 as source
    m_Routing.setClkSrcRead(timestamp,value + 1);
}

void DeviceRouting::PV_Routing_TermSet_Writer(const timespec& timestamp, const std::int32_t& value){
    std::int32_t RoutingSetCode=0;

    //This code has been developed just for testing purposes. Should be replaced with HW API.
    if(value==1){//Value==1 simulates a terminal routing set.

        //Retrieve the data from the PVs that have been previously configured
        std::int32_t terminalSrc = m_Routing.getTermSrc();
        std::int32_t terminalDst = m_Routing.getTermDst();
        std::int32_t terminalSyncSet = m_Routing.getTermSyncSet();
        std::int32_t terminalInvertSet = m_Routing.getTermInvertSet();

        //Just check PVs have been written
        if(terminalSrc!=0 && terminalDst!=0 && terminalSyncSet!=0 && terminalInvertSet!=0){
            RoutingSetCode=1;
        }
        else{
            RoutingSetCode=-1;
        }

        //Fill the Status and Code PVs with some information
        if(RoutingSetCode==1){
            m_Routing.setTermSetStatus(timestamp,"OK");
            m_Routing.setTermSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }else{
            m_Routing.setTermSetStatus(timestamp,"WRONG");
            m_Routing.setTermSetCode(timestamp,(std::int32_t)RoutingSetCode);
        }
    }else{
        m_Routing.setTermSetStatus(timestamp,"OK");
        m_Routing.setTermSetCode(timestamp,(std::int32_t)RoutingSetCode);
    }

}

void DeviceRouting::PV_Routing_TermDstRead_Writer(const timespec& timestamp, const std::int32_t& value){
    //This code has been developed just for testing purposes. Should be replaced with HW API.

    //Fill the readable PV with the connection information for this destination terminal
    // As an example any given TermDst is connected to TermDst+1 as source
    // and set Sync and Invert PVs with 0 when Dst is 0 and with 1 in any other case
    m_Routing.setTermSrcRead(timestamp,value + 1);

    if (value == 0){
        m_Routing.setTermSyncRead(timestamp,0);
        m_Routing.setTermInvertRead(timestamp,0);
    } else {
        m_Routing.setTermSyncRead(timestamp,1);
        m_Routing.setTermInvertRead(timestamp,1);
    }
}


void DeviceRouting::PV_Routing_ClkSet_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = 0;  //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceRouting::PV_Routing_ClkDstRead_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 0;  //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceRouting::PV_Routing_TermSet_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 30};
    *value = 0;  //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceRouting::PV_Routing_TermDstRead_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 40};
    *value = 3;  //Note that this value has no sense and it is fixed only for testing purposes.
}


// NDS_DEFINE_DRIVER(DeviceRouting, DeviceRouting)
/**
 * Allocation function
 *********************/
void* DeviceRouting::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
{
    return new DeviceRouting(factory, deviceName, parameters);
}

/**
 * Deallocation function
 ***********************/
void DeviceRouting::deallocateDevice(void* deviceName)
{
    delete (DeviceRouting*)deviceName;
}


