/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSSTATEMACHINE_H
#define NDSSTATEMACHINE_H

/**
 * @file stateMachine.h
 *
 * @brief Defines the nds::StateMachine class.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include <cstdint>

#include "nds3/node.h"
#include "nds3/definitions.h"

namespace nds
{

/**
 * @brief Type defined to gather the arguments required by @ref StateMachine constructors.
 * It can be used in auxiliary methods or additional nodes to simplify its number
 * of arguments required.
 */
struct StateMachineArgs_t {
    /**
     * @brief Flag to be set as @c true to run the state transition in a parallel thread.
     *      If @c true then the state transitions are executed in a dedicated thread, if @c false then the state transitions block
     */
    const bool bAsync;
    /**
     * @brief Function to run the transition @c state_t::off -> @c state_t::initializing -> @c state_t::on
     *      Function to be called to switch the node state_t::on. @n
     *      The function is guaranteed to be called only when the current state is state_t::off.
     *      The state machine sets the state to state_t::initializing before
     *      calling the function and to state_t::on after the function returns.
     */
    const stateChange_t switchOnFunction;
    /**
     * @brief Function to run the transition @c state_t::on -> @c state_t::switchingOff -> @c state_t::off
     *      Function to be called to switch the node @c state_t::off. @n
     *      The function is guaranteed to be called only when the current state
     *      is @c state_t::on. The state machine sets the state to state_t::switchingOff before
     *      calling the function and to @c state_t::off after the function returns.
     */
    const stateChange_t switchOffFunction;
    /**
     * @brief Function to run the transition @c state_t::on -> @c state_t::starting -> @c state_t::running
     *      Function to be called to switch to the @c state_t::running state. @n
     *      The function is guaranteed to be called only when the current state
     *      is @c state_t::on. The state machine sets the state to @c state_t::starting before
     *      calling the function and to @c state_t::running after the function returns.
     */
    const stateChange_t startFunction;
    /**
     * @brief Function to run the transition @c state_t::running -> @c state_t::stopping -> @c state_t::on
     *      Function to be called to switch to the @c state_t::off state. @n
     *      The function is guaranteed to be called only when the current state
     *      is @c state_t::running. The state machine sets the state to @c state_t::switchingOff
     *      before calling the function and to @c state_t::off after the function returns
     */
    const stateChange_t stopFunction;
    /**
     * @brief Function to run the transition @c state_t::fault -> @c state_t::off
     *      Function to be called to switch the state from @c state_t::fault to @c state_t::off. @n
     *      The function is guaranteed to be called only when the current state
     *      is @c state_t::fault. The state machine sets the state to @c state_t::switchingOff
     *      before calling the function and to @c state_t::off after the function returns.
     */
    const stateChange_t recoverFunction;
    /**
     * @brief Function to allow or not the transition between states.
     *      Function to be called before every state switching to receive a confirmation
     *      that the state switch is allowed. @n
     *      The function is called only after other internal checks clear
     *      the state switch.
     */
    const allowChange_t allowStateChangeFunction;

    /**
     * @brief Variable to indicate which level will follow the child to its parent STM
     */
    autoEnable_t autoEnable;

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param _bAsync Boolean to be set in @ref StateMachineArgs_t.bAsync.
     * @param _switchOnFunction Switch-on function to be set in @ref StateMachineArgs_t.switchOnFunction.
     * @param _switchOffFunction Switch-off function to be set in @ref StateMachineArgs_t.switchOffFunction.
     * @param _startFunction Start function to be set in @ref StateMachineArgs_t.startFunction.
     * @param _stopFunction Stop function to be set in @ref StateMachineArgs_t.stopFunction.
     * @param _recoverFunction Recover function to be set in @ref StateMachineArgs_t.recoverFunction.
     * @param _allowStateChangeFunction Checking state transitions function to be set in @ref StateMachineArgs_t.allowStateChangeFunction.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    StateMachineArgs_t(bool _bAsync, stateChange_t _switchOnFunction,
            stateChange_t _switchOffFunction, stateChange_t _startFunction,
            stateChange_t _stopFunction, stateChange_t _recoverFunction,
            allowChange_t _allowStateChangeFunction,
            autoEnable_t autoEnable=autoEnable_t::none):
                bAsync(_bAsync),
                switchOnFunction(_switchOnFunction),
                switchOffFunction(_switchOffFunction),
                startFunction(_startFunction),
                stopFunction(_stopFunction),
                recoverFunction(_recoverFunction),
                allowStateChangeFunction(_allowStateChangeFunction),
                autoEnable(autoEnable){}
};

/**
 * @brief Represents a state machine to be attached to a node.
 *
 * Attach the state machine to a node by using Node::addChild().
 *
 * The state machine provides a <b>local state</b> and a <b>global state</b>:
 * - the local state represents the state of a single state machine
 * - the global state takes into account all the state machines attached to
 *   the children nodes (the state with the higher priority)
 *
 * In EPICS the state machine PVs will be available as "StateMachine-setState"
 *  (set the state) and "StateMachine-getState" (to retrieve the state).
 * The PV "StateMachine-globalState" represents the global state (state
 *  with higher priority among the current node and all its children).
 *
 * Transitions between the different states can happen on the same
 *  thread that requested the transition or on a different thread when
 *  the StateMachine is created with the bAsync flag set to true.
 *
 * The StateMachine node also registers the following commands on the
 *  state machine node itself and on the parent node:
 * - switchOn (causes a transition to the state ON)
 * - start (causes a transition to the state RUNNING)
 * - stop (causes a transition to the state ON)
 * - switchOff (causes a transition to the state OFF)
 * - recover (causes a transition to the state OFF)
 */
