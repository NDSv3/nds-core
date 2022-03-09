/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/FTEImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
FTEImpl<T>::FTEImpl(const std::string& name,
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
        NodeImpl(name, nodeType_t::dataSourceChannel),
        m_OnStartDelegate(startFunction),
        m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    FTEArgs_t handlerFTE = FTEArgs_t(switchOnFunction,
                                     switchOffFunction,
                                     startFunction,
                                     stopFunction,
                                     recoverFunction,
                                     allowStateChangeFunction,
                                     PV_Set_Writer,
                                     PV_Suppress_Writer,
                                     PV_ChgPeriod_Writer,
                                     PV_PendingValue_Writer,
                                     autoEnable);
    constructorBody(handlerFTE);
}

template<typename T>
FTEImpl<T>::FTEImpl(const std::string& name, const FTEArgs_t& handlerFTE):
        NodeImpl(name, nodeType_t::dataSourceChannel),
        m_OnStartDelegate(handlerFTE.handlerSTM.startFunction),
        m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    constructorBody(handlerFTE);
}

template<typename T>
inline void FTEImpl<T>::constructorBody(const FTEArgs_t& handlerFTE) {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Set FTE PVs
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    timespec tst;
    tst.tv_nsec=0;
    tst.tv_sec=0;
    m_TerminalSet_PV.reset(new PVVariableOutImpl<std::int32_t>("TerminalSet"));
    m_TerminalSet_PV->setDescription("Terminal to set FTE");
    m_TerminalSet_PV->setScanType(scanType_t::passive, 0);
    m_TerminalSet_PV->write(tst,handlerFTE.m_TerminalSet_Init); //Initial value
    addChild(m_TerminalSet_PV);

    m_ModeSet_PV.reset(new PVVariableOutImpl<std::int32_t>("ModeSet"));
    m_ModeSet_PV->setDescription("Mode (Single, Pulse, Clk, InmLVL)");
    m_ModeSet_PV->setScanType(scanType_t::passive, 0);
    m_ModeSet_PV->write(tst,handlerFTE.m_ModeSet_Init);
    addChild(m_ModeSet_PV);

    m_StartTimeSet_PV.reset(new PVVariableOutImpl<timespec>("StartTimeSet"));
    m_StartTimeSet_PV->setDescription("Start Time of FTE");
    m_StartTimeSet_PV->setScanType(scanType_t::passive, 0);
    m_StartTimeSet_PV->write(tst,handlerFTE.m_StartTimeSet_Init);
    addChild(m_StartTimeSet_PV);

    m_StopTimeSet_PV.reset(new PVVariableOutImpl<timespec>("StopTimeSet"));
    m_StopTimeSet_PV->setDescription("Stop Time of FTE");
    m_StopTimeSet_PV->setScanType(scanType_t::interrupt, 0);
    m_StopTimeSet_PV->write(tst, handlerFTE.m_StopTimeSet_Init);
    addChild(m_StopTimeSet_PV);

    m_LevelSet_PV.reset(new PVVariableOutImpl<std::int32_t>("LevelSet"));
    m_LevelSet_PV->setDescription("Signal level of FTE");
    m_LevelSet_PV->setScanType(scanType_t::interrupt, 0);
    m_LevelSet_PV->write(tst, handlerFTE.m_LevelSet_Init);
    addChild(m_LevelSet_PV);

    m_PeriodNsecSet_PV.reset(new PVVariableOutImpl<std::int32_t>("PeriodNsecSet"));
    m_PeriodNsecSet_PV->setDescription("Period in Nanoseconds");
    m_PeriodNsecSet_PV->setScanType(scanType_t::interrupt, 0);
    m_PeriodNsecSet_PV->write(tst, handlerFTE.m_PeriodNsecSet_Init);
    addChild(m_PeriodNsecSet_PV);

    m_DutyCycleSet_PV.reset(new PVVariableOutImpl<std::int32_t>("DutyCycleSet"));
    m_DutyCycleSet_PV->setDescription("Duty Cycle percentage");
    m_DutyCycleSet_PV->setScanType(scanType_t::interrupt, 0);
    m_DutyCycleSet_PV->write(tst, handlerFTE.m_DutyCycleSet_Init);
    addChild(m_DutyCycleSet_PV);

    if (handlerFTE.PV_Set_Initializer) {
        m_Set_PV.reset(new PVDelegateOutImpl<std::int32_t>("Set",
                                                           handlerFTE.PV_Set_Writer,
                                                           handlerFTE.PV_Set_Initializer));
    } else {
        m_Set_PV.reset(new PVDelegateOutImpl<std::int32_t>("Set",handlerFTE.PV_Set_Writer));
    }
    m_Set_PV->setDescription("Set FTE");
    m_Set_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Set_PV);

    m_SetStatus_PV.reset(new PVVariableInImpl<std::string>("SetStatus"));
    m_SetStatus_PV->setDescription("Report on FTE configuration");
    m_SetStatus_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SetStatus_PV);

    m_SetCode_PV.reset(new PVVariableInImpl<std::int32_t>("SetCode"));
    m_SetCode_PV->setDescription("Code of success/error setting FTE configuration");
    m_SetCode_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SetCode_PV);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Suppress FTE PVs
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_TerminalSuppress_PV.reset(new PVVariableOutImpl<std::int32_t>("TerminalSuppress"));
    m_TerminalSuppress_PV->setDescription("Terminal to suppress FTE");
    m_TerminalSuppress_PV->setScanType(scanType_t::interrupt, 0);
    m_TerminalSuppress_PV->write(tst, handlerFTE.m_TerminalSuppress_Init);
    addChild(m_TerminalSuppress_PV);

    m_ModeSuppress_PV.reset(new PVVariableOutImpl<std::int32_t>("ModeSuppress"));
    m_ModeSuppress_PV->setDescription("Mode of suppress FTE/clock");
    m_ModeSuppress_PV->setScanType(scanType_t::interrupt, 0);
    m_ModeSuppress_PV->write(tst, handlerFTE.m_ModeSuppress_Init);
    addChild(m_ModeSuppress_PV);

    m_AllSuppress_PV.reset(new PVVariableOutImpl<std::int32_t>("AllSuppress"));
    m_AllSuppress_PV->setDescription("Suppress one or Suppress all FTEs");
    m_AllSuppress_PV->setScanType(scanType_t::interrupt, 0);
    m_AllSuppress_PV->write(tst, handlerFTE.m_AllSuppress_Init);
    addChild(m_AllSuppress_PV);

    m_StartTimeSuppress_PV.reset(new PVVariableOutImpl<timespec>("StartTimeSuppress"));
    m_StartTimeSuppress_PV->setDescription("Start Time on FTE suppression");
    m_StartTimeSuppress_PV->setScanType(scanType_t::interrupt, 0);
    m_StartTimeSuppress_PV->write(tst, handlerFTE.m_StartTimeSuppress_Init);
    addChild(m_StartTimeSuppress_PV);

    if (handlerFTE.PV_Suppress_Initializer) {
        m_Suppress_PV.reset(new PVDelegateOutImpl<std::int32_t>("Suppress",
                                                                handlerFTE.PV_Suppress_Writer,
                                                                handlerFTE.PV_Suppress_Initializer));
    } else {
        m_Suppress_PV.reset(new PVDelegateOutImpl<std::int32_t>("Suppress",handlerFTE.PV_Suppress_Writer));
    }
    m_Suppress_PV->setDescription("Suppress");
    m_Suppress_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Suppress_PV);

    m_SuppressStatus_PV.reset(new PVVariableInImpl<std::string>("SuppressStatus"));
    m_SuppressStatus_PV->setDescription("Report on FTE suppression");
    m_SuppressStatus_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SuppressStatus_PV);

    m_SuppressCode_PV.reset(new PVVariableInImpl<std::int32_t>("SuppressCode"));
    m_SuppressCode_PV->setDescription("Code of success/error on FTE suppression");
    m_SuppressCode_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SuppressCode_PV);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Change Period FTE PVs
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_TerminalChgPeriod_PV.reset(new PVVariableOutImpl<std::int32_t>("TerminalChgPeriod"));
    m_TerminalChgPeriod_PV->setDescription("Terminal to change the period of FTE");
    m_TerminalChgPeriod_PV->setScanType(scanType_t::interrupt, 0);
    m_TerminalChgPeriod_PV->write(tst,handlerFTE.m_TerminalChgPeriod_Init);
    addChild(m_TerminalChgPeriod_PV);

    m_PeriodChgPeriod_PV.reset(new PVVariableOutImpl<std::int32_t>("PeriodChgPeriod"));
    m_PeriodChgPeriod_PV->setDescription("Period in nanoseconds to change on FTE");
    m_PeriodChgPeriod_PV->setScanType(scanType_t::interrupt, 0);
    m_PeriodChgPeriod_PV->write(tst, handlerFTE.m_PeriodChgPeriod_Init);
    addChild(m_PeriodChgPeriod_PV);

    if (handlerFTE.PV_ChgPeriod_Initializer) {
        m_ChgPeriod_PV.reset(new PVDelegateOutImpl<std::int32_t>("ChgPeriod",
                                                                 handlerFTE.PV_ChgPeriod_Writer,
                                                                 handlerFTE.PV_ChgPeriod_Initializer));
    } else {
        m_ChgPeriod_PV.reset(new PVDelegateOutImpl<std::int32_t>("ChgPeriod",handlerFTE.PV_ChgPeriod_Writer));
    }
    m_ChgPeriod_PV->setDescription("ChgPeriod");
    m_ChgPeriod_PV->setScanType(scanType_t::passive, 0);
    addChild(m_ChgPeriod_PV);

    m_ChgPeriodStatus_PV.reset(new PVVariableInImpl<std::string>("ChgPeriodStatus"));
    m_ChgPeriodStatus_PV->setDescription("Report on FTE clock period change");
    m_ChgPeriodStatus_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ChgPeriodStatus_PV);

    m_ChgPeriodCode_PV.reset(new PVVariableInImpl<std::int32_t>("ChgPeriodCode"));
    m_ChgPeriodCode_PV->setDescription("Code of success/error on FTE changing clock period");
    m_ChgPeriodCode_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ChgPeriodCode_PV);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Pending FTEs PV
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (handlerFTE.PV_PendingValue_Initializer) {
        m_TerminalPending_PV.reset(new PVDelegateOutImpl<std::int32_t>("TerminalPending",
                                                                       handlerFTE.PV_PendingValue_Writer,
                                                                       handlerFTE.PV_PendingValue_Initializer));
    } else {
        m_TerminalPending_PV.reset(new PVDelegateOutImpl<std::int32_t>("TerminalPending",handlerFTE.PV_PendingValue_Writer));
    }
    m_TerminalPending_PV->setDescription("Terminal Pending");
    m_TerminalPending_PV->setScanType(scanType_t::passive, 0);
    addChild(m_TerminalPending_PV);

    m_PendingValue_PV.reset(new PVVariableInImpl<std::int32_t>("PendingValue"));
    m_PendingValue_PV->setDescription("Pending FTEs in the terminal");
    m_PendingValue_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_PendingValue_PV);

    m_PendingStatus_PV.reset(new PVVariableInImpl<std::string>("PendingStatus"));
    m_PendingStatus_PV->setDescription("Report on getting pending FTEs");
    m_PendingStatus_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_PendingStatus_PV);

    m_PendingCode_PV.reset(new PVVariableInImpl<std::int32_t>("PendingCode"));
    m_PendingCode_PV->setDescription("Code of success/error on getting pending FTEs");
    m_PendingCode_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_PendingCode_PV);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Maximum FTEs PV
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_Maximum_PV.reset(new PVVariableInImpl<std::int32_t>("Maximum"));
    m_Maximum_PV->setDescription("Maximum FTEs");
    m_Maximum_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Maximum_PV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(false,
            handlerFTE.handlerSTM.switchOnFunction,
            handlerFTE.handlerSTM.switchOffFunction,
            std::bind(&FTEImpl::onStart, this),
            handlerFTE.handlerSTM.stopFunction,
            handlerFTE.handlerSTM.recoverFunction,
            handlerFTE.handlerSTM.allowStateChangeFunction,
            handlerFTE.handlerSTM.autoEnable));
    addChild(m_StateMachine);
}

