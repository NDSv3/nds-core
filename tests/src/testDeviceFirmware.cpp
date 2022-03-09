#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

/**
 * @brief Internal function to test all PVs included in the Firmware node.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * PVsTest and PVsTestInitialization test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonPVsTest(const bool testInitializers = false);

TEST(testDeviceFirmware, StateMachineTest)
{
  const timespec* pFirmwareStateMachineSwitchTime;
  const std::int32_t* pFirmwareStateMachineState;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceFirmware and name it deviceFirmware
  factory.createDevice("DeviceFirmware", "deviceFirmware", nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceFirmware");


  // Check FirmwareNode initial state (OFF)
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pFirmwareStateMachineState);


  //Get the Firmware global state
  std::int32_t globalStateFirmware;
  pInterface->readCSValue("/deviceFirmware-Firm.StateMachine.getGlobalState",
              &timestamp, &globalStateFirmware);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateFirmware);

  //Change FirmwareNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pFirmwareStateMachineState);

  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pFirmwareStateMachineState);

  //Get the Firmware global state
  pInterface->readCSValue("/deviceFirmware-Firm.StateMachine.getGlobalState",
              &timestamp, &globalStateFirmware);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateFirmware);

  //Change FirmwareNode state:  ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pFirmwareStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pFirmwareStateMachineState);
  ::sleep(2);

  //Get the Firmware global state
  pInterface->readCSValue("/deviceFirmware-Firm.StateMachine.getGlobalState",
              &timestamp, &globalStateFirmware);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateFirmware);

  //Change FirmwareNode state:  RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pFirmwareStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pFirmwareStateMachineState);

  //Change FirmwareNode state:  ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
               timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pFirmwareStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState",
                 pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pFirmwareStateMachineState);

  // Destroy test device
  factory.destroyDevice("deviceFirmware");

}

TEST(testDeviceFirmware, PVsTest){
    commonPVsTest();
}

TEST(testDeviceFirmware, PVsTestInitialization){
    commonPVsTest(true);
}

static void commonPVsTest(bool testInitializers){

      const timespec * ptimestamp;
      timespec timestamp = {0, 0};

      //Create factory
      nds::Factory factory("test");

      // Create test device of type DeviceFirmware and name it deviceFirmware
      nds::namedParameters_t parameters; //(1);
      if (testInitializers) {
          parameters["INIT"]="YES";
      }
      factory.createDevice("DeviceFirmware", "deviceFirmware", parameters);

      //Get instance of the Test Control System
      nds::tests::TestControlSystemInterfaceImpl* pInterface =
        nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceFirmware");

      //Change FirmwareNode state:  OFF -> (initializing) -> ON
      pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::on);
      ::sleep(2);

      //Get the firmware version
      const std::string * firmwareVersion = NULL;
      pInterface->getPushedString("/deviceFirmware-Firm.Version",
                      ptimestamp, firmwareVersion);
      EXPECT_EQ((std::string)"Firmware test version", *firmwareVersion);

      //Get the firmware status
      const std::int32_t * firmwareStatus = NULL;
      pInterface->getPushedInt32("/deviceFirmware-Firm.Status",
                     ptimestamp, firmwareStatus);
      EXPECT_EQ((std::int32_t)0, *firmwareStatus);

      //Get the hardware revision
      const std::string * hardwareRevision = NULL;
      pInterface->getPushedString("/deviceFirmware-Firm.HWRevision",
                      ptimestamp, hardwareRevision);
      EXPECT_EQ((std::string)"Firmware test hardware revision", *hardwareRevision);

      //Get the serial number
      const std::string * serialNumber = NULL;
      pInterface->getPushedString("/deviceFirmware-Firm.SerialNumber",
                      ptimestamp, serialNumber);
      EXPECT_EQ((std::string)"Firmware test serial number", *serialNumber);

      //Get the device model
      const std::string * deviceModel = NULL;
      pInterface->getPushedString("/deviceFirmware-Firm.Model",
                      ptimestamp, deviceModel);
      EXPECT_EQ((std::string)"Firmware test device model", *deviceModel);

      //Get the device type
      const std::string * deviceType = NULL;
      pInterface->getPushedString("/deviceFirmware-Firm.Type",
                      ptimestamp, deviceType);
      EXPECT_EQ((std::string)"Firmware test device type", *deviceType);

      //Get the driver version
      const std::string * driverVersion = NULL;
      pInterface->getPushedString("/deviceFirmware-Firm.DriverVersion",
                      ptimestamp, driverVersion);
      EXPECT_EQ("<major_id>.<minor_id>.<maintenance_id>", *driverVersion);

      //Get the chassis number
      const std::int32_t * chassisNumber = NULL;
      pInterface->getPushedInt32("/deviceFirmware-Firm.ChassisNumber",
                     ptimestamp, chassisNumber);
      EXPECT_EQ((std::int32_t)42, *chassisNumber);

      //Get the slot number
      const std::int32_t * slotNumber = NULL;
      pInterface->getPushedInt32("/deviceFirmware-Firm.SlotNumber",
                       ptimestamp, slotNumber);
      EXPECT_EQ((std::int32_t)42, *slotNumber);

      //Get the initial firmware path
      const std::string * firmwarePath;
      if (testInitializers) {
          std::string initialPath;
          pInterface->readCSValue("/deviceFirmware-Firm.FilePath", &timestamp, &initialPath);
          EXPECT_EQ((std::string)"not available", initialPath);
          std::cout<<"\tInitial FilePath = "<< initialPath <<std::endl;
      }
      pInterface->getPushedString("/deviceFirmware-Firm.FilePath_RBV",
                      ptimestamp, firmwarePath);
      EXPECT_EQ((std::string)"Firmware path to be uploaded", *firmwarePath);


      //Change FirmwareNode state:  ON -> (starting) -> RUNNING
      pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::running);
      ::sleep(3);

      //Change Firmware Path
      std::string NewFirmwarePath="New FirmwarePath";
      pInterface->writeCSValue("/deviceFirmware-Firm.FilePath", timestamp, NewFirmwarePath);
      ::sleep(2);
      pInterface->getPushedString("/deviceFirmware-Firm.FilePath_RBV",
                      ptimestamp, firmwarePath);
      EXPECT_EQ((std::string)"New FirmwarePath", *firmwarePath);

      //Change FirmwareNode state:  RUNNING -> (stopping) -> ON
      pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::on);
      ::sleep(3);
      //Change FirmwareNode state:  ON -> (switchingOff) -> OFF
      pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState",
                   timestamp, (std::int32_t)nds::state_t::off);

      // Destroy test device
      factory.destroyDevice("deviceFirmware");
}

