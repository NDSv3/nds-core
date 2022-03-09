#include <functional>
#include <sstream>
#include <iostream>
#include <unistd.h>

#include "simulated_signal.h"

#include <nds3/nds.h>
#include "exampleDrv.h"

exampleDrv::exampleDrv(nds::Factory& factory,
               const std::string& device,
               const nds::namedParameters_t& /*parameters*/) {

  /**
   *   This will be the main class of the driver. To enable interface with Asyn
   * the the Root class is declared as Port.
   */
  nds::Port rootNode(device);

  /**
   *   Initializing a vector of data acquisition channels. Note that a name is
   * given as an identificator allowing that each DAQ node has a unique name.
   * Also, the rootNode is passed so each terminal Node can be added as a child
   * of the main driver class.
   *
   */
  size_t maxCh = 2; /* Number of channels. */
  for(size_t i = 0; i < maxCh; i++) {

    std::ostringstream chName;
    chName << "CH" << i;
    m_channels.push_back(std::make_shared<channel>(chName.str(), rootNode));
  }

  /**
   *   Initializing a vector of terminals with time and timestamping
   * functionalities. See the above description for DAQ for a more detailed
   * description of how to do this.
   *
   */
  size_t maxTr= 2; /* Number of channels. */
  for(size_t i = 0; i < maxTr; i++) {

    std::ostringstream trName;
    trName << "TR" << i;
    m_terminals.push_back(std::make_shared<terminal>(trName.str(), rootNode));
  }


  /**
   * @brief Calling the Firmware node constructor.
   *
   *   Adding Firmware node to the control system. Note that in this case is
   * added as a child of the Root node. See the exampleDrv cpp file for more
   * information.
   *
   *   All the delegate methods and the state machine functions must be passed
   * to the constructor of the node.
   *
   */
  m_firmware = rootNode.addChild(nds::Firmware(
         "Firmware", /* Node name*/
     256, /* Max. string length. */
     std::bind(&exampleDrv::switchon_firmware, this),
     std::bind(&exampleDrv::switchoff_firmware, this),
     std::bind(&exampleDrv::start_firmware, this),
     std::bind(&exampleDrv::stop_firmware, this),
     std::bind(&exampleDrv::recover_firmware, this),
     std::bind(&exampleDrv::allow_firmware_change,
           this, std::placeholders::_1,
           std::placeholders::_2, std::placeholders::_3),
     std::bind(&exampleDrv::pv_path_writer, this,
           std::placeholders::_1, std::placeholders::_2)));

  rootNode.initialize(this, factory);

}



exampleDrv::~exampleDrv() {

}

/**
 *   Firmware methods.
 *
 *   This are the methods the driver developer should implement to build the
 * driver. This is the space left for the developer to interact with the HW API
 * and write most of the driver code.
 *
 */

/**
 *   A description of the state machine functions is given in the terminal.cpp
 * file.
 *
 */
void exampleDrv::switchon_firmware() {

  // This example simulates tha calls to the API interface to know the details
  // of the firmware:

  /*Call to HW to get a firmware description. */
  hw_firm firmware = firmware_description();

  // Once the values are obtained they are sent to the CS:
  m_firmware.setFirmwareVersion(m_firmware.getTimestamp(),
                firmware.version);

  m_firmware.setFirmwareStatus(m_firmware.getTimestamp(),
                   firmware.status);

  m_firmware.setHardwareRevision(m_firmware.getTimestamp(),
                 firmware.revision);

  m_firmware.setSerialNumber(m_firmware.getTimestamp(),
                 firmware.serialNumber);

  m_firmware.setDeviceModel(m_firmware.getTimestamp(),
                firmware.deviceModel);

  m_firmware.setDeviceType(m_firmware.getTimestamp(),
               firmware.deviceType);

  m_firmware.setDriverVersion(m_firmware.getTimestamp(),
                  firmware.driverVersion);

  m_firmware.setChassisNumber(m_firmware.getTimestamp(),
                  firmware.chassisNum);

  m_firmware.setSlotNumber(m_firmware.getTimestamp(),
               firmware.slotNum);

  m_firmware.setFirmwarePath(m_firmware.getTimestamp(),
                 firmware.firmPath);
}



void exampleDrv::switchoff_firmware() {

}



