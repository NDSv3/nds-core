#ifndef DEVICE_HQMON_H_
#define DEVICE_HQMON_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a HealthMonitor node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceHQMonitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceHQMonitor(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceHQMonitor();

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
    static DeviceHQMonitor* getInstance(const std::string& deviceName);


private:

    /*
     * @brief name of the device
     */
    std::string m_Name;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // TEST HEALTH MONITOR NODE
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief HealthMonitor node
     */
    nds::HQMonitor m_HQMonitor;

    /**
     * Methods to control the HQMonitor state machine
     */
    void switchOn_HQMonitor();  ///< Called to switch on the HQMonitor node.
    void switchOff_HQMonitor(); ///< Called to switch off the HQMonitor node.
    void start_HQMonitor();     ///< Called to start the HQMonitor node.
    void stop_HQMonitor();      ///< Called to stop the HQMonitor node.
    void recover_HQMonitor();   ///< Called to recover the HQMonitor node from a failure.

    bool allow_HQMonitor_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * HealthMonitor getters
     */
    void PV_HQMonitor_Power_Reader(timespec* timestamp, double* value);
    void PV_HQMonitor_Temperature_Reader(timespec* timestamp, double* value);
    void PV_HQMonitor_Voltage_Reader(timespec* timestamp, double* value);
    void PV_HQMonitor_Current_Reader(timespec* timestamp, double* value);
    void PV_HQMonitor_SelfTestTxt_Reader(timespec* timestamp, std::string* value);
    void PV_HQMonitor_SignalQualityFlag_Reader(timespec* timestamp, std::int32_t* value);

    /**
     * HealthMonitor setters
     */
    void PV_HQMonitor_SEUEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_DAQEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SelfTestEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SelfTestType_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SelfTestVerboseEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SelfTestIDEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SelfTestTxtEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SelfTestCodeResultEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_HQMonitor_SignalQualityFlagLevel_Writer(const timespec& timestamp, const double& value);

    /**
     * Health monitor initializers
     */
    void PV_HQMonitor_SEUEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_DAQEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SelfTestEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SelfTestType_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SelfTestVerboseEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SelfTestIDEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SelfTestTxtEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SelfTestCodeResultEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_HQMonitor_SignalQualityFlagLevel_Initializer(timespec* timestamp, double* value);

    /**
     * @brief Function that emulates the changes of status of the HQMonitor node.
     *        It is launched by start_HQMonitor() in a separate thread.
     */
    void HQMonitor_thread_body();

    /**
     * @brief A thread that runs HQMonitor_thread_body().
     */
    nds::Thread m_HQMonitor_Thread;

    /**
     * @brief A boolean flag that stop the HQMonitor loop in HQMonitor_thread_body()
     *        when true.
     */
    volatile bool m_bStop_HQMonitor;


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

#endif // DEVICE_HQMON_H_
