/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/routingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
RoutingImpl<T>::RoutingImpl(const std::string& name,
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
                            autoEnable_t autoEnable
):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    RoutingArgs_t handlerRTN = RoutingArgs_t(switchOnFunction,
                                             switchOffFunction,
                                             startFunction,
                                             stopFunction,
                                             recoverFunction,
                                             allowStateChangeFunction,
                                             PV_ClkSet_Writer,
                                             PV_ClkDstRead_Writer,
                                             PV_TermSet_Writer,
                                             PV_TermDstRead_Writer,
                                             autoEnable);
    constructorBody(handlerRTN);
}

template<typename T>
RoutingImpl<T>::RoutingImpl(const std::string& name, const RoutingArgs_t& handlerRTN
):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(handlerRTN.handlerSTM.startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    constructorBody(handlerRTN);
}


template<typename T>
inline void RoutingImpl<T>::constructorBody(const RoutingArgs_t& handlerRTN)
{

    ////////////////////////////////////////////////////////////////////////////
    // Set Route Clocks PVs
    ////////////////////////////////////////////////////////////////////////////

    m_ClkSrc_PV.reset(new PVVariableOutImpl<std::int32_t>("ClkSrc"));
    m_ClkSrc_PV->setDescription("Clock source");
    m_ClkSrc_PV->setScanType(scanType_t::interrupt, 0);
    m_ClkSrc_PV->write(getTimestamp(), handlerRTN.m_ClkSrc_Init);
    addChild(m_ClkSrc_PV);

    m_ClkDst_PV.reset(new PVVariableOutImpl<std::int32_t>("ClkDst"));
    m_ClkDst_PV->setDescription("Clock Destination");
    m_ClkDst_PV->setScanType(scanType_t::interrupt, 0);
    m_ClkDst_PV->write(getTimestamp(), handlerRTN.m_ClkDst_Init);
    addChild(m_ClkDst_PV);

    if (handlerRTN.PV_ClkSet_Initializer) {
        m_ClkSet_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClkSet",
                handlerRTN.PV_ClkSet_Writer,
                handlerRTN.PV_ClkSet_Initializer));
    } else {
        m_ClkSet_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClkSet",handlerRTN.PV_ClkSet_Writer));
    }
    m_ClkSet_PV->setDescription("Clock Routing Set");
    m_ClkSet_PV->setScanType(scanType_t::passive, 0);
    addChild(m_ClkSet_PV);

    m_ClkSetStatus_PV.reset(new PVVariableInImpl<std::string>("ClkSetStatus"));
    m_ClkSetStatus_PV->setDescription("Clock routing set status message");
    m_ClkSetStatus_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ClkSetStatus_PV);

    m_ClkSetCode_PV.reset(new PVVariableInImpl<std::int32_t>("ClkSetCode"));
    m_ClkSetCode_PV->setDescription("Clock routing set status code");
    m_ClkSetCode_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ClkSetCode_PV);

    ////////////////////////////////////////////////////////////////////////////
    // Set Read Clock Route Status PVs
    ////////////////////////////////////////////////////////////////////////////

    if (handlerRTN.PV_ClkDstRead_Initializer) {
        m_ClkDstRead_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClkDstRead",
                handlerRTN.PV_ClkDstRead_Writer,
                handlerRTN.PV_ClkDstRead_Initializer));
    } else {
        m_ClkDstRead_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClkDstRead",handlerRTN.PV_ClkDstRead_Writer));
    }
    m_ClkDstRead_PV->setDescription("Clock destination connection configuration read");
    m_ClkDstRead_PV->setScanType(scanType_t::passive, 0);
    addChild(m_ClkDstRead_PV);

    m_ClkSrcRead_PV.reset(new PVVariableInImpl<std::int32_t>("ClkSrcRead"));
    m_ClkSrcRead_PV->setDescription("Clock source of selected clock destination");
    m_ClkSrcRead_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ClkSrcRead_PV);

    ////////////////////////////////////////////////////////////////////////////
    // Set Route Terminals PVs
    ////////////////////////////////////////////////////////////////////////////

    m_TermSrc_PV.reset(new PVVariableOutImpl<std::int32_t>("TermSrc"));
    m_TermSrc_PV->setDescription("Terminal source");
    m_TermSrc_PV->setScanType(scanType_t::interrupt, 0);
    m_TermSrc_PV->write(getTimestamp(), handlerRTN.m_TermSrc_Init);
    addChild(m_TermSrc_PV);

    m_TermDst_PV.reset(new PVVariableOutImpl<std::int32_t>("TermDst"));
    m_TermDst_PV->setDescription("Terminal destination");
    m_TermDst_PV->setScanType(scanType_t::interrupt, 0);
    m_TermDst_PV->write(getTimestamp(), handlerRTN.m_TermDst_Init);
    addChild(m_TermDst_PV);

    m_TermSyncSet_PV.reset(new PVVariableOutImpl<std::int32_t>("TermSyncSet"));
    m_TermSyncSet_PV->setDescription("Terminal sync mode set");
    m_TermSyncSet_PV->setScanType(scanType_t::interrupt, 0);
    m_TermSyncSet_PV->write(getTimestamp(), handlerRTN.m_TermSyncSet_Init);
    addChild(m_TermSyncSet_PV);

    m_TermInvertSet_PV.reset(new PVVariableOutImpl<std::int32_t>("TermInvertSet"));
    m_TermInvertSet_PV->setDescription("Terminal inversion mode set");
    m_TermInvertSet_PV->setScanType(scanType_t::interrupt, 0);
    m_TermInvertSet_PV->write(getTimestamp(), handlerRTN.m_TermInvertSet_Init);
    addChild(m_TermInvertSet_PV);

    if (handlerRTN.PV_TermSet_Initializer) {
        m_TermSet_PV.reset(new PVDelegateOutImpl<std::int32_t>("TermSet",
                handlerRTN.PV_TermSet_Writer,
                handlerRTN.PV_TermSet_Initializer));
    } else {
        m_TermSet_PV.reset(new PVDelegateOutImpl<std::int32_t>("TermSet",handlerRTN.PV_TermSet_Writer));
    }
    m_TermSet_PV->setDescription("Terminal Routing Set");
    m_TermSet_PV->setScanType(scanType_t::passive, 0);
    addChild(m_TermSet_PV);

    m_TermSetStatus_PV.reset(new PVVariableInImpl<std::string>("TermSetStatus"));
    m_TermSetStatus_PV->setDescription("Terminal routing set status message");
    m_TermSetStatus_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_TermSetStatus_PV);

    m_TermSetCode_PV.reset(new PVVariableInImpl<std::int32_t>("TermSetCode"));
    m_TermSetCode_PV->setDescription("Terminal routing set status code");
    m_TermSetCode_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_TermSetCode_PV);

    ////////////////////////////////////////////////////////////////////////////
    // Set Read Routing Configuration PVs
    ////////////////////////////////////////////////////////////////////////////

    if (handlerRTN.PV_TermDstRead_Initializer) {
        m_TermDstRead_PV.reset(new PVDelegateOutImpl<std::int32_t>("TermDstRead",
                handlerRTN.PV_TermDstRead_Writer,
                handlerRTN.PV_TermDstRead_Initializer));
    } else {
        m_TermDstRead_PV.reset(new PVDelegateOutImpl<std::int32_t>("TermDstRead",handlerRTN.PV_TermDstRead_Writer));
    }
    m_TermDstRead_PV->setDescription("Terminal destination connection configuration read");
    m_TermDstRead_PV->setScanType(scanType_t::passive, 0);
    addChild(m_TermDstRead_PV);

    m_TermSrcRead_PV.reset(new PVVariableInImpl<std::int32_t>("TermSrcRead"));
    m_TermSrcRead_PV->setDescription("Terminal source of selected Terminal destination");
    m_TermSrcRead_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_TermSrcRead_PV);

    m_TermSyncRead_PV.reset(new PVVariableInImpl<std::int32_t>("TermSyncRead"));
    m_TermSyncRead_PV->setDescription("Terminal sync mode of selected Terminal destination");
    m_TermSyncRead_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_TermSyncRead_PV);

    m_TermInvertRead_PV.reset(new PVVariableInImpl<std::int32_t>("TermInvertRead"));
    m_TermInvertRead_PV->setDescription("Terminal invert mode of selected Terminal destination");
    m_TermInvertRead_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_TermInvertRead_PV);


    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(false,
            handlerRTN.handlerSTM.switchOnFunction,
            handlerRTN.handlerSTM.switchOffFunction,
            std::bind(&RoutingImpl::onStart, this),
            handlerRTN.handlerSTM.stopFunction,
            handlerRTN.handlerSTM.recoverFunction,
            handlerRTN.handlerSTM.allowStateChangeFunction,
            handlerRTN.handlerSTM.autoEnable));
    addChild(m_StateMachine);
}


