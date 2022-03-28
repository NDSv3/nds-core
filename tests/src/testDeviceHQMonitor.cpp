#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

/**
 * @brief Internal function to test all PVs included in the HQMonitor node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * PVsTest and PVsTestInitialization test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonPVsTest(const bool testInitializers = false);

TEST(testDeviceHQMonitor, stateMachine)
{
  const timespec* pHQMonitorStateMachineSwitchTime;
  const std::int32_t* pHQMonitorStateMachineState;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type deviceHQMonitor and name it deviceHQMonitor
  factory.createDevice("DeviceHQMonitor", "deviceHQMonitor", nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceHQMonitor");

  // Check HQMonitor initial state (OFF)
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pHQMonitorStateMachineState);

  //Get the HQMonitor global state
  std::int32_t globalStateHQMonitor;
  pInterface->readCSValue("/deviceHQMonitor-HQMonitor.StateMachine.getGlobalState",
              &timestamp, &globalStateHQMonitor);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateHQMonitor);

  //Change HQMonitor state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pHQMonitorStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pHQMonitorStateMachineState);

  //Get the HQMonitor global state
  pInterface->readCSValue("/deviceHQMonitor-HQMonitor.StateMachine.getGlobalState",
              &timestamp, &globalStateHQMonitor);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateHQMonitor);

  //Change HQMonitor state:  ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pHQMonitorStateMachineState);
  ::sleep(2);

  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pHQMonitorStateMachineState);
  ::sleep(2);

  //Get the HQMonitor global state
  pInterface->readCSValue("/deviceHQMonitor-HQMonitor.StateMachine.getGlobalState",
              &timestamp, &globalStateHQMonitor);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateHQMonitor);

  //Change HQMonitor state:  RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pHQMonitorStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on,
        *pHQMonitorStateMachineState);

  //Change HQMonitor state:  ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pHQMonitorStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.StateMachine.getState",
                 pHQMonitorStateMachineSwitchTime,
                 pHQMonitorStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pHQMonitorStateMachineState);

  // Destroy test device
  factory.destroyDevice("deviceHQMonitor");

}

TEST(testDeviceHQMonitor, PVs){
    commonPVsTest();
}

TEST(testDeviceHQMonitor, PVsInitialization){
    commonPVsTest(true);
}

static void commonPVsTest(bool testInitializers){

      timespec timestamp = {0, 0};

      //Create factory
      nds::Factory factory("test");

      // Create test device of type deviceHQMonitor and name it deviceHQMonitor
      nds::namedParameters_t parameters; //(1);
      if (testInitializers) {
          parameters["INIT"]="YES";
      }
      factory.createDevice("DeviceHQMonitor", "deviceHQMonitor", parameters);
      //Get instance of the Test Control System
      nds::tests::TestControlSystemInterfaceImpl* pInterface =
        nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceHQMonitor");

      //Change HQMonitor state:  OFF -> (initializing) -> ON
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::on);
      ::sleep(2);

      //Change HQMonitor state:  ON -> (starting) -> RUNNING
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::running);
      ::sleep(2);

      /*
       * ---------------------------------------------------------------------------
       * Test PVs
       * ---------------------------------------------------------------------------
       */

      const timespec* pTimestamp;

      const double* power;
      //Initial Power
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevPower",
                      pTimestamp, power);
      EXPECT_EQ((double) 0.42, *power);

      //Updated Power
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevPower",
                      pTimestamp, power);
      EXPECT_EQ((double) 0.25, *power);

      const double* temperature;
      //Initial temperature
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevTemperature",
                      pTimestamp, temperature);
      EXPECT_EQ((double)296.5, *temperature);

      //Updated temperature
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevTemperature",
                      pTimestamp, temperature);
      EXPECT_EQ((double)295.5, *temperature);

      const double* voltage;
      //Initial Voltage
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevVoltage",
                      pTimestamp, voltage);
      EXPECT_EQ((double)2.6, *voltage);

      //Updated Voltage
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevVoltage",
                      pTimestamp,
                      voltage);
      EXPECT_EQ((double)2.5, *voltage);

      const double* current;
      //Initial Current
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevCurrent",
                      pTimestamp, current);
      EXPECT_EQ((double)0.2, *current);

      //Updated Current
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.DevCurrent",
                      pTimestamp,
                      current);
      EXPECT_EQ((double)0.1, *current);


      const std::int32_t* SEUEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialSEUEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.SEUEnable", &timestamp, &initialSEUEnable);
          EXPECT_EQ((std::int32_t) 101, initialSEUEnable);
          std::cout<<"\tInitial SEUEnable = "<< initialSEUEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.SEUEnable_RBV",
                     pTimestamp, SEUEnable);
      EXPECT_EQ((std::int32_t)0, *SEUEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.SEUEnable",
                   timestamp, (std::int32_t)1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.SEUEnable_RBV",
                     pTimestamp, SEUEnable);
      EXPECT_EQ((std::int32_t)1, *SEUEnable);

      const std::int32_t* DAQEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialDAQEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.DAQEnable", &timestamp, &initialDAQEnable);
          EXPECT_EQ((std::int32_t) 102, initialDAQEnable);
          std::cout<<"\tInitial DAQEnable = "<< initialDAQEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.DAQEnable_RBV",
                     pTimestamp, DAQEnable);
      EXPECT_EQ((std::int32_t)0, *DAQEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.DAQEnable",
                   timestamp, (std::int32_t) 1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.DAQEnable_RBV",
                     pTimestamp, DAQEnable);
      EXPECT_EQ((std::int32_t)1, *DAQEnable);

      const std::string * selfTestTxtResult;
      pInterface->getPushedString("/deviceHQMonitor-HQMonitor.TestTxtResult",
                  pTimestamp, selfTestTxtResult);
      EXPECT_EQ((std::string)"", *selfTestTxtResult);

      const std::int32_t* testEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialTestEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestEnable", &timestamp, &initialTestEnable);
          EXPECT_EQ((std::int32_t) 103, initialTestEnable);
          std::cout<<"\tInitial TestEnable = "<< initialTestEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestEnable_RBV",
                     pTimestamp, testEnable);
      EXPECT_EQ((std::int32_t)0, *testEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.TestEnable",
                   timestamp, (std::int32_t)1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestEnable_RBV",
                     pTimestamp, testEnable);
      EXPECT_EQ((std::int32_t)1, *testEnable);

      const std::int32_t* testType;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialTestType;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestType", &timestamp, &initialTestType);
          EXPECT_EQ((std::int32_t) 1, initialTestType);
          std::cout<<"\tInitial TestType = "<< initialTestType <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestType_RBV",
                     pTimestamp, testType);
      EXPECT_EQ((std::int32_t)0, *testType);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.TestType",
                   timestamp, (std::int32_t)1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestType_RBV",
                     pTimestamp, testType);
      EXPECT_EQ((std::int32_t)1, *testType);

      const std::int32_t* testVerboseEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialTestVerboseEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestVerboseEnable", &timestamp, &initialTestVerboseEnable);
          EXPECT_EQ((std::int32_t) 105, initialTestVerboseEnable);
          std::cout<<"\tInitial TestVerboseEnable = "<< initialTestVerboseEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestVerboseEnable_RBV",
                     pTimestamp, testVerboseEnable);
      EXPECT_EQ((std::int32_t)0, *testVerboseEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.TestVerboseEnable",
                   timestamp, (std::int32_t)1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestVerboseEnable_RBV",
                     pTimestamp, testVerboseEnable);
      EXPECT_EQ((std::int32_t)1, *testVerboseEnable);

      const std::int32_t* testIDEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialTestIDEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestIDEnable", &timestamp, &initialTestIDEnable);
          EXPECT_EQ((std::int32_t) 106, initialTestIDEnable);
          std::cout<<"\tInitial TestIDEnable = "<< initialTestIDEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestIDEnable_RBV",
                     pTimestamp, testIDEnable);
      EXPECT_EQ((std::int32_t)0, *testIDEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.TestIDEnable",
                   timestamp, (std::int32_t) 1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestIDEnable_RBV",
                     pTimestamp, testIDEnable);
      EXPECT_EQ((std::int32_t)1, *testIDEnable);

      const std::int32_t* tesTxtEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialTestTxtEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestTxtEnable", &timestamp, &initialTestTxtEnable);
          EXPECT_EQ((std::int32_t) 107, initialTestTxtEnable);
          std::cout<<"\tInitial TestTxtEnable = "<< initialTestTxtEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestTxtEnable_RBV",
                     pTimestamp, tesTxtEnable);
      EXPECT_EQ((std::int32_t)0, *tesTxtEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.TestTxtEnable",
                   timestamp, (std::int32_t)1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestTxtEnable_RBV",
                     pTimestamp, tesTxtEnable);
      EXPECT_EQ((std::int32_t)1, *tesTxtEnable);

      const std::int32_t* testCodeResultEnable;
      if (testInitializers) {
          //Check initial value
          std::int32_t initialTestCodeResultEnable;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestCodeResultEnable", &timestamp, &initialTestCodeResultEnable);
          EXPECT_EQ((std::int32_t) 108, initialTestCodeResultEnable);
          std::cout<<"\tInitial TestCodeResultEnable = "<< initialTestCodeResultEnable <<std::endl;
      }
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestCodeResultEnable_RBV",
                     pTimestamp, testCodeResultEnable);

      EXPECT_EQ((std::int32_t)0, *testCodeResultEnable);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.TestCodeResultEnable",
                   timestamp, (std::int32_t)1);
      pInterface->getPushedInt32("/deviceHQMonitor-HQMonitor.TestCodeResultEnable_RBV",
                     pTimestamp, testCodeResultEnable);
      EXPECT_EQ((std::int32_t)1, *testCodeResultEnable);

      ::sleep(2);
      timespec valueTimespec;
      std::string valueString;
      pInterface->readCSValue("/deviceHQMonitor-HQMonitor.TestTxtResult", &valueTimespec, &valueString);
      EXPECT_EQ((std::string)"Verbose information\nTest ID: 123\nTest TxtID: Test-01\nCodeResult: 1\n",
              valueString);

      std::int32_t signalQualityFlag;
      pInterface->readCSValue("/deviceHQMonitor-HQMonitor.SignalQFlag",
                  &timestamp, &signalQualityFlag);
      EXPECT_EQ((std::int32_t)0, signalQualityFlag);

      const double* signalQFlagTrigLevel;
      if (testInitializers) {
          //Check initial value
          double initialSignalQualityFlagLevel;
          pInterface->readCSValue("/deviceHQMonitor-HQMonitor.SignalQFlagTrigLevel", &timestamp, &initialSignalQualityFlagLevel);
          EXPECT_EQ((double) 1.65, initialSignalQualityFlagLevel);
          std::cout<<"\tInitial SignalQFlagTrigLevel = "<< initialSignalQualityFlagLevel <<std::endl;
      }
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.SignalQFlagTrigLevel_RBV",
                      pTimestamp, signalQFlagTrigLevel);
      EXPECT_EQ((double)0.5, *signalQFlagTrigLevel);
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.SignalQFlagTrigLevel",
                   timestamp, (double)1.2);
      pInterface->getPushedDouble("/deviceHQMonitor-HQMonitor.SignalQFlagTrigLevel_RBV",
                      pTimestamp, signalQFlagTrigLevel);
      EXPECT_EQ((double)1.2, *signalQFlagTrigLevel);

      //Change HQMonitor state:  RUNNING -> (stopping) -> ON
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::on);
      ::sleep(2);

      //Change HQMonitor state:  ON -> (switchingOff) -> OFF
      pInterface->writeCSValue("/deviceHQMonitor-HQMonitor.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::off);
      ::sleep(2);

      // Destroy test device
      factory.destroyDevice("deviceHQMonitor");
}

