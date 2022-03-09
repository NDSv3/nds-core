#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"


/**
 * @brief Internal function to test all constructors included in the StateMachine node.
 * @param structuredConstructor Flag to set to @c true when the constructor that
 * receives a structure shall be tested.
 * As the constructor used for the StateMachine node is the only difference between
 * stateMachine and stateMachineStructure test, this function shares the full test
 * and the constructor used depends on the @a structuredConstructor flag.
 */
static void commonPVsTest(const bool structuredConstructor = false);

TEST(testDeviceStateMachine, stateMachine)
{
    commonPVsTest();
}

TEST(testDeviceStateMachine, stateMachineStructure)
{
    commonPVsTest(true);
}

static void commonPVsTest(const bool structuredConstructor) {
    const timespec* pStateMachineStateMachineSwitchTime;
    const std::int32_t* pStateMachineStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    nds::namedParameters_t parameters;
    if (structuredConstructor) {
      parameters["INIT"]="YES";
    }
    // Create test device of type deviceStateMachine and name it deviceStateMachine
    factory.createDevice("DeviceStateMachine", "deviceStateMachine", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceStateMachine");
    // Check StateMachine initial state (OFF)
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineStateMachineState);

    //Get the StateMachine global state
    std::int32_t globalStateStateMachine;
    pInterface->readCSValue("/deviceStateMachine-StateMachine.getGlobalState", &timestamp, &globalStateStateMachine);
    EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateStateMachine);

    //Change StateMachine state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineStateMachineState);

    //Get the StateMachine global state
    pInterface->readCSValue("/deviceStateMachine-StateMachine.getGlobalState", &timestamp, &globalStateStateMachine);
    EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateStateMachine);

    //Change StateMachine state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineStateMachineState);
    ::sleep(2);

    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineStateMachineState);
    ::sleep(2);

    //Get the StateMachine global state
    pInterface->readCSValue("/deviceStateMachine-StateMachine.getGlobalState", &timestamp, &globalStateStateMachine);
    EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateStateMachine);

    //Change StateMachine state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineStateMachineState);

    //Change StateMachine state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineStateMachineState);
    ::sleep(2);
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineStateMachineState);

    // Destroy test device
    factory.destroyDevice("deviceStateMachine");

    if (structuredConstructor) {
      std::cout<<"\t(Constructor that receives a structure tested)" <<std::endl;
    }

}

