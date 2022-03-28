#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"


/**
 * @brief Internal function to test all PVs included in the HQMonitor node.
 * @param structuredConstructor Flag to set to @c true when the constructor that
 * receives a structure shall be tested.
 * As the constructor used for the Timing node is the only difference between
 * fullTest and fullTestStructure test, this function shares the full test
 * and the constructor used depends on the @a structuredConstructor flag.
 */
static void commonPVsTest(const bool structuredConstructor = false);

TEST(testDeviceTiming, fullTest){
	commonPVsTest();
}

TEST(testDeviceTiming, fullTestStructure){
	commonPVsTest(true);
}

static void commonPVsTest(const bool structuredConstructor) {


  const timespec* pTimingStateMachineSwitchTime;
  const std::int32_t* pTimingStateMachineState;
  timespec timestamp = {0, 0};
  const timespec *ptimestamp = &timestamp;

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceTiming and name it deviceTiming
  nds::namedParameters_t parameters;
  if (structuredConstructor) {
	  parameters["INIT"]="YES";
  }
  factory.createDevice("DeviceTiming", "deviceTiming",parameters);

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceTiming");


  // Check TimingNode initial state (OFF)
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
              pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimingStateMachineState);

  //Get the Timing global state
  std::int32_t globalStateTiming;
  pInterface->readCSValue("/deviceTiming-Timing.StateMachine.getGlobalState",
      &timestamp, &globalStateTiming);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateTiming);


  //Change TimingNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pTimingStateMachineState);

  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimingStateMachineState);

  //Get the Timing global state
  pInterface->readCSValue("/deviceTiming-Timing.StateMachine.getGlobalState",
      &timestamp, &globalStateTiming);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateTiming);


  //Get clock frequency (UTC).
  const double *pClkFreqVal;
  pInterface->getPushedDouble("/deviceTiming-Timing.ClkFrequency",
			  ptimestamp, pClkFreqVal);
  EXPECT_EQ((double)100.001, *pClkFreqVal);

  //Get clock multiplier (UTC).
  const int32_t *pClkMultiplier;
  pInterface->getPushedInt32("/deviceTiming-Timing.ClkMultiplier",
			  ptimestamp, pClkMultiplier);
  EXPECT_EQ((int32_t)2, *pClkMultiplier);

  //Get synchronizing status.
  const int32_t *psyncStatVal;
  pInterface->getPushedInt32("/deviceTiming-Timing.SyncStatus",
			  ptimestamp, psyncStatVal);
  EXPECT_EQ((int32_t)1, *psyncStatVal);

  //Get seconds since last synchronization.
  const int32_t *psecsLastSyncVal;
  pInterface->getPushedInt32("/deviceTiming-Timing.SecsLastSync",
			 ptimestamp, psecsLastSyncVal);
  EXPECT_EQ((int32_t)0, *psecsLastSyncVal);

  //Get reference base time.
  timespec timeval = {0, 0};
  const timespec *ptimeval = &timeval;
  pInterface->getPushedTimespec("/deviceTiming-Timing.RefTimeBase",
			  ptimestamp, ptimeval);
  EXPECT_EQ((time_t)1514764800, ptimeval->tv_sec);
  EXPECT_EQ((long int)1514764810, ptimeval->tv_nsec);

  //Change TimingNode state: ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pTimingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pTimingStateMachineState);
  ::sleep(2);


  //Get the Timing global state
  pInterface->readCSValue("/deviceTiming-Timing.StateMachine.getGlobalState",
      &timestamp, &globalStateTiming);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateTiming);

  //Get time (UTC).
  pInterface->getPushedTimespec("/deviceTiming-Timing.Time", ptimestamp, ptimeval);
  EXPECT_EQ((time_t)1514764800, ptimeval->tv_sec);
  EXPECT_EQ((long int)20091982, ptimeval->tv_nsec);

  //Get human readable time (UTC).
  const std::string *phtime;
  pInterface->getPushedString("/deviceTiming-Timing.HTime", ptimestamp, phtime);
  EXPECT_EQ((const char*)"Mon Jan  1 00:00:00 2018\n", *phtime);

  //Get Synchronization status.
  pInterface->getPushedInt32("/deviceTiming-Timing.SyncStatus",
			  ptimestamp, psyncStatVal);
  EXPECT_EQ((int32_t)2, *psyncStatVal);


  //Change TimingNode state: RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pTimingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimingStateMachineState);

  //Change TimingNode state: ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pTimingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimingStateMachineState);

  //Get Syncronizing status.
  pInterface->getPushedInt32("/deviceTiming-Timing.SyncStatus",
			  ptimestamp, psyncStatVal);
  EXPECT_EQ((int32_t)0, *psyncStatVal);

  //Get Syncronizing status.
  pInterface->getPushedInt32("/deviceTiming-Timing.SecsLastSync",
			  ptimestamp, psecsLastSyncVal);
  EXPECT_EQ((int32_t)10, *psecsLastSyncVal);

  // Destroy test device
  factory.destroyDevice("deviceTiming");


  if (structuredConstructor) {
	  std::cout<<"\t(Constructor that receives a structure tested)" <<std::endl;
  }

}
