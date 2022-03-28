#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

/**
 * @brief Internal function to test the ClockSet-PVs included in the Routing node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testClockSet and testClockSetInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonClkSetPVsTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the Terminal Set-PVs included in the Routing node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testTermSet and testTermSetInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonTermSetPVsTest(const bool testInitializers = false);

TEST(testRouting, testStateMachineRouting)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type Device and named rootNode
    factory.createDevice("DeviceRouting", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-RoutingNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-RoutingNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-RoutingNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-RoutingNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-RoutingNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}


TEST(testRouting, testClockSet)
{
    commonClkSetPVsTest();
}

TEST(testRouting, testClockSetInit)
{
    commonClkSetPVsTest(true);
}

static void commonClkSetPVsTest(const bool testInitializers)
{

    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }
    factory.createDevice("DeviceRouting", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-RoutingNode.ClkSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ClkSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.ClkDstRead", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ClkDstRead = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TermSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermDstRead", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 3, initialInt32Value);
        std::cout<<"\tInitial TermDstRead = "<< initialInt32Value <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-RoutingNode.ClkSrc", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -1, initialInt32Value);
        std::cout<<"\tInitial ClkSrc = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.ClkDst", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -2, initialInt32Value);
        std::cout<<"\tInitial ClkDst = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermSrc", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -3, initialInt32Value);
        std::cout<<"\tInitial TermSrc = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermDst", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -4, initialInt32Value);
        std::cout<<"\tInitial TermDst = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermSyncSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TermSyncSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermInvertSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TermInvertSet = "<< initialInt32Value <<std::endl;
    }


    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    // Set/Get ClkSrc
    std::int32_t clkSrc;
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkSrc",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-RoutingNode.ClkSrc",&readTimestamp,&clkSrc); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,clkSrc);

    // Set/Get ClkDst
    std::int32_t clkDst;
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkDst",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-RoutingNode.ClkDst",&readTimestamp,&clkDst); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,clkDst);

    ////////////////////////////////////////////////////////////////
    ///TEST CLK SET with correct values ClkSet=1
    ////////////////////////////////////////////////////////////////

    const std::int32_t* clkSetCode;
    const std::string* clkSetStatus;
    const timespec* clkTimestamp;
    // Set ClkSet
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-RoutingNode.ClkSetStatus",clkTimestamp,clkSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*clkSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-RoutingNode.ClkSetCode",clkTimestamp,clkSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*clkSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST CLK SET with correct values ClkSet=0
    ////////////////////////////////////////////////////////////////
    // Set ClkSet
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-RoutingNode.ClkSetStatus",clkTimestamp,clkSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*clkSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-RoutingNode.ClkSetCode",clkTimestamp,clkSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*clkSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST CLK SET with incorrect values
    ////////////////////////////////////////////////////////////////

    // Set ClkSet
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkSrc",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkDst",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-RoutingNode.ClkSetStatus",clkTimestamp,clkSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*clkSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-RoutingNode.ClkSetCode",clkTimestamp,clkSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)-1,*clkSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST CLK DST READ
    ////////////////////////////////////////////////////////////////
    std::int32_t clkDstRead(2);
    const std::int32_t* clkSrcRead;
    // Set ClkDstRead
    pInterface->writeCSValue("/rootNode-RoutingNode.ClkDstRead",readTimestamp,clkDstRead); // PVVariables are thread safe
    // Get ClkSrcRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.ClkSrcRead",clkTimestamp,clkSrcRead); // PVVariables are thread safe
    EXPECT_EQ(clkDstRead + 1,*clkSrcRead);


    factory.destroyDevice("rootNode");

}

TEST(testRouting, testTermSet)
{
    commonTermSetPVsTest();
}

TEST(testRouting, testTermSetInit)
{
    commonTermSetPVsTest(true);
}

static void commonTermSetPVsTest(const bool testInitializers)
{

    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }
    factory.createDevice("DeviceRouting", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-RoutingNode.ClkSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ClkSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.ClkDstRead", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial ClkDstRead = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TermSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermDstRead", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 3, initialInt32Value);
        std::cout<<"\tInitial TermDstRead = "<< initialInt32Value <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-RoutingNode.ClkSrc", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -1, initialInt32Value);
        std::cout<<"\tInitial ClkSrc = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.ClkDst", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -2, initialInt32Value);
        std::cout<<"\tInitial ClkDst = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermSrc", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -3, initialInt32Value);
        std::cout<<"\tInitial TermSrc = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermDst", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -4, initialInt32Value);
        std::cout<<"\tInitial TermDst = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermSyncSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TermSyncSet = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-RoutingNode.TermInvertSet", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial TermInvertSet = "<< initialInt32Value <<std::endl;
    }

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    // Set/Get TermSrc
    std::int32_t TermSrc;
    pInterface->writeCSValue("/rootNode-RoutingNode.TermSrc",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-RoutingNode.TermSrc",&readTimestamp,&TermSrc); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermSrc);

    // Set/Get TermDst
    std::int32_t TermDst;
    pInterface->writeCSValue("/rootNode-RoutingNode.TermDst",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-RoutingNode.TermDst",&readTimestamp,&TermDst); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermDst);

    // Set/Get TermSync
    std::int32_t TermSync;
    pInterface->writeCSValue("/rootNode-RoutingNode.TermSyncSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-RoutingNode.TermSyncSet",&readTimestamp,&TermSync); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermSync);

    // Set/Get TermInvert
    std::int32_t TermInvert;
    pInterface->writeCSValue("/rootNode-RoutingNode.TermInvertSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
    pInterface->readCSValue("/rootNode-RoutingNode.TermInvertSet",&readTimestamp,&TermInvert); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,TermInvert);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM SET with correct values TermSet=1
    ////////////////////////////////////////////////////////////////

    const std::int32_t* termSetCode;
    const std::string* termSetStatus;
    const timespec* termTimestamp;
    // Set TermSet
    pInterface->writeCSValue("/rootNode-RoutingNode.TermSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-RoutingNode.TermSetStatus",termTimestamp,termSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*termSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSetCode",termTimestamp,termSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*termSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM SET with correct values TermSet=0
    ////////////////////////////////////////////////////////////////
    // Set TermSet
    pInterface->writeCSValue("/rootNode-RoutingNode.TermSet",readTimestamp,(std::int32_t)0); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-RoutingNode.TermSetStatus",termTimestamp,termSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"OK",*termSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSetCode",termTimestamp,termSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*termSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM SET with incorrect values
    ////////////////////////////////////////////////////////////////

    // Set TermSet
    pInterface->writeCSValue("/rootNode-RoutingNode.TermSrc",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-RoutingNode.TermDst",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
    pInterface->writeCSValue("/rootNode-RoutingNode.TermSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe

    // Get SetStatus
    pInterface->getPushedString("/rootNode-RoutingNode.TermSetStatus",termTimestamp,termSetStatus); // PVVariables are thread safe
    EXPECT_EQ((std::string)"WRONG",*termSetStatus);

    // Get SetCode
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSetCode",termTimestamp,termSetCode); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)-1,*termSetCode);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM DST READ
    ////////////////////////////////////////////////////////////////
    std::int32_t TermDstRead(2);
    const std::int32_t* termSrcRead;
    const std::int32_t* termSyncRead;
    const std::int32_t* termInvertRead;
    // Set TermDstRead
    pInterface->writeCSValue("/rootNode-RoutingNode.TermDstRead",readTimestamp,TermDstRead); // PVVariables are thread safe
    // Get TermSrcRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSrcRead",termTimestamp,termSrcRead); // PVVariables are thread safe
    EXPECT_EQ(TermDstRead + 1,*termSrcRead);
    // Get TermSyncRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSyncRead",termTimestamp,termSyncRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*termSyncRead);
    // Get TermInvertRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermInvertRead",termTimestamp,termInvertRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1,*termInvertRead);

    ////////////////////////////////////////////////////////////////
    ///TEST TERM DST READ
    ////////////////////////////////////////////////////////////////
    TermDstRead=0;

    // Set TermDstRead
    pInterface->writeCSValue("/rootNode-RoutingNode.TermDstRead",readTimestamp,TermDstRead); // PVVariables are thread safe
    // Get TermSrcRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSrcRead",termTimestamp,termSrcRead); // PVVariables are thread safe
    EXPECT_EQ(TermDstRead + 1,*termSrcRead);
    // Get TermSyncRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermSyncRead",termTimestamp,termSyncRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*termSyncRead);
    // Get TermInvertRead
    pInterface->getPushedInt32("/rootNode-RoutingNode.TermInvertRead",termTimestamp,termInvertRead); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0,*termInvertRead);


    factory.destroyDevice("rootNode");

}