template<typename T>
timespec RoutingImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void RoutingImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void RoutingImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_OnStartDelegate();
}

template<typename T>
nds::state_t RoutingImpl<T>::getState()
{
    return m_StateMachine->getLocalState();
}


///////////////////////////////////////////////////////////////
// Route Clocks getters
///////////////////////////////////////////////////////////////

template<typename T>
size_t RoutingImpl<T>::getClkSrc()
{
    std::int32_t ClkSrc;
    timespec timestamp;
    m_ClkSrc_PV->read(&timestamp, &ClkSrc);
    return (std::int32_t)ClkSrc;
}

template<typename T>
size_t RoutingImpl<T>::getClkDst()
{
    std::int32_t ClkDst;
    timespec timestamp;
    m_ClkDst_PV->read(&timestamp, &ClkDst);
    return (std::int32_t)ClkDst;
}

////////////////////////////////////////////////////////////////////////////
// Route Terminals getters
////////////////////////////////////////////////////////////////////////////

template<typename T>
size_t RoutingImpl<T>::getTermSrc()
{
    std::int32_t TermSrc;
    timespec timestamp;
    m_TermSrc_PV->read(&timestamp, &TermSrc);
    return (std::int32_t)TermSrc;
}

template<typename T>
size_t RoutingImpl<T>::getTermDst()
{
    std::int32_t TermDst;
    timespec timestamp;
    m_TermDst_PV->read(&timestamp, &TermDst);
    return (std::int32_t)TermDst;
}

