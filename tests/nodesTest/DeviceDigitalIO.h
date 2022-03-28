#ifndef DEVICEDIGITALIO_H_
#define DEVICEDIGITALIO_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a DigitalIO node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceDigitalIO
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceDigitalIO(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceDigitalIO();

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
    static DeviceDigitalIO* getInstance(const std::string& deviceName);


private:

    /**
     * @brief name of the device
     */
    std::string m_name;

    timespec timestamp_device, readtimeStamp;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TEST DEVICE STATE MACHINE
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DeviceDigitalIO state machine
     */
    nds::StateMachine m_DeviceDigitalIO_stateMachine;

    /**
     * Methods to control DeviceDigitalIO state machine
     */
    void switchOn_DeviceDigitalIO();  ///< Called to switch on the DeviceDigitalIO (rootnode).
    void switchOff_DeviceDigitalIO(); ///< Called to switch off the DeviceDigitalIO (rootnode).
    void start_DeviceDigitalIO();     ///< Called to start the DeviceDigitalIO (rootnode).
    void stop_DeviceDigitalIO();      ///< Called to stop the DeviceDigitalIO (rootnode).
    void recover_DeviceDigitalIO();   ///< Called to recover the DeviceDigitalIO (rootnode) from a failure.
    bool allow_DeviceDigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    nds::PVVariableOut<std::int32_t> m_setCurrentTime;
    timespec getCurrentTime();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DIGITAL I/O
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DigitalIO node
     */
    nds::DigitalIO<std::vector<bool> > m_DigitalIO_Bool;

    /**
     * Methods to control DigitalIO state machine
     */
    void switchOn_DigitalIO();  ///< Called to switch on the DigitalIO node.
    void switchOff_DigitalIO(); ///< Called to switch off the DigitalIO node.
    void start_DigitalIO();     ///< Called to start the DigitalIO node.
    void stop_DigitalIO();      ///< Called to stop the DigitalIO node.
    void recover_DigitalIO();   ///< Called to recover the DigitalIO node from a failure.

    bool allow_DigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * DigitalIO setters
     */
    void PV_DigitalIO_dataOutMask_Writer(const timespec& timestamp, const std::vector<bool>& value);
    void PV_DigitalIO_voltLevelHigh_Writer(const timespec& timestamp, const double& value);
    void PV_DigitalIO_voltLevelLow_Writer(const timespec& timestamp, const double& value);
    void PV_DigitalIO_ChannelDir_Writer(const timespec& timestamp, const std::vector<bool>& value);

    /**
     * DigitalIO initializers for std::vector<bool>
     */
    void PV_DigitalIO_dataOutMask_Initializer(timespec* timestamp, std::vector<bool>* value);
    void PV_DigitalIO_voltLevelHigh_Initializer(timespec* timestamp, double* value);
    void PV_DigitalIO_voltLevelLow_Initializer(timespec* timestamp, double* value);
    void PV_DigitalIO_ChannelDir_Initializer(timespec* timestamp, std::vector<bool>* value);

    /**
     * @brief Function that continuously acquires digital IO data.
     *        It is launched by start_DigitalIO() in a separate thread.
     */
    void DigitalIO_thread_body_Bool();

    /**
     * @brief A thread that runs DataProcessing_thread_body().
     */
    nds::Thread m_DigitalIO_Thread_Bool;

    /**
     * @brief A boolean flag that stop the DigitalIO loop in DigitalIO_thread_body()
     *        when true.
     */
    volatile bool m_bStop_DigitalIO_Bool;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DIGITAL I/O int8_t
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DigitalIO node
     */
    nds::DigitalIO<std::vector<std::int8_t> > m_DigitalIO_I8;

    /**
     * Methods to control DigitalIO state machine
     */
    void switchOn_DigitalIO_I8();  ///< Called to switch on the DigitalIO node.
    void switchOff_DigitalIO_I8(); ///< Called to switch off the DigitalIO node.
    void start_DigitalIO_I8();     ///< Called to start the DigitalIO node.
    void stop_DigitalIO_I8();      ///< Called to stop the DigitalIO node.
    void recover_DigitalIO_I8();   ///< Called to recover the DigitalIO node from a failure.

    bool allow_DigitalIO_Change_I8(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * DigitalIO setters
     */
    void PV_DigitalIO_dataOutMask_Writer_I8(const timespec& timestamp, const std::vector<bool>& value);
    void PV_DigitalIO_voltLevelHigh_Writer_I8(const timespec& timestamp, const double& value);
    void PV_DigitalIO_voltLevelLow_Writer_I8(const timespec& timestamp, const double& value);
    void PV_DigitalIO_ChannelDir_Writer_I8(const timespec& timestamp, const std::vector<bool>& value);

    /**
     * DigitalIO initializers for std::vector<std::int8_t>
     */
    void PV_DigitalIO_dataOutMask_Initializer_I8(timespec* timestamp, std::vector<bool>* value);
    void PV_DigitalIO_ChannelDir_Initializer_I8(timespec* timestamp, std::vector<bool>* value);

    /**
     * @brief Function that continuously acquires digital IO data.
     *        It is launched by start_DigitalIO() in a separate thread.
     */
    void DigitalIO_thread_body_I8();

    /**
     * @brief A thread that runs DataProcessing_thread_body().
     */
    nds::Thread m_DigitalIO_Thread_I8;

    /**
     * @brief A boolean flag that stop the DigitalIO loop in DigitalIO_thread_body()
     *        when true.
     */
    volatile bool m_bStop_DigitalIO_I8;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DIGITAL I/O int16_t
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief DigitalIO node
     */

    nds::DigitalIO<std::vector<std::int16_t> > m_DigitalIO_I16;

    /**
     * Methods to control DigitalIO state machine
     */
    void switchOn_DigitalIO_I16();  ///< Called to switch on the DigitalIO node.
    void switchOff_DigitalIO_I16(); ///< Called to switch off the DigitalIO node.
    void start_DigitalIO_I16();     ///< Called to start the DigitalIO node.
    void stop_DigitalIO_I16();      ///< Called to stop the DigitalIO node.
    void recover_DigitalIO_I16();   ///< Called to recover the DigitalIO node from a failure.

    bool allow_DigitalIO_Change_I16(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * DigitalIO setters
     */
    void PV_DigitalIO_dataOutMask_Writer_I16(const timespec& timestamp, const std::vector<bool>& value);
    void PV_DigitalIO_voltLevelHigh_Writer_I16(const timespec& timestamp, const double& value);
    void PV_DigitalIO_voltLevelLow_Writer_I16(const timespec& timestamp, const double& value);
    void PV_DigitalIO_ChannelDir_Writer_I16(const timespec& timestamp, const std::vector<bool>& value);

    /**
     * DigitalIO initializers for std::vector<std::int16_t>
     */
    void PV_DigitalIO_dataOutMask_Initializer_I16(timespec* timestamp, std::vector<bool>* value);
    void PV_DigitalIO_ChannelDir_Initializer_I16(timespec* timestamp, std::vector<bool>* value);

    /**
     * @brief Function that continuously acquires digital IO data.
     *        It is launched by start_DigitalIO() in a separate thread.
     */
    void DigitalIO_thread_body_I16();

    /**
     * @brief A thread that runs DataProcessing_thread_body().
     */
    nds::Thread m_DigitalIO_Thread_I16;

    /**
     * @brief A boolean flag that stop the DigitalIO loop in DigitalIO_thread_body()
     *        when true.
     */
    volatile bool m_bStop_DigitalIO_I16;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DIGITAL I/O int32_t
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DigitalIO node
     */
    nds::DigitalIO<std::vector<std::int32_t> > m_DigitalIO_I32;

    /**
     * Methods to control DigitalIO state machine
     */
    void switchOn_DigitalIO_I32();  ///< Called to switch on the DigitalIO node.
    void switchOff_DigitalIO_I32(); ///< Called to switch off the DigitalIO node.
    void start_DigitalIO_I32();     ///< Called to start the DigitalIO node.
    void stop_DigitalIO_I32();      ///< Called to stop the DigitalIO node.
    void recover_DigitalIO_I32();   ///< Called to recover the DigitalIO node from a failure.

    bool allow_DigitalIO_Change_I32(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * DigitalIO setters
     */
    void PV_DigitalIO_dataOutMask_Writer_I32(const timespec& timestamp, const std::vector<bool>& value);
    void PV_DigitalIO_voltLevelHigh_Writer_I32(const timespec& timestamp, const double& value);
    void PV_DigitalIO_voltLevelLow_Writer_I32(const timespec& timestamp, const double& value);
    void PV_DigitalIO_ChannelDir_Writer_I32(const timespec& timestamp, const std::vector<bool>& value);

    /**
     * DigitalIO initializers for std::vector<std::int32_t>
     */
    void PV_DigitalIO_dataOutMask_Initializer_I32(timespec* timestamp, std::vector<bool>* value);
    void PV_DigitalIO_ChannelDir_Initializer_I32(timespec* timestamp, std::vector<bool>* value);

    /**
     * @brief Function that continuously acquires digital IO data.
     *        It is launched by start_DigitalIO() in a separate thread.
     */
    void DigitalIO_thread_body_I32();

    /**
     * @brief A thread that runs DataProcessing_thread_body().
     */
    nds::Thread m_DigitalIO_Thread_I32;

    /**
     * @brief A boolean flag that stop the DigitalIO loop in DigitalIO_thread_body()
     *        when true.
     */
    volatile bool m_bStop_DigitalIO_I32;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DIGITAL I/O int64_t
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief DigitalIO node
     */

    nds::DigitalIO<std::vector<std::int64_t> > m_DigitalIO_I64;

    /**
     * Methods to control DigitalIO state machine
     */
    void switchOn_DigitalIO_I64();  ///< Called to switch on the DigitalIO node.
    void switchOff_DigitalIO_I64(); ///< Called to switch off the DigitalIO node.
    void start_DigitalIO_I64();     ///< Called to start the DigitalIO node.
    void stop_DigitalIO_I64();      ///< Called to stop the DigitalIO node.
    void recover_DigitalIO_I64();   ///< Called to recover the DigitalIO node from a failure.

    bool allow_DigitalIO_Change_I64(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * DigitalIO setters
     */
    void PV_DigitalIO_dataOutMask_Writer_I64(const timespec& timestamp, const std::vector<bool>& value);
    void PV_DigitalIO_voltLevelHigh_Writer_I64(const timespec& timestamp, const double& value);
    void PV_DigitalIO_voltLevelLow_Writer_I64(const timespec& timestamp, const double& value);
    void PV_DigitalIO_ChannelDir_Writer_I64(const timespec& timestamp, const std::vector<bool>& value);

    /**
     * DigitalIO initializers for std::vector<std::int64_t>
     */
    void PV_DigitalIO_dataOutMask_Initializer_I64(timespec* timestamp, std::vector<bool>* value);
    void PV_DigitalIO_ChannelDir_Initializer_I64(timespec* timestamp, std::vector<bool>* value);

    /**
     * @brief Function that continuously acquires digital IO data.
     *        It is launched by start_DigitalIO() in a separate thread.
     */
    void DigitalIO_thread_body_I64();

    /**
     * @brief A thread that runs DataProcessing_thread_body().
     */
    nds::Thread m_DigitalIO_Thread_I64;

    /**
     * @brief A boolean flag that stop the DigitalIO loop in DigitalIO_thread_body()
     *        when true.
     */
    volatile bool m_bStop_DigitalIO_I64;

};

#endif // DEVICEDIGITALIO_H_