template<typename T>
timespec FTEImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void FTEImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void FTEImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_OnStartDelegate();
}

template<typename T>
nds::state_t FTEImpl<T>::getState()
{
    return m_StateMachine->getLocalState();
}

///////////////////////////////////////////////////////////////
// Set FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalSet()
{
    std::int32_t terminalSet;
    timespec timestamp;
    m_TerminalSet_PV->read(&timestamp, &terminalSet);
    return terminalSet;
}


template<typename T>
std::int32_t FTEImpl<T>::getModeSet()
{
    std::int32_t modeSet;
    timespec timestamp;
    m_ModeSet_PV->read(&timestamp, &modeSet);
    return modeSet;
}

template<typename T>
timespec FTEImpl<T>::getStartTimeSet()
{
    timespec startTimeSet;
    timespec timestamp;
    m_StartTimeSet_PV->read(&timestamp, &startTimeSet);
    return startTimeSet;
}

template<typename T>
timespec FTEImpl<T>::getStopTimeSet()
{
    timespec stopTimeSet;
    timespec timestamp;
    m_StopTimeSet_PV->read(&timestamp, &stopTimeSet);
    return stopTimeSet;
}

template<typename T>
std::int32_t FTEImpl<T>::getLevelSet()
{
    std::int32_t levelSet;
    timespec timestamp;
    m_LevelSet_PV->read(&timestamp, &levelSet);
    return levelSet;
}

