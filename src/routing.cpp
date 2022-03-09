/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/routing.h"
#include "nds3/impl/routingImpl.h"

namespace nds
{

///////////////////////////////////////// Routing SUPPORT /////////////////////////////////////////

template <typename T>
Routing<T>::Routing(): Node()
{
}

/**
 * @brief Constructs the routing node.
 */
template <typename T>
Routing<T>::Routing(const std::string& name,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerInt32_t PV_ClkSet_Writer,
                    writerInt32_t PV_ClkDstRead_Writer,
                    writerInt32_t PV_TermSet_Writer,
                    writerInt32_t PV_TermDstRead_Writer,
                    autoEnable_t autoEnable):
    Node(std::shared_ptr<RoutingImpl<T> >(new RoutingImpl<T>(name,
                                                            switchOnFunction,
                                                            switchOffFunction,
                                                            startFunction,
                                                            stopFunction,
                                                            recoverFunction,
                                                            allowStateChangeFunction,
                                                            PV_ClkSet_Writer,
                                                            PV_ClkDstRead_Writer,
                                                            PV_TermSet_Writer,
                                                            PV_TermDstRead_Writer,
                                                            autoEnable)))
{
}

template <typename T>
Routing<T>::Routing(const std::string& name, const RoutingArgs_t& handlerRTN):
    Node(std::shared_ptr<RoutingImpl<T> >(new RoutingImpl<T>(name, handlerRTN)))
{
}

template <typename T>
Routing<T>::Routing(const Routing<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
Routing<T>& Routing<T>::operator=(const Routing<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void Routing<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec Routing<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
nds::state_t Routing<T>::getState()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getState();
}

///////////////////////////////////////////////////////////////
// Route Clocks getters
///////////////////////////////////////////////////////////////

template<typename T>
size_t Routing<T>::getClkSrc()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getClkSrc();
}

template<typename T>
size_t Routing<T>::getClkDst()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getClkDst();
}

////////////////////////////////////////////////////////////////////////////
// Route Terminals getters
////////////////////////////////////////////////////////////////////////////

template<typename T>
size_t Routing<T>::getTermSrc()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getTermSrc();
}

template<typename T>
size_t Routing<T>::getTermDst()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getTermDst();
}

template<typename T>
size_t Routing<T>::getTermSyncSet()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getTermSyncSet();
}

template<typename T>
size_t Routing<T>::getTermInvertSet()
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->getTermInvertSet();
}

/** TODO some setters have been commented. Should they exist?
 *
 */

///////////////////////////////////////////////////////////////
// Route Clocks setters
///////////////////////////////////////////////////////////////

//template<typename T>
//void Routing<T>::setClkSet(const timespec& timestamp, const std::int32_t& value)
//{
//    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setClkSet(timestamp, value);
//}


template<typename T>
void Routing<T>::setClkSetStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setClkSetStatus(timestamp, value);
}


template<typename T>
void Routing<T>::setClkSetCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setClkSetCode(timestamp, value);
}



////////////////////////////////////////////////////////////////////////////
// Read Clock Route Status setters
////////////////////////////////////////////////////////////////////////////

//template<typename T>
//void Routing<T>::setClkDstRead(const timespec& timestamp, const std::int32_t& value)
//{
//    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setClkDstRead(timestamp, value);
//}


template<typename T>
void Routing<T>::setClkSrcRead(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setClkSrcRead(timestamp, value);
}



////////////////////////////////////////////////////////////////////////////
// Route Terminals setters
////////////////////////////////////////////////////////////////////////////

//template<typename T>
//void Routing<T>::setTermSet(const timespec& timestamp, const std::int32_t& value)
//{
//    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermSet(timestamp, value);
//}


template<typename T>
void Routing<T>::setTermSetStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermSetStatus(timestamp, value);
}


template<typename T>
void Routing<T>::setTermSetCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermSetCode(timestamp, value);
}




////////////////////////////////////////////////////////////////////////////
// Read Routing Configuration setters
////////////////////////////////////////////////////////////////////////////

//template<typename T>
//void Routing<T>::setTermDstRead(const timespec& timestamp, const std::int32_t& value)
//{
//    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermDstRead(timestamp, value);
//}


template<typename T>
void Routing<T>::setTermSrcRead(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermSrcRead(timestamp, value);
}


template<typename T>
void Routing<T>::setTermSyncRead(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermSyncRead(timestamp, value);
}


template<typename T>
void Routing<T>::setTermInvertRead(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setTermInvertRead(timestamp, value);
}

template <typename T>
void Routing<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<RoutingImpl<T> >(m_pImplementation)->setState(newState);
}

template class Routing<std::string>;
}
