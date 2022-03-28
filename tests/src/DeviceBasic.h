#ifndef DEVICE_H_
#define DEVICE_H_

#include <memory>

#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceBasic
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceBasic(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    ~DeviceBasic();

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
    static DeviceBasic* getInstance(const std::string& deviceName);


private:

    /*
     * @brief name of the device
     */
    std::string m_name;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// Variables to store simulated data
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    std::int32_t PVVariable_value_I32, PVDelegate_value_I32;
    std::int64_t PVVariable_value_I64, PVDelegate_value_I64;
    double  PVVariable_value_DBL, PVDelegate_value_DBL;
    std::vector<std::int8_t> PVVariable_vector_I8, PVDelegate_vector_I8;
    std::vector<std::uint8_t> PVVariable_vector_UI8, PVDelegate_vector_UI8;
    std::vector<std::int32_t> PVVariable_vector_I32, PVDelegate_vector_I32;
    std::vector<std::int64_t> PVVariable_vector_I64, PVDelegate_vector_I64;
    std::vector<double> PVVariable_vector_DBL, PVDelegate_vector_DBL;
    std::string PVVariable_value_string, PVDelegate_value_string;

    timespec timestamp_device, readtimeStamp;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TEST DEVICE STATE MACHINE
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Device state machine
     */
    nds::StateMachine m_Device_stateMachine;

    /**
     * Methods to control Device state machine
     */
    void switchOn_Device();  ///< Called to switch on the Device (rootnode).
    void switchOff_Device(); ///< Called to switch off the Device (rootnode).
    void start_Device();     ///< Called to start the Device (rootnode).
    void stop_Device();      ///< Called to stop the Device (rootnode).
    void recover_Device();   ///< Called to recover the Device (rootnode) from a failure.

    bool allow__Device_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      //  EXTRA PVs for testing purposes
      ///////////////////////////////////////////////////////////////////////////////////////////////////////

      /**
       * PVVariables In and Out for testing purposes
       *      - std::int32_t
       *      - std::int64_t
       *      - double
       *      - std::vector<std::int8_t>
       *      - std::vector<std::uint8_t>
       *      - std::vector<std::int32_t>
       *      - std::vector<std::int64_t>
       *      - std::vector<double>
       *      - std::string
       */
       nds::PVVariableIn<std::int32_t> m_int32_VariableIn;
       nds::PVVariableOut<std::int32_t> m_int32_VariableOut;

       nds::PVVariableIn<std::int64_t> m_int64_VariableIn;
       nds::PVVariableOut<std::int64_t> m_int64_VariableOut;

       nds::PVVariableIn<double> m_double_VariableIn;
       nds::PVVariableOut<double> m_double_VariableOut;

       nds::PVVariableIn<std::vector<std::int8_t>> m_vectorI8_VariableIn;
       nds::PVVariableOut<std::vector<std::int8_t>> m_vectorI8_VariableOut;

       nds::PVVariableIn<std::vector<std::uint8_t>> m_vectorUI8_VariableIn;
       nds::PVVariableOut<std::vector<std::uint8_t>> m_vectorUI8_VariableOut;

       nds::PVVariableIn<std::vector<std::int32_t>> m_vectorI32_VariableIn;
       nds::PVVariableOut<std::vector<std::int32_t>> m_vectorI32_VariableOut;

       nds::PVVariableIn<std::vector<std::int64_t>> m_vectorI64_VariableIn;
       nds::PVVariableOut<std::vector<std::int64_t>> m_vectorI64_VariableOut;

       nds::PVVariableIn<std::vector<double>> m_vectorDBL_VariableIn;
       nds::PVVariableOut<std::vector<double>> m_vectorDBL_VariableOut;

       nds::PVVariableIn<std::string> m_string_VariableIn;
       nds::PVVariableOut<std::string> m_string_VariableOut;

       /**
        * PVDelegates In and Out for testing purposes
        *      - std::int32_t
        *      - std::int64_t
        *      - double
        *      - std::vector<std::int8_t>
        *      - std::vector<std::uint8_t>
        *      - std::vector<std::int32_t>
        *      - std::vector<std::int64_t>
        *      - std::vector<double>
        *      - std::string
        */
       nds::PVDelegateIn<std::int32_t> m_int32_DelegateIn;
       nds::PVDelegateOut<std::int32_t> m_int32_DelegateOut;
       nds::PVDelegateOut<std::int32_t> m_int32_DelegateOut_init;

       nds::PVDelegateIn<std::int64_t> m_int64_DelegateIn;
       nds::PVDelegateOut<std::int64_t> m_int64_DelegateOut;
       nds::PVDelegateOut<std::int64_t> m_int64_DelegateOut_init;

       nds::PVDelegateIn<double> m_double_DelegateIn;
       nds::PVDelegateOut<double> m_double_DelegateOut;
       nds::PVDelegateOut<double> m_double_DelegateOut_init;

       nds::PVDelegateIn<std::vector<std::int8_t>> m_vectorI8_DelegateIn;
       nds::PVDelegateOut<std::vector<std::int8_t>> m_vectorI8_DelegateOut;
       nds::PVDelegateOut<std::vector<std::int8_t>> m_vectorI8_DelegateOut_init;

       nds::PVDelegateIn<std::vector<std::uint8_t>> m_vectorUI8_DelegateIn;
       nds::PVDelegateOut<std::vector<std::uint8_t>> m_vectorUI8_DelegateOut;
       nds::PVDelegateOut<std::vector<std::uint8_t>> m_vectorUI8_DelegateOut_init;

       nds::PVDelegateIn<std::vector<std::int32_t>> m_vectorI32_DelegateIn;
       nds::PVDelegateOut<std::vector<std::int32_t>> m_vectorI32_DelegateOut;
       nds::PVDelegateOut<std::vector<std::int32_t>> m_vectorI32_DelegateOut_init;

       nds::PVDelegateIn<std::vector<std::int64_t>> m_vectorI64_DelegateIn;
       nds::PVDelegateOut<std::vector<std::int64_t>> m_vectorI64_DelegateOut;
       nds::PVDelegateOut<std::vector<std::int64_t>> m_vectorI64_DelegateOut_init;

       nds::PVDelegateIn<std::vector<double>> m_vectorDBL_DelegateIn;
       nds::PVDelegateOut<std::vector<double>> m_vectorDBL_DelegateOut;
       nds::PVDelegateOut<std::vector<double>> m_vectorDBL_DelegateOut_init;

       nds::PVDelegateIn<std::string> m_string_DelegateIn;
       nds::PVDelegateOut<std::string> m_string_DelegateOut;
       nds::PVDelegateOut<std::string> m_string_DelegateOut_init;

       /*
        * Setters and getters
        */

       void read_I32_DelegateIn(timespec* timestamp, std::int32_t* value);
       void read_I64_DelegateIn(timespec* timestamp, std::int64_t* value);
       void read_DBL_DelegateIn(timespec* timestamp, double* value);
       void read_vectorI8_DelegateIn(timespec* timestamp, std::vector<std::int8_t>* value);
       void read_vectorUI8_DelegateIn(timespec* timestamp, std::vector<std::uint8_t>* value);
       void read_vectorI32_DelegateIn(timespec* timestamp, std::vector<std::int32_t>* value);
       void read_vectorI64_DelegateIn(timespec* timestamp, std::vector<std::int64_t>* value);
       void read_vectorDBL_DelegateIn(timespec* timestamp, std::vector<double>* value);
       void read_string_DelegateIn(timespec* timestamp, std::string* value);

       void write_I32_DelegateOut(const timespec& timestamp, const std::int32_t& value);
       void write_I64_DelegateOut(const timespec& timestamp, const std::int64_t& value);
       void write_DBL_DelegateOut(const timespec& timestamp,const double& value);
       void write_vectorI8_DelegateOut(const timespec& timestamp,const std::vector<std::int8_t>& value);
       void write_vectorUI8_DelegateOut(const timespec& timestamp,const std::vector<std::uint8_t>& value);
       void write_vectorI32_DelegateOut(const timespec& timestamp,const std::vector<std::int32_t>& value);
       void write_vectorI64_DelegateOut(const timespec& timestamp,const std::vector<std::int64_t>& value);
       void write_vectorDBL_DelegateOut(const timespec& timestamp,const std::vector<double>& value);
       void write_string_DelegateOut(const timespec& timestamp,const std::string& value);

       void init_I32_DelegateOut(timespec* timestamp, std::int32_t* value);
       void init_I64_DelegateOut(timespec* timestamp, std::int64_t* value);
       void init_DBL_DelegateOut(timespec* timestamp, double* value);
       void init_vectorI8_DelegateOut(timespec* timestamp, std::vector<std::int8_t>* value);
       void init_vectorUI8_DelegateOut(timespec* timestamp, std::vector<std::uint8_t>* value);
       void init_vectorI32_DelegateOut(timespec* timestamp, std::vector<std::int32_t>* value);
       void init_vectorI64_DelegateOut(timespec* timestamp, std::vector<std::int64_t>* value);
       void init_vectorDBL_DelegateOut(timespec* timestamp, std::vector<double>* value);
       void init_string_DelegateOut(timespec* timestamp, std::string* value);

       /*
        * PVs to test subscription, replication and decimation.
        */

       nds::PVVariableIn<std::string> m_testVariableIn;
       nds::PVVariableOut<std::string> m_testVariableOut;

       nds::PVDelegateIn<std::string> m_delegateIn;
       nds::PVDelegateOut<std::string> m_delegateOut;
       nds::PVDelegateOut<std::string>  m_writeTestVariableIn;
       nds::PVDelegateOut<std::string>  m_pushTestVariableIn;
       nds::PVDelegateIn<std::string>   m_readTestVariableOut;

       void readDelegate(timespec* pTimestamp, std::string* pValue);
       void writeDelegate(const timespec& timestamp, const std::string& value);

       void writeTestVariableIn(const timespec& timestamp, const std::string& value);
       void pushTestVariableIn(const timespec& timestamp, const std::string& value);
       void readTestVariableOut(timespec* pTimestamp, std::string* pValue);

       std::string m_writtenByDelegate;

       nds::PVVariableOut<std::int32_t> m_setCurrentTime;
       timespec getCurrentTime();

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //  FIRMWARE SUPPORT
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * @brief Firmware node
         */
        nds::Firmware m_Firmware;

        /**
         * Methods to control Firmware state machine
         */
        void switchOn_Firmware();  ///< Called to switch on the Firmware node.
        void switchOff_Firmware(); ///< Called to switch off the Firmware node.
        void start_Firmware();     ///< Called to start the Firmware node.
        void stop_Firmware();      ///< Called to stop the Firmware node.
        void recover_Firmware();   ///< Called to recover the Firmware node from a failure.

        bool allow_Firmware_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

        /**
         * Firmware setters
         */
        void PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value);

        /**
         * @brief Function that continuously acquires data generated by m_WaveformGeneration.
         *        It is launched by start_Firmware() in a separate thread.
         */
        void Firmware_thread_body();

        /**
         * @brief A thread that runs Firmware_thread_body().
         */
        std::thread m_Firmware_Thread;

        /**
         * @brief A boolean flag that stop the Firmware loop in Firmware_thread_body()
         *        when true.
         */
        volatile bool m_bStop_Firmware;

};

#endif // DEVICE_H_
