/*
 * Nominal Device Support v.3 (NDS3)
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/definitions.h"
#include "nds3/impl/waveformGenerationImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
WaveformGenerationImpl<T>::WaveformGenerationImpl(const std::string& name,
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
                                          autoEnable_t autoEnable):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    WaveformGenerationArgs_t<T> handlerWFG = WaveformGenerationArgs_t<T>(switchOnFunction,
                                                            switchOffFunction,
                                                            startFunction,
                                                            stopFunction,
                                                            recoverFunction,
                                                            allowStateChangeFunction,
                                                            PV_Frequency_Writer,
                                                            PV_RefFrequency_Writer,
                                                            PV_Amp_Writer,
                                                            PV_Phase_Writer,
                                                            PV_UpdateRate_Writer,
                                                            PV_DutyCycle_Writer,
                                                            PV_Gain_Writer,
                                                            PV_Offset_Writer,
                                                            PV_Bandwidth_Writer,
                                                            PV_Resolution_Writer,
                                                            PV_Impedance_Writer,
                                                            PV_Coupling_Writer,
                                                            PV_SignalRef_Writer,
                                                            PV_SignalType_Writer,
                                                            PV_Ground_Writer,
                                                            autoEnable);
    constructorBody(maxElements, handlerWFG);
}

template<typename T>
WaveformGenerationImpl<T>::WaveformGenerationImpl(const std::string& name,
                                          size_t maxElements,
                                          const WaveformGenerationArgs_t<T>& handlerWFG):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(handlerWFG.handlerSTM.startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    constructorBody(maxElements, handlerWFG);
}

template<typename T>
inline void WaveformGenerationImpl<T>::constructorBody(size_t maxElements, const WaveformGenerationArgs_t<T>& handlerWFG)
{

    // Add the children PVs
    m_Data_PV.reset(new PVVariableInImpl<T>("Data"));
    m_Data_PV->setDescription("Waveform Generated");
    m_Data_PV->setMaxElements(maxElements);
    m_Data_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Data_PV);

    m_DataAWG_PV.reset(new PVVariableOutImpl<T>("DataAWG"));
    m_DataAWG_PV->setMaxElements(maxElements);
    m_DataAWG_PV->setDescription("AWG Samples for signal generation");
    m_DataAWG_PV->setScanType(scanType_t::passive, 0);
    m_DataAWG_PV->write(getTimestamp(), handlerWFG.m_DataAWG_Init);
    addChild(m_DataAWG_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), handlerWFG.m_Decimation_Init);
    addChild(m_Decimation_PV);

    m_Frequency_RBVPV.reset(new PVVariableInImpl<double>("Frequency_RBV"));
    m_Frequency_RBVPV->setDescription("Generation frequency ReadBack");
    m_Frequency_RBVPV->setScanType(scanType_t::interrupt, 0);
    m_Frequency_RBVPV->getDataDirection();
    addChild(m_Frequency_RBVPV);

    if (handlerWFG.PV_Frequency_Initializer) {
        m_Frequency_PV.reset(new PVDelegateOutImpl<double>("Frequency",
                handlerWFG.PV_Frequency_Writer,
                handlerWFG.PV_Frequency_Initializer));
    } else {
        m_Frequency_PV.reset(new PVDelegateOutImpl<double>("Frequency", handlerWFG.PV_Frequency_Writer));
    }
    m_Frequency_PV->setDescription("Generation frequency");
    m_Frequency_PV->setScanType(scanType_t::passive, 0);
    m_Frequency_PV->setUnits("Hz");
    m_Frequency_PV->getDescription();
    m_Frequency_PV->getUnits();
    m_Frequency_PV->getScanType();
    m_Frequency_PV->getScanPeriodSeconds();
    m_Frequency_PV->getEnumerations();
    m_Frequency_PV->getProcessAtInit();
    m_Frequency_PV->getDataDirection();
    addChild(m_Frequency_PV);

    if (handlerWFG.PV_RefFrequency_Initializer) {
        m_RefFrequency_PV.reset(new PVDelegateOutImpl<double>("RefFrequency",
                handlerWFG.PV_RefFrequency_Writer,
                handlerWFG.PV_RefFrequency_Initializer));
    } else {
        m_RefFrequency_PV.reset(new PVDelegateOutImpl<double>("RefFrequency", handlerWFG.PV_RefFrequency_Writer));
    }
    m_RefFrequency_PV->setDescription("Reference frequency");
    m_RefFrequency_PV->setScanType(scanType_t::passive, 0);
    addChild(m_RefFrequency_PV);

    m_RefFrequency_RBVPV.reset(new PVVariableInImpl<double>("RefFrequency_RBV"));
    m_RefFrequency_RBVPV->setDescription("Reference frequency");
    m_RefFrequency_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_RefFrequency_RBVPV);

    if (handlerWFG.PV_Amp_Initializer) {
        m_Amplitude_PV.reset(new PVDelegateOutImpl<double>("Amplitude",
                handlerWFG.PV_Amp_Writer,
                handlerWFG.PV_Amp_Initializer));
    } else {
        m_Amplitude_PV.reset(new PVDelegateOutImpl<double>("Amplitude", handlerWFG.PV_Amp_Writer));
    }
    m_Amplitude_PV->setDescription("Amplitude");
    m_Amplitude_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Amplitude_PV);

    //m_Aplitude_RBPV is PVVariable because every time that the CS updates m_Amplitude value, the m_Amplitude writer updates the AmplitudeRBPV value.
    m_Amplitude_RBVPV.reset(new PVVariableInImpl<double>("Amplitude_RBV"/*, PV_Amp_Reader*/));
    m_Amplitude_RBVPV->setDescription("Amplitude ReadBack");
    m_Amplitude_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Amplitude_RBVPV);

    if (handlerWFG.PV_Phase_Initializer) {
        m_Phase_PV.reset(new PVDelegateOutImpl<double>("Phase",
                handlerWFG.PV_Phase_Writer,
                handlerWFG.PV_Phase_Initializer));
    } else {
        m_Phase_PV.reset(new PVDelegateOutImpl<double>("Phase", handlerWFG.PV_Phase_Writer));
    }
    m_Phase_PV->setDescription("Phase");
    m_Phase_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Phase_PV);

    m_Phase_RBVPV.reset(new PVVariableInImpl<double>("Phase_RBV"));
    m_Phase_RBVPV->setDescription("Phase ReadBack");
    m_Phase_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Phase_RBVPV);

    if (handlerWFG.PV_UpdateRate_Initializer) {
        m_UpdateRate_PV.reset(new PVDelegateOutImpl<double>("UpdateRate",
                handlerWFG.PV_UpdateRate_Writer,
                handlerWFG.PV_UpdateRate_Initializer));
    } else {
        m_UpdateRate_PV.reset(new PVDelegateOutImpl<double>("UpdateRate", handlerWFG.PV_UpdateRate_Writer));
    }
    m_UpdateRate_PV->setDescription("Update Rate");
    m_UpdateRate_PV->setScanType(scanType_t::passive, 0);
    addChild(m_UpdateRate_PV);

    m_UpdateRate_RBVPV.reset(new PVVariableInImpl<double>("UpdateRate_RBV"));
    m_UpdateRate_RBVPV->setDescription("Update Rate ReadBack");
    m_UpdateRate_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_UpdateRate_RBVPV);

    if (handlerWFG.PV_DutyCycle_Initializer) {
        m_DutyCycle_PV.reset(new PVDelegateOutImpl<double>("DutyCycle",
                handlerWFG.PV_DutyCycle_Writer,
                handlerWFG.PV_DutyCycle_Initializer));
    } else {
        m_DutyCycle_PV.reset(new PVDelegateOutImpl<double>("DutyCycle", handlerWFG.PV_DutyCycle_Writer));
    }
    m_DutyCycle_PV->setDescription("Signal Duty Cycle");
    m_DutyCycle_PV->setScanType(scanType_t::passive, 0);
    addChild(m_DutyCycle_PV);

    m_DutyCycle_RBVPV.reset(new PVVariableInImpl<double>("DutyCycle_RBV"));
    m_DutyCycle_RBVPV->setDescription("Signal Duty Cycle ReadBack");
    m_DutyCycle_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DutyCycle_RBVPV);

    if (handlerWFG.PV_Gain_Initializer) {
        m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",
                handlerWFG.PV_Gain_Writer,
                handlerWFG.PV_Gain_Initializer));
    } else {
        m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",handlerWFG.PV_Gain_Writer));
    }
    m_Gain_PV->setDescription("Gain of the Channel");
    m_Gain_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Gain_PV);

    m_Gain_RBVPV.reset(new PVVariableInImpl<double>("Gain_RBV"));
    m_Gain_RBVPV->setDescription("Gain of the Channel ReadBack");
    m_Gain_RBVPV-> setScanType(scanType_t::interrupt,0);
    addChild(m_Gain_RBVPV);

    if (handlerWFG.PV_Offset_Initializer) {
        m_Offset_PV.reset(new PVDelegateOutImpl<double>("Offset",
                handlerWFG.PV_Offset_Writer,
                handlerWFG.PV_Offset_Initializer));
    } else {
        m_Offset_PV.reset(new PVDelegateOutImpl<double>("Offset",handlerWFG.PV_Offset_Writer));
    }
    m_Offset_PV->setDescription("Offset");
    m_Offset_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Offset_PV);

    m_Offset_RBVPV.reset(new PVVariableInImpl<double>("Offset_RBV"));
    m_Offset_RBVPV->setDescription("Offset ReadBack");
    m_Offset_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Offset_RBVPV);

    if (handlerWFG.PV_Bandwidth_Initializer) {
        m_Bandwidth_PV.reset(new PVDelegateOutImpl<double>("BandWidth",
                handlerWFG.PV_Bandwidth_Writer,
                handlerWFG.PV_Bandwidth_Initializer));
    } else {
        m_Bandwidth_PV.reset(new PVDelegateOutImpl<double>("BandWidth",handlerWFG.PV_Bandwidth_Writer));
    }
    m_Bandwidth_PV->setDescription("BandWidth");
    m_Bandwidth_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Bandwidth_PV);

    m_Bandwidth_RBVPV.reset(new PVVariableInImpl<double>("BandWidth_RBV"));
    m_Bandwidth_RBVPV->setDescription("BandWidth ReadBack");
    m_Bandwidth_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Bandwidth_RBVPV);

    if (handlerWFG.PV_Resolution_Initializer) {
        m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",
                handlerWFG.PV_Resolution_Writer,
                handlerWFG.PV_Resolution_Initializer));
    } else {
        m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",handlerWFG.PV_Resolution_Writer));
    }
    m_Resolution_PV->setDescription("Resolution: Number of Bits per Sample");
    m_Resolution_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Resolution_PV);

    m_Resolution_RBVPV.reset(new PVVariableInImpl<double>("Resolution_RBV"));
    m_Resolution_RBVPV->setDescription("Resolution: Number of Bits per Sample ReadBack");
    m_Resolution_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Resolution_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t ImpedanceEnumeratorStrings;
    ImpedanceEnumeratorStrings.push_back("50ohm");
    ImpedanceEnumeratorStrings.push_back("Inf");

    if (handlerWFG.PV_Impedance_Initializer) {
        m_Impedance_PV.reset(new PVDelegateOutImpl<std::int32_t>("Impedance",
                handlerWFG.PV_Impedance_Writer,
                handlerWFG.PV_Impedance_Initializer));
    } else {
        m_Impedance_PV.reset(new PVDelegateOutImpl<std::int32_t>("Impedance",handlerWFG.PV_Impedance_Writer));
    }
    m_Impedance_PV->setDescription("Impedance: 50 ohm or Inf ");
    m_Impedance_PV->setEnumeration(ImpedanceEnumeratorStrings);
    m_Impedance_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Impedance_PV);

    m_Impedance_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Impedance_RBV"));
    m_Impedance_RBVPV->setDescription("Impedance ReadBack");
    m_Impedance_RBVPV->setEnumeration(ImpedanceEnumeratorStrings);
    m_Impedance_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Impedance_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t CouplingEnumeratorStrings;
    CouplingEnumeratorStrings.push_back("AC");
    CouplingEnumeratorStrings.push_back("DC");

    if (handlerWFG.PV_Coupling_Initializer) {
        m_Coupling_PV.reset(new PVDelegateOutImpl<std::int32_t>("Coupling",
                handlerWFG.PV_Coupling_Writer,
                handlerWFG.PV_Coupling_Initializer));
    } else {
        m_Coupling_PV.reset(new PVDelegateOutImpl<std::int32_t>("Coupling",handlerWFG.PV_Coupling_Writer));
    }
    m_Coupling_PV->setDescription("Coupling: AC or DC");
    m_Coupling_PV->setEnumeration(CouplingEnumeratorStrings);
    m_Coupling_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Coupling_PV);


    m_Coupling_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Coupling_RBV"));
    m_Coupling_RBVPV->setDescription("Coupling: AC or DC ReadBack");
    m_Coupling_RBVPV->setScanType(scanType_t::interrupt, 0);
    m_Coupling_RBVPV->setEnumeration(CouplingEnumeratorStrings);
    addChild(m_Coupling_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t SignalRefTypeEnumeratorStrings;
    SignalRefTypeEnumeratorStrings.push_back("SingleEnded");
    SignalRefTypeEnumeratorStrings.push_back("Differential");

    if (handlerWFG.PV_SignalRef_Initializer) {
        m_SignalRefType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalRefType",
                handlerWFG.PV_SignalRef_Writer,
                handlerWFG.PV_SignalRef_Initializer));
    } else {
        m_SignalRefType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalRefType",handlerWFG.PV_SignalRef_Writer));
    }
    m_SignalRefType_PV->setDescription("Type of input: Differential or Single Ended");
    m_SignalRefType_PV->setEnumeration(SignalRefTypeEnumeratorStrings);
    m_SignalRefType_PV->setScanType(scanType_t::passive, 0);
    addChild(m_SignalRefType_PV);

    m_SignalRefType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SignalRefType_RBV"));
    m_SignalRefType_RBVPV->setDescription("Type of input: Differential or Single Ended ReadBack");
    m_SignalRefType_RBVPV->setScanType(scanType_t::interrupt, 0);
    m_SignalRefType_RBVPV->setEnumeration(SignalRefTypeEnumeratorStrings);
    addChild(m_SignalRefType_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t SignalTypeEnumeratorStrings;
    SignalTypeEnumeratorStrings.push_back("WaveForm");
    SignalTypeEnumeratorStrings.push_back("Spline");
    SignalTypeEnumeratorStrings.push_back("DC");
    SignalTypeEnumeratorStrings.push_back("Sin");
    SignalTypeEnumeratorStrings.push_back("Square");
    SignalTypeEnumeratorStrings.push_back("Triangle");
    SignalTypeEnumeratorStrings.push_back("Pulse");
    SignalTypeEnumeratorStrings.push_back("Sawtooth");

    if (handlerWFG.PV_SignalType_Initializer) {
        m_SignalType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalType",
                handlerWFG.PV_SignalType_Writer,
                handlerWFG.PV_SignalType_Initializer));
    } else {
        m_SignalType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalType",handlerWFG.PV_SignalType_Writer));
    }
    m_SignalType_PV->setDescription("Signal type generated");
    m_SignalType_PV->setEnumeration(SignalTypeEnumeratorStrings);
    m_SignalType_PV->setScanType(scanType_t::passive, 0);
    addChild(m_SignalType_PV);

    m_SignalType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SignalType_RBV"));
    m_SignalType_RBVPV->setDescription("Signal type generated ReadBack");
    m_SignalType_RBVPV->setScanType(scanType_t::interrupt, 0);
    m_SignalType_RBVPV->setEnumeration(SignalTypeEnumeratorStrings);
    addChild(m_SignalType_RBVPV);

    //ad enumerator for ground state
    enumerationStrings_t groundEnumeratorStrings;
    groundEnumeratorStrings.push_back("On");
    groundEnumeratorStrings.push_back("Off");

    if (handlerWFG.PV_Ground_Initializer) {
        m_Ground_PV.reset(new PVDelegateOutImpl<std::int32_t>("Ground",
                handlerWFG.PV_Ground_Writer,
                handlerWFG.PV_Ground_Initializer));
    } else {
        m_Ground_PV.reset(new PVDelegateOutImpl<std::int32_t>("Ground",handlerWFG.PV_Ground_Writer));
    }
    m_Ground_PV->setDescription("Ground State");
    m_Ground_PV->setEnumeration(groundEnumeratorStrings);
    m_Ground_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Ground_PV);

    m_Ground_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Ground_RBV"));
    m_Ground_RBVPV->setDescription("Ground State ReadBack");
    m_Ground_RBVPV->setScanType(scanType_t::interrupt,0);
    m_Ground_RBVPV->setEnumeration(groundEnumeratorStrings);
    addChild(m_Ground_RBVPV);

    // Add the children PVs
    m_NumberOfPushedDataBlocks.reset(new PVVariableInImpl<std::int32_t>("NumberOfPushedDataBlocks"));
    m_NumberOfPushedDataBlocks->setDescription("Number Of Pushed Data Blocks");
    m_NumberOfPushedDataBlocks->setScanType(scanType_t::interrupt, 0);
    addChild(m_NumberOfPushedDataBlocks);


    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(false,
                                   handlerWFG.handlerSTM.switchOnFunction,
                                   handlerWFG.handlerSTM.switchOffFunction,
                                   std::bind(&WaveformGenerationImpl::onStart, this),
                                   handlerWFG.handlerSTM.stopFunction,
                                   handlerWFG.handlerSTM.recoverFunction,
                                   handlerWFG.handlerSTM.allowStateChangeFunction,
                                   handlerWFG.handlerSTM.autoEnable));
    addChild(m_StateMachine);
}


