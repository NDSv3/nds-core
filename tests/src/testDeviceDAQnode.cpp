#include <gtest/gtest.h>
#include "nds3/nds.h"
#include "ndsTestInterface.h"
#include "testUtils.h"

/**
 * @brief Internal function to test the PVs included in the DAQ node for vectors of type @c double.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataAcquiredVDBL and testPushDataAcquiredVDBLInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVDBLTest(const bool testInitializers = false, const bool autoEnable = false);

TEST(testDataAcquisition, testDataAcquiredVectorDoubles)
{
    commonPVsVDBLTest();
}

TEST(testDataAcquisition, testDataAcquiredVectorDoublesInit)
{
    commonPVsVDBLTest(true);
}

TEST(testDataAcquisition, testDataAcquiredVectorDoublesHSTM)
{
    commonPVsVDBLTest(true, true);
}

static void commonPVsVDBLTest(const bool testInitializers,  const bool autoEnable ){

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (testInitializers){
        parameters["INIT"]="YES";
    }

    if (autoEnable){
        parameters["AUTOENABLE"]="RUNNING";
    }else{
        parameters["AUTOENABLE"]="NONE";
    }

    factory.createDevice("Device", "rootNode", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");
    EXPECT_NE((void*)0, pInterface);

    if (testInitializers) {
        std::int32_t initialInt32Value;
        double initialDoubleValue;

        TestUtils::displayTitle("Verifying initial values");
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


    TestUtils::displayTitle("Configuring DAQ PV values");

    std::vector<double> datos(2,0);
    pInterface->readCSValue("/rootNode-VarIn_vDBL",&timestamp,&datos);

    // Set/Get DecimationType
    std::int32_t decimationType;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DecimationType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DecimationType",&readTimestamp,&decimationType); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, decimationType);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)10);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ(10.0, Gain);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

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
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (double)0.0);
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


    TestUtils::displayTitle("Turning ON the Device");
    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    TestUtils::displayTitle("Starting the Device");
    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    if(!autoEnable){
        // Check initial state (OFF)
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        TestUtils::displayTitle("Turning ON the DAQ Node");
        //Change state:  OFF -> (initializing) -> ON
        pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        TestUtils::displayTitle("Starting the DAQ Node");
        //Change state:  ON -> (starting) -> RUNNING
        pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
    }

    ::sleep(2);//Data is being generated and pushed to Control system

    if(!autoEnable){
        TestUtils::displayTitle("Stopping the DAQ Node");
        //Change state:  RUNNING -> (stopping) -> ON
        pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        TestUtils::displayTitle("Turning OFF the DAQ Node");
        //Change state:  ON -> (switchingOff) -> OFF
        pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
    }

    TestUtils::displayTitle("Stopping the Device");
    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    TestUtils::displayTitle("Turning OFF the Device");
    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
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

    try{
        while(pushCounter<=NumberOfPushedDataBlocks){
            pushData.clear();
            pushData.resize(128,valueData);

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
        std::cout<<"Number of pushed data blocks read is: " <<pushCounter << std::endl;
        std::cerr << e.what() << std::endl;
    }
    factory.destroyDevice("rootNode");

}
