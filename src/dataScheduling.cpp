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

#include "nds3/dataScheduling.h"
#include "nds3/impl/dataSchedulingImpl.h"

namespace nds
{

template <typename T>
DataScheduling<T>::DataScheduling(): Node()
{
}

template <typename T>
DataScheduling<T>::DataScheduling(const DataScheduling& right):
     Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DataScheduling<T>::DataScheduling(const std::string& name,  ///< The node's name.
               size_t numberInputs, ///< Node's number of PV inputs.
               size_t numberOutputs, ///< Node's number of PV outputs.
               stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on.
               stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off.
               stateChange_t startFunction,            ///< Delegate function that performs the actions to start the data scheduling.
               stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the data scheduling.
               stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state.
               allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true.
               writerInt32_t triggerAction ///< Delegate function that defines what the node does to generate its outputs.
               ):
    Node(std::shared_ptr<DataSchedulingImpl<T>>(new DataSchedulingImpl<T>(name,
                                                                    numberInputs,
                                                                    numberOutputs,
                                                                    switchOnFunction,
                                                                    switchOffFunction,
                                                                    startFunction,
                                                                    stopFunction,
                                                                    recoverFunction,
                                                                    allowStateChangeFunction,
                                                                    triggerAction)))
{
}


template <typename T>
DataScheduling<T>& DataScheduling<T>::operator=(const DataScheduling& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}


template <typename T>
void DataScheduling<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DataSchedulingImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec DataScheduling<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DataSchedulingImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class DataScheduling<std::int32_t>;
template class DataScheduling<std::int64_t>;
template class DataScheduling<float>;
template class DataScheduling<double>;
template class DataScheduling<std::vector<std::int8_t> >;
template class DataScheduling<std::vector<std::uint8_t> >;
template class DataScheduling<std::vector<std::int16_t> >;
template class DataScheduling<std::vector<std::int32_t> >;
template class DataScheduling<std::vector<std::int64_t> >;
template class DataScheduling<std::vector<float> >;
template class DataScheduling<std::vector<double> >;

}