class NDS3_API StateMachine: public Node
{

public:
    /**
     * @brief Initializes an empty state machine node.
     *
     * You must assign a valid StateMachine node before calling Node::initialize() on the root node.
     */
    StateMachine();

    /**
     * @brief Construct the state machine.
     *
     * @param bAsync                   if true then the state transitions are executed
     *                                  in a dedicated thread, if false then the state
     *                                  transitions block
     * @param switchOnFunction         function to be called to switch the node state_t::on.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is state_t::off.
     *                                 The state machine sets the state to state_t::initializing before
     *                                  calling the function and to state_t::on after the function returns.
     * @param switchOffFunction        function to be called to switch the node state_t::off.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is state_t::on.
     *                                 The state machine sets the state to state_t::switchingOff before
     *                                  calling the function and to state_t::off after the function returns.
     * @param startFunction            function to be called to switch to the state_t::running state.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is state_t::on.
     *                                 The state machine sets the state to state_t::starting before
     *                                  calling the function and to state_t::running after the function returns.
     * @param stopFunction             function to be called to switch to the state_t::off state.
     *                                 The function is guaranteed to be called only when the current
     *                                  state is state_t::running.
     *                                 The state machine sets the state to state_t::switchingOff before
     *                                  calling the function and to state_t::off after the function returns
     * @param recoverFunction          function to be called to switch the state from state_t::fault to
     *                                  state_t::off.
     *                                 The function is guaranteed to be called only when the current
     *                                  state is state_t::fault.
     *                                 The state machine sets the state to state_t::switchingOff before
     *                                  calling the function and to state_t::off after the function returns
     * @param allowStateChangeFunction function to be called before every state switching to receive
     *                                  a confirmation that the state switch is allowed.
     *                                 The function is called only after other internal checks clear
     *                                  the state switch
     * @param autoState Parameter value
     */
    StateMachine(bool bAsync,
                 stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction,
                 autoEnable_t autoState=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the state machine.
     * @param handlerSTM Structure with the arguments required by the state machine.
     * See @ref StateMachineArgs_t for further details.
     */
    StateMachine(const StateMachineArgs_t& handlerSTM);

    /**
     * @brief Instruct the state machine to change the local state to the requested one.
     *
     * The state machine will first check that the change from the current state to
     *  the requested state is legal: if the change is not legal then a StateMachineNoSuchTransition
     *  exception is thrown.
     *
     * Then the delegate function allowStateChangeFunction defined in the state machine constructor
     *  will be called: if the function returns false then a StateMachineTransitionDenied exception
     *  will be thrown.
     *
     * The state will be changed to an intermediate states while the state change delegate function is
     *  executed. If the function executing the state change fails then it can throw one of the following
     *  exceptions:
     * - StateMachineRollBack: the state change failed and the state should go back to the initial one
     *   (the state active before the transition was requested)
     * - any exception: the state switches to Fault.
     *
     * @warning If the state machine is asynchronous (parameter bAsync set to true in the constructor)
     *           then exceptions thrown in state change functions are silenced and only the log messages
     *           are sent to the control system.
     *
     * @param newState the new desidered local state. Intermediate states can be set only by
     *                 the state machine. The user can set only the following states:
     *                 - state_t::off
     *                 - state_t::on
     *                 - state_t::running
     *                 - state_t::fault
     */
    void setState(const state_t newState);

    /**
     * @brief Returns the local state of the state machine.
     *
     * The local state does not reflect the state of the children nodes.
     *
     * @return the local state
     */
    state_t getLocalState();

    /**
     * @brief Returns the global state of the node to which this state machine is
     *        attached.
     *
     * The global state is the state with higher priority amongs the local state and the states
     *  of the children nodes of the node that owns this state machine.
     *
     * @return the global state
     */
    state_t getGlobalState();

    /**
     * @brief Returns the lowest global state
     *
     * @return the lowest children state
     */
    state_t getLowestGlobalState();

    /**
     * @brief Returns the highest global state
     *
     * @return the highest children state
     */
    state_t getHighestGlobalState();

    /**
     * @brief Returns the lowest state of all the children of the node
     *
     * @return the lowest children state
     */
    state_t getLowestChildState();

    /**
     * @brief Returns the highest state of all the children of the node.
     *
     * @return the highest children state
     */
    state_t getHighestChildState();

    /**
     * @brief Check if the transition to the new state is legal and is not denied by
     *        the delegate function.
     *
     * @param newState the new requested state
     * @return true if the transition is legal and has not been denied, false otherwise.
     */
    bool canChange(const state_t newState);

    /**
     * @brief  Method to get which is the state until the transitions will be done automatically following its parent
     *
     * @return the state enum
     */
    autoEnable_t getAutoEnable();

    /**
     * @brief  Method to set which is the state until the transitions will be done automatically following its parent
     *
     *@param autoState state until the transitions will be done automatically following its parent
     */
    void setAutoEnable(autoEnable_t autoState);

};

}
#endif // NDSSTATEMACHINE_H
