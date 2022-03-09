/*
 * Nominal Device Support v.3 (NDS3)
 *
 * By GMV & UPM
 *
 */

#include "nds3/digitalIO.h"
#include "nds3/impl/digitalIOImpl.h"

namespace nds
{

template <typename T>
DigitalIO<T>::DigitalIO(): Node()
{
}

template <typename T>
DigitalIO<T>::DigitalIO( const std::string& name,
                         size_t maxElements,
                         stateChange_t switchOnFunction,
                         stateChange_t switchOffFunction,
                         stateChange_t startFunction,
                         stateChange_t stopFunction,
                         stateChange_t recoverFunction,
                         allowChange_t allowStateChangeFunction,
                         writerVectorBool_t PV_dataOutMask_Writer,
                         writerDouble_t PV_voltLevelHigh_Writer,
                         writerDouble_t PV_voltLevelLow_Writer,
                         writerVectorBool_t PV_ChannelDir_Writer,
                         autoEnable_t autoEnable):

    Node(std::shared_ptr<DigitalIOImpl<T> >(new DigitalIOImpl<T>( name,
                                                                  maxElements,
                                                                  switchOnFunction,
                                                                  switchOffFunction,
                                                                  startFunction,
                                                                  stopFunction,
                                                                  recoverFunction,
                                                                  allowStateChangeFunction,
                                                                  PV_dataOutMask_Writer,
                                                                  PV_voltLevelHigh_Writer,
                                                                  PV_voltLevelLow_Writer,
                                                                  PV_ChannelDir_Writer,
                                                                  autoEnable)))
{
}

template <typename T>
DigitalIO<T>::DigitalIO( const std::string& name,
                         size_t maxElements,
                         const DigitalIOArgs_t<T>& handlerDIO):
    Node(std::shared_ptr<DigitalIOImpl<T> >(new DigitalIOImpl<T>( name,
                                                                  maxElements,
                                                                  handlerDIO)))
{
}

template <typename T>
DigitalIO<T>::DigitalIO(const DigitalIO<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DigitalIO<T>& DigitalIO<T>::operator=(const DigitalIO<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void DigitalIO<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
nds::state_t DigitalIO<T>::getState()
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getState();
}

template <typename T>
void DigitalIO<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t DigitalIO<T>::getMaxElements()
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec DigitalIO<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
std::vector<bool> DigitalIO<T>::getDataOutMask()
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getDataOutMask();
}

template <typename T>
double DigitalIO<T>::getVoltLevelHigh()
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getVoltLevelHigh();
}

template <typename T>
double DigitalIO<T>::getVoltLevelLow()
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getVoltLevelLow();
}

template <typename T>
std::vector<bool> DigitalIO<T>::getChannelDir()
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getChannelDir();
}

template <typename T>
void DigitalIO<T>::setDataOutMask(const timespec& timestamp, const std::vector<bool>& value)
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setDataOutMask(timestamp, value);
}

template <typename T>
void DigitalIO<T>::setVoltLevelHigh(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setVoltLevelHigh(timestamp, value);
}

template <typename T>
void DigitalIO<T>::setVoltLevelLow(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setVoltLevelLow(timestamp, value);
}

template <typename T>
void DigitalIO<T>::setChannelDir(const timespec& timestamp, const std::vector<bool>& value)
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setChannelDir(timestamp, value);
}

template <typename T>
void DigitalIO<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setNumberOfPushedDataBlocks(timestamp, value);
}
template <typename T>
void DigitalIO<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setState(newState);
}
/*
 * TODO: Major modifications must be done to include this new data types.
 */
//template class DigitalIO<bool>;
//template class DigitalIO<std::uint8_t>;
//template class DigitalIO<std::uint16_t>;
//template class DigitalIO<std::uint32_t>;

template class DigitalIO<std::vector<bool>>;
template class DigitalIO<std::vector<std::int8_t>>;
template class DigitalIO<std::vector<std::int16_t>>;
template class DigitalIO<std::vector<std::int32_t>>;
template class DigitalIO<std::vector<std::int64_t>>;

}
