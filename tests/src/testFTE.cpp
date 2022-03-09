#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"
#include "testUtils.h"

/**
 * @brief Internal function to test the Set-PVs included in the FTE node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testSetPVManaging and testSetPVManagingInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonSetPVsTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the Suppress-PVs included in the FTE node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testSuppressPVManaging and testSuppressPVManagingInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonSuppressPVsTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the ChgPeriod-PVs included in the FTE node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testChgPeriodPVManaging and testChgPeriodPVManagingInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonChgPeriodPVsTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the Pending-PVs included in the FTE node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPendingAndMaximumPVManaging and testPendingAndMaximumPVManagingInit test,
 * this function shares the full test and the evaluation or not of the
 * initialization features depends on the @a testInitializers flag.
 */
static void commonPendingPVsTest(const bool testInitializers = false);

TEST(testFTE, testStateMachineFTE)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceFTE and named rootNode
    factory.createDevice("DeviceFTE", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testFTE, testSetPVManaging)
{
    commonSetPVsTest();
}
TEST(testFTE, testSetPVManagingInit)
{
    commonSetPVsTest(true);
}

TEST(testFTE, testSuppressPVManaging)
{
    commonSuppressPVsTest();
}
TEST(testFTE, testSuppressPVManagingInit)
{
    commonSuppressPVsTest(true);
}

TEST(testFTE, testChgPeriodPVManaging)
{
    commonChgPeriodPVsTest();
}

TEST(testFTE, testChgPeriodPVManagingInit)
{
    commonChgPeriodPVsTest(true);
}

TEST(testFTE, testPendingAndMaximumPVManaging)
{
    commonPendingPVsTest();
}

TEST(testFTE, testPendingAndMaximumPVManagingInit)
{
    commonPendingPVsTest(true);
}