template<typename T>
size_t RoutingImpl<T>::getTermSyncSet()
{
    std::int32_t TermSyncSet;
    timespec timestamp;
    m_TermSyncSet_PV->read(&timestamp, &TermSyncSet);
    return (std::int32_t)TermSyncSet;
}

template<typename T>
size_t RoutingImpl<T>::getTermInvertSet()
{
    std::int32_t TermInvertSet;
    timespec timestamp;
    m_TermInvertSet_PV->read(&timestamp, &TermInvertSet);
    return (std::int32_t)TermInvertSet;
}

///////////////////////////////////////////////////////////////
// Route Clocks setters
///////////////////////////////////////////////////////////////
template<typename T>
void RoutingImpl<T>::setClkSetStatus(const timespec& timestamp, const std::string& value)
{
    m_ClkSetStatus_PV->setValue(timestamp, value);
    m_ClkSetStatus_PV->push(timestamp, value);
}


template<typename T>
void RoutingImpl<T>::setClkSetCode(const timespec& timestamp, const std::int32_t& value)
{
    m_ClkSetCode_PV->setValue(timestamp, value);
    m_ClkSetCode_PV->push(timestamp, value);
}



////////////////////////////////////////////////////////////////////////////
// Read Clock Route Status setters
////////////////////////////////////////////////////////////////////////////
template<typename T>
void RoutingImpl<T>::setClkSrcRead(const timespec& timestamp, const std::int32_t& value)
{
    m_ClkSrcRead_PV->setValue(timestamp, value);
    m_ClkSrcRead_PV->push(timestamp, value);
}



////////////////////////////////////////////////////////////////////////////
// Route Terminals setters
////////////////////////////////////////////////////////////////////////////
template<typename T>
void RoutingImpl<T>::setTermSetStatus(const timespec& timestamp, const std::string& value)
{
    m_TermSetStatus_PV->setValue(timestamp, value);
    m_TermSetStatus_PV->push(timestamp, value);
}


template<typename T>
void RoutingImpl<T>::setTermSetCode(const timespec& timestamp, const std::int32_t& value)
{
    m_TermSetCode_PV->setValue(timestamp, value);
    m_TermSetCode_PV->push(timestamp, value);
}




////////////////////////////////////////////////////////////////////////////
// Read Routing Configuration setters
////////////////////////////////////////////////////////////////////////////
template<typename T>
void RoutingImpl<T>::setTermSrcRead(const timespec& timestamp, const std::int32_t& value)
{
    m_TermSrcRead_PV->setValue(timestamp, value);
    m_TermSrcRead_PV->push(timestamp, value);
}


template<typename T>
void RoutingImpl<T>::setTermSyncRead(const timespec& timestamp, const std::int32_t& value)
{
    m_TermSyncRead_PV->setValue(timestamp, value);
    m_TermSyncRead_PV->push(timestamp, value);
}


template<typename T>
void RoutingImpl<T>::setTermInvertRead(const timespec& timestamp, const std::int32_t& value)
{
    m_TermInvertRead_PV->setValue(timestamp, value);
    m_TermInvertRead_PV->push(timestamp, value);
}

template<typename T>
void RoutingImpl<T>::setState(const nds::state_t& newState)
{
    m_StateMachine->setState(newState);
}

template class RoutingImpl<std::string>;

}
