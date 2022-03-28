#ifdef _WIN32
static const double M_PI = 3.14159265358979323846;
#endif
#include <math.h>

#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "nds3/exceptions.h"

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

/**
 * @brief Internal function to test the PVs included in the WFG node for vectors of type @c double.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedVDBL and testPushDataGeneratedVDBLInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVDBLTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the WFG node for vectors of type @c int32_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedVI32 and testPushDataGeneratedVI32Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVI32Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the WFG node for vectors of type @c int64_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedVI64 and testPushDataGeneratedVI64Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVI64Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the WFG node for vectors of type @c int8_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedVI8 and testPushDataGeneratedVI8Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVI8Test(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the WFG node for vectors of type @c uint8_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedVUI8 and testPushDataGeneratedVUI8Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsVUI8Test(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the WFG node for data type @c double.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedDBL and testPushDataGeneratedDBLInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsDBLTest(const bool testInitializers = false);

/**
 * @brief Internal function to test the PVs included in the WFG node for data type @c int32_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedI32 and testPushDataGeneratedI32Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsI32Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the PVs included in the WFG node for data type @c int64_t.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testPushDataGeneratedI64 and testPushDataGeneratedI64Init test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonPVsI64Test(const bool testInitializers = false);
/**
 * @brief Internal function to test the decimation PVs included in the WFG node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * testdecimation and testdecimationInit test,
 * this function shares the full test and the evaluation or not of
 * the initialization features depends on the @a testInitializers flag.
 */
static void commonDecimationTest(const bool testInitializers = false);


TEST(testWFG, testStateMachine)
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
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
    ::sleep(2);

    //TODO: Check generated data

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testWFG, testPushDataGeneratedVDBL)
{
    commonPVsVDBLTest();
}

TEST(testWFG, testPushDataGeneratedVDBLInit)
{
    commonPVsVDBLTest(true);
}

static void commonPVsVDBLTest(const bool testInitializers) {
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        std::vector<double> initialVDoubleValues;
        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialVDoubleValues);
        std::vector<double> expectedValues(128, 4.25); //128 doubles with value 4.25
        EXPECT_EQ((bool) true, (expectedValues == initialVDoubleValues));
        std::cout<<"\tInitial DataAWG = 128 doubles with value 4.25"<< std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)5);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)5, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<double> pushData(128);
    size_t last_sample(0);
    const std::vector<double>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 1:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 2:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 3:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
                }
                break;
            case 4:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                }
                break;
            case 5:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 6:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 7:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            }
            last_sample+=scanVector;

            pInterface->getPushedVectorDouble("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }

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

TEST(testWFG, testPushDataGeneratedVI8)
{
    commonPVsVI8Test();
}

TEST(testWFG, testPushDataGeneratedVI8Init)
{
    commonPVsVI8Test(true);
}

static void commonPVsVI8Test(const bool testInitializers)
{
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        std::vector<std::int8_t> initialVInt8Values;
        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialVInt8Values);
        std::vector<std::int8_t> expectedValues(128, 2); //128 integers(8) with value 2
        EXPECT_EQ((bool) true, (expectedValues == initialVInt8Values));
        std::cout<<"\tInitial DataAWG = 128 integers(8) with value 2"<< std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)127);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)127, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::int8_t> pushData(128);
    size_t last_sample(0);
    const std::vector<std::int8_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 1:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 2:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 3:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
                }
                break;
            case 4:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                }
                break;
            case 5:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 6:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 7:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;

            default:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            }
            last_sample+=scanVector;

            pInterface->getPushedVectorInt8("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testWFG, testPushDataGeneratedVUI8)
{
    commonPVsVUI8Test();
}

TEST(testWFG, testPushDataGeneratedVUI8Init)
{
    commonPVsVUI8Test(true);
}

static void commonPVsVUI8Test(const bool testInitializers)
{
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        std::vector<std::uint8_t> initialVUInt8Values;
        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialVUInt8Values);
        std::vector<std::uint8_t> expectedValues(128, 2); //128 integers(8) with value 2
        EXPECT_EQ((bool) true, (expectedValues == initialVUInt8Values));
        std::cout<<"\tInitial DataAWG = 128 unsigned integers(8) with value 2"<< std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)255);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)255, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::uint8_t> pushData(128);
    size_t last_sample(0);
    const std::vector<std::uint8_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 1:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 2:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 3:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
                }
                break;
            case 4:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                }
                break;
            case 5:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 6:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 7:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            }
            last_sample+=scanVector;

            pInterface->getPushedVectorUint8("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testWFG, testPushDataGeneratedVI32)
{
    commonPVsVI32Test();
}

TEST(testWFG, testPushDataGeneratedVI32Init)
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        std::vector<std::int32_t> initialVInt32Values;
        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialVInt32Values);
        std::vector<std::int32_t> expectedValues(128, 5); //128 integers(32) with value 5
        EXPECT_EQ((bool) true, (expectedValues == initialVInt32Values));
        std::cout<<"\tInitial DataAWG = 128 integers(32) with value 5"<< std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)1024);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)1024, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::int32_t> pushData(128);
    size_t last_sample(0);
    const std::vector<std::int32_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 1:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 2:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 3:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
                }
                break;
            case 4:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                }
                break;
            case 5:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 6:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 7:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            }
            last_sample+=scanVector;

            pInterface->getPushedVectorInt32("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}
TEST(testWFG, testPushDataGeneratedVI64)
{
    commonPVsVI64Test();
}

TEST(testWFG, testPushDataGeneratedVI64Init)
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        std::vector<std::int64_t> initialVInt64Values;
        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialVInt64Values);
        std::vector<std::int64_t> expectedValues(128, 2); //128 integers(8) with value 2
        EXPECT_EQ((bool) true, (expectedValues == initialVInt64Values));
        std::cout<<"\tInitial DataAWG = 128 integers(64) with value 2"<< std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)127);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)127, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::int64_t> pushData(128);
    size_t last_sample(0);
    const std::vector<std::int64_t>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    size_t scanVector(0);
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 1:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 2:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 3:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
                }
                break;
            case 4:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                }
                break;
            case 5:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 6:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 7:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;

            default:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            }
            last_sample+=scanVector;

            pInterface->getPushedVectorInt64("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
            for(size_t compare(0); compare != pushData.size(); ++compare)
            {
                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
            }
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testWFG, testPushDataGeneratedDBL)
{
    commonPVsDBLTest();
}

