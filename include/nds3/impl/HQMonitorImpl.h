/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSHEALTHMONITORINGSUPIMPL_H
#define NDSHEALTHMONITORINGSUPIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/HQMonitor.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;

class HQMonitorImpl: public NodeImpl
{
public:
    /**
     * @brief HQMonitorImpl constructor
     *
     * @param name Name (unique) to identify the instance of the node.
     * @param switchOnFunction Delegate function that performs the actions to switch the node on
     * @param switchOffFunction Delegate function that performs the actions to switch the node off
     * @param startFunction Delegate function that performs the actions to start the monitoring
     * @param stopFunction Delegate function that performs the actions to stop the monitoring
     * @param recoverFunction Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction Delegate function that can deny a state change.
     * @param PV_DevicePower_Reader
     * @param PV_DeviceTemperature_Reader
     * @param PV_DeviceVoltage_Reader
     * @param PV_DeviceCurrent_Reader
     * @param PV_SEUEnable_Writer
     * @param PV_DAQEnable_Writer
     * @param PV_SelfTestEnable_Writer
     * @param PV_SelfTestType_Writer
     * @param PV_SelfTestVerboseEnable_Writer
     * @param PV_SelfTestIDEnable_Writer
     * @param PV_SelfTestTextEnable_Writer
     * @param PV_SelfTestCodeResultEnable_Writer
     * @param PV_SelfTestTextResult_Reader
     * @param PV_SignalQualityFlag_Reader
     * @param PV_SignalQualityFlagLevel_Writer
     * @param autoEnable See @ref autoEnable_t for further details
     */
    HQMonitorImpl( const std::string& name,
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
                        writerInt32_t PV_SelfTestTextEnable_Writer,
                        writerInt32_t PV_SelfTestCodeResultEnable_Writer,
                        readerString_t PV_SelfTestTextResult_Reader,
                        readerInt32_t PV_SignalQualityFlag_Reader,
                        writerDouble_t PV_SignalQualityFlagLevel_Writer,
                        autoEnable_t autoEnable);

    /**
     *
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerHQM Structure with delegate functions that perform the required actions.
     */
    HQMonitorImpl(const std::string& name,
                    const HQMonitorArgs_t& handlerHQM);


    /**
     * @brief Specifies the function to call to get the timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

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
     * @brief Returns the timestamp at start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

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
    std::string getSelfTextTxtResult();

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
    void setSelfTextTxtResult(const timespec& timestamp, const std::string& value);

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

protected:

    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_onStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief start time. via the delegate function declared in
     * m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs

    std::shared_ptr<PVDelegateInImpl<double> > m_DevPower_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DevTemperature_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DevVoltage_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DevCurrent_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SEUEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SEUEnable_RBVPV;


    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_HQMonitorDAQEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_HQMonitorDAQEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TestEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TestType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestVerboseEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TestVerboseEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestIDEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TestIDEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestTxtEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TestTxtEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestCodeResultEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TestCodeResultEnable_RBVPV;

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_SignalQFlag_PV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_SignalQFlagTrigLevel_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_SignalQFlagTrigLevel_RBVPV;

    std::shared_ptr<PVDelegateInImpl<std::string> > m_TestTxtResult_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;


private:

    /**
    * Common source code to define the body of any constructor
    * @param handlerHQM Structure containing both compulsory and optional functions.
    */
    void constructorBody(const HQMonitorArgs_t& handlerHQM);

};

}
#endif // NDSHEALTHMONITORINGSUPIMPL_H

