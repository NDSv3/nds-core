#ifndef DEVICEROUTING_H_
#define DEVICEROUTING_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Routing node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceRouting
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceRouting(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceRouting();

    /**
     * Allocation/deallocation
     *
     *******************************************************/
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    static void deallocateDevice(void* deviceName);

    /**
     * For test purposes we make it possible to retrieve running instances of
     *  the device
     */
    static DeviceRouting* getInstance(const std::string& deviceName);


private:

    /**
     * @brief name of the device
     */
    std::string m_name;

    timespec timestamp_device, readtimeStamp;

    /**
     * @brief Routing node
     */
    nds::Routing<std::string> m_Routing;

    /**
     * Methods to control Routing state machine
     */
    void switchOn_Routing();  ///< Called to switch on the Routing node.
    void switchOff_Routing(); ///< Called to switch off the Routing node.
    void start_Routing();     ///< Called to start the Routing node.
    void stop_Routing();      ///< Called to stop the Routing node.
    void recover_Routing();   ///< Called to recover the Routing node from a failure.
    bool allow_Routing_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    nds::PVVariableOut<std::int32_t> m_setCurrentTime;
    timespec getCurrentTime();

    /**
     * Routing setters
     */
    void PV_Routing_ClkSet_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_Routing_ClkDstRead_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_Routing_TermSet_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_Routing_TermDstRead_Writer(const timespec& timestamp, const std::int32_t& value);

    /**
     * Routing initializers
     */
    void PV_Routing_ClkSet_Initializer(timespec* timestamp, int32_t* value);
    void PV_Routing_ClkDstRead_Initializer(timespec* timestamp, int32_t* value);
    void PV_Routing_TermSet_Initializer(timespec* timestamp, int32_t* value);
    void PV_Routing_TermDstRead_Initializer(timespec* timestamp, int32_t* value);

};

#endif // DeviceRouting_H_