TEST(testWFG, testPushDataGeneratedDBLInit)
{
    commonPVsDBLTest(true);
}

static void commonPVsDBLTest(const bool testInitializers)
{
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double)  2.25, initialDoubleValue);
        std::cout<<"\tInitial DataAWG = "<< initialDoubleValue << std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)5);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)5, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/get DMAEnable
    std::int32_t DMAEnable;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DMAEnable", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1, DMAEnable);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe





    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    double pushData(0);
    size_t last_sample(0);
    const double* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                    pushData = amplitude;

                break;
            case 1:
                    pushData = amplitude;

                break;
            case 2:
                    pushData = amplitude;

                break;
            case 3:
                    pushData = (double)amplitude * sin((2*M_PI*(last_sample)*frequency)/updateRate + phase) + offset;
                break;
            case 4:
                    pushData = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                break;
            case 5:
                    pushData = amplitude;
                break;
            case 6:
                    pushData = amplitude;
                break;
            case 7:
                    pushData = amplitude;
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                    pushData = amplitude;
                break;
            }
            last_sample++;

            pInterface->getPushedDouble("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testWFG, testPushDataGeneratedI32)
{
    commonPVsI32Test();
}

TEST(testWFG, testPushDataGeneratedI32Init)
{
    commonPVsI32Test(true);
}

static void commonPVsI32Test(const bool testInitializers)
{
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -5, initialInt32Value);
        std::cout<<"\tInitial DataAWG = " << initialInt32Value << std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)1024);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)1024, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/get DMAEnable
    std::int32_t DMAEnable;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DMAEnable", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1, DMAEnable);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::int32_t pushData(0);
    size_t last_sample(0);
    const std::int32_t* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
        std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                    pushData = amplitude;

                break;
            case 1:
                    pushData = amplitude;

                break;
            case 2:
                    pushData = amplitude;

                break;
            case 3:
                    pushData = (double)amplitude * sin((2*M_PI*(last_sample)*frequency)/updateRate + phase) + offset;
                break;
            case 4:
                    pushData = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                break;
            case 5:
                    pushData = amplitude;
                break;
            case 6:
                    pushData = amplitude;
                break;
            case 7:
                    pushData = amplitude;
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                    pushData = amplitude;
                break;
            }
            last_sample++;

            pInterface->getPushedInt32("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}
TEST(testWFG, testPushDataGeneratedI64)
{
    commonPVsI64Test();
}

TEST(testWFG, testPushDataGeneratedI64Init)
{
    commonPVsI64Test(true);
}

static void commonPVsI64Test(const bool testInitializers)
{
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

        pInterface->readCSValue("/rootNode-WFGNode.Frequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.5e6, initialDoubleValue);
        std::cout<<"\tInitial Frequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.RefFrequency", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.0e3, initialDoubleValue);
        std::cout<<"\tInitial RefFrequency = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Amplitude", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 3.3, initialDoubleValue);
        std::cout<<"\tInitial Amplitude = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Phase", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 60.0, initialDoubleValue);
        std::cout<<"\tInitial Phase = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.UpdateRate", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 4.0, initialDoubleValue);
        std::cout<<"\tInitial UpdateRate = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DutyCycle", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 75.0, initialDoubleValue);
        std::cout<<"\tInitial DutyCycle = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Gain", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 5.0, initialDoubleValue);
        std::cout<<"\tInitial Gain = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Offset", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 1.1, initialDoubleValue);
        std::cout<<"\tInitial Offset = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.BandWidth", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 20.0, initialDoubleValue);
        std::cout<<"\tInitial BandWidth = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Resolution", &timestamp, &initialDoubleValue);
        EXPECT_EQ((double) 2.0, initialDoubleValue);
        std::cout<<"\tInitial Resolution = "<< initialDoubleValue <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Impedance", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Impedance = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Coupling", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Coupling = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalRefType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalRefType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.SignalType", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial SignalType = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.Ground", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 0, initialInt32Value);
        std::cout<<"\tInitial Ground = "<< initialInt32Value <<std::endl;


        //--------------------------------------------------------------
        //Verifies values provided by initialization variables
        //--------------------------------------------------------------

        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;

        pInterface->readCSValue("/rootNode-WFGNode.DataAWG", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) -5, initialInt32Value);
        std::cout<<"\tInitial DataAWG = " << initialInt32Value << std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)1024);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)1024, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/get DMAEnable
    std::int32_t DMAEnable;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DMAEnable", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1, DMAEnable);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::int64_t pushData(0);
    size_t last_sample(0);
    const std::int64_t* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                    pushData = amplitude;

                break;
            case 1:
                    pushData = amplitude;

                break;
            case 2:
                    pushData = amplitude;

                break;
            case 3:
                    pushData = (double)amplitude * sin((2*M_PI*(last_sample)*frequency)/updateRate + phase) + offset;
                break;
            case 4:
                    pushData = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                break;
            case 5:
                    pushData = amplitude;
                break;
            case 6:
                    pushData = amplitude;
                break;
            case 7:
                    pushData = amplitude;
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                    pushData = amplitude;
                break;
            }
            last_sample++;

            pInterface->getPushedInt64("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
            ++pushCounter;
            EXPECT_EQ(pushData, (*pRetrievedPushedValues));
        }
    }
    catch(const std::runtime_error& e)
    {

        std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
    }

    factory.destroyDevice("rootNode");

}

