/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSROUTINGIMPL_H
#define NDSROUTINGIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/routing.h"

#include <vector>

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class RoutingImpl: public NodeImpl
{
public:
    RoutingImpl(const std::string& name,
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
            );

    /**
     * @brief Constructs the Routing node by means of its structure of functions.
     * @param name Name of the node.
     * @param handlerRTN Structure with delegate functions that perform the required actions.
     */
    RoutingImpl(const std::string& name, const RoutingArgs_t& handlerRTN);

    /**
     * @brief Specifies the function to call to get the routing start timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    size_t getClkSrc();
    size_t getClkDst();

    size_t getTermSrc();
    size_t getTermDst();
    size_t getTermSyncSet();
    size_t getTermInvertSet();

    void setClkSetStatus(const timespec& timestamp, const std::string& value);
    void setClkSetCode(const timespec& timestamp, const std::int32_t& value);
    void setClkSrcRead(const timespec& timestamp, const std::int32_t& value);

    void setTermSetStatus(const timespec& timestamp, const std::string& value);
    void setTermSetCode(const timespec& timestamp, const std::int32_t& value);
    void setTermSrcRead(const timespec& timestamp, const std::int32_t& value);
    void setTermSyncRead(const timespec& timestamp, const std::int32_t& value);
    void setTermInvertRead(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Returns the timestamp at the start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the start time
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
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Routing start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

    ////////////////////////////////////////////////////////////////////////////
    // Route Clocks PVs
    ////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_ClkSrc_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_ClkDst_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ClkSet_PV;
    std::shared_ptr<PVVariableInImpl<std::string> > m_ClkSetStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ClkSetCode_PV;

    ////////////////////////////////////////////////////////////////////////////
    // Read Clock Route Status PVs
    ////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ClkDstRead_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ClkSrcRead_PV;

    ////////////////////////////////////////////////////////////////////////////
    // Route Terminals PVs
    ////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_TermSrc_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_TermDst_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_TermSyncSet_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_TermInvertSet_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TermSet_PV;
    std::shared_ptr<PVVariableInImpl<std::string> > m_TermSetStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TermSetCode_PV;

    ////////////////////////////////////////////////////////////////////////////
    // Read Routing Configuration PVs
    ////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TermDstRead_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TermSrcRead_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TermSyncRead_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TermInvertRead_PV;

private:

    /**
    * Common source code to define the body of any constructor
    * @param handlerRTN Structure containing both compulsory and optional functions.
    */
    void constructorBody(const RoutingArgs_t& handlerRTN);

};

}
#endif // NDSROUTINGIMPL_H

