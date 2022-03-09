#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"


/**
 * @brief Internal function to test all PVs included in the Timestamping node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * SetGetTest and SetGetInitializationTest test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonPVsTest(const bool testInitializers = false);

TEST(testDeviceTimestamping, StateMachineTest){

  const timespec* pTimestampingStateMachineSwitchTime;
  const std::int32_t* pTimestampingStateMachineState;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceTimestamping and name it deviceTimestamping
  factory.createDevice("DeviceTimestamping",
               "deviceTimestamping",
               nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceTimestamping");

  // Check TimestampingNode initial state (OFF)
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimestampingStateMachineState);

  // Get the Timestamping global state
  std::int32_t globalStateTimestamping = -1;
  pInterface->readCSValue("/deviceTimestamping-Timestamping.StateMachine.getGlobalState",
              &timestamp, &globalStateTimestamping);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateTimestamping);


  // Change TimestampingNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pTimestampingStateMachineState);

  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimestampingStateMachineState);

  // Get the Timestamping global state
  pInterface->readCSValue("/deviceTimestamping-Timestamping.StateMachine.getGlobalState",
              &timestamp, &globalStateTimestamping);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateTimestamping);

  //Change TimestampingNode state: ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pTimestampingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pTimestampingStateMachineState);
  ::sleep(2);

  // Get the Timestamping global state
  pInterface->readCSValue("/deviceTimestamping-Timestamping.StateMachine.getGlobalState",
              &timestamp, &globalStateTimestamping);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateTimestamping);

  // Change TimestampingNode state: RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pTimestampingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimestampingStateMachineState);

  // Change TimestampingNode state: ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pTimestampingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
                 pTimestampingStateMachineSwitchTime,
                 pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimestampingStateMachineState);


  // Destroy test device
  factory.destroyDevice("deviceTimestamping");

}



TEST(testDeviceTimestamping, SetGetTest){
    commonPVsTest();
}

TEST(testDeviceTimestamping, SetGetInitializationTest){
    commonPVsTest(true);
}


static void commonPVsTest(bool testInitializers){

  const timespec* ptimestamp = NULL;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceTimestamping and name it deviceTimestamping
  nds::namedParameters_t parameters;
  if (testInitializers) {
      parameters["INIT"]="YES";
  }
  factory.createDevice("DeviceTimestamping",
               "deviceTimestamping",
               parameters);

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
  nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceTimestamping");


  // Change TimestampingNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  ::sleep(2);

  // Get enable status.
  const std::int32_t * enable_val = NULL;
  if (testInitializers) {
      //Check initial value
      std::int32_t initialEnable;
      pInterface->readCSValue("/deviceTimestamping-Timestamping.Enable", &timestamp, &initialEnable);
      EXPECT_EQ((std::int32_t) 1, initialEnable);
      std::cout<<"\tInitial Enable = "<< initialEnable <<std::endl;
  }
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Enable_RBV",
                 ptimestamp, enable_val);
  EXPECT_EQ((std::int32_t)0 /* OFF */, *enable_val);

  // Get edge value.
  const std::int32_t * edge_val = NULL;
  if (testInitializers) {
      //Check initial value
      std::int32_t initialEdge;
      pInterface->readCSValue("/deviceTimestamping-Timestamping.Edge", &timestamp, &initialEdge);
      EXPECT_EQ((std::int32_t) 1, initialEdge);
      std::cout<<"\tInitial Edge = "<< initialEdge <<std::endl;
  }
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Edge_RBV",
                 ptimestamp, edge_val);
  EXPECT_EQ((std::int32_t)1 /* FALLING */, *edge_val);

  // Get maximum number of timestamps.
  const std::int32_t * max_tstamp = NULL;
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.MaxTimestamps",
                 ptimestamp, max_tstamp);
  EXPECT_EQ((std::int32_t)5, *max_tstamp);

  //Get overflow state.
  const std::int32_t * overflow_val = NULL;
  if (testInitializers) {
      //Check initial value
      std::int32_t initialClearOverflow;
      pInterface->readCSValue("/deviceTimestamping-Timestamping.ClearOverflow", &timestamp, &initialClearOverflow);
      EXPECT_EQ((std::int32_t) 1, initialClearOverflow);
      std::cout<<"\tInitial ClearOverflow = "<< initialClearOverflow <<std::endl;
  }
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
                 ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)0, *overflow_val);

  //Change TimestampingNode state: ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::running);
  ::sleep(2);

  // Set Edge to ANY
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.Edge",
               timestamp, (std::int32_t)2);

  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Edge_RBV",
                 ptimestamp, edge_val);
  EXPECT_EQ((std::int32_t)2 /* ANY */, *edge_val);

  // Set Enable to ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.Enable",
               timestamp, (std::int32_t)1);

  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Enable_RBV",
                 ptimestamp, enable_val);
  EXPECT_EQ((std::int32_t)1 /* ON */, *enable_val);
  ::sleep(4);

  // Testing values of the timestamps pushed.
  const nds::timestamp_t * pushed_timestamp;
  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
                   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->edge);
  EXPECT_EQ((std::int32_t)1, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
                   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->edge);
  EXPECT_EQ((std::int32_t)2, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
                   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->edge);
  EXPECT_EQ((std::int32_t)3, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
                   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)false, pushed_timestamp->edge);
  EXPECT_EQ((std::int32_t)4, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
                   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)false, pushed_timestamp->edge);
  EXPECT_EQ((std::int32_t)5, pushed_timestamp->id);

  // Get overflow state.
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
                 ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)2 /* FULL */, *overflow_val);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
                 ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->edge);
  EXPECT_EQ((std::int32_t)6, pushed_timestamp->id);

  // Get overflow state.
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
                 ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)1 /* OVERFLOWED */, *overflow_val);

  // Clear overflow
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.ClearOverflow",
               timestamp, (std::int32_t)2736);
               /*TODO: this function should not accept any value. */
  ::sleep(2);

  // Get overflow state.
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
                 ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)0 /* NO OVERFLOW */, *overflow_val);


  // Change TimestampingNode state: RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  ::sleep(2);

  // Change TimestampingNode state: ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::off);
  ::sleep(2);

  // Destroy test device
  factory.destroyDevice("deviceTimestamping");

}
