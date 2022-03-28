#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <functional>

#include <nds3/nds.h>

#include "DeviceStateMachine.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */


static std::map<std::string, DeviceStateMachine*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DeviceStateMachine::DeviceStateMachine(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t & parameters):
                        m_Name(DeviceName),
                        m_bStop_StateMachine(true)
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
    m_setCurrentTime.setDescription("Set timestamp (in seconds)");
    // For testing purposes the current time is set to a constant bigger
    // than 1 of January of 1990 which is the EPICS epoch.
    timespec timestamp = {0, 0};
    m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

    nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");

    // Add State Machine node
    if (findParam != parameters.end() && findParam->second=="YES") {
        nds::StateMachineArgs_t stateMachineArgs = nds::StateMachineArgs_t(true,
                std::bind(&DeviceStateMachine::switchOn_StateMachine, this),
                std::bind(&DeviceStateMachine::switchOff_StateMachine, this),
                std::bind(&DeviceStateMachine::start_StateMachine, this),
                std::bind(&DeviceStateMachine::stop_StateMachine, this),
                std::bind(&DeviceStateMachine::recover_StateMachine, this),
                std::bind(&DeviceStateMachine::allow_StateMachine_Change, this,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3));
        m_StateMachine = rootNode.addChild(nds::StateMachine(stateMachineArgs));
    } else {
        m_StateMachine = rootNode.addChild(nds::StateMachine(true,
                    std::bind(&DeviceStateMachine::switchOn_StateMachine, this),
                    std::bind(&DeviceStateMachine::switchOff_StateMachine, this),
                    std::bind(&DeviceStateMachine::start_StateMachine, this),
                    std::bind(&DeviceStateMachine::stop_StateMachine, this),
                    std::bind(&DeviceStateMachine::recover_StateMachine, this),
                    std::bind(&DeviceStateMachine::allow_StateMachine_Change, this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          std::placeholders::_3)));
    }

    // We have declared all the nodes and PVs in our Device: now we register
    // them with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);

    rootNode.setTimestampDelegate(std::bind(&DeviceStateMachine::getCurrentTime,this));

    //Stream information for debugging purposes
    rootNode.setLogLevel(nds::logLevel_t::debug);
    rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger:The DeviceStateMachine is created" << std::endl;
    ndsDebugStream(rootNode) << "This is the ndsDebugStream: The DeviceStateMachine named " << rootNode.getFullName() << " is created" << std::endl;


}



DeviceStateMachine::~DeviceStateMachine()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_Name);

}

DeviceStateMachine* DeviceStateMachine::getInstance(const std::string& DeviceName)
{

    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceStateMachine*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }

    return findDevice->second;
}


////////////////////////////////////////////////////////////////////////////////
// TEST STATE MACHINE
////////////////////////////////////////////////////////////////////////////////

/*
 * Methods to control State Machine
 **********************************/

// Called when the StateMachine node has to be switched on.
void DeviceStateMachine::switchOn_StateMachine(){

}

// Called when the StateMachine node has to be switched off.
void DeviceStateMachine::switchOff_StateMachine(){

}

// Called when the StateMachine node has to start working. We start the StateMachine thread.
void DeviceStateMachine::start_StateMachine(){

    m_bStop_StateMachine = false; //< We will set to true to stop the StateMachine thread
    /**
     *   Start the StateMachine thread.
     *   We don't need to check if the thread was already started because the state
     *   machine guarantees that the start handler is called only while the state
     *   is ON.
     */
    m_StateMachine_Thread =
      m_StateMachine.runInThread("SM",
                        std::bind(&DeviceStateMachine::StateMachine_thread_body, this));
}

// Stop the StateMachine node thread
void DeviceStateMachine::stop_StateMachine(){
    m_bStop_StateMachine = true;
    m_StateMachine_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to
// the failure state. For now we don't plan for this and every time the
// state machine wants to recover we throw StateMachineRollBack to force the
// state machine to stay on the failure state.
void DeviceStateMachine::recover_StateMachine(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this
// function the state machine has already verified that the requested state
// transition is legal.
bool DeviceStateMachine::allow_StateMachine_Change(const nds::state_t, const nds::state_t, const nds::state_t){
    return true;
}

void DeviceStateMachine::StateMachine_thread_body(){

    // Run until the state machine stops us
    while(!m_bStop_StateMachine){

        // Rest for a while
        ::usleep(1000000);
    }
}

timespec DeviceStateMachine::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

//NDS_DEFINE_DRIVER(DeviceStateMachine, DeviceStateMachine)

/*
 * Allocation function
 *********************/
void* DeviceStateMachine::allocateDevice(nds::Factory& factory,
                     const std::string& DeviceName,
                     const nds::namedParameters_t& parameters) {

  return new DeviceStateMachine(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceStateMachine::deallocateDevice(void* DeviceName) {

  delete (DeviceStateMachine*)DeviceName;
}