template<typename T>
void WaveformGenerationImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
timespec WaveformGenerationImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void WaveformGenerationImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_Data_PV->push(timestamp,data);
}

template<typename T>
size_t WaveformGenerationImpl<T>::getMaxElements()
{
    return m_Data_PV->getMaxElements();
}

template<typename T>
T WaveformGenerationImpl<T>::getDataAWG()
{
    T data;
    timespec timestamp;
    m_DataAWG_PV->read(&timestamp, &data);
    return data;
}

template<typename T>
int32_t WaveformGenerationImpl<T>::getSignalType()
{
    std::int32_t signalType;
    timespec timestamp;
    m_SignalType_RBVPV->read(&timestamp, &signalType);
    return signalType;
}

template<typename T>
double WaveformGenerationImpl<T>::getAmplitude()
{
    double amplitude;
    timespec timestamp;
    m_Amplitude_RBVPV->read(&timestamp, &amplitude);
    return amplitude;
}

template<typename T>
double WaveformGenerationImpl<T>::getFrequency()
{
    double frequency;
    timespec timestamp;
    m_Frequency_RBVPV->read(&timestamp, &frequency);
    return frequency;
}

template<typename T>
double WaveformGenerationImpl<T>::getUpdateRate()
{
    double updateRate;
    timespec timestamp;
    m_UpdateRate_RBVPV->read(&timestamp, &updateRate);
    return updateRate;
}