template<typename T>
std::int32_t FTEImpl<T>::getPeriodNsecSet()
{
    std::int32_t periodNsecSet;
    timespec timestamp;
    m_PeriodNsecSet_PV->read(&timestamp, &periodNsecSet);
    return periodNsecSet;
}


template<typename T>
std::int32_t FTEImpl<T>::getDutyCycleSet()
{
    std::int32_t dutyCycleSet;
    timespec timestamp;
    m_DutyCycleSet_PV->read(&timestamp, &dutyCycleSet);
    return dutyCycleSet;
}

///////////////////////////////////////////////////////////////
// Suppress FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalSuppress()
{
    std::int32_t terminalSuppress;
    timespec timestamp;
    m_TerminalSuppress_PV->read(&timestamp, &terminalSuppress);
    return terminalSuppress;
}


template<typename T>
std::int32_t FTEImpl<T>::getModeSuppress()
{
    std::int32_t modeSuppress;
    timespec timestamp;
    m_ModeSuppress_PV->read(&timestamp, &modeSuppress);
    return modeSuppress;
}

template<typename T>
std::int32_t FTEImpl<T>::getAllSuppress()
{
    std::int32_t allSuppress;
    timespec timestamp;
    m_AllSuppress_PV->read(&timestamp, &allSuppress);
    return allSuppress;
}

