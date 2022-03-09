/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <memory>
#include <mutex>
#include <sstream>
#include <iostream>

#include "nds3/definitions.h"
#include "nds3/exceptions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/factoryBaseImpl.h"

namespace nds
{

static std::mutex m_initializationMutex;

NodeImpl::NodeImpl(const std::string &name, const nodeType_t nodeType): BaseImpl(name), m_nodeType(nodeType)
{}

void NodeImpl::addChild(std::shared_ptr<BaseImpl> pChild)
{
    std::string name(pChild->getComponentName());

    if(m_children.find(name) != m_children.end())
    {
        std::ostringstream errorString;
        errorString << "A node with the name " << name << " has already been registered into " << getFullName() << std::endl;
        throw std::logic_error(errorString.str());
    }

    m_children[name] = pChild;

    std::shared_ptr<StateMachineImpl> stateMachine = std::dynamic_pointer_cast<StateMachineImpl>(pChild);
    if(stateMachine.get() != 0)
    {
        m_pStateMachine = stateMachine;

        // Add the state machine commands also to this node
        ///////////////////////////////////////////////////
        defineCommand("switchOn", "", 0, std::bind(&StateMachineImpl::commandSetState, m_pStateMachine, state_t::on, std::placeholders::_1));
        defineCommand("switchOff", "", 0, std::bind(&StateMachineImpl::commandSetState, m_pStateMachine, state_t::off, std::placeholders::_1));
        defineCommand("start", "", 0, std::bind(&StateMachineImpl::commandSetState, m_pStateMachine, state_t::running, std::placeholders::_1));
        defineCommand("stop", "", 0, std::bind(&StateMachineImpl::commandSetState, m_pStateMachine, state_t::on, std::placeholders::_1));
    }
}


void NodeImpl::initializeRootNode(void *pDeviceObject, FactoryBaseImpl &controlSystem)
{
    if(getParent().get() != 0)
    {
        throw std::logic_error("You can initialize only the root nodes");
    }

    std::lock_guard<std::mutex> serializeInitialization(m_initializationMutex);

    initialize(controlSystem);

    controlSystem.holdNode(pDeviceObject, std::static_pointer_cast<NodeImpl>(shared_from_this()) );
}

void NodeImpl::initialize(FactoryBaseImpl& controlSystem)
{
    BaseImpl::initialize(controlSystem);

    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->setParent(std::static_pointer_cast<NodeImpl>(shared_from_this()), m_nodeLevel);
    }
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->initialize(controlSystem);
    }
}

void NodeImpl::deinitializeRootNode()
{
    if(getParent().get() != 0)
    {
        throw std::logic_error("You can call deinitialize only on root nodes");
    }

    std::lock_guard<std::mutex> serializeInitialization(m_initializationMutex);
    deinitialize();
}

void NodeImpl::deinitialize()
{
    BaseImpl::deinitialize();
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->deinitialize();
    }
}

state_t NodeImpl::getLocalState() const
{
    if(m_pStateMachine.get() == 0)
    {
        return state_t::unknown;
    }
    return m_pStateMachine->getLocalState();
}

void NodeImpl::getGlobalState(timespec* pTimestamp, state_t* pState) const
{
    if(m_pStateMachine.get() == 0)
    {
        getChildrenState(pTimestamp, pState);
        return;
    }
    m_pStateMachine->getGlobalState(pTimestamp, pState);
}
void NodeImpl::getLowestGlobalState(timespec* pTimestamp, state_t* pState) const
{
    if(m_pStateMachine.get() == 0)
    {
        getLowestChildrenState(pTimestamp,pState, m_nodeLevel);
        return;
    }
    m_pStateMachine->getLowestGlobalState(pTimestamp, pState);
}

void NodeImpl::getHighestGlobalState(timespec* pTimestamp, state_t* pState) const
{
    if(m_pStateMachine.get() == 0)
    {
        getHighestChildrenState(pTimestamp, pState, m_nodeLevel);
        return;
    }
    m_pStateMachine->getHighestGlobalState(pTimestamp, pState);
}

void NodeImpl::getLowestChildState(timespec* pTimestamp, state_t* pState) const
{
    getLowestChildrenState(pTimestamp,pState, m_nodeLevel);
}

void NodeImpl::getHighestChildState(timespec* pTimestamp, state_t* pState) const
{
    getHighestChildrenState(pTimestamp, pState, m_nodeLevel);
}

void NodeImpl::getChildrenState(timespec* pTimestamp, state_t* pState) const
{
    *pState = state_t::unknown;
    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        if(scanChildren->second.get() != m_pStateMachine.get())
        {
            std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
            if(child.get() != 0 && child->m_pStateMachine.get()!=0)
            {
                timespec childTimestamp;
                state_t childState;
                child->getGlobalState(&childTimestamp, &childState);

                if(
                        ((int)*pState < (int)childState) ||
                        ((int)*pState == (int)childState &&
                         (pTimestamp->tv_sec < childTimestamp.tv_sec ||
                          (pTimestamp->tv_sec == childTimestamp.tv_sec && pTimestamp->tv_nsec < childTimestamp.tv_nsec))))
                {
                    *pTimestamp = childTimestamp;
                    *pState = childState;
                }
            }
        }
    }
}

