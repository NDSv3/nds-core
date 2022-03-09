/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSHEALTHMONITORINGSUP_H
#define NDSHEALTHMONITORINGSUP_H

/**
 * @file HQMonitor.h
 * @brief Node to provide support for Health and Quality Monitoring
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"
#include "nds3/stateMachine.h"

namespace nds
{

/**
 * @brief Type defined to gather the arguments required by @ref HQMonitor constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct HQMonitorArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to read the power of the device.
     * This is a compulsory field of the structure.
     */
    const readerDouble_t PV_DevicePower_Reader;

    /**
     * @brief Function to read the temperature of the device.
     * This is a compulsory field of the structure.
     */
    const readerDouble_t PV_DeviceTemperature_Reader;

    /**
     * @brief Function to read the voltage of the device.
     * This is a compulsory field of the structure.
     */
    const readerDouble_t PV_DeviceVoltage_Reader;

    /**
     * @brief Function to read the current of the device.
     * This is a compulsory field of the structure.
     */
    const readerDouble_t PV_DeviceCurrent_Reader;

    /**
     * @brief Function to write (enable/disable) the detection of SEU.
     * It sets the flag to enable or disable the detection of Single
     * Events Upsets.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SEUEnable_Writer;

    /**
     * @brief Function to load the initial value of the SEU detection.
     * It reads the status of the flag to detect Single Events Upsets
     * and it is loaded as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SEUEnable_Initializer;

    /**
     * @brief Function to write (enable/disable) the monitoring of DAQ anomalies.
     * It sets the flag to enable or disable the monitoring of Data Acquisition
     * Anomalies.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_DAQEnable_Writer;

    /**
     * @brief Function to load the initial value of the DAQ anomalies detection.
     * It reads the status of the flag to monitor Data Acquisition Anomalies
     * and it is loaded as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_DAQEnable_Initializer;

    /**
     * @brief Function to enable (and start) the Self-Test.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SelfTestEnable_Writer;

    /**
     * @brief Function to load the initial value of the Self-Test.
     * It reads the status of the flag to enable the Self-Test
     * and it is loaded as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SelfTestEnable_Initializer;

    /**
     * @brief Function to write the type of Self-test.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SelfTestType_Writer;

    /**
     * @brief Function to load the initial value of the type of Self-Test.
     * It reads the type of Self-Test and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SelfTestType_Initializer;

    /**
     * @brief Function to write (enable/disable) the Self-test verbosity.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SelfTestVerboseEnable_Writer;

    /**
     * @brief Function to load the initial value of the Self-Test verbosity.
     * It reads the status of verbosity for the Self-Test and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SelfTestVerboseEnable_Initializer;
    /**
     * @brief Function to write (enable/disable) the Self-Test ID.
     * It allows generating the numeric identifier of the test.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SelfTestIDEnable_Writer;

    /**
     * @brief Function to load the initial value for enabling the Self-Test ID.
     * It reads the status of ID visualization for the Self-Test and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SelfTestIDEnable_Initializer;

    /**
     * @brief Function to write (enable/disable) the Self-Test text.
     * It allows generating a textual description of the test.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SelfTestTxtEnable_Writer;

    /**
     * @brief Function to load the initial value for enabling the Self-Test text.
     * It reads the status of textual description for the Self-Test and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SelfTestTxtEnable_Initializer;

    /**
     * @brief Function to write (enable/disable) the Self-Test result code.
     * It allows generating a numeric code to show the result of the test.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SelfTestCodeResultEnable_Writer;

    /**
     * @brief Function to load the initial value for enabling the Self-Test result code.
     * It reads the status of result code for the Self-Test and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SelfTestCodeResultEnable_Initializer;

    /**
     * @brief Function to read the text resulting of the test.
     * It shows the textual description of the test, whenever the
     * corresponding flag (@ref PV_SelfTestTxtEnable_Writer) has been
     * enabled. @n
     * This is a compulsory field of the structure.
     */
    const readerString_t PV_SelfTestTextResult_Reader;

    /**
     * @brief Function to read the flag of low quality signal.
     * This is a compulsory field of the structure.
     */
    const readerInt32_t PV_SignalQualityFlag_Reader;

    /**
     * @brief Function to write the quality flag trigger level.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_SignalQualityFlagLevel_Writer;
    /**
     * @brief Function to load the initial Signal Quality Flag Level.
     * It reads the status of Signal Quality Flag Level and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_SignalQualityFlagLevel_Initializer;

    /**
     * Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref HQMonitorArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref HQMonitorArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref HQMonitorArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref HQMonitorArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref HQMonitorArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref HQMonitorArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_DevicePower_Reader Power reader to be set in @ref HQMonitorArgs_t.PV_DevicePower_Reader.
     * @param _PV_DeviceTemperature_Reader Temperature reader to be set in @ref HQMonitorArgs_t.PV_DeviceTemperature_Reader.
     * @param _PV_DeviceVoltage_Reader Voltage reader to be set in @ref HQMonitorArgs_t.PV_DeviceVoltage_Reader.
     * @param _PV_DeviceCurrent_Reader Current reader to be set in @ref HQMonitorArgs_t.PV_DeviceCurrent_Reader.
     * @param _PV_SEUEnable_Writer Function to enable/disable the SEU detection. To be set in @ref HQMonitorArgs_t.PV_SEUEnable_Writer.
     * @param _PV_DAQEnable_Writer Function to enable/disable the DAQ anomalies monitoring. To be set in @ref HQMonitorArgs_t.PV_DAQEnable_Writer.
     * @param _PV_SelfTestEnable_Writer Function to enable/disable the Self-test. To be set in @ref HQMonitorArgs_t.PV_SelfTestEnable_Writer.
     * @param _PV_SelfTestType_Writer Self-Test type writer to be set in @ref HQMonitorArgs_t.PV_SelfTestType_Writer.
     * @param _PV_SelfTestVerboseEnable_Writer Function to enable/disable the verbosity in the Self-Test. To be set in @ref HQMonitorArgs_t.PV_SelfTestVerboseEnable_Writer.
     * @param _PV_SelfTestIDEnable_Writer Function to enable/disable the Self-Test ID. To be set in @ref HQMonitorArgs_t.PV_SelfTestIDEnable_Writer.
     * @param _PV_SelfTestTxtEnable_Writer Function to enable/disable the Self-Test text. To be set in @ref HQMonitorArgs_t.PV_SelfTestTxtEnable_Writer.
     * @param _PV_SelfTestCodeResultEnable_Writer Function to enable/disable the generation of Self-Test result code. To be set in @ref HQMonitorArgs_t.PV_SelfTestCodeResultEnable_Writer.
     * @param _PV_SelfTestTextResult_Reader Text result reader to be set in @ref HQMonitorArgs_t.PV_SelfTestTextResult_Reader.
     * @param _PV_SignalQualityFlag_Reader Signal Quality Flag reader to be set in @ref HQMonitorArgs_t.PV_SignalQualityFlag_Reader.
     * @param _PV_SignalQualityFlagLevel_Writer Function to set the Flag of Signal Quality Level. To be set in @ref HQMonitorArgs_t.PV_SignalQualityFlagLevel_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    HQMonitorArgs_t(stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    readerDouble_t _PV_DevicePower_Reader,
                    readerDouble_t _PV_DeviceTemperature_Reader,
                    readerDouble_t _PV_DeviceVoltage_Reader,
                    readerDouble_t _PV_DeviceCurrent_Reader,
                    writerInt32_t _PV_SEUEnable_Writer,
                    writerInt32_t _PV_DAQEnable_Writer,
                    writerInt32_t _PV_SelfTestEnable_Writer,
                    writerInt32_t _PV_SelfTestType_Writer,
                    writerInt32_t _PV_SelfTestVerboseEnable_Writer,
                    writerInt32_t _PV_SelfTestIDEnable_Writer,
                    writerInt32_t _PV_SelfTestTxtEnable_Writer,
                    writerInt32_t _PV_SelfTestCodeResultEnable_Writer,
                    readerString_t _PV_SelfTestTextResult_Reader,
                    readerInt32_t _PV_SignalQualityFlag_Reader,
                    writerDouble_t _PV_SignalQualityFlagLevel_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none) :
            handlerSTM( true, ///Asynchronous state transitions.
                        switchOnFunction,
                        switchOffFunction,
                        startFunction,
                        stopFunction,
                        recoverFunction,
                        allowStateChangeFunction,
                        autoEnable),
            PV_DevicePower_Reader(_PV_DevicePower_Reader),
            PV_DeviceTemperature_Reader(_PV_DeviceTemperature_Reader),
            PV_DeviceVoltage_Reader(_PV_DeviceVoltage_Reader),
            PV_DeviceCurrent_Reader(_PV_DeviceCurrent_Reader),
            PV_SEUEnable_Writer(_PV_SEUEnable_Writer),
            PV_DAQEnable_Writer(_PV_DAQEnable_Writer),
            PV_SelfTestEnable_Writer(_PV_SelfTestEnable_Writer),
            PV_SelfTestType_Writer(_PV_SelfTestType_Writer),
            PV_SelfTestVerboseEnable_Writer(_PV_SelfTestVerboseEnable_Writer),
            PV_SelfTestIDEnable_Writer(_PV_SelfTestIDEnable_Writer),
            PV_SelfTestTxtEnable_Writer(_PV_SelfTestTxtEnable_Writer),
            PV_SelfTestCodeResultEnable_Writer(_PV_SelfTestCodeResultEnable_Writer),
            PV_SelfTestTextResult_Reader(_PV_SelfTestTextResult_Reader),
            PV_SignalQualityFlag_Reader(_PV_SignalQualityFlag_Reader),
            PV_SignalQualityFlagLevel_Writer(_PV_SignalQualityFlagLevel_Writer){}

};

class NDS3_API HQMonitor: public Node
{
public:
    /**
     * @brief TBD
     *
     */
    HQMonitor();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
    HQMonitor(const HQMonitor& right);

    HQMonitor& operator=(const HQMonitor& right);

    /**
     * @brief Constructs the node.
     * @param name Name (unique) to identify the instance of the node.
     * @param switchOnFunction Switch-on function to be set in @ref HQMonitorArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref HQMonitorArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref HQMonitorArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref HQMonitorArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref HQMonitorArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref HQMonitorArgs_t.handlerSTM allowStateChangeFunction.
     * @param PV_DevicePower_Reader Power reader to be set in @ref HQMonitorArgs_t.PV_DevicePower_Reader.
     * @param PV_DeviceTemperature_Reader Temperature reader to be set in @ref HQMonitorArgs_t.PV_DeviceTemperature_Reader.
     * @param PV_DeviceVoltage_Reader Voltage reader to be set in @ref HQMonitorArgs_t.PV_DeviceVoltage_Reader.
     * @param PV_DeviceCurrent_Reader Current reader to be set in @ref HQMonitorArgs_t.PV_DeviceCurrent_Reader.
     * @param PV_SEUEnable_Writer Function to enable/disable the SEU detection. To be set in @ref HQMonitorArgs_t.PV_SEUEnable_Writer.
     * @param PV_DAQEnable_Writer Function to enable/disable the DAQ anomalies monitoring. To be set in @ref HQMonitorArgs_t.PV_DAQEnable_Writer.
     * @param PV_SelfTestEnable_Writer Function to enable/disable the Self-test. To be set in @ref HQMonitorArgs_t.PV_SelfTestEnable_Writer.
     * @param PV_SelfTestType_Writer Self-Test type writer to be set in @ref HQMonitorArgs_t.PV_SelfTestType_Writer.
     * @param PV_SelfTestVerboseEnable_Writer Function to enable/disable the verbosity in the Self-Test. To be set in @ref HQMonitorArgs_t.PV_SelfTestVerboseEnable_Writer.
     * @param PV_SelfTestIDEnable_Writer Function to enable/disable the Self-Test ID. To be set in @ref HQMonitorArgs_t.PV_SelfTestIDEnable_Writer.
     * @param PV_SelfTestTxtEnable_Writer Function to enable/disable the Self-Test text. To be set in @ref HQMonitorArgs_t.PV_SelfTestTxtEnable_Writer.
     * @param PV_SelfTestCodeResultEnable_Writer Function to enable/disable the generation of Self-Test result code. To be set in @ref HQMonitorArgs_t.PV_SelfTestCodeResultEnable_Writer.
     * @param PV_SelfTestTextResult_Reader Text result reader to be set in @ref HQMonitorArgs_t.PV_SelfTestTextResult_Reader.
     * @param PV_SignalQualityFlag_Reader Signal Quality Flag reader to be set in @ref HQMonitorArgs_t.PV_SignalQualityFlag_Reader.
     * @param PV_SignalQualityFlagLevel_Writer Function to set the Flag of Signal Quality Level. To be set in @ref HQMonitorArgs_t.PV_SignalQualityFlagLevel_Writer.
     * @param autoEnable See @ref autoEnable_t for further details
     */
    HQMonitor(  const std::string& name,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    readerDouble_t PV_DevicePower_Reader,
                    readerDouble_t PV_DeviceTemperature_Reader,
                    readerDouble_t PV_DeviceVoltage_Reader,
                    readerDouble_t PV_DeviceCurrent_Reader,
                    writerInt32_t PV_SEUEnable_Writer,
                    writerInt32_t PV_DAQEnable_Writer,
                    writerInt32_t PV_SelfTestEnable_Writer,
                    writerInt32_t PV_SelfTestType_Writer,
                    writerInt32_t PV_SelfTestVerboseEnable_Writer,
                    writerInt32_t PV_SelfTestIDEnable_Writer,
                    writerInt32_t PV_SelfTestTxtEnable_Writer,
                    writerInt32_t PV_SelfTestCodeResultEnable_Writer,
                    readerString_t PV_SelfTestTextResult_Reader,
                    readerInt32_t PV_SignalQualityFlag_Reader,
                    writerDouble_t PV_SignalQualityFlagLevel_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the HQMonitor node.
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerHQM Structure with the arguments required by the HQMonitor node.
     * See @ref HQMonitorArgs_t for further details.
     */
    HQMonitor(const std::string& name, const HQMonitorArgs_t& handlerHQM);

    /**
     *
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate the function that returns the exact starting time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    /**
     *
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const std::int32_t& data);

    /**
     *
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    /**
     * ---------------------------------------------------
     * Getter functions
     * ---------------------------------------------------
     */

    /**
     * @brief Retrieve the Device Power
     */
    double getDevicePower();
    /**
     * @brief Retrieve the Device Temperature
     */
    double getDeviceTemperature();
    /**
     * @brief Retrieve the Device Voltage
     */
    double getDeviceVoltage();
    /**
     * @brief Retrieve the Device Temperature
     */
    double getDeviceCurrent();

    /**
     * @brief Retrieve the status of the flag for detecting Single Event Upsets (SEU)
     */
    size_t getSEUEnable();

    /**
     * @brief Retrieve the status of the flag for monitoring DAQ anomalies
     */
    size_t getDAQMonitorEnable();

    /**
     * @brief Retrieve the status of the flag for enabling the self-test
     */
    size_t getSelfTestEnable();

    /**
     * @brief Retrieve the type of self-test selected (Quick-Test/Full-Test)
     */
    size_t getSelfTestType();

    /**
     * @brief Retrieve the status of the flag for enabling verbose in the self-test
     */
    size_t getSelfTestVerboseEnable();

    /**
     * @brief Retrieve the status of the flag for enabling the identifier in the self-test
     */
    size_t getSelfTestIDEnable();

    /**
     * @brief Retrieve the status of the flag for enabling the textual description in the self-test
     */
    size_t getSelfTestTextEnable();

    /**
     * @brief Retrieve the status of the flag for enabling the numeric code with the result of the self-test
     */
    size_t getSelfTestCodeResultEnable();

    /**
     * @brief Retrieve a text summarizing the self-test result with the fields whose flags are enabled
     */
    std::string getSelfTestTextResult();

    /**
     * @brief Retrieve the flag that indicates whether the quality signal is too low
     */
    size_t getSignalQualityFlag();

    /**
     * @brief Retrieve the trigger level below the signal quality flag should be flagged
     */
    double getSignalQualityFlagLevel();


    /**
     * ---------------------------------------------------
     * Setter functions
     * ---------------------------------------------------
     */

    /**
     * @brief Set the Device Power
     */
    void setDevicePower(const timespec& timestamp, const double& value);

    /**
     * @brief Set the Device Temperature
     */
    void setDeviceTemperature(const timespec& timestamp, const double& value);

    /**
     * @brief Set the Device Voltage
     */
    void setDeviceVoltage(const timespec& timestamp, const double& value);

    /**
     * @brief Set the Device Current
     */
    void setDeviceCurrent(const timespec& timestamp, const double& value);

    /**
     * @brief Set the status of the flag for detecting Single Event Upsets (SEU)
     */
    void setSEUEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the status of the flag for monitoring DAQ anomalies
     */
    void setDAQMonitorEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the status of the flag for enabling the self-test
     */
    void setSelfTestEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the type of self-test selected (Quick-Test/Full-Test)
     */
    void setSelfTestType(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the status of the flag for enabling verbose in the self-test
     */
    void setSelfTestVerboseEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the status of the flag for enabling the identifier in the self-test
     */
    void setSelfTestIDEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the status of the flag for enabling the textual description in the self-test
     */
    void setSelfTestTextEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the status of the flag for enabling the numeric code with the result of the self-test
     */
    void setSelfTestCodeResultEnable(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the text that summarizes the self-test result with the fields whose flags are enabled
     */
    void setSelfTestTextResult(const timespec& timestamp, const std::string& value);

    /**
     * @brief Set the flag that indicates whether the quality signal is too low
     */
    void setSignalQualityFlag(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the the trigger level below the signal quality flag should be flagged
     */
    void setSignalQualityFlagLevel(const timespec& timestamp, const double& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

};

}
#endif // NDSHEALTHMONITORINGSUP_H