template<typename T>
timespec FTEImpl<T>::getStartTimeSuppress()
{
    timespec startTimeSuppress;
    timespec timestamp;
    m_StartTimeSuppress_PV->read(&timestamp, &startTimeSuppress);
    return startTimeSuppress;
}

///////////////////////////////////////////////////////////////
// ChgPeriod FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalChgPeriod()
{
    std::int32_t terminalChgPeriod;
    timespec timestamp;
    m_TerminalChgPeriod_PV->read(&timestamp, &terminalChgPeriod);
    return terminalChgPeriod;
}


template<typename T>
std::int32_t FTEImpl<T>::getPeriodChgPeriod()
{
    std::int32_t periodChgPeriod;
    timespec timestamp;
    m_PeriodChgPeriod_PV->read(&timestamp, &periodChgPeriod);
    return periodChgPeriod;
}

///////////////////////////////////////////////////////////////
// Set FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setSetStatus(const timespec& timestamp, const std::string& value)
{
    m_SetStatus_PV->setValue(timestamp, value);
    m_SetStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSetCode(const timespec& timestamp, const std::int32_t& value)
{
    m_SetCode_PV->setValue(timestamp, value);
    m_SetCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Suppress FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setSuppressStatus(const timespec& timestamp, const std::string& value)
{
    m_SuppressStatus_PV->setValue(timestamp, value);
    m_SuppressStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSuppressCode(const timespec& timestamp, const std::int32_t& value)
{
    m_SuppressCode_PV->setValue(timestamp, value);
    m_SuppressCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// ChgPeriod FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setChgPeriodStatus(const timespec& timestamp, const std::string& value)
{
    m_ChgPeriodStatus_PV->setValue(timestamp, value);
    m_ChgPeriodStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setChgPeriodCode(const timespec& timestamp, const std::int32_t& value)
{
    m_ChgPeriodCode_PV->setValue(timestamp, value);
    m_ChgPeriodCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Pending FTE setter
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setPendingValue(const timespec& timestamp, const std::int32_t& value)
{
    m_PendingValue_PV->setValue(timestamp, value);
    m_PendingValue_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setPendingStatus(const timespec& timestamp, const std::string& value)
{
    m_PendingStatus_PV->setValue(timestamp, value);
    m_PendingStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setPendingCode(const timespec& timestamp, const std::int32_t& value)
{
    m_PendingCode_PV->setValue(timestamp, value);
    m_PendingCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Maximum FTE setter
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setMaximum(const timespec& timestamp, const std::int32_t& value)
{
    m_Maximum_PV->setValue(timestamp, value);
    m_Maximum_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setState(const nds::state_t& newState)
{
    m_StateMachine->setState(newState);
}
template class FTEImpl<std::string>;
}