void NodeImpl::getLowestChildrenState(timespec* pTimestamp, state_t* pState, uint32_t nodeLevel) const
{
    *pState = state_t::MAX_STATE_NUM;
    state_t childState = state_t::MAX_STATE_NUM;
    //Iterates the map of all its children.
    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        //Excluding the STM because addChild method adds the STM as another child
        if(scanChildren->second.get() != m_pStateMachine.get())
        {
            std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
            if(child.get() != 0)
            {
                timespec timestamp;
                child->getLowestChildrenState(&timestamp, &childState, nodeLevel);
                //If the child doesn't have children, this function returns state_t::unknown
            }
        }else{
            if(m_nodeLevel>(nodeLevel)){
                std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
                if(child.get() != 0)
                {
                    childState = child->getLocalState();
                }
            }
        }
        if(((int)*pState >= (int)childState))
        {
            *pTimestamp = getTimestamp();
            *pState = childState;
        }
    }
}

void NodeImpl::getHighestChildrenState(timespec* pTimestamp, state_t* pState, uint32_t nodeLevel) const
{
    *pState = state_t::unknown;
    state_t childState = state_t::unknown;
    //Iterates the map of all its children.
    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        //Excluding the STM because addChild method adds the STM as another child
        if(scanChildren->second.get() != m_pStateMachine.get())
        {
            std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
            if(child.get() != 0)
            {
                timespec timestamp;
                child->getHighestChildrenState(&timestamp, &childState, nodeLevel);
                //If the child doesn't have children, this function returns state_t::unknown
            }
        }else{
            if(m_nodeLevel>(nodeLevel)){
                std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
                if(child.get() != 0)
                {
                    childState = child->getLocalState();
                }
            }
        }
        if(((int)*pState <= (int)childState))
        {
            *pTimestamp = getTimestamp();
            *pState = childState;
        }
    }
}

bool nds::NodeImpl::setChildrenState(timespec /*pTimestamp*/, state_t futureState) {

    bool error=false;
    std::map<std::string,state_t> prevChildStatus;
    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        if(scanChildren->second.get() != m_pStateMachine.get())
        {
            std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
            if(child.get() != 0)
            {
                if(child->getAutoEnable()>=(autoEnable_t)futureState){
                    prevChildStatus.insert(std::pair<std::string,state_t>(child->getFullNameFromPort(),child->getLocalState()));
                    //Only execute child transition if autoEnable state is greater or equal to futureState
                    try{
                        child->setLocalState(futureState);
                    }catch(nds::StateMachineError& e){
                        error=true;
                        break; //Break the for
                    }
                    //Not considered the asynchronous thread executing the transition
//                  if(child->getLocalState()!=futureState){
//                      error=true;
//                      break; //Break the for
//                  }
                }
            }
        }
    }//end for

    //If error==true, we revert all children to the previous state
    if(error){
        std::map<std::string,state_t>::iterator scanPrevChildren;
        for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
        {
            if(scanChildren->second.get() != m_pStateMachine.get())
            {
                std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
                if(child.get() != 0)
                {
                    scanPrevChildren = prevChildStatus.find(child->getFullNameFromPort());
                    if(scanPrevChildren!= prevChildStatus.end()){
                        child->setLocalState(scanPrevChildren->second);
                    }
                }
            }
        }
    }
    return error;

}

void nds::NodeImpl::setLocalState(state_t pState) {
    m_pStateMachine->setState(pState);
}

autoEnable_t nds::NodeImpl::getAutoEnable() {
    if(m_pStateMachine.get()==0){
        return autoEnable_t::none;
    }
    return m_pStateMachine->getAutoEnable();
}


void NodeImpl::setLogLevel(const logLevel_t logLevel)
{
    BaseImpl::setLogLevel(logLevel);

    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->setLogLevel(logLevel);
    }

}

std::string NodeImpl::buildFullExternalName(const FactoryBaseImpl& controlSystem) const
{
    return buildFullExternalName(controlSystem, false);
}

std::string NodeImpl::buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const
{
    return buildFullExternalName(controlSystem, true);
}

std::string NodeImpl::buildFullExternalName(const FactoryBaseImpl& controlSystem, const bool bStopAtPort) const
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer == 0)
    {
        return controlSystem.getSeparator(0) + controlSystem.getRootNodeName(m_externalName);
    }

    std::string name;
    switch(m_nodeType)
    {
    case nodeType_t::generic:
        name = controlSystem.getGenericChannelName(m_externalName);
        break;
    case nodeType_t::inputChannel:
        name = controlSystem.getInputChannelName(m_externalName);
        break;
    case nodeType_t::outputChannel:
        name = controlSystem.getOutputChannelName(m_externalName);
        break;
    case nodeType_t::dataSourceChannel:
        name = controlSystem.getSourceChannelName(m_externalName);
        break;
    case nodeType_t::dataSinkChannel:
        name = controlSystem.getSinkChannelName(m_externalName);
        break;
    case nodeType_t::stateMachine:
        name = controlSystem.getStateMachineNodeName(m_externalName);
        break;
    }

    if(name.empty())
    {
        if(bStopAtPort)
        {
            return temporaryPointer->buildFullExternalNameFromPort(controlSystem);
        }
        else
        {
            return temporaryPointer->buildFullExternalName(controlSystem);
        }
    }

    if(bStopAtPort)
    {
        return temporaryPointer->buildFullExternalNameFromPort(controlSystem) + controlSystem.getSeparator(m_nodeLevel) + name;
    }
    return temporaryPointer->buildFullExternalName(controlSystem) + controlSystem.getSeparator(m_nodeLevel) + name;
}


}


