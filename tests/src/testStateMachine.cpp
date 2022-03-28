#include <functional>
#include <iostream>

#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

std::string stateToString(nds::state_t state){
    switch(state){
    case nds::state_t::unknown:
        return "unknown";
        break;
    case nds::state_t::off:
        return "off";
        break;
    case nds::state_t::switchingOff:
        return "switchingOff";
        break;
    case nds::state_t::initializing:
        return "initializing";
        break;
    case nds::state_t::on:
        return "on";
        break;
    case nds::state_t::stopping:
        return "stopping";
        break;
    case nds::state_t::starting:
        return "starting";
        break;
    case nds::state_t::running:
        return "running";
        break;
    case nds::state_t::fault:
        return "fault";
        break;
    default:
        return "wrong state";
        break;
    }
}

void wait1sec()
{
    ::sleep(1);
}

void rollback()
{
    ::usleep(100000);
    throw nds::StateMachineRollBack("rolling back");
}


bool returnTrue(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}
/*
 * This is the tested structure
 *
 *
 * rootNode
 *      /
 *      |--> STM (stateMachine0)
 *      |--> ch0
 *      |       |--> STM (stateMachine1)
 *      |--> ch1
 *      |       |--> STM (stateMachine2)
 *
 */

TEST(testStateMachine, testLocalGlobalState)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachine0 = rootNode.addChild(nds::StateMachine(true,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0 = rootNode.addChild(nds::Node("ch0"));
    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));

    nds::StateMachine stateMachine1 = ch0.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));


    nds::StateMachine stateMachine2 = ch1.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&rollback),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    // Switch on state machine 1
    stateMachine1.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    // Switch state machine 2 to on and then to running.
    // Should go back to on because of the rollback
    stateMachine2.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)stateMachine2.getLocalState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());

    stateMachine2.setState(nds::state_t::running);
    EXPECT_EQ((int)nds::state_t::starting, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());
    EXPECT_EQ((int)nds::state_t::starting, (int)stateMachine2.getLocalState());
    ::sleep(1);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());

    stateMachine2.setState(nds::state_t::off);
    EXPECT_EQ((int)nds::state_t::switchingOff, (int)stateMachine2.getLocalState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());

    factory.destroyDevice("");
}

/*
 * This tests shows how to use the hierarchical STMs in an NDS node.
 *
 * This is the tested structure
 *
 * rootNode
 *      /
 *      |--> STM (stateMachine0)
 *      |--> ch0
 *      |       |--> STM (stateMachine1)
 *      |--> ch1
 *      |       |--> STM (stateMachine2)
 *
 */

TEST(testStateMachine, testNodeStates)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachine0 = rootNode.addChild(nds::StateMachine(true,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0 = rootNode.addChild(nds::Node("ch0"));
    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));

    nds::StateMachine stateMachine1 = ch0.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));


    nds::StateMachine stateMachine2 = ch1.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&rollback),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0
    EXPECT_EQ((int)nds::state_t::off, (int)rootNode.getState());

    // Switch on state machine 1. This STM is a child of ch0 node
    ch0.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)ch0.getState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)ch0.getState());

    // Switch state machine 2 to on and then to running. This STM is a child of ch1 node
    // Should go back to on because of the rollback
    ch1.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)ch1.getState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)ch1.getState());

    ch1.setState(nds::state_t::running);
    EXPECT_EQ((int)nds::state_t::starting, (int)ch1.getState());
    ::sleep(1);
    EXPECT_EQ((int)nds::state_t::on, (int)ch1.getState());

    ch1.setState(nds::state_t::off);
    EXPECT_EQ((int)nds::state_t::switchingOff, (int)ch1.getState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::off, (int)ch1.getState());

    factory.destroyDevice("");
}

/*
 * This is the tested structure
 *
 * An NDS node can have as children:
 *    a) Different nodes and one STM (only one is allowed)
 *    b) Only nodes
 *    c) Only an STM
 *
 * rootNode
 *      /
 *      |--> STM (stateMachineRN0)
 *      |--> ch0
 *      |       |--> STM (stateMachineCH0)
 *      |       |--> ch0A
 *      |               |--> STM (stateMachineCH0A)
 *      |       |--> ch0B
 *      |--> ch1
 *      |       |--> STM (stateMachineCH1)
 *      |       |--> ch1A
 *      |               |-->ch1AA
 *
 */

