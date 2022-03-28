#ifndef DEVICE_STATEMACHINE_H_
#define DEVICE_STATEMACHINE_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a StateMachine node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceStateMachine
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceStateMachine(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceStateMachine();

    /*
     * Allocation/deallocation
     *
     *******************************************************/
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    static void deallocateDevice(void* deviceName);

    /*
     * For test purposes we make it possible to retrieve running instances of
     *  the device
     */
    static DeviceStateMachine* getInstance(const std::string& deviceName);


private:

    /*
     * @brief name of the device
     */
    std::string m_Name;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // TEST STATE MACHINE NODE
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief HealthMonitor node
     */
    nds::StateMachine m_StateMachine;

    /**
     * Methods to control the StateMachine state machine
     */
    void switchOn_StateMachine();  ///< Called to switch on the StateMachine node.
    void switchOff_StateMachine(); ///< Called to switch off the StateMachine node.
    void start_StateMachine();     ///< Called to start the StateMachine node.
    void stop_StateMachine();      ///< Called to stop the StateMachine node.
    void recover_StateMachine();   ///< Called to recover the StateMachine node from a failure.
    bool allow_StateMachine_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * @brief Function that emulates the changes of status of the StateMachine node.
     *        It is launched by start_StateMachine() in a separate thread.
     */
    void StateMachine_thread_body();

    /**
     * @brief A thread that runs StateMachine_thread_body().
     */
    nds::Thread m_StateMachine_Thread;

    /**
     * @brief A boolean flag that stop the StateMachine loop in StateMachine_thread_body()
     *        when true.
     */
    volatile bool m_bStop_StateMachine;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // TIMESTAMP HANDLING
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief PV to set the timestamp of the device, in seconds
     */
    nds::PVVariableOut<std::int32_t> m_setCurrentTime;

    /**
     * @brief Function to get the timestamp of the device
     */
    timespec getCurrentTime();

};

#endif // DEVICE_STATEMACHINE_H_