void exampleDrv::start_firmware() {

  m_stop_firmware = false;
  /**
   *  This thread only gets the firmware path. It could alos provide the
   *  installation process of a new firmware.
   *
   */
  m_firmware_thread =
    m_firmware.runInThread("Firmware",std::bind(&exampleDrv::firmware_thread_body, this));
}



void exampleDrv::stop_firmware() {
  m_stop_firmware = true;
  m_firmware_thread.join();
}



void exampleDrv::recover_firmware() {
  throw nds::StateMachineRollBack("Cannot recover");
}



bool exampleDrv::allow_firmware_change(const nds::state_t,
                     const nds::state_t,
                     const nds::state_t) {
  return true;
}



void exampleDrv::pv_path_writer(const timespec& timestamp,
                  const std::string& /*value*/) {

  // Call to function programming the hardware. This function returns the
  // path to the firmware to be installed. This value has to be set to the
  // readback attribute.
  hw_firm new_firmware = change_hw_firmware();

  std::string new_firmwarePath = new_firmware.firmPath;

  // firmwarePath has the new firmware path to be programmed on the hardware.
  m_firmware.setFirmwarePath(timestamp, new_firmwarePath);
}



void exampleDrv::firmware_thread_body() {

  // Get FirmwareVersion
  std::string FirmwareVersion = m_firmware.getFirmwareVersion();
  // Get FirmwareStatus
  std::int32_t FirmwareStatus = m_firmware.getFirmwareStatus();
  // Get HardwareRevision
  std::string HardwareRevision = m_firmware.getHardwareRevision();
  // Get SerialNumber
  std::string SerialNumber = m_firmware.getSerialNumber();
  // Get DeviceModel
  std::string DeviceModel = m_firmware.getDeviceModel();
  // Get DeviceType
  std::string DeviceType = m_firmware.getDeviceType();
  // Get DriverVersion
  std::string DriverVersion = m_firmware.getDriverVersion();
  // Get ChassisNumber
  std::int32_t ChassisNumber = m_firmware.getChassisNumber();
  // Get SlotNumber
  std::int32_t SlotNumber = m_firmware.getSlotNumber();
  // Get FirmwarePath
  std::string FirmwarePath = m_firmware.getFirmwarePath();
  std::string FirmwarePathOld = m_firmware.getFirmwarePath();


  std::cout << "Firmware support information:" << std::endl;
  std::cout << "\tFirmwareVersion = " << FirmwareVersion<<std::endl;
  std::cout << "\tFirmwareStatus = " << FirmwareStatus << std::endl;
  std::cout << "\tHardwareRevision = " << HardwareRevision << std::endl;
  std::cout << "\tSerialNumber = " << SerialNumber << std::endl;
  std::cout << "\tDeviceModel = " << DeviceModel << std::endl;
  std::cout << "\tDeviceType = " << DeviceType << std::endl;
  std::cout << "\tDriverVersion = " << DriverVersion << std::endl;
  std::cout << "\tChassisNumber = " << ChassisNumber << std::endl;
  std::cout << "\tSlotNumber = " << SlotNumber << std::endl;
  std::cout << "\tFirmwarePath = " << FirmwarePath << std::endl;

  while(!m_stop_firmware) {


    // Get FirmwarePath
    std::string FirmwarePath = m_firmware.getFirmwarePath();
    if(FirmwarePath.compare(FirmwarePathOld) != 0) {

      // Push the Firmware data to the control system
      m_firmware.push(m_firmware.getTimestamp(), FirmwarePath);
      FirmwarePathOld = FirmwarePath;
    }

    ::usleep(1000000);
  }
}

/* End firmware methods. */


#ifdef EPICS
/**
 * This macro allows the driver to be included by EPICS.
 */
NDS_DEFINE_DRIVER(exampleDrv, exampleDrv)
#else
/**
 * If this device is not going to be used in EPICS then function for allocating
 * it are given.
 *
 */
/*
 * Allocation function
 *********************/
void* exampleDrv::allocateDevice(nds::Factory& factory,
                   const std::string& DeviceName,
                   const nds::namedParameters_t& parameters) {

  return new exampleDrv(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void exampleDrv::deallocateDevice(void* DeviceName) {

  delete (exampleDrv*)DeviceName;
}
#endif