static void commonSetPVsTest(const bool testInitializers) {

    const std::string* setStatus;
    const std::int32_t* setCode;
    const timespec* timestampSet;
    timespec timestamp = {0, 0}, readTimestamp{0,0}, timeSet{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }
    factory.createDevice("DeviceFTE", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200; //TODO Study this
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    if (testInitializers) {
        std::int32_t initialInt32Value;
        timespec initialTimespecValue;
        bool validValue = false;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.Set", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Set = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.Suppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Suppress = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.ChgPeriod", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ChgPeriod = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.TerminalPending", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TerminalPending = "<< initialInt32Value <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.TerminalSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -1, initialInt32Value);
        std::cout<<"\tInitial TerminalSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.ModeSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ModeSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.StartTimeSet", &timestamp, &initialTimespecValue);
        validValue = (initialTimespecValue.tv_sec == 10) && (initialTimespecValue.tv_nsec == 20);
        EXPECT_EQ((bool) true, validValue);
        std::cout<<"\tInitial StartTimeSet = "<< TestUtils::getString(initialTimespecValue) <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.StopTimeSet", &timestamp, &initialTimespecValue);
        validValue = (initialTimespecValue.tv_sec == 20) && (initialTimespecValue.tv_nsec == 10);
        EXPECT_EQ((bool) true, validValue);
        std::cout<<"\tInitial StopTimeSet = "<< TestUtils::getString(initialTimespecValue) <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.LevelSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial LevelSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.PeriodNsecSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1000, initialInt32Value);
        std::cout<<"\tInitial PeriodNsecSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.DutyCycleSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 80, initialInt32Value);
        std::cout<<"\tInitial DutyCycleSet = "<< initialInt32Value <<std::endl;
    }

    ////////////////////////////////////////////////////////////////
    ///TEST SET with correct values
    ////////////////////////////////////////////////////////////////
    // Set/Get TerminalSet
    std::int32_t terminalSet;
    pInterface->writeCSValue("/rootNode-FTENode.TerminalSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.TerminalSet",&readTimestamp,&terminalSet); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,terminalSet);

    // Set/Get ModeSet
    std::int32_t modeSet;
    pInterface->writeCSValue("/rootNode-FTENode.ModeSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.ModeSet",&readTimestamp,&modeSet); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,modeSet);

    // Set/Get LevelSet
    std::int32_t levelSet;
    pInterface->writeCSValue("/rootNode-FTENode.LevelSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.LevelSet",&readTimestamp,&levelSet); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,levelSet);

    // Set/Get StartTimeSet
    timeSet = {1, 1};
    timespec startTimeSet;
    pInterface->writeCSValue("/rootNode-FTENode.StartTimeSet",readTimestamp,timeSet); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.StartTimeSet",&readTimestamp,&startTimeSet); // PVVariables are thread safe
    EXPECT_EQ(1,startTimeSet.tv_sec);
    EXPECT_EQ(1,startTimeSet.tv_nsec);

    // Set/Get SopTimeSet
    timespec stopTimeSet;
    pInterface->writeCSValue("/rootNode-FTENode.StopTimeSet",readTimestamp,timeSet); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.StopTimeSet",&readTimestamp,&stopTimeSet); // PVVariables are thread safe
    EXPECT_EQ(1,stopTimeSet.tv_sec);
    EXPECT_EQ(1,stopTimeSet.tv_nsec);

    // Set/Get PeriodNsecSet
    std::int32_t periodNsecSet;
    pInterface->writeCSValue("/rootNode-FTENode.PeriodNsecSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.PeriodNsecSet",&readTimestamp,&periodNsecSet); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,periodNsecSet);

    // Set/Get DutyCycleSet
    std::int32_t dutyCycleSet;
    pInterface->writeCSValue("/rootNode-FTENode.DutyCycleSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.DutyCycleSet",&readTimestamp,&dutyCycleSet); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,dutyCycleSet);

    ////////////////////////////////////////////////////////////////
    ///TEST Set==1
    ////////////////////////////////////////////////////////////////
    // Set/Get SetStatus
    pInterface->writeCSValue("/rootNode-FTENode.Set",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.SetStatus",timestampSet,setStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.SetCode",timestampSet,setCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*setStatus);
    EXPECT_EQ((std::int32_t)0,*setCode);

    ////////////////////////////////////////////////////////////////
    ///TEST Set==0
    ////////////////////////////////////////////////////////////////
    // Set/Get SuppressStatus
    pInterface->writeCSValue("/rootNode-FTENode.Set",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.SetStatus",timestampSet,setStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.SetCode",timestampSet,setCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*setStatus);
    EXPECT_EQ((std::int32_t)1,*setCode);

    ////////////////////////////////////////////////////////////////
    ///TEST SET with incorrect values
    ////////////////////////////////////////////////////////////////
    // Set/Get TerminalSet
    timeSet = {0, 0};
    pInterface->writeCSValue("/rootNode-FTENode.TerminalSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.ModeSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.LevelSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.StartTimeSet",readTimestamp, timeSet); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.StopTimeSet",readTimestamp, timeSet); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.PeriodNsecSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.DutyCycleSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Set/Get SuppressStatus
    pInterface->writeCSValue("/rootNode-FTENode.Set",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.SetStatus",timestampSet,setStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.SetCode",timestampSet,setCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*setStatus);
    EXPECT_EQ((std::int32_t)-1,*setCode);

    factory.destroyDevice("rootNode");

}