TEST(testStateMachine, testChildrenStates)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN0 = rootNode.addChild(nds::StateMachine(false,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0 = rootNode.addChild(nds::Node("ch0"));
    nds::StateMachine stateMachineCH0 = ch0.addChild(nds::StateMachine(false,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    nds::Node ch0A = ch0.addChild(nds::Node("ch0A"));
    nds::StateMachine stateMachineCH0A = ch0A.addChild(nds::StateMachine(false,
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0B = ch0.addChild(nds::Node("ch0B"));

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));

    nds::StateMachine stateMachineCH1 = ch1.addChild(nds::StateMachine(false,
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    nds::Node ch1A = ch1.addChild(nds::Node("ch1A"));
    nds::Node ch1AA = ch1A.addChild(nds::Node("ch1AA"));

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    //All STMs have as default state is off
    stateMachineRN0.setState(nds::state_t::on); //Turn on the rootNode (The rest of the STM stay at off state)
    //stateMachineRN0.setState(nds::state_t::running); //Turn running the rootNode (The rest of the STM stay at off state)
    /*    * rootNode
         *      /
         *      |--> STM (stateMachineRN0) ON
         *      |--> ch0
         *      |       |--> STM (stateMachineCH0)  OFF
         *      |       |--> ch0A
         *      |               |--> STM (stateMachineCH0A) OFF
         *      |       |--> ch0B
         *      |--> ch1
         *      |       |--> STM (stateMachineCH1) OFF
         *      |       |--> ch1A
         *      |               |-->ch1AA
    */
    // Get Lowest and Highest states in the hierarchy
    EXPECT_EQ((int)nds::state_t::off, (int)rootNode.getLowestGlobalState());
    EXPECT_EQ((int)nds::state_t::on, (int)rootNode.getHighestGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)rootNode.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::off, (int)rootNode.getHighestChildState());

    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getLowestGlobalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN0.getHighestGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getHighestChildState());

    //Setting a rootNode to running state
    stateMachineRN0.setState(nds::state_t::running); //Turn running the rootNode (The rest of the STM stay at off state)

    //Setting a children to running state
    stateMachineCH0A.setState(nds::state_t::on);
    stateMachineCH0A.setState(nds::state_t::running);

    /*    * rootNode
         *      /
         *      |--> STM (stateMachineRN0) RUNNING
         *      |--> ch0
         *      |       |--> STM (stateMachineCH0) OFF
         *      |       |--> ch0A
         *      |               |--> STM (stateMachineCH0A) On->RUNNING
         *      |       |--> ch0B
         *      |--> ch1
         *      |       |--> STM (stateMachineCH1) OFF
         *      |       |--> ch1A
         *      |               |-->ch1AA
    */

    EXPECT_EQ((int)nds::state_t::off, (int)rootNode.getLowestGlobalState());
    EXPECT_EQ((int)nds::state_t::running, (int)rootNode.getHighestGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getLowestGlobalState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN0.getHighestGlobalState());

    EXPECT_EQ((int)nds::state_t::off, (int)rootNode.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)rootNode.getHighestChildState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN0.getHighestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineCH0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineCH0.getHighestChildState());

    //Turn ON remaining children

    stateMachineCH0.setState(nds::state_t::on);
    stateMachineCH1.setState(nds::state_t::on);

    /*    * rootNode
             *      /
             *      |--> STM (stateMachineRN0) RUNNING
             *      |--> ch0
             *      |       |--> STM (stateMachineCH0) ON
             *      |       |--> ch0A
             *      |               |--> STM (stateMachineCH0A) RUNNING
             *      |       |--> ch0B
             *      |--> ch1
             *      |       |--> STM (stateMachineCH1) ON
             *      |       |--> ch1A
             *      |               |-->ch1AA
     */
    EXPECT_EQ((int)nds::state_t::on, (int)rootNode.getLowestGlobalState());
    EXPECT_EQ((int)nds::state_t::running, (int)rootNode.getHighestGlobalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN0.getLowestGlobalState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN0.getHighestGlobalState());

    EXPECT_EQ((int)nds::state_t::on, (int)rootNode.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)rootNode.getHighestChildState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN0.getHighestChildState());

    factory.destroyDevice("");

}

