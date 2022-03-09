/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/timestamping.h"
#include "nds3/impl/timestampingImpl.h"

namespace nds
{

///////////////////////////////////////// TIME STAMPING /////////////////////////////////////////

// ---------------- Constructors and assignment operator -------------------//
template <typename T>
Timestamping<T>::Timestamping(): Node()
{
}

template <typename T>
Timestamping<T>::Timestamping(const Timestamping& right):
     Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
Timestamping<T>& Timestamping<T>::operator=(const Timestamping& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
Timestamping<T>::Timestamping(const std::string& name,
    size_t maxElements,
    stateChange_t switchOnFunction,
    stateChange_t switchOffFunction,
    stateChange_t startFunction,
    stateChange_t stopFunction,
    stateChange_t recoverFunction,
    allowChange_t allowStateChangeFunction,
    writerInt32_t PV_Enable_Writer,
    writerInt32_t PV_Edge_Writer,
    writerInt32_t PV_ClearOverflow_Writer,
    autoEnable_t autoEnable):

    Node(std::shared_ptr<TimestampingImpl<T>>(new TimestampingImpl<T>(name,
    maxElements,
    switchOnFunction,
    switchOffFunction,
    startFunction,
    stopFunction,
    recoverFunction,
    allowStateChangeFunction,
    PV_Enable_Writer,
    PV_Edge_Writer,
    PV_ClearOverflow_Writer,
    autoEnable)))
{
}

template <typename T>
Timestamping<T>::Timestamping(const std::string& name,
        const TimestampingArgs_t& handlerTMS) :
     Node(std::shared_ptr<TimestampingImpl<T>>(new TimestampingImpl<T>(name, handlerTMS)))
{
}

// -------------------- Functions common to all nodes ----------------------- //
template <typename T>
void Timestamping<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->
          setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void Timestamping<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->push(timestamp, data);
}

template <typename T>
timespec Timestamping<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->getStartTimestamp();
}

template <typename T>
nds::state_t Timestamping<T>::getState()
{
    return std::static_pointer_cast<TimestampingImpl<T> >(m_pImplementation)->getState();
}


// -------------------------------- Getters --------------------------------- //
template <typename T>
std::int32_t Timestamping<T>::getEnable(){
  return std::static_pointer_cast<TimestampingImpl<T> >(m_pImplementation)->getEnable();
}

template <typename T>
std::int32_t Timestamping<T>::getEdge(){
  return std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->getEdge();
}

template <typename T>
std::int32_t Timestamping<T>::getMaxTimestamps(){
  return std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->getMaxTimestamps();
}

template <typename T>
std::int32_t Timestamping<T>::getOverflow(){
  return std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->getOverflow();
}

template <typename T>
std::int32_t Timestamping<T>::getDecimation(){
  return std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->getDecimation();
}

// -------------------------------- Setters --------------------------------- //
template <typename T>
void Timestamping<T>::setEnable(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->setEnable(timestamp, value);
}

template <typename T>
void Timestamping<T>::setEdge(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->setEdge(timestamp, value);
}

template <typename T>
void Timestamping<T>::setMaxTimestamps(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->setMaxTimestamps(timestamp, value);
}

template <typename T>
void Timestamping<T>::setOverflow(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->setOverflow(timestamp, value);
}

template <typename T>
void Timestamping<T>::setDecimation(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimestampingImpl<T>>(m_pImplementation)->setDecimation(timestamp, value);
}

template <typename T>
void Timestamping<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<TimestampingImpl<T> >(m_pImplementation)->setState(newState);
}

template class Timestamping<timestamp_t>;

}
