/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/triggerAndClk.h"
#include "nds3/impl/triggerAndClkImpl.h"
#include "nds3/routing.h"


namespace nds
{


///////////////////////////////////////// TRIGGER SUPPORT /////////////////////////////////////////

template <typename T>
TriggerAndClk<T>::TriggerAndClk(): Node()
{
}

template <typename T>
TriggerAndClk<T>::TriggerAndClk(
        const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        writerInt32_t PV_SetSW_Writer,
        writerInt32_t PV_LoadTrigConf_Writer,
        writerInt32_t PV_ResetTrigConf_Writer,
        writerInt32_t PV_PLLSyncSET_Writer,
        writerInt32_t PV_EnableDisablePLL_Writer,
        nds::Routing<std::string> routingNode,
        autoEnable_t autoEnable):    //Routing PV Delegate
        Node(std::shared_ptr<TriggerAndClkImpl<T> >(new TriggerAndClkImpl<T>(name,
                switchOnFunction,
                switchOffFunction,
                startFunction,
                stopFunction,
                recoverFunction,
                allowStateChangeFunction,
                PV_SetSW_Writer,
                PV_LoadTrigConf_Writer,
                PV_ResetTrigConf_Writer,
                PV_PLLSyncSET_Writer,
                PV_EnableDisablePLL_Writer,
                routingNode,
                autoEnable))),m_Routing(routingNode)
{
    addChild(m_Routing);
}


template <typename T>
TriggerAndClk<T>::TriggerAndClk(const std::string& name,
                                const TriggerAndClkArgs_t& handlerTrig) :   //Routing PV Delegate
                                Node(std::shared_ptr<TriggerAndClkImpl<T> >(new TriggerAndClkImpl<T>(name,handlerTrig))),
                                m_Routing(handlerTrig.routingNode)
{
  addChild(m_Routing);
}

template <typename T>
TriggerAndClk<T>::TriggerAndClk(const TriggerAndClk<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation)),m_Routing(right.m_Routing)
{
}

template <typename T>
TriggerAndClk<T>& TriggerAndClk<T>::operator=(const TriggerAndClk<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    m_Routing=right.m_Routing;
    return *this;
}

template <typename T>
void TriggerAndClk<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec TriggerAndClk<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
nds::state_t TriggerAndClk<T>::getState()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getState();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getHWBlock()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getHWBlock();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getDAQStartTimeDelay()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getDAQStartTimeDelay();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getTriggPeriod()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getTriggPeriod();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getLevel()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getLevel();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getEdge()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getEdge();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getChange()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getChange();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getMode()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getMode();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getClkDivider()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getClkDivider();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getPreTrigSamples()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getPreTrigSamples();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getPostTrigSamples()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getPostTrigSamples();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getSyncMode()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getSyncMode();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getPLLRefFreq()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getPLLRefFreq();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getPLLRefDiv()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getPLLRefDiv();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getPLLRefMult()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getPLLRefMult();
}

template <typename T>
std::int32_t TriggerAndClk<T>::getPLLRefDivALL()
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->getPLLRefDivALL();
}


//Setters
template <typename T>
void TriggerAndClk<T>::setHWBlockRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setHWBlockRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setDAQStartTimeDelayRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setDAQStartTimeDelayRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setTriggPeriodRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setTriggPeriodRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setLevelRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setLevelRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setEdgeRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setEdgeRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setChangeRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setChangeRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setModeRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setModeRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setClkDividerRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setClkDividerRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPreTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPreTrigSamplesRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPostTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPostTrigSamplesRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setTrigLoadStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setTrigLoadStatus(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setTrigLoadCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setTrigLoadCode(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setSyncModeRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setSyncModeRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPLLRefFreqRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPLLRefFreqRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPLLRefDivRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPLLRefDivRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPLLRefMultRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPLLRefMultRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPLLRefDivALLRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPLLRefDivALLRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPLLLoadStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPLLLoadStatus(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setPLLLoadCode(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setPLLLoadCode(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setEnableDisablePLLRBV(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setEnableDisablePLLRBV(timestamp, value);
}

template <typename T>
void TriggerAndClk<T>::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<TriggerAndClkImpl<T> >(m_pImplementation)->setState(newState);
}


template class TriggerAndClk<std::vector<timespec>>;

///////////////////////////////////////// END TRIGGER SUPPORT /////////////////////////////////////////


}
