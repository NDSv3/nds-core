
#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c float.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVFloat and testPushDataAcquiredVFloatInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVFloatTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c double.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVDBL and testPushDataAcquiredVDBLInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVDBLTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c int32_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVI32 and testPushDataAcquiredVI32Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVI32Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c int64_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVI64 and testPushDataAcquiredVI64Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVI64Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c int8_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVI8 and testPushDataAcquiredVI8Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVI8Test(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c uint8_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVUI8 and testPushDataAcquiredVUI8Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVUI8Test(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DAQ node for data type @c float.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredFloat and testPushDataAcquiredFloatInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsFloatTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DAQ node for data type @c double.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredDBL and testPushDataAcquiredDBLInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsDBLTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the DAQ node for data type @c int32_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredI32 and testPushDataAcquiredI32Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsI32Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the DAQ node for data type @c int64_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredI64 and testPushDataAcquiredI64Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsI64Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the decimation PVs included in the DAQ node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testDecimation and testDecimationInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonDecimationTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the DMA PVs included in the DAQ node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testDMAParameters and testDMAParametersInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonDMATest(const bool testInitializers = false);

TEST(testDataAcquisition, testStateMachine)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceVectorDBL and named rootNode
    factory.createDevice("DeviceVectorDBL", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
    ::sleep(2);

    //TODO: Check generated data

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}


TEST(testDataAcquisition, testPushDataAcquiredVFloat)
{
    commonPVsVFloatTest();
}
TEST(testDataAcquisition, testPushDataAcquiredVFloatInit)
{
    commonPVsVFloatTest(true);
}

static void commonPVsVFloatTest(const bool testInitializers)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorFloat", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);


    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0.0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<float> pushData(128);
    const std::vector<float>* pRetrievedPushedValues;
    const timespec* pTime;
    double valueData=0;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }

            pInterface->getPushedVectorFloat("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVDBL)
{
    commonPVsVDBLTest();
}
TEST(testDataAcquisition, testPushDataAcquiredVDBLInit)
{
    commonPVsVDBLTest(true);
}

static void commonPVsVDBLTest(const bool testInitializers)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorDBL", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);


    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0.0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<double> pushData(128);
    const std::vector<double>* pRetrievedPushedValues;
    const timespec* pTime;
    double valueData=0;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }

            pInterface->getPushedVectorDouble("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(0, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVI32)
{
    commonPVsVI32Test();
}

TEST(testDataAcquisition, testPushDataAcquiredVI32Init)
{
    commonPVsVI32Test(true);
}

static void commonPVsVI32Test(const bool testInitializers)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorI32", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int32_t> pushData(128);
    const std::vector<std::int32_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t valueData=0;
    std::int32_t pushCounter=0;

    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }

            pInterface->getPushedVectorInt32("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}
TEST(testDataAcquisition, testPushDataAcquiredVI64)
{
    commonPVsVI64Test();
}
TEST(testDataAcquisition, testPushDataAcquiredVI64Init)
{
    commonPVsVI64Test(true);
}

static void commonPVsVI64Test(const bool testInitializers)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorI64", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);


    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0.0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int64_t> pushData(128);
    const std::vector<std::int64_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int64_t valueData=0;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }

            pInterface->getPushedVectorInt64("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");

}
TEST(testDataAcquisition, testPushDataAcquiredVI8)
{
    commonPVsVI8Test();
}

TEST(testDataAcquisition, testPushDataAcquiredVI8Init)
{
    commonPVsVI8Test(true);
}

