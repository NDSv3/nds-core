/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/FTE.h"
#include "nds3/impl/FTEImpl.h"

namespace nds
{

///////////////////////////////////////// FTE SUPPORT /////////////////////////////////////////

template <typename T>
FTE<T>::FTE(): Node()
{
}

/**
 * @brief Constructs the FTE node.
 *
 */
template <typename T>
FTE<T>::FTE(
        const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        writerInt32_t PV_Set_Writer,
        writerInt32_t PV_Suppress_Writer,
        writerInt32_t PV_ChgPeriod_Writer,
        writerInt32_t PV_PendingValue_Writer,
        autoEnable_t autoEnable):
                        Node(std::shared_ptr<FTEImpl<T> >(new FTEImpl<T>(   name,
                                                                switchOnFunction,
                                                                switchOffFunction,
                                                                startFunction,
                                                                stopFunction,
                                                                recoverFunction,
                                                                allowStateChangeFunction,
                                                                PV_Set_Writer,
                                                                PV_Suppress_Writer,
                                                                PV_ChgPeriod_Writer,
                                                                PV_PendingValue_Writer,
                                                                autoEnable)))
{
}

template <typename T>
FTE<T>::FTE(
        const std::string& name,
        const FTEArgs_t& handlerFTE ):
                        Node(std::shared_ptr<FTEImpl<T> >(new FTEImpl<T>(name, handlerFTE)))
{
}

template <typename T>
FTE<T>::FTE(const FTE<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
FTE<T>& FTE<T>::operator=(const FTE<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void FTE<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec FTE<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
nds::state_t FTE<T>::getState()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getState();
}

///////////////////////////////////////////////////////////////
// Set FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTE<T>::getTerminalSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getTerminalSet();
}


template<typename T>
std::int32_t FTE<T>::getModeSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getModeSet();
}

template<typename T>
timespec FTE<T>::getStartTimeSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getStartTimeSet();
}

template<typename T>
timespec FTE<T>::getStopTimeSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getStopTimeSet();
}

template<typename T>
std::int32_t FTE<T>::getLevelSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getLevelSet();
}

template<typename T>
std::int32_t FTE<T>::getPeriodNsecSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getPeriodNsecSet();
}


template<typename T>
std::int32_t FTE<T>::getDutyCycleSet()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getDutyCycleSet();
}

///////////////////////////////////////////////////////////////
// Suppress FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTE<T>::getTerminalSuppress()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getTerminalSuppress();
}

template<typename T>
std::int32_t FTE<T>::getModeSuppress()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getModeSuppress();
}

template<typename T>
std::int32_t FTE<T>::getAllSuppress()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getAllSuppress();
}

template<typename T>
timespec FTE<T>::getStartTimeSuppress()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getStartTimeSuppress();
}

///////////////////////////////////////////////////////////////
// ChgPeriod FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTE<T>::getTerminalChgPeriod()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getTerminalChgPeriod();
}


template<typename T>
std::int32_t FTE<T>::getPeriodChgPeriod()
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->getPeriodChgPeriod();
}

///////////////////////////////////////////////////////////////
// Set FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTE<T>::setSetStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setSetStatus(timestamp, value);
}

template<typename T>
void FTE<T>::setSetCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setSetCode(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Suppress FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTE<T>::setSuppressStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setSuppressStatus(timestamp, value);
}

template<typename T>
void FTE<T>::setSuppressCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setSuppressCode(timestamp, value);
}

///////////////////////////////////////////////////////////////
// ChgPeriod FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTE<T>::setChgPeriodStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setChgPeriodStatus(timestamp, value);
}

template<typename T>
void FTE<T>::setChgPeriodCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setChgPeriodCode(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Pending FTE setter
///////////////////////////////////////////////////////////////
template<typename T>
void FTE<T>::setPendingValue(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setPendingValue(timestamp, value);
}

template<typename T>
void FTE<T>::setPendingStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setPendingStatus(timestamp, value);
}

template<typename T>
void FTE<T>::setPendingCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setPendingCode(timestamp, value);
}
///////////////////////////////////////////////////////////////
// Maximum FTE setter
///////////////////////////////////////////////////////////////
template<typename T>
void FTE<T>::setMaximum(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setMaximum(timestamp, value);
}

template <typename T>
void FTE<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<FTEImpl<T> >(m_pImplementation)->setState(newState);
}

template class FTE<std::string>;
}
