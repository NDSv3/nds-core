/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/dataAcquisition.h"
#include "nds3/impl/dataAcquisitionImpl.h"

namespace nds
{

template <typename T>
DataAcquisition<T>::DataAcquisition(): Node()
{
}

/**
 * @brief Constructs the data acquisition node.
 *
 */
template <typename T>
DataAcquisition<T>::DataAcquisition(const std::string& name,
                                    size_t maxElements,
                                    stateChange_t switchOnFunction,
                                    stateChange_t switchOffFunction,
                                    stateChange_t startFunction,
                                    stateChange_t stopFunction,
                                    stateChange_t recoverFunction,
                                    allowChange_t allowStateChangeFunction,
                                    writerDouble_t  PV_Gain_Writer,
                                    writerDouble_t PV_Offset_Writer,
                                    writerDouble_t PV_Bandwidth_Writer,
                                    writerDouble_t PV_Resolution_Writer,
                                    writerDouble_t PV_Impedance_Writer,
                                    writerInt32_t PV_Coupling_Writer,
                                    writerInt32_t PV_SignalRef_Writer,
                                    writerInt32_t PV_Ground_Writer,
                                    writerInt32_t PV_DMAEnable_Writer,
                                    writerDouble_t PV_SamplingRate_Writer,
                                    autoEnable_t autoEnable):
    Node(std::shared_ptr<DataAcquisitionImpl<T> >(new DataAcquisitionImpl<T>(name,
                                                                             maxElements,
                                                                             switchOnFunction,
                                                                             switchOffFunction,
                                                                             startFunction,
                                                                             stopFunction,
                                                                             recoverFunction,
                                                                             allowStateChangeFunction,
                                                                             PV_Gain_Writer,
                                                                             PV_Offset_Writer,
                                                                             PV_Bandwidth_Writer,
                                                                             PV_Resolution_Writer,
                                                                             PV_Impedance_Writer,
                                                                             PV_Coupling_Writer,
                                                                             PV_SignalRef_Writer,
                                                                             PV_Ground_Writer,
                                                                             PV_DMAEnable_Writer,
                                                                             PV_SamplingRate_Writer,
                                                                             autoEnable)))
{
}

template <typename T>
DataAcquisition<T>::DataAcquisition(const std::string& name,
                                    size_t maxElements,
                                    const DataAcquisitionArgs_t& handlerDAQ):
    Node(std::shared_ptr<DataAcquisitionImpl<T> >(new DataAcquisitionImpl<T>(name,
                                                                             maxElements,
                                                                             handlerDAQ)))
{
}

template <typename T>
DataAcquisition<T>::DataAcquisition(const DataAcquisition<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DataAcquisition<T>& DataAcquisition<T>::operator=(const DataAcquisition<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}


template <typename T>
void DataAcquisition<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void DataAcquisition<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
double DataAcquisition<T>::getGain()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getGain();
}

template <typename T>
double DataAcquisition<T>::getOffset()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getOffset();
}

template <typename T>

double DataAcquisition<T>::getBandwidth()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getBandwidth();
}

template <typename T>
double DataAcquisition<T>::getResolution()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getResolution();
}

template <typename T>
double DataAcquisition<T>::getImpedance()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getImpedance();
}

template <typename T>
int32_t DataAcquisition<T>::getCoupling()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getCoupling();
}

template <typename T>
int32_t DataAcquisition<T>::getSignalRefType()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getSignalRefType();
}

template <typename T>
int32_t DataAcquisition<T>::getGround()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getGround();
}

template <typename T>
size_t DataAcquisition<T>::getMaxElements()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec DataAcquisition<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
int32_t DataAcquisition<T>::getNumberOfPushedDataBlocks()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getNumberOfPushedDataBlocks();
}

template <typename T>
int32_t DataAcquisition<T>::getDMABufferSize()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDMABufferSize();
}

template <typename T>
int32_t DataAcquisition<T>::getDMAEnable()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDMAEnable();
}

template <typename T>
int32_t DataAcquisition<T>::getDMANumChannels()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDMANumChannels();
}

template <typename T>
int32_t DataAcquisition<T>::getDMAFrameType()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDMAFrameType();
}

template <typename T>
int32_t DataAcquisition<T>::getDMASampleSize()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDMASampleSize();
}

template <typename T>
double DataAcquisition<T>::getSamplingRate()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getSamplingRate();
}

template <typename T>
nds::state_t DataAcquisition<T>::getState()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getState();
}

template <typename T>
void DataAcquisition<T>::setGain(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setGain(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setOffset(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setOffset(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setBandwidth(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setBandwidth(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setResolution(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setResolution(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setImpedance(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setImpedance(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setCoupling(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setSignalRefType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setSignalRefType(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setGround(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setNumberOfPushedDataBlocks(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setDMABufferSize(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setDMABufferSize(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setDMAEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setDMAEnable(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setDMANumChannels(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setDMANumChannels(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setDMAFrameType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setDMAFrameType(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setDMASampleSize(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setDMASampleSize(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setSamplingRate(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setSamplingRate(timestamp, value);
}

template <typename T>
void DataAcquisition<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setState(newState);
}

template class DataAcquisition<std::int32_t>;
template class DataAcquisition<std::int64_t>;
template class DataAcquisition<float>;
template class DataAcquisition<double>;
template class DataAcquisition<std::vector<std::int8_t> >;
template class DataAcquisition<std::vector<std::uint8_t> >;
template class DataAcquisition<std::vector<std::int16_t> >;
template class DataAcquisition<std::vector<std::int32_t> >;
template class DataAcquisition<std::vector<std::int64_t> >;
template class DataAcquisition<std::vector<float> >;
template class DataAcquisition<std::vector<double> >;


}
