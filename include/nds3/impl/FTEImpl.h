/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFTEIMPL_H
#define NDSFTEIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/FTE.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class FTEImpl: public NodeImpl
{
public:

    FTEImpl(const std::string& name,
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
            autoEnable_t autoEnable);

    /**
     * @brief Constructs the FTE node by means of its structure of functions.
     * @param name Name of the node.
     * @param handlerFTE Structure with delegate functions that perform the required actions.
     */
    FTEImpl(const std::string& name, const FTEArgs_t& handlerFTE);


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

    std::int32_t getTerminalSet();
    std::int32_t getModeSet();
    timespec getStartTimeSet();
    timespec getStopTimeSet();
    std::int32_t getLevelSet();
    std::int32_t getPeriodNsecSet();
    std::int32_t getDutyCycleSet();

    std::int32_t getTerminalSuppress();
    std::int32_t getModeSuppress();
    std::int32_t getAllSuppress();
    timespec getStartTimeSuppress();

    std::int32_t getTerminalChgPeriod();
    std::int32_t getPeriodChgPeriod();

    void setSetStatus(const timespec& timestamp, const std::string& value);
    void setSetCode(const timespec& timestamp, const std::int32_t& value);

    void setSuppressStatus(const timespec& timestamp, const std::string& value);
    void setSuppressCode(const timespec& timestamp, const std::int32_t& value);

    void setChgPeriodStatus(const timespec& timestamp, const std::string& value);
    void setChgPeriodCode(const timespec& timestamp, const std::int32_t& value);

    void setPendingValue(const timespec& timestamp, const std::int32_t& value);
    void setPendingStatus(const timespec& timestamp, const std::string& value);
    void setPendingCode(const timespec& timestamp, const std::int32_t& value);

    void setMaximum(const timespec& timestamp, const std::int32_t& value);



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

    // PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    // Set FTE PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_TerminalSet_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_ModeSet_PV;
    std::shared_ptr<PVVariableOutImpl<timespec> > m_StartTimeSet_PV;
    std::shared_ptr<PVVariableOutImpl<timespec> > m_StopTimeSet_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_LevelSet_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_PeriodNsecSet_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_DutyCycleSet_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Set_PV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_SetStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SetCode_PV;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Suppress FTE PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_TerminalSuppress_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_ModeSuppress_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_AllSuppress_PV;
    std::shared_ptr<PVVariableOutImpl<timespec> > m_StartTimeSuppress_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Suppress_PV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_SuppressStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SuppressCode_PV;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Change Period FTE PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_TerminalChgPeriod_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_PeriodChgPeriod_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ChgPeriod_PV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_ChgPeriodStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ChgPeriodCode_PV;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Pending FTEs PV
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_TerminalPending_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PendingValue_PV;
    std::shared_ptr<PVVariableInImpl<std::string> > m_PendingStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PendingCode_PV;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Maximum FTEs PV
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Maximum_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

private:

    /**
    * Common source code to define the body of any constructor
    * @param handlerFTE Structure containing both compulsory and optional functions.
    */
    void constructorBody(const FTEArgs_t& handlerFTE);
};

}
#endif // NDSFTEIMPL_H

