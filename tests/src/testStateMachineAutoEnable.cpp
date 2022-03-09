#include <functional>

#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

/*
 * STM0
 */
#define STM(NAME,TIME) void switchOn##NAME(){\
    sleep(TIME);\
    std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void starting##NAME(){\
    sleep(TIME);\
    std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void switchOff##NAME(){\
    sleep(TIME);\
    std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void stopping##NAME(){\
    sleep(TIME);\
    std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void recover##NAME(){\
    std::cout<<"I'm on "<<__func__<<std::endl;\
}\

STM(_RootNode,0)
STM(_Child1,0)
STM(_Child2,0)
STM(_Child3,0)

STM(_RootNode_asyn,0)
STM(_Child1_asyn,3)
STM(_Child2_asyn,2)
STM(_Child3_asyn,1)

#define STM_NODE(PARENT,NAME,RET,AUTOENABLE) PARENT.addChild(nds::StateMachine(false,\
                                                                          std::bind(&switchOn##NAME),\
                                                                          std::bind(&switchOff##NAME),\
                                                                          std::bind(&starting##NAME),\
                                                                          std::bind(&stopping##NAME),\
                                                                          std::bind(&recover##NAME),\
                                                                          std::bind(&RET, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),\
                                                                          AUTOENABLE));\

#define STM_NODE_ASYN(PARENT,NAME,RET,AUTOENABLE) PARENT.addChild(nds::StateMachine(true,\
                                                                          std::bind(&switchOn##NAME),\
                                                                          std::bind(&switchOff##NAME),\
                                                                          std::bind(&starting##NAME),\
                                                                          std::bind(&stopping##NAME),\
                                                                          std::bind(&recover##NAME),\
                                                                          std::bind(&RET, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),\
                                                                          AUTOENABLE));\

bool alwaysReturnTrue(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}

bool alwaysReturnFalse(const nds::state_t,const nds::state_t , const nds::state_t)
{
    return false;
}


TEST(testStateMachineAutoEnable, testSuccesfulTransitionState)
{

    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN = STM_NODE(rootNode,_RootNode,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));
    nds::Node ch2 = rootNode.addChild(nds::Node("ch2"));
    nds::Node ch3 = rootNode.addChild(nds::Node("ch3"));

    nds::StateMachine stateMachine1 = STM_NODE(ch1,_Child1,alwaysReturnTrue,nds::autoEnable_t::running);
    nds::StateMachine stateMachine2 = STM_NODE(ch2,_Child2,alwaysReturnTrue,nds::autoEnable_t::on);
    nds::StateMachine stateMachine3 = STM_NODE(ch3,_Child3,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0 (off)
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    // Switch on state machine of the RootNode
    stateMachineRN.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());

    // Starting state machine of the RootNode
    stateMachineRN.setState(nds::state_t::running);
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 go automatically to RUNNING state, STM1 stays in ON and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());

    // Starting state machine of the RootNode
    stateMachineRN.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 go automatically to RUNNING state, STM1 stays in ON and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());

    // Starting state machine of the RootNode
    stateMachineRN.setState(nds::state_t::off);
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 go automatically to RUNNING state, STM1 stays in ON and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());

    factory.destroyDevice("");
}


TEST(testStateMachineAutoEnable, testErrorTransitionState)
{

    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN = STM_NODE(rootNode,_RootNode,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));
    nds::Node ch2 = rootNode.addChild(nds::Node("ch2"));

    nds::StateMachine stateMachine1 = STM_NODE(ch1,_Child1,alwaysReturnTrue,nds::autoEnable_t::running);
    nds::StateMachine stateMachine2 = STM_NODE(ch2,_Child2,alwaysReturnFalse,nds::autoEnable_t::on);


    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0 (off)
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    // Switch on state machine of the RootNode
    try{
    stateMachineRN.setState(nds::state_t::on);
    }catch(nds::StateMachineTransitionDenied& e){
        std::cout<<e.what()<<std::endl;
    }
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());

    factory.destroyDevice("");
}

TEST(testStateMachineAutoEnable, testAsynTransitionState)
{

    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN = STM_NODE_ASYN(rootNode,_RootNode_asyn,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));
    nds::Node ch2 = rootNode.addChild(nds::Node("ch2"));
    nds::Node ch3 = rootNode.addChild(nds::Node("ch3"));

    nds::StateMachine stateMachine1 = STM_NODE_ASYN(ch1,_Child1_asyn,alwaysReturnTrue,nds::autoEnable_t::running);
    nds::StateMachine stateMachine2 = STM_NODE_ASYN(ch2,_Child2_asyn,alwaysReturnTrue,nds::autoEnable_t::running);
    nds::StateMachine stateMachine3 = STM_NODE_ASYN(ch3,_Child3_asyn,alwaysReturnTrue,nds::autoEnable_t::running);


    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Switch on state machine of the RootNode
    try{
        stateMachineRN.setState(nds::state_t::on);
    }catch(nds::StateMachineTransitionDenied& e){
        std::cout<<e.what()<<std::endl;
    }

    sleep(4); //Wait untill all the State Machines are in ON State
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine3.getLocalState());

    // Switch on state machine of the RootNode
    try{
        stateMachineRN.setState(nds::state_t::running);
    }catch(nds::StateMachineTransitionDenied& e){
        std::cout<<e.what()<<std::endl;
    }

    sleep(4); //Wait untill all the State Machines are in ON State
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachine3.getLocalState());

    // Switch on state machine of the RootNode
    try{
        stateMachineRN.setState(nds::state_t::on);
    }catch(nds::StateMachineTransitionDenied& e){
        std::cout<<e.what()<<std::endl;
    }

    sleep(4); //Wait untill all the State Machines are in ON State
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine3.getLocalState());

    // Switch on state machine of the RootNode
    try{
        stateMachineRN.setState(nds::state_t::off);
    }catch(nds::StateMachineTransitionDenied& e){
        std::cout<<e.what()<<std::endl;
    }

    sleep(4); //Wait untill all the State Machines are in ON State
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());

    factory.destroyDevice("");
}