template<typename T>
double WaveformGenerationImpl<T>::getOffset()
{
    double offset;
    timespec timestamp;
    m_Offset_RBVPV->read(&timestamp, &offset);
    return offset;
}

template<typename T>
double WaveformGenerationImpl<T>::getPhase()
{
    double phase;
    timespec timestamp;
    m_Phase_RBVPV->read(&timestamp, &phase);
    return phase;
}

template<typename T>
int32_t WaveformGenerationImpl<T>::getImpedance()
{
    std::int32_t impedance;
    timespec timestamp;
    m_Impedance_RBVPV->read(&timestamp, &impedance);
    return impedance;
}

template<typename T>
double WaveformGenerationImpl<T>::getRefFrequency()
{
    double RefFrequency;
    timespec timestamp;
    m_RefFrequency_RBVPV->read(&timestamp, &RefFrequency);
    return RefFrequency;
}

template<typename T>
double WaveformGenerationImpl<T>::getDutyCycle()
{
    double DutyCycle;
    timespec timestamp;
    m_DutyCycle_RBVPV->read(&timestamp, &DutyCycle);
    return DutyCycle;
}

template<typename T>
double WaveformGenerationImpl<T>::getGain()
{
    double Gain;
    timespec timestamp;
    m_Gain_RBVPV->read(&timestamp, &Gain);
    return Gain;
}

