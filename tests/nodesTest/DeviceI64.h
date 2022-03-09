#ifndef DEVICEI64_H_
#define DEVICEI64_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Data Acquisition node and a Waveform Generator node
 *          of type int64_t for testing purposes of nds-core V3.
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceI64
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceI64(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceI64();

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
    static DeviceI64* getInstance(const std::string& deviceName);


private:

    /*
     * @brief name of the device
     */
    std::string m_name;

    timespec timestamp_device, readtimeStamp;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TEST DEVICE STATE MACHINE
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DeviceI64 state machine
     */
    nds::StateMachine m_DeviceI64_stateMachine;

    /**
     * Methods to control DeviceI64 state machine
     */
    void switchOn_DeviceI64();  ///< Called to switch on the DeviceI64 (rootnode).
    void switchOff_DeviceI64(); ///< Called to switch off the DeviceI64 (rootnode).
    void start_DeviceI64();     ///< Called to start the DeviceI64 (rootnode).
    void stop_DeviceI64();      ///< Called to stop the DeviceI64 (rootnode).
    void recover_DeviceI64();   ///< Called to recover the DeviceI64 (rootnode) from a failure.

    bool allow_DeviceI64_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    nds::PVVariableOut<std::int32_t> m_setCurrentTime;
    timespec getCurrentTime();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DATA ACQUISITION
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DataAcquisition node
     */
    nds::DataAcquisition<std::int64_t> m_DataAcquisition;

    /**
     * Methods to control DataAcquisition state machine
     */
    void switchOn_DataAcquisition();  ///< Called to switch on the DataAcquisition node.
    void switchOff_DataAcquisition(); ///< Called to switch off the DataAcquisition node.
    void start_DataAcquisition();     ///< Called to start the DataAcquisition node.
    void stop_DataAcquisition();      ///< Called to stop the DataAcquisition node.
    void recover_DataAcquisition();   ///< Called to recover the DataAcquisition node from a failure.

    bool allow_DataAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * DataAcquisition setters
     */
    void PV_DataAcquisition_Gain_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Offset_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Bandwidth_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Resolution_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Impedance_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Coupling_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SignalRefType_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_Ground_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_DMAEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SamplingRate_Writer(const timespec& timestamp, const double& value);

    /**
     * DataAcquisition initializers
     */
    void PV_DataAcquisition_Gain_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Offset_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Bandwidth_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Resolution_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Impedance_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Coupling_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SignalRefType_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_Ground_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_DMAEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SamplingRate_Initializer(timespec* timestamp, double* value);


    /**
     * @brief Function that continuously acquires data generated by m_WaveformGeneration.
     *        It is launched by start_DataAcquisition() in a separate thread.
     */
    void DataAcquisition_thread_body();

    /**
     * @brief A thread that runs DataAcquisition_thread_body().
     */
    nds::Thread m_DataAcquisition_Thread;

    /**
     * @brief A boolean flag that stop the DataAcquisition loop in DataAcquisition_thread_body()
     *        when true.
     */
    volatile bool m_bStop_DataAcquisition;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DATA GENERATION
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief WaveformGeneration node
     */
    nds::WaveformGeneration<std::int64_t> m_WaveformGeneration;

    /**
     * Methods to control WaveformGeneration state machine
     */
    void switchOn_WaveformGeneration();  ///< Called to switch on the WaveformGeneration node.
    void switchOff_WaveformGeneration(); ///< Called to switch off the WaveformGeneration node.
    void start_WaveformGeneration();     ///< Called to start the WaveformGeneration node.
    void stop_WaveformGeneration();      ///< Called to stop the WaveformGeneration node.
    void recover_WaveformGeneration();   ///< Called to recover the WaveformGeneration node from a failure.

    bool allow_WaveformGeneration_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * WaveformGeneration setters
     */
    void PV_WaveformGeneration_Frequency_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_RefFrequency_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Amp_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Phase_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_UpdateRate_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_DutyCycle_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Gain_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Offset_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Bandwidth_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Resolution_Writer(const timespec& timestamp, const double& value);
    void PV_WaveformGeneration_Impedance_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_WaveformGeneration_Coupling_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_WaveformGeneration_SignalRef_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_WaveformGeneration_SignalType_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_WaveformGeneration_Ground_Writer(const timespec& timestamp, const std::int32_t& value);

    /**
     * WaveformGeneration Initializers
     */
    void PV_WaveformGeneration_Frequency_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_RefFrequency_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Amp_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Phase_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_UpdateRate_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_DutyCycle_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Gain_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Offset_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Bandwidth_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Resolution_Initializer(timespec* timestamp, double* value);
    void PV_WaveformGeneration_Impedance_Initializer(timespec* timestamp, std::int32_t* value);
    void PV_WaveformGeneration_Coupling_Initializer(timespec* timestamp, std::int32_t* value);
    void PV_WaveformGeneration_SignalRef_Initializer(timespec* timestamp, std::int32_t* value);
    void PV_WaveformGeneration_SignalType_Initializer(timespec* timestamp, std::int32_t* value);
    void PV_WaveformGeneration_Ground_Initializer(timespec* timestamp, std::int32_t* value);


     /**
      * @brief Function that continuously generates a sinusoidal wave to m_WaveformGeneration.
      *        It is launched by start_WaveformGeneration() in a separate thread.
      */
     void WaveformGeneration_thread_body();

     /**
      * @brief A thread that runs WaveformGeneration_thread_body().
      */
     nds::Thread m_WaveformGeneration_Thread;

     /**
      * @brief A boolean flag that stop the WaveformGeneration loop in WaveformGeneration_thread_body()
      *        when true.
      */
     volatile bool m_bStop_WaveformGeneration;

};

#endif // DEVICEI64_H_
