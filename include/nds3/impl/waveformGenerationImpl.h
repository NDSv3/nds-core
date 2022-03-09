/*
 * Nominal Device Support v.3 (NDS3)
 *  by GMV & UPM
 *
 */

#ifndef NDSWAVEFORMGENERATIONIMPL_H
#define NDSWAVEFORMGENERATIONIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/waveformGeneration.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;

template<typename T>
class WaveformGenerationImpl: public NodeImpl
{
public:
    WaveformGenerationImpl( const std::string& name,
                        size_t maxElements,
                        stateChange_t switchOnFunction,
                        stateChange_t switchOffFunction,
                        stateChange_t startFunction,
                        stateChange_t stopFunction,
                        stateChange_t recoverFunction,
                        allowChange_t allowStateChangeFunction,
                        writerDouble_t PV_Frequency_Writer,
                        writerDouble_t PV_RefFrequency_Writer,
                        writerDouble_t PV_Amp_Writer,
                        writerDouble_t PV_Phase_Writer,
                        writerDouble_t PV_UpdateRate_Writer,
                        writerDouble_t PV_DutyCycle_Writer,
                        writerDouble_t PV_Gain_Writer,
                        writerDouble_t PV_Offset_Writer,
                        writerDouble_t PV_Bandwidth_Writer,
                        writerDouble_t PV_Resolution_Writer,
                        writerInt32_t PV_Impedance_Writer,
                        writerInt32_t PV_Coupling_Writer,
                        writerInt32_t PV_SignalRef_Writer,
                        writerInt32_t PV_SignalType_Writer,
                        writerInt32_t PV_Ground_Writer,
                        autoEnable_t autoEnable);

    /**
     * @brief Constructs the WaveformGeneration node by means of its structure of functions.
     * @param name Name of the node.
     * @param maxElements Maximum size of data to be generated.
     * @param handlerWFG Structure with delegate functions that perform the required actions.
     */
    WaveformGenerationImpl(const std::string& name,
            size_t maxElements,
            const WaveformGenerationArgs_t<T>& handlerWFG);

    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start of the AWG and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Returns the timestamp at the moment of the start of the waveform generation.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the waveform generation started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

    void push(const timespec& timestamp, const T& data);
    T getDataAWG();
    size_t getMaxElements();
    double getAmplitude();
    int32_t getSignalType();
    double getFrequency();
    double getUpdateRate();
    double getOffset();
    double getPhase();
    int32_t getImpedance();
    double getRefFrequency();
    double getDutyCycle();
    double getGain();
    double getBandwidth();
    double getResolution();
    int32_t getCoupling();
    int32_t getSignalRef();
    int32_t getGround();
    int32_t getNumberOfPushedDataBlocks();

    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    void setAmplitude(const timespec& timestamp, const double& value);
    void setSignalType(const timespec& timestamp, const std::int32_t& value);
    void setFrequency(const timespec& timestamp, const double& value);
    void setUpdateRate(const timespec& timestamp, const double& value);
    void setOffset(const timespec& timestamp, const double& value);
    void setPhase(const timespec& timestamp, const double& value);
    void setImpedance(const timespec& timestamp, const std::int32_t& value);
    void setRefFrequency(const timespec& timestamp, const double& value);
    void setDutyCycle(const timespec& timestamp, const double& value);
    void setGain(const timespec& timestamp, const double& value);
    void setBandwidth(const timespec& timestamp, const double& value);
    void setResolution(const timespec& timestamp, const double& value);
    void setCoupling(const timespec& timestamp, const std::int32_t& value);
    void setSignalRef(const timespec& timestamp, const std::int32_t& value);
    void setGround(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();


protected:
    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Generation start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_Data_PV;
    std::shared_ptr<PVVariableOutImpl<T> > m_DataAWG_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Frequency_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Frequency_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_RefFrequency_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_RefFrequency_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Amplitude_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Amplitude_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Phase_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Phase_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_UpdateRate_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_UpdateRate_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_DutyCycle_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_DutyCycle_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Gain_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Gain_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Offset_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Offset_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Bandwidth_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Bandwidth_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Resolution_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Resolution_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Impedance_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Impedance_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Coupling_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Coupling_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SignalRefType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SignalRefType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SignalType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SignalType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Ground_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Ground_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_NumberOfPushedDataBlocks;

    std::shared_ptr<StateMachineImpl> m_StateMachine;


private:
    /**
    * @brief Common source code to define the body of any constructor.
    * @param maxElements Maximum size of data to be generated.
    * @param handlerWFG Structure containing both compulsory and optional functions.
    */
    void constructorBody(size_t maxElements, const WaveformGenerationArgs_t<T>& handlerWFG);
};

}
#endif // NDSWAVEFORMGENERATIONIMPL_H