static void commonSuppressPVsTest(const bool testInitializers) {

    const std::string* suppressStatus;
    const std::int32_t* suppressCode;
    const timespec* timestampSuppress;
    timespec timestamp = {0, 0}, readTimestamp{0,0}, timeSet{0, 0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }
    factory.createDevice("DeviceFTE", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200; //TODO Study this
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    if (testInitializers) {
        std::int32_t initialInt32Value;
        timespec initialTimespecValue;
        bool validValue = false;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.Set", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Set = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.Suppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Suppress = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.ChgPeriod", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ChgPeriod = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.TerminalPending", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TerminalPending = "<< initialInt32Value <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.TerminalSuppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -2, initialInt32Value);
        std::cout<<"\tInitial TerminalSuppress = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.ModeSuppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ModeSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.AllSuppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial AllSuppress = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.StartTimeSuppress", &timestamp, &initialTimespecValue);
        validValue = (initialTimespecValue.tv_sec == 20) && (initialTimespecValue.tv_nsec == 20);
        EXPECT_EQ((bool) true, validValue);
        std::cout<<"\tInitial StartTimeSuppress = "<< TestUtils::getString(initialTimespecValue) <<std::endl;
    }

    ////////////////////////////////////////////////////////////////
    ///TEST SUPPRESS with correct values
    ////////////////////////////////////////////////////////////////
    // Set/Get TerminalSet
    std::int32_t terminalSuppress;
    pInterface->writeCSValue("/rootNode-FTENode.TerminalSuppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.TerminalSuppress",&readTimestamp,&terminalSuppress); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,terminalSuppress);

    // Set/Get ModeSet
    std::int32_t modeSuppress;
    pInterface->writeCSValue("/rootNode-FTENode.ModeSuppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.ModeSuppress",&readTimestamp,&modeSuppress); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,modeSuppress);

    // Set/Get AllSuppress
    std::int32_t allSuppress;
    pInterface->writeCSValue("/rootNode-FTENode.AllSuppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.AllSuppress",&readTimestamp,&allSuppress); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,allSuppress);

    // Set/Get StartTimeSet
    timespec startTimeSuppress;
    timeSet = {1, 1};
    pInterface->writeCSValue("/rootNode-FTENode.StartTimeSuppress",readTimestamp, timeSet); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.StartTimeSuppress",&readTimestamp,&startTimeSuppress); // PVVariables are thread safe
    EXPECT_EQ(1,startTimeSuppress.tv_sec);
    EXPECT_EQ(1,startTimeSuppress.tv_nsec);

    ////////////////////////////////////////////////////////////////
    ///TEST Suppress==0
    ////////////////////////////////////////////////////////////////
    // Set/Get SuppressStatus
    pInterface->writeCSValue("/rootNode-FTENode.Suppress",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.SuppressStatus",timestampSuppress,suppressStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.SuppressCode",timestampSuppress,suppressCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*suppressStatus);
    EXPECT_EQ((std::int32_t)0,*suppressCode);

    ////////////////////////////////////////////////////////////////
    ///TEST Suppress==1
    ////////////////////////////////////////////////////////////////
    // Set/Get SuppressStatus
    pInterface->writeCSValue("/rootNode-FTENode.Suppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.SuppressStatus",timestampSuppress,suppressStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.SuppressCode",timestampSuppress,suppressCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*suppressStatus);
    EXPECT_EQ((std::int32_t)1,*suppressCode);

    ////////////////////////////////////////////////////////////////
    ///TEST SUPPRESS with incorrect values
    ////////////////////////////////////////////////////////////////
    // Set/Get TerminalSet
    timeSet = {0, 0};
    pInterface->writeCSValue("/rootNode-FTENode.TerminalSuppress",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.ModeSuppress",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.AllSuppress",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.StartTimeSuppress",readTimestamp, timeSet); // PVVariables are thread safe

    // Set/Get SuppressStatus
    pInterface->writeCSValue("/rootNode-FTENode.Suppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.SuppressStatus",timestampSuppress,suppressStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.SuppressCode",timestampSuppress,suppressCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*suppressStatus);
    EXPECT_EQ((std::int32_t)-1,*suppressCode);

    factory.destroyDevice("rootNode");

}

static void commonChgPeriodPVsTest(const bool testInitializers){

    const std::string* ChgPeriodStatus;
    const std::int32_t* ChgPeriodCode;
    const timespec* timestampChgPeriod;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }
    factory.createDevice("DeviceFTE", "rootNode", parameters);
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200; //TODO Study this
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    if (testInitializers) {
        std::int32_t initialInt32Value;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.Set", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Set = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.Suppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Suppress = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.ChgPeriod", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ChgPeriod = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.TerminalPending", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TerminalPending = "<< initialInt32Value <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.TerminalChgPeriod", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -3, initialInt32Value);
        std::cout<<"\tInitial TerminalChgPeriod = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.PeriodChgPeriod", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 500, initialInt32Value);
        std::cout<<"\tInitial PeriodChgPeriod = "<< initialInt32Value <<std::endl;
    }

    ////////////////////////////////////////////////////////////////
    ///TEST CHG PERIOD with correct values
    ////////////////////////////////////////////////////////////////
    // Set/Get TerminalSet
    std::int32_t terminalChgPeriod;
    pInterface->writeCSValue("/rootNode-FTENode.TerminalChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.TerminalChgPeriod",&readTimestamp,&terminalChgPeriod); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,terminalChgPeriod);

    // Set/Get ModeSet
    std::int32_t periodChgPeriod;
    pInterface->writeCSValue("/rootNode-FTENode.PeriodChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-FTENode.PeriodChgPeriod",&readTimestamp,&periodChgPeriod); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,periodChgPeriod);

    ////////////////////////////////////////////////////////////////
    ///TEST ChgPeriod==0
    ////////////////////////////////////////////////////////////////
    // Set/Get ChgPeriodStatus
    pInterface->writeCSValue("/rootNode-FTENode.ChgPeriod",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.ChgPeriodStatus",timestampChgPeriod,ChgPeriodStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.ChgPeriodCode",timestampChgPeriod,ChgPeriodCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*ChgPeriodStatus);
    EXPECT_EQ((std::int32_t)0,*ChgPeriodCode);

    ////////////////////////////////////////////////////////////////
    ///TEST ChgPeriod==1
    ////////////////////////////////////////////////////////////////
    // Set/Get ChgPeriodStatus
    pInterface->writeCSValue("/rootNode-FTENode.ChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.ChgPeriodStatus",timestampChgPeriod,ChgPeriodStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.ChgPeriodCode",timestampChgPeriod,ChgPeriodCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*ChgPeriodStatus);
    EXPECT_EQ((std::int32_t)1,*ChgPeriodCode);

    ////////////////////////////////////////////////////////////////
    ///TEST CHG PERIOD with incorrect values
    ////////////////////////////////////////////////////////////////
    // Set/Get TerminalSet
    pInterface->writeCSValue("/rootNode-FTENode.TerminalChgPeriod",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-FTENode.PeriodChgPeriod",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Set/Get ChgPeriodStatus
    pInterface->writeCSValue("/rootNode-FTENode.ChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.ChgPeriodStatus",timestampChgPeriod,ChgPeriodStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.ChgPeriodCode",timestampChgPeriod,ChgPeriodCode); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*ChgPeriodStatus);
    EXPECT_EQ((std::int32_t)-1,*ChgPeriodCode);

    factory.destroyDevice("rootNode");

}

static void commonPendingPVsTest(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }
    factory.createDevice("DeviceFTE", "rootNode", parameters);
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200; //TODO Study this
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    if (testInitializers) {
        std::int32_t initialInt32Value;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-FTENode.Set", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Set = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.Suppress", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Suppress = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.ChgPeriod", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ChgPeriod = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-FTENode.TerminalPending", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TerminalPending = "<< initialInt32Value <<std::endl;
    }

    ////////////////////////////////////////////////////////////////
    ///TEST read MAXIMUM FTEs PV
    ////////////////////////////////////////////////////////////////
    //Maximum Value Testing
    std::int32_t maximum;
    pInterface->readCSValue("/rootNode-FTENode.Maximum",&readTimestamp,&maximum); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)20,maximum);

    ////////////////////////////////////////////////////////////////
    ///TEST TERMINAL PENDING with correct values
    ////////////////////////////////////////////////////////////////
    // Pending Value Testing
    const std::int32_t* pendingValue;
    const std::string* pendingStatus;
    const std::int32_t* pendingCode;
    const timespec* pendingTimestamp;
    pInterface->writeCSValue("/rootNode-FTENode.TerminalPending",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.PendingValue",pendingTimestamp,pendingValue); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.PendingStatus",pendingTimestamp,pendingStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.PendingCode",pendingTimestamp,pendingCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*pendingValue);
    EXPECT_EQ((std::string)"OK",*pendingStatus);
    EXPECT_EQ((std::int32_t)1,*pendingCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERMINAL PENDING with incorrect values
    ////////////////////////////////////////////////////////////////
    pInterface->writeCSValue("/rootNode-FTENode.TerminalPending",readTimestamp,(std::int32_t)-1); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.PendingValue",pendingTimestamp,pendingValue); // PVVariables are thread safe
    pInterface->getPushedString("/rootNode-FTENode.PendingStatus",pendingTimestamp,pendingStatus); // PVVariables are thread safe
    pInterface->getPushedInt32("/rootNode-FTENode.PendingCode",pendingTimestamp,pendingCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*pendingValue);
    EXPECT_EQ((std::string)"WRONG",*pendingStatus);
    EXPECT_EQ((std::int32_t)-1,*pendingCode);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    factory.destroyDevice("rootNode");
}


