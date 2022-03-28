#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"


/**
 * @brief Internal function to test the PVs included in the DigitalIO node for vectors of type @c bool.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataBool and testPushDataBoolInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsBoolTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DigitalIO node for vectors of type @c int8_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataI8 and testPushDataI8Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsInt8Test(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DigitalIO node for vectors of type @c int16_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataI16 and testPushDataI16Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsInt16Test(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DigitalIO node for vectors of type @c int32_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataI32 and testPushDataI32Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsInt32Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the DigitalIO node for vectors of type @c int64_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataI64 and testPushDataI64Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsInt64Test(const bool testInitializers = false);

TEST(testDigitalIO, testStateMachineBool)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDigitalIO and named rootNode
    factory.createDevice("DeviceDigitalIO", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testDigitalIO, testPushDataBool)
{
    commonPVsBoolTest();
}

TEST(testDigitalIO, testPushDataBoolInit)
{
    commonPVsBoolTest(true);
}

static void commonPVsBoolTest(const bool testInitializers)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceDigitalIO", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;
        double initialDoubleValue;
        std::vector<bool> initialVBoolValues;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOBoolNode.DataOutMask", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolMask(1, true); //Vector with one data to true.
        EXPECT_EQ((bool) true, (expectedBoolMask == initialVBoolValues));
        std::cout<<"\tInitial DataOutMask = 1 bool with value true" <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOBoolNode.VoltLevelHigh", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOBoolNode.VoltLevelLow", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOBoolNode.ChannelDir", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolDir(1, false); //Vector with one data to false.
        EXPECT_EQ((bool) true, (expectedBoolDir == initialVBoolValues));
        std::cout<<"\tInitial ChannelDir = 1 bool with value false" <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOBoolNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOBoolNode.DataOut", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolData(128, true); //Vector with 128 data to true.
        EXPECT_EQ((bool) true, (expectedBoolData == initialVBoolValues));
        std::cout<<"\tInitial DataOut = 128 bools with value true" <<std::endl;

    }

    // Set/Get dataOutMask
    std::vector<bool> dataOutMaskIn = {1,1,0,1,0};
    std::vector<bool> dataOutMaskOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.DataOutMask", timestamp, dataOutMaskIn);
    pInterface->readCSValue("/rootNode-DigitalIOBoolNode.DataOutMask_RBV",&readTimestamp,&dataOutMaskOut); // PVVariables are thread safe
    for(size_t i=0; i<dataOutMaskOut.size();i++){
        EXPECT_EQ(dataOutMaskIn[i],dataOutMaskOut[i]);
    }

    //Set/Get voltLevelHigh
    double voltLevelHigh;
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.VoltLevelHigh", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOBoolNode.VoltLevelHigh_RBV",&readTimestamp,&voltLevelHigh); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelHigh);

    // Set/Get voltLevelLow
    double voltLevelLow;
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.VoltLevelLow", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOBoolNode.VoltLevelLow_RBV",&readTimestamp,&voltLevelLow); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelLow);

    // Set/Get channelDir
    std::vector<bool> channelDirIn = {1,0,1,0,1};
    std::vector<bool> channelDirOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.ChannelDir", timestamp, channelDirIn);
    pInterface->readCSValue("/rootNode-DigitalIOBoolNode.ChannelDir_RBV",&readTimestamp,&channelDirOut); // PVVariables are thread safe
    for(size_t i=0; i<channelDirOut.size();i++){
        EXPECT_EQ(channelDirIn[i],channelDirOut[i]);
    }

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200; //TODO Study this
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOBoolNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOBoolNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DigitalIOBoolNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<bool> pushData(128);
    const std::vector<bool>* pRetrievedPushedValues;
    bool pushVar = false;
    const timespec* pTime;
    double readCount=0;

    size_t scanVector(0);
    try{
        while(readCount<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = pushVar;
                pushVar = (scanVector%2 ? true:false);
            }

            pInterface->getPushedVectorBool("/rootNode-DigitalIOBoolNode.DataIn", pTime, pRetrievedPushedValues);
            ++readCount;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ((bool)pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}

TEST(testDigitalIO, testStateMachineI8)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDigitalIO and named rootNode
    factory.createDevice("DeviceDigitalIO", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}


TEST(testDigitalIO, testPushDataI8)
{
    commonPVsInt8Test();
}

TEST(testDigitalIO, testPushDataI8Init)
{
    commonPVsInt8Test(true);
}

static void commonPVsInt8Test(const bool testInitializers)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceDigitalIO", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;
        double initialDoubleValue;
        std::vector<bool> initialVBoolValues;
        std::vector<std::int8_t> initialVInt8Values;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI8Node.DataOutMask", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolMask(8, false); //Vector with 8 data to false.
        EXPECT_EQ((bool) true, (expectedBoolMask == initialVBoolValues));
        std::cout<<"\tInitial DataOutMask = 8 bool with value false" <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI8Node.VoltLevelHigh", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI8Node.VoltLevelLow", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI8Node.ChannelDir", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolDir(8, true); //Vector with 8 data to true.
        EXPECT_EQ((bool) true, (expectedBoolDir == initialVBoolValues));
        std::cout<<"\tInitial ChannelDir = 8 bool with value true" <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI8Node.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI8Node.DataOut", &timestamp, &initialVInt8Values);
        std::vector<std::int8_t> expectedInt8Dir(128, 100); //Vector with 128 data to 100.
        EXPECT_EQ((bool) true, (expectedInt8Dir == initialVInt8Values));
        std::cout<<"\tInitial DataOut = 128 int8 data with value 100" <<std::endl;

    }

    // Set/Get dataOutMask
    std::vector<bool> dataOutMaskIn = {1,1,0,1,0};
    std::vector<bool> dataOutMaskOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.DataOutMask", timestamp, dataOutMaskIn);
    pInterface->readCSValue("/rootNode-DigitalIOI8Node.DataOutMask_RBV",&readTimestamp,&dataOutMaskOut); // PVVariables are thread safe
    for(size_t i=0; i<dataOutMaskOut.size();i++){
        EXPECT_EQ(dataOutMaskIn[i],dataOutMaskOut[i]);
    }

    // Set/Get voltLevelHigh
    double voltLevelHigh;
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.VoltLevelHigh", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI8Node.VoltLevelHigh_RBV",&readTimestamp,&voltLevelHigh); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelHigh);

    // Set/Get voltLevelLow
    double voltLevelLow;
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.VoltLevelLow", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI8Node.VoltLevelLow_RBV",&readTimestamp,&voltLevelLow); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelLow);

    // Set/Get channelDir
    std::vector<bool> channelDirIn = {1,0,1,0,1};
    std::vector<bool> channelDirOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.ChannelDir", timestamp, channelDirIn);
    pInterface->readCSValue("/rootNode-DigitalIOI8Node.ChannelDir_RBV",&readTimestamp,&channelDirOut); // PVVariables are thread safe
    for(size_t i=0; i<channelDirOut.size();i++){
        EXPECT_EQ(channelDirIn[i],channelDirOut[i]);
    }

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI8Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI8Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DigitalIOI8Node.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int8_t> pushData(128);
    const std::vector<std::int8_t>* pRetrievedPushedValues;
    const timespec* pTime;
    double readCount=0;
    size_t scanVector(0);
    try{
        while(readCount<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = readCount;
            }

            pInterface->getPushedVectorInt8("/rootNode-DigitalIOI8Node.DataIn", pTime, pRetrievedPushedValues);
            ++readCount;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}


TEST(testDigitalIO, testStateMachineI16)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDigitalIO and named rootNode
    factory.createDevice("DeviceDigitalIO", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testDigitalIO, testPushDataI16)
{
    commonPVsInt16Test();
}

TEST(testDigitalIO, testPushDataI16Init)
{
    commonPVsInt16Test(true);
}

static void commonPVsInt16Test(const bool testInitializers)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceDigitalIO", "rootNode",  parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;
        double initialDoubleValue;
        std::vector<bool> initialVBoolValues;
        std::vector<std::int16_t> initialVInt16Values;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI16Node.DataOutMask", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolMask(16, true); //Vector with 16 data to true.
        EXPECT_EQ((bool) true, (expectedBoolMask == initialVBoolValues));
        std::cout<<"\tInitial DataOutMask = 16 bool with value true" <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI16Node.VoltLevelHigh", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI16Node.VoltLevelLow", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI16Node.ChannelDir", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolDir(16, false); //Vector with 16 data to false.
        EXPECT_EQ((bool) true, (expectedBoolDir == initialVBoolValues));
        std::cout<<"\tInitial ChannelDir = 16 bool with value false" <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI16Node.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI16Node.DataOut", &timestamp, &initialVInt16Values);
        std::vector<std::int16_t> expectedInt16Dir(128, 200); //Vector with 128 data to 200.
        EXPECT_EQ((bool) true, (expectedInt16Dir == initialVInt16Values));
        std::cout<<"\tInitial DataOut = 128 int16 data with value 200" <<std::endl;

    }

    // Set/Get dataOutMask
    std::vector<bool> dataOutMaskIn = {1,1,0,1,0};
    std::vector<bool> dataOutMaskOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.DataOutMask", timestamp, dataOutMaskIn);
    pInterface->readCSValue("/rootNode-DigitalIOI16Node.DataOutMask_RBV",&readTimestamp,&dataOutMaskOut); // PVVariables are thread safe
    for(size_t i=0; i<dataOutMaskOut.size();i++){
        EXPECT_EQ(dataOutMaskIn[i],dataOutMaskOut[i]);
    }

    // Set/Get voltLevelHigh
    double voltLevelHigh;
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.VoltLevelHigh", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI16Node.VoltLevelHigh_RBV",&readTimestamp,&voltLevelHigh); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelHigh);

    // Set/Get voltLevelLow
    double voltLevelLow;
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.VoltLevelLow", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI16Node.VoltLevelLow_RBV",&readTimestamp,&voltLevelLow); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelLow);

    // Set/Get channelDir
    std::vector<bool> channelDirIn = {1,0,1,0,1};
    std::vector<bool> channelDirOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.ChannelDir", timestamp, channelDirIn);
    pInterface->readCSValue("/rootNode-DigitalIOI16Node.ChannelDir_RBV",&readTimestamp,&channelDirOut); // PVVariables are thread safe
    for(size_t i=0; i<channelDirOut.size();i++){
        EXPECT_EQ(channelDirIn[i],channelDirOut[i]);
    }

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI16Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI16Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DigitalIOI16Node.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int16_t> pushData(128);
    const std::vector<std::int16_t>* pRetrievedPushedValues;
    const timespec* pTime;
    double readCount=0;
    size_t scanVector(0);
    try{
        while(readCount<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = readCount;
            }

            pInterface->getPushedVectorInt16("/rootNode-DigitalIOI16Node.DataIn", pTime, pRetrievedPushedValues);
            ++readCount;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}



TEST(testDigitalIO, testStateMachineI32)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDigitalIO and named rootNode
    factory.createDevice("DeviceDigitalIO", "rootNode",  nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testDigitalIO, testPushDataI32)
{
    commonPVsInt32Test();
}

TEST(testDigitalIO, testPushDataI32Init)
{
    commonPVsInt32Test(true);
}

static void commonPVsInt32Test(const bool testInitializers)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceDigitalIO", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;
        double initialDoubleValue;
        std::vector<bool> initialVBoolValues;
        std::vector<std::int32_t> initialVInt32Values;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI32Node.DataOutMask", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolMask(32, false); //Vector with 32 data to false.
        EXPECT_EQ((bool) true, (expectedBoolMask == initialVBoolValues));
        std::cout<<"\tInitial DataOutMask = 32 bool with value false" <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI32Node.VoltLevelHigh", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI32Node.VoltLevelLow", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI32Node.ChannelDir", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolDir(32, true); //Vector with 32 data to true.
        EXPECT_EQ((bool) true, (expectedBoolDir == initialVBoolValues));
        std::cout<<"\tInitial ChannelDir = 32 bool with value true" <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI32Node.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI32Node.DataOut", &timestamp, &initialVInt32Values);
        std::vector<std::int32_t> expectedInt32Dir(128, 300); //Vector with 128 data to 300.
        EXPECT_EQ((bool) true, (expectedInt32Dir == initialVInt32Values));
        std::cout<<"\tInitial DataOut = 128 int32 data with value 300" <<std::endl;

    }

    // Set/Get dataOutMask
    std::vector<bool> dataOutMaskIn = {1,1,0,1,0};
    std::vector<bool> dataOutMaskOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.DataOutMask", timestamp, dataOutMaskIn);
    pInterface->readCSValue("/rootNode-DigitalIOI32Node.DataOutMask_RBV",&readTimestamp,&dataOutMaskOut); // PVVariables are thread safe
    for(size_t i=0; i<dataOutMaskOut.size();i++){
        EXPECT_EQ(dataOutMaskIn[i],dataOutMaskOut[i]);
    }

    // Set/Get voltLevelHigh
    double voltLevelHigh;
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.VoltLevelHigh", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI32Node.VoltLevelHigh_RBV",&readTimestamp,&voltLevelHigh); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelHigh);

    // Set/Get voltLevelLow
    double voltLevelLow;
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.VoltLevelLow", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI32Node.VoltLevelLow_RBV",&readTimestamp,&voltLevelLow); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelLow);

    // Set/Get channelDir
    std::vector<bool> channelDirIn = {1,0,1,0,1};
    std::vector<bool> channelDirOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.ChannelDir", timestamp, channelDirIn);
    pInterface->readCSValue("/rootNode-DigitalIOI32Node.ChannelDir_RBV",&readTimestamp,&channelDirOut); // PVVariables are thread safe
    for(size_t i=0; i<channelDirOut.size();i++){
        EXPECT_EQ(channelDirIn[i],channelDirOut[i]);
    }

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI32Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI32Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DigitalIOI32Node.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int32_t> pushData(128);
    const std::vector<std::int32_t>* pRetrievedPushedValues;
    const timespec* pTime;
    double readCount=0;
    size_t scanVector(0);
    try{
        while(readCount<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = readCount;
            }

            pInterface->getPushedVectorInt32("/rootNode-DigitalIOI32Node.DataIn", pTime, pRetrievedPushedValues);
            ++readCount;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}

TEST(testDigitalIO, testStateMachineI64)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDigitalIO and named rootNode
    factory.createDevice("DeviceDigitalIO", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}


TEST(testDigitalIO, testPushDataI64)
{
    commonPVsInt64Test();
}

TEST(testDigitalIO, testPushDataI64Init)
{
    commonPVsInt64Test(true);
}

static void commonPVsInt64Test(const bool testInitializers)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceDigitalIO", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {

        std::int32_t initialInt32Value;
        double initialDoubleValue;
        std::vector<bool> initialVBoolValues;
        std::vector<std::int64_t> initialVInt64Values;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI64Node.DataOutMask", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolMask(64, false); //Vector with 64 data to false.
        EXPECT_EQ((bool) true, (expectedBoolMask == initialVBoolValues));
        std::cout<<"\tInitial DataOutMask = 64 bool with value false" <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI64Node.VoltLevelHigh", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI64Node.VoltLevelLow", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial VoltLevelHigh = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI64Node.ChannelDir", &timestamp, &initialVBoolValues);
        std::vector<bool> expectedBoolDir(64, true); //Vector with 64 data to true.
        EXPECT_EQ((bool) true, (expectedBoolDir == initialVBoolValues));
        std::cout<<"\tInitial ChannelDir = 64 bool with value true" <<std::endl;

        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-DigitalIOI64Node.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DigitalIOI64Node.DataOut", &timestamp, &initialVInt64Values);
        std::vector<std::int64_t> expectedInt64Dir(128, 300); //Vector with 128 data to 100.
        EXPECT_EQ((bool) true, (expectedInt64Dir == initialVInt64Values));
        std::cout<<"\tInitial DataOut = 128 int64 data with value 100" <<std::endl;

    }

    // Set/Get dataOutMask
    std::vector<bool> dataOutMaskIn = {1,1,0,1,0};
    std::vector<bool> dataOutMaskOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.DataOutMask", timestamp, dataOutMaskIn);
    pInterface->readCSValue("/rootNode-DigitalIOI64Node.DataOutMask_RBV",&readTimestamp,&dataOutMaskOut); // PVVariables are thread safe
    for(size_t i=0; i<dataOutMaskOut.size();i++){
        EXPECT_EQ(dataOutMaskIn[i],dataOutMaskOut[i]);
    }

    // Set/Get voltLevelHigh
    double voltLevelHigh;
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.VoltLevelHigh", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI64Node.VoltLevelHigh_RBV",&readTimestamp,&voltLevelHigh); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelHigh);

    // Set/Get voltLevelLow
    double voltLevelLow;
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.VoltLevelLow", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DigitalIOI64Node.VoltLevelLow_RBV",&readTimestamp,&voltLevelLow); // PVVariables are thread safe
    EXPECT_EQ((double)0, voltLevelLow);

    // Set/Get channelDir
    std::vector<bool> channelDirIn = {1,0,1,0,1};
    std::vector<bool> channelDirOut = {0,0,0,0,0};
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.ChannelDir", timestamp, channelDirIn);
    pInterface->readCSValue("/rootNode-DigitalIOI64Node.ChannelDir_RBV",&readTimestamp,&channelDirOut); // PVVariables are thread safe
    for(size_t i=0; i<channelDirOut.size();i++){
        EXPECT_EQ(channelDirIn[i],channelDirOut[i]);
    }

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIOI64Node.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIOI64Node.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DigitalIOI64Node.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int64_t> pushData(128);
    const std::vector<std::int64_t>* pRetrievedPushedValues;
    const timespec* pTime;
    double readCount=0;
    size_t scanVector(0);
    try{
        while(readCount<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = readCount;
            }

            pInterface->getPushedVectorInt64("/rootNode-DigitalIOI64Node.DataIn", pTime, pRetrievedPushedValues);
            ++readCount;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}