static void commonPVsVI8Test(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorI8", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::int8_t> pushData(128);
    const std::vector<std::int8_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int8_t valueData=0;
    std::int32_t pushCounter=0;

    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }

            pInterface->getPushedVectorInt8("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVUI8)
{
    commonPVsVUI8Test();
}

TEST(testDataAcquisition, testPushDataAcquiredVUI8Init)
{
    commonPVsVUI8Test(true);
}

static void commonPVsVUI8Test(const bool testInitializers){
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorUI8", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<std::uint8_t> pushData(128);
    const std::vector<std::uint8_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::uint8_t valueData=0;
    std::int32_t pushCounter=0;

    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }

            pInterface->getPushedVectorUint8("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}


TEST(testDataAcquisition, testPushDataAcquiredFloat)
{
    commonPVsFloatTest();
}

TEST(testDataAcquisition, testPushDataAcquiredFloatInit)
{
    commonPVsFloatTest(true);
}

static void commonPVsFloatTest(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceFloat", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    float pushData(0);
    const float* pRetrievedPushedValues;
    const timespec* pTime;
    float valueData=0;
    std::int32_t pushCounter=0;


    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            pushData = valueData;
            pInterface->getPushedFloat("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredDBL)
{
    commonPVsDBLTest();
}

TEST(testDataAcquisition, testPushDataAcquiredDBLInit)
{
    commonPVsDBLTest(true);
}

static void commonPVsDBLTest(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceDBL", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    double pushData(0);
    const double* pRetrievedPushedValues;
    const timespec* pTime;
    double valueData=0;
    std::int32_t pushCounter=0;


    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            pushData = valueData;
            pInterface->getPushedDouble("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredI32)
{
    commonPVsI32Test();
}
TEST(testDataAcquisition, testPushDataAcquiredI32Init)
{
    commonPVsI32Test(true);
}

static void commonPVsI32Test(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceI32", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::int32_t pushData(0);
    const std::int32_t* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t valueData=0;
    std::int32_t pushCounter=0;

    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            pushData = valueData;

            pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}
TEST(testDataAcquisition, testPushDataAcquiredI64)
{
    commonPVsI64Test();
}

TEST(testDataAcquisition, testPushDataAcquiredI64Init)
{
    commonPVsI64Test(true);
}

static void commonPVsI64Test(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceI64", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::int64_t pushData(0);
    const std::int64_t* pRetrievedPushedValues;
    const timespec* pTime;
    std::int64_t valueData=0;
    std::int32_t pushCounter=0;


    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            pushData = valueData;
            pInterface->getPushedInt64("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
            ++valueData;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}


TEST(testDataAcquisition, testDecimation)
{
    commonDecimationTest();
}

TEST(testDataAcquisition, testDecimationInit)
{
    commonDecimationTest(true);
}

static void commonDecimationTest(const bool testInitializers) {


    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorDBL", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        //--------------------------------------------------------------
        //Verifies values provided by initializers methods
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.0, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 15.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.2, initialDoubleValue);
        std::cout<<"\tInitial Impedance = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 10.0, initialDoubleValue);
        std::cout<<"\tInitial SamplingRate = "<< initialDoubleValue <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 2, initialInt32Value);
        std::cout<<"\tInitial DecimationType = "<< initialInt32Value <<std::endl;
    }

  // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get DecimationType
    std::int32_t decimationType;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DecimationType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType",&readTimestamp,&decimationType); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, decimationType);


    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    double Impedance;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((double)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/Get SamplingRate
    double SamplingRate;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SamplingRate", timestamp, (double)5000);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.SamplingRate_RBV",&readTimestamp,&SamplingRate); // PVVariables are thread safe
    EXPECT_EQ((double)5000, SamplingRate);

    // Set/Get Decimation
    std::int32_t decimation;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Decimation", timestamp, (std::int32_t)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation",&readTimestamp,&decimation); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)10, decimation);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the Acquisition node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

    //Initialize comparison vector
    std::vector<double> pushData(128);
    const std::vector<double>* pRetrievedPushedValues;
    const timespec* pTime;
    double valueData=0;
    std::int32_t pushCounter=0;
    std::int32_t decimationCounter=0;
    size_t scanVector(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){
            for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                pushData[scanVector] = valueData;
            }
            ++valueData;
            if((std::int32_t)(decimationCounter+1) % decimation ==0){
                pInterface->getPushedVectorDouble("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
                ++pushCounter;
                ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
                for(size_t compare(0); compare != pushData.size(); ++compare)
                {
                    EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
                }
            }
            ++decimationCounter;
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }
    EXPECT_EQ(startTimestamp, pTime->tv_sec);
    EXPECT_EQ(0, pTime->tv_nsec);
    ++startTimestamp;

    factory.destroyDevice("rootNode");


}


TEST(testDataAcquisition, testDMAParameters)
{
    commonDMATest();
}

TEST(testDataAcquisition, testDMAParametersInit)
{
    commonDMATest(true);
}

static void commonDMATest(const bool testInitializers){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers) {
        parameters["INIT"]="YES";
    }

    factory.createDevice("DeviceVectorDBL", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    if (testInitializers) {
        std::int32_t initialInt32Value;
        pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial DMAEnable = "<< initialInt32Value <<std::endl;
    }

    // Check DAQ Node initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change DAQ Node state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Get DMABufferSize initial value
    std::int32_t DMABufferSize;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMABufferSize",&readTimestamp,&DMABufferSize); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)4194304, DMABufferSize);

    // Get DMANumChannels initial value
    std::int32_t DMANumChannels;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMANumChannels",&readTimestamp,&DMANumChannels); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)4, DMANumChannels);

    // Get DMAFrameType initial value
    std::int32_t DMAFrameType;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAFrameType",&readTimestamp,&DMAFrameType); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1, DMAFrameType);

    // Get DMASampleSize initial value
    std::int32_t DMASampleSize;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMASampleSize",&readTimestamp,&DMASampleSize); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)4, DMASampleSize);

    // Get DMAEnable initial value
    std::int32_t DMAEnable;
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, DMAEnable);


    //Change DAQ Node state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    // Get DMAEnable initial value
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DMAEnable", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1, DMAEnable);

    //Change DAQ Node state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change DAQ Node state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    factory.destroyDevice("rootNode");

}
