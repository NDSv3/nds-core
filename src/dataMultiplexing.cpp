/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/dataMultiplexing.h"
#include "nds3/impl/dataMultiplexingImpl.h"

namespace nds
{

template <typename T>
DataMultiplexing<T>::DataMultiplexing(): DataScheduling<T>()
{
}

template <typename T>
DataMultiplexing<T>::DataMultiplexing(const std::string& name,  ///< The node's name.
                                      size_t numberInputs ///< Node's number of PV inputs for the node. They are defined as NDS Output PVs.
                                      ):
    Node(std::shared_ptr<DataMultiplexingImpl<T>>(new DataMultiplexingImpl<T>(name,
                                                                    numberInputs)))
{
}


template <typename T>
void DataMultiplexing<T>::setSamplesPerChannel(const timespec &timestamp, const std::int32_t &maxSamples)
{
    std::static_pointer_cast<DataMultiplexingImpl<T> >(this->m_pImplementation)->setSamplesPerChannel(timestamp, maxSamples);
}

template <typename T>
std::int32_t DataMultiplexing<T>::getSamplesPerChannel()
{
    return std::static_pointer_cast<DataMultiplexingImpl<T> >(this->m_pImplementation)->getSamplesPerChannel();
}

template class DataMultiplexing<std::vector<std::int8_t>>;
template class DataMultiplexing<std::vector<std::uint8_t>>;
template class DataMultiplexing<std::vector<std::int16_t>>;
template class DataMultiplexing<std::vector<std::int32_t>>;
template class DataMultiplexing<std::vector<std::int64_t>>;
template class DataMultiplexing<std::vector<float>>;
template class DataMultiplexing<std::vector<double>>;
}
