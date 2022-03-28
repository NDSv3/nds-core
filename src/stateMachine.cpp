/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/stateMachine.h"
#include "nds3/impl/stateMachineImpl.h"

namespace nds
{

/*
 * Default Constructor
 *
 *********************/
StateMachine::StateMachine()
{
}


/*
 * Constructor. Setup the delegates
 *
 **********************************/
StateMachine::StateMachine(bool bAsync,
                           stateChange_t switchOnFunction,
                           stateChange_t switchOffFunction,
                           stateChange_t startFunction,
                           stateChange_t stopFunction,
                           stateChange_t recoverFunction,
                           allowChange_t allowStateChangeFunction,
                           autoEnable_t autoState):
    Node(std::shared_ptr<NodeImpl>(new StateMachineImpl(bAsync,
                                                        switchOnFunction,
                                                        switchOffFunction,
                                                        startFunction,
                                                        stopFunction,
                                                        recoverFunction,
                                                        allowStateChangeFunction,
                                                        autoState)))
{
}

/*
 * Constructor. Setup the delegates
 *
 **********************************/
StateMachine::StateMachine(const StateMachineArgs_t& handlerSTM):
    Node(std::shared_ptr<NodeImpl>(new StateMachineImpl(handlerSTM)))
{
}


/*
 * Change the local state
 *
 ************************/
void StateMachine::setState(state_t newState)
{
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->setState(newState);
}


/*
 * Get the local state
 *
 *********************/
state_t StateMachine::getLocalState()
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getLocalState();

}


/*
 * Get the global state
 *
 **********************/
state_t StateMachine::getGlobalState()
{
    state_t state;
    timespec unused;
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getGlobalState(&unused, &state);
    return state;
}

/*
 * Get the lowest global state
 *
 **********************/
state_t StateMachine::getLowestGlobalState()
{
    state_t state;
    timespec unused;
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getLowestGlobalState(&unused, &state);
    return state;
}

/*
 * Get the highest global state
 *
 **********************/
state_t StateMachine::getHighestGlobalState()
{
    state_t state;
    timespec unused;
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getHighestGlobalState(&unused, &state);
    return state;
}

/*
 * Get the lowest state of all its children
 *
 **********************/
state_t StateMachine::getLowestChildState()
{
    state_t state;
    timespec unused;
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getLowestChildState(&unused, &state);
    return state;
}

/*
 * Get the highest state of all its children
 *
 **********************/
state_t StateMachine::getHighestChildState()
{
    state_t state;
    timespec unused;
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getHighestChildState(&unused, &state);
    return state;
}


/*
 * Check if a state transition is allowed
 *
 ****************************************/
bool StateMachine::canChange(const state_t newState)
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->canChange(newState);

}



autoEnable_t nds::StateMachine::getAutoEnable() {
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getAutoEnable();
}

void nds::StateMachine::setAutoEnable(autoEnable_t autoState) {
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->setAutoEnable(autoState);
}

}
