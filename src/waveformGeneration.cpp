/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/waveformGeneration.h"
#include "nds3/impl/waveformGenerationImpl.h"

namespace nds
{

template <typename T>
WaveformGeneration<T>::WaveformGeneration(): Node()
{
}

template <typename T>
WaveformGeneration<T>::WaveformGeneration(const std::string& name,
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
    Node(std::shared_ptr<WaveformGenerationImpl<T> >(new WaveformGenerationImpl<T>(name,
                                                                           maxElements,
                                                                           switchOnFunction,
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
                                                                           autoEnable)))
{
}

template <typename T>
WaveformGeneration<T>::WaveformGeneration(const std::string& name,
                                  size_t maxElements,
                                  const WaveformGenerationArgs_t<T>& handlerWFG):
    Node(std::shared_ptr<WaveformGenerationImpl<T> >(new WaveformGenerationImpl<T>(name,
                                                                           maxElements,
                                                                           handlerWFG)))
{
}

template <typename T>
WaveformGeneration<T>::WaveformGeneration(const WaveformGeneration<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
WaveformGeneration<T>& WaveformGeneration<T>::operator=(const WaveformGeneration<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void WaveformGeneration<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec WaveformGeneration<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
nds::state_t WaveformGeneration<T>::getState()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getState();
}

template <typename T>
void WaveformGeneration<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
T WaveformGeneration<T>::getDataAWG()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getDataAWG();
}

template <typename T>
size_t WaveformGeneration<T>::getMaxElements()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
int32_t WaveformGeneration<T>::getSignalType()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getSignalType();
}

template <typename T>
double WaveformGeneration<T>::getAmplitude()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getAmplitude();
}

template <typename T>
double WaveformGeneration<T>::getFrequency()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getFrequency();
}

template <typename T>
double WaveformGeneration<T>::getUpdateRate()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getUpdateRate();
}

template <typename T>
double WaveformGeneration<T>::getOffset()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getOffset();
}

template <typename T>
double WaveformGeneration<T>::getPhase()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getPhase();
}

template <typename T>
int32_t WaveformGeneration<T>::getImpedance()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getImpedance();
}

template <typename T>
double WaveformGeneration<T>::getRefFrequency()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getRefFrequency();
}

template <typename T>
double WaveformGeneration<T>::getDutyCycle()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getDutyCycle();
}

template <typename T>
double WaveformGeneration<T>::getGain()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getGain();
}

template <typename T>
double WaveformGeneration<T>::getBandwidth()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getBandwidth();
}

template <typename T>
double WaveformGeneration<T>::getResolution()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getResolution();
}

template <typename T>
int32_t WaveformGeneration<T>::getCoupling()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getCoupling();
}

template <typename T>
int32_t WaveformGeneration<T>::getSignalRef()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getSignalRef();
}

template <typename T>
int32_t WaveformGeneration<T>::getGround()
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->getGround();
}

template <typename T>
void WaveformGeneration<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setNumberOfPushedDataBlocks(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setAmplitude(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setAmplitude(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setSignalType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setSignalType(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setFrequency(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setFrequency(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setUpdateRate(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setUpdateRate(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setOffset(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setOffset(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setPhase(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setPhase(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setImpedance(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setImpedance(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setRefFrequency(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setRefFrequency(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setDutyCycle(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setDutyCycle(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setGain(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setGain(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setBandwidth(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setBandwidth(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setResolution(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setResolution(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setCoupling(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setSignalRef(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setSignalRef(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setGround(timestamp, value);
}

template <typename T>
void WaveformGeneration<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<WaveformGenerationImpl<T> >(m_pImplementation)->setState(newState);
}

template class WaveformGeneration<std::int32_t>;
template class WaveformGeneration<std::int64_t>;
template class WaveformGeneration<float>;
template class WaveformGeneration<double>;
template class WaveformGeneration<std::vector<std::int8_t> >;
template class WaveformGeneration<std::vector<std::uint8_t> >;
template class WaveformGeneration<std::vector<std::int16_t> >;
template class WaveformGeneration<std::vector<std::int32_t> >;
template class WaveformGeneration<std::vector<std::int64_t> >;
template class WaveformGeneration<std::vector<float> >;
template class WaveformGeneration<std::vector<double> >;

}