TEST(testWFG, testdecimation)
{
    commonDecimationTest();
}

TEST(testWFG, testdecimationInit)
{
    commonDecimationTest(true);
}

static void commonDecimationTest(const bool testInitializers)
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
        pInterface->readCSValue("/rootNode-WFGNode.Decimation", &timestamp, &initialInt32Value);
        EXPECT_EQ((std::int32_t) 1, initialInt32Value);
        std::cout<<"\tInitial Decimation = "<< initialInt32Value <<std::endl;
    }

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-WFGNode.Amplitude", timestamp, (double)5);
    pInterface->readCSValue("/rootNode-WFGNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)5, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-WFGNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-WFGNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-WFGNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-WFGNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-WFGNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-WFGNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-WFGNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-WFGNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-WFGNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-WFGNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-WFGNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-WFGNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-WFGNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-WFGNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-WFGNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-WFGNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-WFGNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    // Set/get DMAEnable
    std::int32_t DMAEnable;
    pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DMAEnable", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)1, DMAEnable);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
     *  Spline=1 (disabled)
     *  DC=2
     *  Sin=3
     *  Square=4
     *  Triangle=5(disabled)
     *  Pulse=6(disabled)
     *  Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-WFGNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-WFGNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

    // Set/Get Ground
    std::int32_t decimation;
    pInterface->writeCSValue("/rootNode-WFGNode.Decimation", timestamp, (std::int32_t)10);
    pInterface->readCSValue("/rootNode-WFGNode.Decimation",&readTimestamp,&decimation); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)10, decimation);

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-WFGNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-WFGNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-WFGNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
    EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<double> pushData(128);
    size_t last_sample(0);
    const std::vector<double>* pRetrievedPushedValues;
    const timespec* pTime;
    std::int32_t pushCounter=0;
    std::int32_t decimationCounter=0;
    size_t scanVector(0);
    std::int64_t angle(0);
    try{
        while(pushCounter<=NumberOfPushedDataBlocks){

            switch(auxsignalType){

            case 0:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 1:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 2:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 3:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
                }
                break;
            case 4:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
                }
                break;
            case 5:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 6:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            case 7:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;

                // Fill the vector with a DC wave of maxamplitude
            default:
                for(scanVector=0; scanVector != pushData.size(); ++scanVector){
                    pushData[scanVector] = amplitude;
                }
                break;
            }
            last_sample+=scanVector;
            if((std::int32_t)(decimationCounter+1) % decimation ==0){
                pInterface->getPushedVectorDouble("/rootNode-WFGNode.Data", pTime, pRetrievedPushedValues);
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