template<typename T>
double WaveformGenerationImpl<T>::getBandwidth()
{
    double Bandwidth;
    timespec timestamp;
    m_Bandwidth_RBVPV->read(&timestamp, &Bandwidth);
    return Bandwidth;
}

template<typename T>
double WaveformGenerationImpl<T>::getResolution()
{
    double Resolution;
    timespec timestamp;
    m_Resolution_RBVPV->read(&timestamp, &Resolution);
    return Resolution;
}

template<typename T>
int32_t WaveformGenerationImpl<T>::getCoupling()
{
    std::int32_t Coupling;
    timespec timestamp;
    m_Coupling_RBVPV->read(&timestamp, &Coupling);
    return Coupling;
}

template<typename T>
int32_t WaveformGenerationImpl<T>::getSignalRef()
{
    std::int32_t SignalRef;
    timespec timestamp;
    m_SignalRefType_RBVPV->read(&timestamp, &SignalRef);
    return SignalRef;
}

template<typename T>
int32_t WaveformGenerationImpl<T>::getGround()
{
    std::int32_t Ground;
    timespec timestamp;
    m_Ground_RBVPV->read(&timestamp, &Ground);
    return Ground;
}

template<typename T>
void WaveformGenerationImpl<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
    m_NumberOfPushedDataBlocks->setValue(timestamp, value);
    m_NumberOfPushedDataBlocks->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setAmplitude(const timespec& timestamp, const double& value)
{
    m_Amplitude_RBVPV->setValue(timestamp, value);
    m_Amplitude_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setSignalType(const timespec& timestamp, const std::int32_t& value)
{
    m_SignalType_RBVPV->setValue(timestamp, value);
    m_SignalType_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setFrequency(const timespec& timestamp, const double& value)
{
    m_Frequency_RBVPV->setValue(timestamp, value);
    m_Frequency_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setUpdateRate(const timespec& timestamp, const double& value)
{
    m_UpdateRate_RBVPV->setValue(timestamp, value);
    m_UpdateRate_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setOffset(const timespec& timestamp, const double& value)
{
    m_Offset_RBVPV->setValue(timestamp, value);
    m_Offset_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setPhase(const timespec& timestamp, const double& value)
{
    m_Phase_RBVPV->setValue(timestamp, value);
    m_Phase_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setImpedance(const timespec& timestamp, const std::int32_t& value)
{
    m_Impedance_RBVPV->setValue(timestamp, value);
    m_Impedance_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setRefFrequency(const timespec& timestamp, const double& value)
{
    m_RefFrequency_RBVPV->setValue(timestamp, value);
    m_RefFrequency_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setDutyCycle(const timespec& timestamp, const double& value)
{
    m_DutyCycle_RBVPV->setValue(timestamp, value);
    m_DutyCycle_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setGain(const timespec& timestamp, const double& value)
{
    m_Gain_RBVPV->setValue(timestamp, value);
    m_Gain_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setBandwidth(const timespec& timestamp, const double& value)
{
    m_Bandwidth_RBVPV->setValue(timestamp, value);
    m_Bandwidth_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setResolution(const timespec& timestamp, const double& value)
{
    m_Resolution_RBVPV->setValue(timestamp, value);
    m_Resolution_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
    m_Coupling_RBVPV->setValue(timestamp, value);
    m_Coupling_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setSignalRef(const timespec& timestamp, const std::int32_t& value)
{
    m_SignalRefType_RBVPV->setValue(timestamp, value);
    m_SignalRefType_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
    m_Ground_RBVPV->setValue(timestamp, value);
    m_Ground_RBVPV->push(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_Data_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
nds::state_t WaveformGenerationImpl<T>::getState()
{
    return m_StateMachine->getLocalState();
}

template<typename T>
void WaveformGenerationImpl<T>::setState(const nds::state_t& newState)
{
    m_StateMachine->setState(newState);
}

template class WaveformGenerationImpl<std::int32_t>;
template class WaveformGenerationImpl<std::int64_t>;
template class WaveformGenerationImpl<float>;
template class WaveformGenerationImpl<double>;
template class WaveformGenerationImpl<std::vector<std::int8_t> >;
template class WaveformGenerationImpl<std::vector<std::uint8_t> >;
template class WaveformGenerationImpl<std::vector<std::int16_t> >;
template class WaveformGenerationImpl<std::vector<std::int32_t> >;
template class WaveformGenerationImpl<std::vector<std::int64_t> >;
template class WaveformGenerationImpl<std::vector<float> >;
template class WaveformGenerationImpl<std::vector<double> >;


}
