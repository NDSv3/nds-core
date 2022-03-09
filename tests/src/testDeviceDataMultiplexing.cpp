#include <gtest/gtest.h>
#include <numeric>

#include <nds3/nds.h>
#include "ndsTestInterface.h"
#include "testUtils.h"


static void dataSelfTest();

TEST(testDeviceDataMultiplexing, Self)
{
  dataSelfTest();
}

inline void compareFloatArrays(const std::vector<float> & inA, const std::vector<float> & inB) {
  EXPECT_EQ(inA.size(), inB.size());
  for (std::uint32_t i = 0; i < inA.size(); i++) {
      EXPECT_EQ(inA[i], inB[i]);
  }
}

static void dataSelfTest() {
    timespec timestamp = {0, 0};
    const timespec* pTimestamp;
    const std::int32_t* pInteger;
    const std::int32_t* pState;
    const std::int32_t maxElementsDevice = 20;
    const std::int32_t nChannelsDevice = 4;
    std::int32_t lastValue = 0;
    const std::vector<float>* pFloatArray;
    std::vector<float> floatArrayValues;
    std::vector<float> targetMultiplexed(maxElementsDevice*nChannelsDevice);
    std::iota (std::begin(targetMultiplexed), std::end(targetMultiplexed), lastValue);

    std::cout << "\t----------------------- Testing Data Sharing (Multiplexing) --------------------------" << std::endl;

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDataMultiplexing and name it deviceDataMultiplexing
    nds::namedParameters_t parameters;
    parameters["SELFTEST"]="YES";
    factory.createDevice("DeviceDataMultiplexing", "deviceDataMultiplexing", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceDataMultiplexing");


    //Subscribe Multiplexing-Input PVs to the Device-Source ones
    std::cout << "\t Subscribing DataMultiplexing inputs to Sources... " << std::endl;
    factory.subscribe("deviceDataMultiplexing-FloatArray_Source_0", "deviceDataMultiplexing-DataMultiplexing_4_float-DataIn_0");
    factory.subscribe("deviceDataMultiplexing-FloatArray_Source_1", "deviceDataMultiplexing-DataMultiplexing_4_float-DataIn_1");
    factory.subscribe("deviceDataMultiplexing-FloatArray_Source_2", "deviceDataMultiplexing-DataMultiplexing_4_float-DataIn_2");
    factory.subscribe("deviceDataMultiplexing-FloatArray_Source_3", "deviceDataMultiplexing-DataMultiplexing_4_float-DataIn_3");


    std::cout << "\t Testing State Machine (OFF -> ON -> RUNNING)... " << std::endl;

    // Check DataMultiplexing_4_floatNode initial state (OFF)
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pState);


    //Get the DataMultiplexing_4_float global state
    std::int32_t globalStateDataMultiplexing_4_float;
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getGlobalState",
                            &timestamp, &globalStateDataMultiplexing_4_float);
    EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateDataMultiplexing_4_float);

    //Change DataMultiplexing_4_floatNode state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.setState",
                             timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pState);

    ::sleep(1);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pState);

    //Get the DataMultiplexing_4_float global state
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getGlobalState",
                            &timestamp, &globalStateDataMultiplexing_4_float);
    EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateDataMultiplexing_4_float);


    //Set the number of samples per channel to be taken into account (it can only be done in ON state)
    pInterface->writeCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.SamplesPerChannel", timestamp, maxElementsDevice);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.SamplesPerChannel_RBV",
                            pTimestamp, pInteger);
    EXPECT_EQ((std::int32_t)maxElementsDevice, *pInteger);


    //Change DataMultiplexing_4_floatNode state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.setState",
                             timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pState);
    ::sleep(1);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pState);
    ::sleep(1);

    //Get the DataMultiplexing_4_float global state
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getGlobalState",
                            &timestamp, &globalStateDataMultiplexing_4_float);
    EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateDataMultiplexing_4_float);



    std::cout << "\t Testing data sharing and multiplexing..." << std::endl;

    //Increase Source PVs and verifies the new value in its target ones that are subscribed
    pInterface->writeCSValue("/deviceDataMultiplexing-IncreaseSources", timestamp, 1);

    //Input - Channel 0
    pInterface->getPushedVectorFloat("/deviceDataMultiplexing-FloatArray_Source_0", pTimestamp, pFloatArray);
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.DataIn_0", &timestamp, &floatArrayValues);
    compareFloatArrays(*pFloatArray, floatArrayValues);
    std::cout << "\t Input - Channel 0 -> " << TestUtils::getString(floatArrayValues) << std::endl;

    //Input - Channel 1
    pInterface->getPushedVectorFloat("/deviceDataMultiplexing-FloatArray_Source_1", pTimestamp, pFloatArray);
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.DataIn_1", &timestamp, &floatArrayValues);
    compareFloatArrays(*pFloatArray, floatArrayValues);
    std::cout << "\t Input - Channel 1 -> " << TestUtils::getString(floatArrayValues) << std::endl;

    //Input - Channel 2
    pInterface->getPushedVectorFloat("/deviceDataMultiplexing-FloatArray_Source_2", pTimestamp, pFloatArray);
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.DataIn_2", &timestamp, &floatArrayValues);
    compareFloatArrays(*pFloatArray, floatArrayValues);
    std::cout << "\t Input - Channel 2 -> " << TestUtils::getString(floatArrayValues) << std::endl;

    //Input - Channel 3
    pInterface->getPushedVectorFloat("/deviceDataMultiplexing-FloatArray_Source_3", pTimestamp, pFloatArray);
    pInterface->readCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.DataIn_3", &timestamp, &floatArrayValues);
    compareFloatArrays(*pFloatArray, floatArrayValues);
    std::cout << "\t Input - Channel 3 -> " << TestUtils::getString(floatArrayValues) << std::endl;

    //Trigger the multiplexing node
    pInterface->writeCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.Trigger", timestamp, 1);

    //Output - Channel 0
    pInterface->getPushedVectorFloat("/deviceDataMultiplexing-DataMultiplexing_4_float.DataOut_0", pTimestamp, pFloatArray);
    std::cout << "\t Output - Channel 0 -> " << TestUtils::getString(targetMultiplexed) << std::endl;
    compareFloatArrays(targetMultiplexed, *pFloatArray);

    std::cout << "\t Testing State Machine (RUNNING -> ON -> OFF)... " << std::endl;

    //Change DataMultiplexing_4_floatNode state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.setState",
                             timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pState);
    ::sleep(1);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pState);

    //Change DataMultiplexing_4_floatNode state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.setState",
                             timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pState);
    ::sleep(1);
    pInterface->getPushedInt32("/deviceDataMultiplexing-DataMultiplexing_4_float.StateMachine.getState",
                               pTimestamp, pState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pState);

    std::cout << "\t-------------------------------------------------------------------------" << std::endl;


    // Destroy test device
    factory.destroyDevice("deviceDataMultiplexing");

}





