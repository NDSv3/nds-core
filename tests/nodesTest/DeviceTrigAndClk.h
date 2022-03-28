#ifndef DeviceTrigAndClk_H_
#define DeviceTrigAndClk_H_

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
class DeviceTrigAndClk
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceTrigAndClk(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceTrigAndClk();

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
    static DeviceTrigAndClk* getInstance(const std::string& deviceName);


private:


    nds::Routing<std::string> routingNode;

    /**
     * @brief name of the device
     */
    std::string m_name;

    timespec timestamp_device, readtimeStamp;

    /**
     * @brief Routing node
     */
    nds::TriggerAndClk<std::vector<timespec>> m_TriggerAndClk;

    /**
     * Methods to control TrigAndClk state machine
     */
    void switchOn_TrigAndClk();  ///< Called to switch on the Routing node.
    void switchOff_TrigAndClk(); ///< Called to switch off the Routing node.
    void start_TrigAndClk();     ///< Called to start the Routing node.
    void stop_TrigAndClk();      ///< Called to stop the Routing node.
    void recover_TrigAndClk();   ///< Called to recover the Routing node from a failure.
    bool allow_TrigAndClk_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

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
     * TriggerAndClk setters
     */
    void PV_SetSW_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_LoadTrigConf_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_ResetTrigConf_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_PLLSyncSET_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_EnableDisablePLL_Writer(const timespec& timestamp, const std::int32_t& value);

        /**
         * TriggerAndClk initializers
         */
        void PV_SetSW_Initializer(timespec* timestamp, std::int32_t* value);
        void PV_LoadTrigConf_Initializer(timespec* timestamp, std::int32_t* value);
        void PV_ResetTrigConf_Initializer(timespec* timestamp, std::int32_t* value);
        void PV_PLLSyncSET_Initializer(timespec* timestamp, std::int32_t* value);
        void PV_EnableDisablePLL_Initializer(timespec* timestamp, std::int32_t* value);

    /**
     * Routing setters
     */
    void PV_Routing_ClkSet_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_Routing_ClkDstRead_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_Routing_TermSet_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_Routing_TermDstRead_Writer(const timespec& timestamp, const std::int32_t& value);

};

#endif // DeviceTrigAndClk_H_
