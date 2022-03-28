#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"


/**
 * @brief Internal function to test the trigger by software included in the TriggerAndClk node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testSWTrig and testSWTrigInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonSWTriggerTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the Load-PVs included in the TriggerAndClk node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testConfigTrig and testConfigTrigInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonConfigTriggerTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PLLSync-PVs included in the TriggerAndClk node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPLLSync and testPLLSyncInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonPLLSyncTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PLLEnabling-PVs included in the TriggerAndClk node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testEnablePLL and testEnablePLLInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonEnablePLLTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the ResetConfiguration-PVs included in the TriggerAndClk node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testResetConfigTrigger and testResetConfigTriggerInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonResetConfigTriggerTest(const bool testInitializers = false);

TEST(testTrigAndClk, testSWTrig) {
  commonSWTriggerTest();
}

TEST(testTrigAndClk, testSWTrigInit) {
  commonSWTriggerTest(true);
}

TEST(testTrigAndClk, testConfigTrig) {
  commonConfigTriggerTest();
}

TEST(testTrigAndClk, testConfigTrigInit) {
  commonConfigTriggerTest(true);
}

TEST(testTrigAndClk, testPLLSync){
  commonPLLSyncTest();
}

TEST(testTrigAndClk, testPLLSyncInit) {
  commonPLLSyncTest(true);
}

TEST(testTrigAndClk, testEnablePLL) {
  commonEnablePLLTest();
}

TEST(testTrigAndClk, testEnablePLLInit) {
  commonEnablePLLTest(true);
}

TEST(testTrigAndClk, testResetConfigTrigger) {
  commonResetConfigTriggerTest();
}

TEST(testTrigAndClk, testResetConfigTriggerInit) {
  commonResetConfigTriggerTest(true);
}

TEST(testTrigAndClk, testStateMachine_TrigAndClk)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-TrigAndClk.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-TrigAndClk.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-TrigAndClk.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-TrigAndClk.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}


TEST(testTrigAndClk, testStateMachine_TrigAndClk_RoutingNode)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}

static void commonSWTriggerTest(const bool testInitializers)
{
    timespec readTimestamp{0,0};

    //Create factory
    nds::Factory factory("test");

    //Output/Input local variables
    std::int32_t HWBlock;

        nds::namedParameters_t parameters;
        if (testInitializers) {
                parameters["INIT"]="YES";
        }
    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

        if (testInitializers) {
            std::int32_t initialInt32Value;

            //--------------------------------------------------------------
            //Verifies values provided by initializers methods
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.SetSW", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 1, initialInt32Value);
            std::cout<<"\tInitial SetSW = "<< initialInt32Value <<std::endl;

            //--------------------------------------------------------------
            //Verifies values provided by initialization variables
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.HWBlock", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial HWBlock = "<< initialInt32Value <<std::endl;
        }

    // Set/Get DAQStartTimeDelay
    pInterface->writeCSValue("/rootNode-TrigAndClk.HWBlock",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.HWBlock",&readTimestamp,&HWBlock); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,HWBlock);

    pInterface->writeCSValue("/rootNode-TrigAndClk.SetSW",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Destroy test device
    factory.destroyDevice("rootNode");
}



static void commonConfigTriggerTest(const bool testInitializers)
{
    timespec readTimestamp{0,0};

    //Create factory
    nds::Factory factory("test");

    //Status and Code variables
    const std::string* trigLoadStatus;
    const std::int32_t* trigLoadCode;
    const timespec* pushedTimestamp;

    //Output/Input local variables
    std::int32_t DAQStartTimeDelay;
    std::int32_t TriggPeriod;
    std::int32_t level;
    std::int32_t edge;
    std::int32_t change;
    std::int32_t mode;
    std::int32_t ClkDivider;
    std::int32_t preTrigSamples;
    std::int32_t postTrigSamples;

    //RBV local variables
    const std::int32_t* DAQStartTimeDelayRBV;
    const std::int32_t* TriggPeriodRBV;
    const std::int32_t* levelRBV;
    const std::int32_t* edgeRBV;
    const std::int32_t* changeRBV;
    const std::int32_t* modeRBV;
    const std::int32_t* ClkDividerRBV;
    const std::int32_t* preTrigSamplesRBV;
    const std::int32_t* postTrigSamplesRBV;

        nds::namedParameters_t parameters;
        if (testInitializers) {
                parameters["INIT"]="YES";
        }
    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

            std::int32_t initialInt32Value;

            //--------------------------------------------------------------
            //Verifies values provided by initializers methods
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.LoadTrigConf", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial SetSW = "<< initialInt32Value <<std::endl;

            //--------------------------------------------------------------
            //Verifies values provided by initialization variables
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.DAQStartTimeDelay", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial DAQStartTimeDelay = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.TrigPeriod", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial TrigPeriod = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.Level", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial Level = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.Edge", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial Edge = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.Change", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial Change = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.Mode", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial Mode = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.ClkDivider", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial ClkDivider = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.PreTrigSamples", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 1, initialInt32Value);
            std::cout<<"\tInitial PreTrigSamples = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.PostTrigSamples", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 1, initialInt32Value);
            std::cout<<"\tInitial PostTrigSamples = "<< initialInt32Value <<std::endl;
    }

    ////////////////////////////////////////////////////////////////
    ///TEST Load Trigger configuration with correct values LoadTrigConf=0
    ////////////////////////////////////////////////////////////////
    // Set LoadTrigConf
    pInterface->writeCSValue("/rootNode-TrigAndClk.LoadTrigConf",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.TrigLoadStatus",pushedTimestamp,trigLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*trigLoadStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigLoadCode",pushedTimestamp,trigLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*trigLoadCode);

    // Get DAQStartTimeDelay
    pInterface->getPushedInt32("/rootNode-TrigAndClk.DAQStartTimeDelay_RBV",pushedTimestamp, DAQStartTimeDelayRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*DAQStartTimeDelayRBV);

    // Get TriggPeriod
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigPeriod_RBV",pushedTimestamp, TriggPeriodRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*TriggPeriodRBV);

    // Get Level
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Level_RBV",pushedTimestamp, levelRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*levelRBV);

    // Get Edge
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Edge_RBV",pushedTimestamp, edgeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*edgeRBV);

    // Get Change
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Change_RBV",pushedTimestamp, changeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*changeRBV);

    // Get Mode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Mode_RBV",pushedTimestamp, modeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*modeRBV);

    // Get ClkDivider
    pInterface->getPushedInt32("/rootNode-TrigAndClk.ClkDivider_RBV",pushedTimestamp, ClkDividerRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*ClkDividerRBV);

    // Get PreTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PreTrigSamples_RBV",pushedTimestamp, preTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*preTrigSamplesRBV);

    // Get PostTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PostTrigSamples_RBV",pushedTimestamp, postTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*postTrigSamplesRBV);

    ////////////////////////////////////////////////////////////////
    ///Loading all configuration parameters
    ////////////////////////////////////////////////////////////////
    // Set/Get DAQStartTimeDelay
    pInterface->writeCSValue("/rootNode-TrigAndClk.DAQStartTimeDelay",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.DAQStartTimeDelay",&readTimestamp,&DAQStartTimeDelay); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,DAQStartTimeDelay);

    // Set/Get TriggPeriod
    pInterface->writeCSValue("/rootNode-TrigAndClk.TrigPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.TrigPeriod",&readTimestamp,&TriggPeriod); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TriggPeriod);

    // Set/Get Level
    pInterface->writeCSValue("/rootNode-TrigAndClk.Level",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Level",&readTimestamp,&level); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,level);

    // Set/Get Edge
    pInterface->writeCSValue("/rootNode-TrigAndClk.Edge",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Edge",&readTimestamp,&edge); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,edge);

    // Set/Get Change
    pInterface->writeCSValue("/rootNode-TrigAndClk.Change",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Change",&readTimestamp,&change); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,change);

    // Set/Get Mode
    pInterface->writeCSValue("/rootNode-TrigAndClk.Mode",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Mode",&readTimestamp,&mode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,mode);

    // Set/Get ClkDivider
    pInterface->writeCSValue("/rootNode-TrigAndClk.ClkDivider",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.ClkDivider",&readTimestamp,&ClkDivider); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,ClkDivider);

    // Set/Get preTrigSamples
    pInterface->writeCSValue("/rootNode-TrigAndClk.PreTrigSamples",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.PreTrigSamples",&readTimestamp,&preTrigSamples); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,preTrigSamples);

    // Set/Get postTrigSamples
    pInterface->writeCSValue("/rootNode-TrigAndClk.PostTrigSamples",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.PostTrigSamples",&readTimestamp,&postTrigSamples); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,postTrigSamples);

    ////////////////////////////////////////////////////////////////
    ///TEST Load Trigger configuration with correct values LoadTrigConf=1 mode=0
    ////////////////////////////////////////////////////////////////
    // Set LoadTrigConf
    pInterface->writeCSValue("/rootNode-TrigAndClk.LoadTrigConf",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.TrigLoadStatus",pushedTimestamp,trigLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*trigLoadStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigLoadCode",pushedTimestamp,trigLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*trigLoadCode);

    // Get DAQStartTimeDelay
    pInterface->getPushedInt32("/rootNode-TrigAndClk.DAQStartTimeDelay_RBV",pushedTimestamp, DAQStartTimeDelayRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*DAQStartTimeDelayRBV);

    // Get TriggPeriod
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigPeriod_RBV",pushedTimestamp, TriggPeriodRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*TriggPeriodRBV);

    // Get Level
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Level_RBV",pushedTimestamp, levelRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*levelRBV);

    // Get Edge
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Edge_RBV",pushedTimestamp, edgeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*edgeRBV);

    // Get Change
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Change_RBV",pushedTimestamp, changeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*changeRBV);

    // Get Mode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Mode_RBV",pushedTimestamp, modeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*modeRBV);

    // Get ClkDivider
    pInterface->getPushedInt32("/rootNode-TrigAndClk.ClkDivider_RBV",pushedTimestamp, ClkDividerRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*ClkDividerRBV);

    // Get postTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PostTrigSamples_RBV",pushedTimestamp, postTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*postTrigSamplesRBV);

    ////////////////////////////////////////////////////////////////
    ///TEST Load Trigger configuration with correct values LoadTrigConf=1 mode=1
    ////////////////////////////////////////////////////////////////
    // Set/Get Mode
    pInterface->writeCSValue("/rootNode-TrigAndClk.Mode",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Mode",&readTimestamp,&mode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,mode);

    // Set  LoadTrigConf
    pInterface->writeCSValue("/rootNode-TrigAndClk.LoadTrigConf",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.TrigLoadStatus",pushedTimestamp,trigLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*trigLoadStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigLoadCode",pushedTimestamp,trigLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*trigLoadCode);

    // Get DAQStartTimeDelay
    pInterface->getPushedInt32("/rootNode-TrigAndClk.DAQStartTimeDelay_RBV",pushedTimestamp, DAQStartTimeDelayRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*DAQStartTimeDelayRBV);

    // Get TriggPeriod
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigPeriod_RBV",pushedTimestamp, TriggPeriodRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*TriggPeriodRBV);

    // Get Level
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Level_RBV",pushedTimestamp, levelRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*levelRBV);

    // Get Edge
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Edge_RBV",pushedTimestamp, edgeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*edgeRBV);

    // Get Change
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Change_RBV",pushedTimestamp, changeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*changeRBV);

    // Get Mode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Mode_RBV",pushedTimestamp, modeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*modeRBV);

    // Get ClkDivider
    pInterface->getPushedInt32("/rootNode-TrigAndClk.ClkDivider_RBV",pushedTimestamp, ClkDividerRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*ClkDividerRBV);

    // Get preTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PreTrigSamples_RBV",pushedTimestamp, preTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*preTrigSamplesRBV);

    // Get postTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PostTrigSamples_RBV",pushedTimestamp, postTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*postTrigSamplesRBV);

    ////////////////////////////////////////////////////////////////
    ///TEST Load Trigger configuration with incorrect mode value LoadTrigConf=1 mode=2
    ////////////////////////////////////////////////////////////////
    // Set/Get Mode
    pInterface->writeCSValue("/rootNode-TrigAndClk.Mode",readTimestamp,(std::int32_t)2); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Mode",&readTimestamp,&mode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)2,mode);

    // Set  LoadTrigConf
    pInterface->writeCSValue("/rootNode-TrigAndClk.LoadTrigConf",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.TrigLoadStatus",pushedTimestamp,trigLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*trigLoadStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigLoadCode",pushedTimestamp,trigLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*trigLoadCode);

    // Get DAQStartTimeDelay
    pInterface->getPushedInt32("/rootNode-TrigAndClk.DAQStartTimeDelay_RBV",pushedTimestamp, DAQStartTimeDelayRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*DAQStartTimeDelayRBV);

    // Get TriggPeriod
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigPeriod_RBV",pushedTimestamp, TriggPeriodRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*TriggPeriodRBV);

    // Get Level
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Level_RBV",pushedTimestamp, levelRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*levelRBV);

    // Get Edge
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Edge_RBV",pushedTimestamp, edgeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*edgeRBV);

    // Get Change
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Change_RBV",pushedTimestamp, changeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*changeRBV);

    // Get Mode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Mode_RBV",pushedTimestamp, modeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)2,*modeRBV);

    // Get ClkDivider
    pInterface->getPushedInt32("/rootNode-TrigAndClk.ClkDivider_RBV",pushedTimestamp, ClkDividerRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*ClkDividerRBV);

    // Get preTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PreTrigSamples_RBV",pushedTimestamp, preTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*preTrigSamplesRBV);

    // Get postTrigSamples
    pInterface->getPushedInt32("/rootNode-TrigAndClk.PostTrigSamples_RBV",pushedTimestamp, postTrigSamplesRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*postTrigSamplesRBV);

    // Destroy test device
    factory.destroyDevice("rootNode");

}

static void commonPLLSyncTest(const bool testInitializers)
{
    timespec readTimestamp{0,0};

    //Create factory
    nds::Factory factory("test");

    //Output/Input local variables
    std::int32_t syncMode;
    std::int32_t PLLRefFreq;
    std::int32_t PLLRefDiv;
    std::int32_t PLLRefMult;
    std::int32_t PLLRefDivALL;

    //RBV local variables
    const timespec* pushedTimestamp;
    const std::int32_t* syncModeRBV;
    const std::int32_t* PLLRefFreqRBV;
    const std::int32_t* PLLRefDivRBV;
    const std::int32_t* PLLRefMultRBV;
    const std::int32_t* PLLRefDivALLRBV;
    const std::string* PLLLoadStatus;
    const std::int32_t* PLLLoadCode;

        nds::namedParameters_t parameters;
        if (testInitializers) {
            parameters["INIT"]="YES";
        }

    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");


        if (testInitializers) {

            std::int32_t initialInt32Value;

            //--------------------------------------------------------------
            //Verifies values provided by initializers methods
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.PLLSyncSET", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 1, initialInt32Value);
            std::cout<<"\tInitial PLLSyncSET = "<< initialInt32Value <<std::endl;

            //--------------------------------------------------------------
            //Verifies values provided by initialization variables
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.SyncMode", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial SyncMode = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefFreq", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial PLLRefFreq = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefDiv", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial PLLRefDiv = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefMult", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial PLLRefMult = "<< initialInt32Value <<std::endl;

            pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefDivALL", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 1, initialInt32Value);
            std::cout<<"\tInitial PLLRefDivALL = "<< initialInt32Value <<std::endl;

        }

    // Set/Get syncMode
    pInterface->writeCSValue("/rootNode-TrigAndClk.SyncMode",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.SyncMode",&readTimestamp,&syncMode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,syncMode);

    // Set/Get PLLRefFreq
    pInterface->writeCSValue("/rootNode-TrigAndClk.PLLRefFreq",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefFreq",&readTimestamp,&PLLRefFreq); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,PLLRefFreq);

    // Set/Get PLLRefDiv
    pInterface->writeCSValue("/rootNode-TrigAndClk.PLLRefDiv",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefDiv",&readTimestamp,&PLLRefDiv); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,PLLRefDiv);

    // Set/Get PLLRefMult
    pInterface->writeCSValue("/rootNode-TrigAndClk.PLLRefMult",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefMult",&readTimestamp,&PLLRefMult); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,PLLRefMult);

    // Set/Get PLLRefDivALL
    pInterface->writeCSValue("/rootNode-TrigAndClk.PLLRefDivALL",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.PLLRefDivALL",&readTimestamp,&PLLRefDivALL); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,PLLRefDivALL);

    // Set PLLSyncSET==1
    pInterface->writeCSValue("/rootNode-TrigAndClk.PLLSyncSET",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    pInterface->getPushedInt32("/rootNode-TrigAndClk.SyncMode_RBV",pushedTimestamp, syncModeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*syncModeRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefFreq_RBV",pushedTimestamp, PLLRefFreqRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*PLLRefFreqRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefDiv_RBV",pushedTimestamp, PLLRefDivRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*PLLRefDivRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefMult_RBV",pushedTimestamp, PLLRefMultRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*PLLRefMultRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefDivALL_RBV",pushedTimestamp, PLLRefDivALLRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*PLLRefDivALLRBV);

    pInterface->getPushedString("/rootNode-TrigAndClk.PLLLoadStatus",pushedTimestamp, PLLLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*PLLLoadStatus);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLLoadCode",pushedTimestamp, PLLLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*PLLLoadCode);

    // Set PLLSyncSET==0
    pInterface->writeCSValue("/rootNode-TrigAndClk.PLLSyncSET",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    pInterface->getPushedInt32("/rootNode-TrigAndClk.SyncMode_RBV",pushedTimestamp, syncModeRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*syncModeRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefFreq_RBV",pushedTimestamp, PLLRefFreqRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*PLLRefFreqRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefDiv_RBV",pushedTimestamp, PLLRefDivRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*PLLRefDivRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefMult_RBV",pushedTimestamp, PLLRefMultRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*PLLRefMultRBV);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLRefDivALL_RBV",pushedTimestamp, PLLRefDivALLRBV); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*PLLRefDivALLRBV);

    pInterface->getPushedString("/rootNode-TrigAndClk.PLLLoadStatus",pushedTimestamp, PLLLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*PLLLoadStatus);

    pInterface->getPushedInt32("/rootNode-TrigAndClk.PLLLoadCode",pushedTimestamp, PLLLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*PLLLoadCode);

    // Destroy test device
    factory.destroyDevice("rootNode");
}


static void commonEnablePLLTest(const bool testInitializers)
{
    timespec readTimestamp{0,0};

    //Create factory
    nds::Factory factory("test");

    //Output/Input local variables
    const timespec* pushedTimestamp;
    const std::int32_t* enabledFlag;

        nds::namedParameters_t parameters;
        if (testInitializers) {
            parameters["INIT"]="YES";
        }

    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

        if (testInitializers) {

            std::int32_t initialInt32Value;

            //--------------------------------------------------------------
            //Verifies values provided by initializers methods
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.EnableDisablePLL", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 0, initialInt32Value);
            std::cout<<"\tInitial EnableDisablePLL = "<< initialInt32Value <<std::endl;

        }

    // Set EnableDisablePLL
    pInterface->writeCSValue("/rootNode-TrigAndClk.EnableDisablePLL",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-TrigAndClk.EnableDisablePLL_RBV",pushedTimestamp, enabledFlag); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*enabledFlag);

    // Set EnableDisablePLL
    pInterface->writeCSValue("/rootNode-TrigAndClk.EnableDisablePLL",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-TrigAndClk.EnableDisablePLL_RBV",pushedTimestamp, enabledFlag); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*enabledFlag);

    // Destroy test device
    factory.destroyDevice("rootNode");
}

static void commonResetConfigTriggerTest(const bool testInitializers)
{
    timespec  readTimestamp{0,0};

    //Create factory
    nds::Factory factory("test");

    //Status and Code variables
    const std::string* trigLoadStatus;
    const std::int32_t* trigLoadCode;
    const timespec* pushedTimestamp;

        nds::namedParameters_t parameters;
        if (testInitializers) {
            parameters["INIT"]="YES";
        }

    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceTrigAndClk", "rootNode", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");


        if (testInitializers) {

            std::int32_t initialInt32Value;

            //--------------------------------------------------------------
            //Verifies values provided by initializers methods
            //--------------------------------------------------------------
            pInterface->readCSValue("/rootNode-TrigAndClk.ResetTrigConf", &readTimestamp, &initialInt32Value);
            EXPECT_EQ((std::int32_t) 1, initialInt32Value);
            std::cout<<"\tInitial ResetTrigConf = "<< initialInt32Value <<std::endl;

        }

    // Set/Get DAQStartTimeDelay
    pInterface->writeCSValue("/rootNode-TrigAndClk.ResetTrigConf",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.TrigLoadStatus",pushedTimestamp,trigLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*trigLoadStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigLoadCode",pushedTimestamp,trigLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*trigLoadCode);

    // Set/Get DAQStartTimeDelay
    pInterface->writeCSValue("/rootNode-TrigAndClk.ResetTrigConf",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.TrigLoadStatus",pushedTimestamp,trigLoadStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*trigLoadStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.TrigLoadCode",pushedTimestamp,trigLoadCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*trigLoadCode);

    // Destroy test device
    factory.destroyDevice("rootNode");
}


TEST(testTrigAndClk, testClockSet)
{

    //  const timespec* pStateMachineSwitchTime;
    //  const std::int32_t* pStateMachineState;

    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceTrigAndClk", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    // Set/Get ClkSrc
    std::int32_t clkSrc;
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkSrc",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Route.ClkSrc",&readTimestamp,&clkSrc); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,clkSrc);


    // Set/Get ClkDst
    std::int32_t clkDst;
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkDst",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Route.ClkDst",&readTimestamp,&clkDst); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,clkDst);


    ////////////////////////////////////////////////////////////////
    ///TEST CLK SET with correct values ClkSet=1
    ////////////////////////////////////////////////////////////////

    const std::int32_t* clkSetCode;
    const std::string* clkSetStatus;
    const timespec* clkTimestamp;
    // Set ClkSet
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.Route.ClkSetStatus",clkTimestamp,clkSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*clkSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.ClkSetCode",clkTimestamp,clkSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*clkSetCode);



    ////////////////////////////////////////////////////////////////
    ///TEST CLK SET with correct values ClkSet=0
    ////////////////////////////////////////////////////////////////
    // Set ClkSet
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.Route.ClkSetStatus",clkTimestamp,clkSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*clkSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.ClkSetCode",clkTimestamp,clkSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*clkSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST CLK SET with incorrect values
    ////////////////////////////////////////////////////////////////

    // Set ClkSet
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkSrc",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkDst",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.Route.ClkSetStatus",clkTimestamp,clkSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*clkSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.ClkSetCode",clkTimestamp,clkSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)-1,*clkSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST CLK DST READ
    ////////////////////////////////////////////////////////////////
    std::int32_t clkDstRead(2);
    const std::int32_t* clkSrcRead;
    // Set ClkDstRead
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.ClkDstRead",readTimestamp,clkDstRead); // PVVariables are thread safe
    // Get ClkSrcRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.ClkSrcRead",clkTimestamp,clkSrcRead); // PVVariables are thread safe
    EXPECT_EQ(clkDstRead + 1,*clkSrcRead);


    factory.destroyDevice("rootNode");

}

TEST(testTrigAndClk, testTermSet)
{

    //  const timespec* pStateMachineSwitchTime;
    //  const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceTrigAndClk", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    // Set/Get TermSrc
    std::int32_t TermSrc;
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermSrc",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Route.TermSrc",&readTimestamp,&TermSrc); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermSrc);

    // Set/Get TermDst
    std::int32_t TermDst;
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermDst",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Route.TermDst",&readTimestamp,&TermDst); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermDst);

    // Set/Get TermSync
    std::int32_t TermSync;
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermSyncSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Route.TermSyncSet",&readTimestamp,&TermSync); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermSync);

    // Set/Get TermInvert
    std::int32_t TermInvert;
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermInvertSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-TrigAndClk.Route.TermInvertSet",&readTimestamp,&TermInvert); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermInvert);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM SET with correct values TermSet=1
    ////////////////////////////////////////////////////////////////

    const std::int32_t* termSetCode;
    const std::string* termSetStatus;
    const timespec* termTimestamp;
    // Set TermSet
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.Route.TermSetStatus",termTimestamp,termSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*termSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSetCode",termTimestamp,termSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*termSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM SET with correct values TermSet=0
    ////////////////////////////////////////////////////////////////
    // Set TermSet
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.Route.TermSetStatus",termTimestamp,termSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*termSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSetCode",termTimestamp,termSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*termSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM SET with incorrect values
    ////////////////////////////////////////////////////////////////

    // Set TermSet
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermSrc",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermDst",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-TrigAndClk.Route.TermSetStatus",termTimestamp,termSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*termSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSetCode",termTimestamp,termSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)-1,*termSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM DST READ
    ////////////////////////////////////////////////////////////////
    std::int32_t TermDstRead(2);
    const std::int32_t* termSrcRead;
    const std::int32_t* termSyncRead;
    const std::int32_t* termInvertRead;
    // Set TermDstRead
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermDstRead",readTimestamp,TermDstRead); // PVVariables are thread safe
    // Get TermSrcRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSrcRead",termTimestamp,termSrcRead); // PVVariables are thread safe
    EXPECT_EQ(TermDstRead + 1,*termSrcRead);
    // Get TermSyncRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSyncRead",termTimestamp,termSyncRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*termSyncRead);
    // Get TermInvertRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermInvertRead",termTimestamp,termInvertRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*termInvertRead);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM DST READ
    ////////////////////////////////////////////////////////////////
    TermDstRead=0;

    // Set TermDstRead
    pInterface->writeCSValue("/rootNode-TrigAndClk.Route.TermDstRead",readTimestamp,TermDstRead); // PVVariables are thread safe
    // Get TermSrcRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSrcRead",termTimestamp,termSrcRead); // PVVariables are thread safe
    EXPECT_EQ(TermDstRead + 1,*termSrcRead);
    // Get TermSyncRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermSyncRead",termTimestamp,termSyncRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*termSyncRead);
    // Get TermInvertRead
    pInterface->getPushedInt32("/rootNode-TrigAndClk.Route.TermInvertRead",termTimestamp,termInvertRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*termInvertRead);


    factory.destroyDevice("rootNode");

}


