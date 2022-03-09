/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESUPPIMPL_H
#define NDSTIMESUPPIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/triggerAndClk.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class TriggerAndClkImpl: public NodeImpl
{
public:
    TriggerAndClkImpl( const std::string& name,
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
            const nds::Routing<std::string>& _routingNode,
            autoEnable_t autoEnable);

        /**
         * @brief Constructs the TriggerAndClk node by means of its structure of functions.
         * @param name Name of the node.
         * @param handlerTrig Structure with delegate functions that perform the required actions.
         */
    TriggerAndClkImpl(const std::string& name, const TriggerAndClkArgs_t& handlerTrig);

    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    //getters
    std::int32_t getHWBlock();
    std::int32_t getDAQStartTimeDelay();
    std::int32_t getTriggPeriod();
    std::int32_t getLevel();
    std::int32_t getEdge();
    std::int32_t getChange();
    std::int32_t getMode();
    std::int32_t getClkDivider();
    std::int32_t getPreTrigSamples();
    std::int32_t getPostTrigSamples();
    std::int32_t getSyncMode();
    std::int32_t getPLLRefFreq();
    std::int32_t getPLLRefDiv();
    std::int32_t getPLLRefMult();
    std::int32_t getPLLRefDivALL();

    //Setters
    void setHWBlockRBV(const timespec& timestamp, const std::int32_t& value);
    void setDAQStartTimeDelayRBV(const timespec& timestamp, const std::int32_t& value);
    void setTriggPeriodRBV(const timespec& timestamp, const std::int32_t& value);
    void setLevelRBV(const timespec& timestamp, const std::int32_t& value);
    void setEdgeRBV(const timespec& timestamp, const std::int32_t& value);
    void setChangeRBV(const timespec& timestamp, const std::int32_t& value);
    void setModeRBV(const timespec& timestamp, const std::int32_t& value);
    void setClkDividerRBV(const timespec& timestamp, const std::int32_t& value);
    void setPreTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value);
    void setPostTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value);
    void setTrigLoadStatus(const timespec& timestamp, const std::string& value);
    void setTrigLoadCode(const timespec& timestamp, const std::int32_t& value);

    void setSyncModeRBV(const timespec& timestamp, const std::int32_t& value);
    void setPLLRefFreqRBV(const timespec& timestamp, const std::int32_t& value);
    void setPLLRefDivRBV(const timespec& timestamp, const std::int32_t& value);
    void setPLLRefMultRBV(const timespec& timestamp, const std::int32_t& value);
    void setPLLRefDivALLRBV(const timespec& timestamp, const std::int32_t& value);
    void setPLLLoadStatus(const timespec& timestamp, const std::string& value);
    void setPLLLoadCode(const timespec& timestamp, const std::int32_t& value);

    void setEnableDisablePLLRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

protected:

    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;
    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Start time. Retrieved via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    std::shared_ptr<StateMachineImpl> m_StateMachine;
    //std::shared_ptr<RoutingImpl<T>> m_RoutingNode;



    //Trigger Configuration PVs
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_HWBlock_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_HWBlock_PVRBV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_SetSW_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_DAQStartTimeDelay_PV;
    std::shared_ptr<PVVariableInImpl <std::int32_t> > m_DAQStartTimeDelay_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_TriggPeriod_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_TriggPeriod_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Level_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_Level_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Edge_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_Edge_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Change_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_Change_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Mode_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_Mode_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_ClkDivider_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_ClkDivider_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_preTrigSamples_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_preTrigSamples_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_postTrigSamples_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_postTrigSamples_RBVPV;

    //HW Trigger Programming PVs
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_LoadTrigConf_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ResetTrigConf_PV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_trigLoadStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_trigLoadCode_PV;

    //Timebase and sampling clocks for HW functional blocks
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_syncMode_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_syncMode_PVRBV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_PLLSyncSet_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_PLLRefFreq_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PLLRefFreq_PVRBV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_PLLRefDiv_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PLLRefDiv_PVRBV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_PLLRefMult_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PLLRefMult_PVRBV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_PLLRefDivALL_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PLLRefDivALL_PVRBV;

    std::shared_ptr<PVVariableInImpl<std::string> >   m_PLLLoadStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_PLLLoadCode_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableDisablePLL_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> >  m_EnableDisablePLL_RBVPV;


private:
    /**
     * @brief Common source code to define the body of any constructor.
     * @param handlerTrig Structure containing both compulsory and optional functions.
     */
    void constructorBody(const TriggerAndClkArgs_t& handlerTrig);

    /**
     *@brief Reference to the routing node required for triggering purposes.
     * Note that it is the same reference that is assigned to the TriggerAndClk
     * class.
     * @internal
     * By the moment, this member variable is never used in this class,
     * but it is added just to be consistent in constructor of classes
     * TriggerAndClk and TriggerAndClkImpl.
     * @endinternal
     */
    const nds::Routing<std::string>& routingNode;


};

}
#endif // NDSTIMESUPPIMPL_H

