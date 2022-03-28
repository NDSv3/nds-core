#include <nds3/nds.h>
#include "channel.h"
#include "terminal.h"


/**
 * @brief This class represents a generic driver that includes a couple of DAQ
 * channels and a terminal with timing and timestamping functionalities.
 *
 *   This example aims to show how different nodes with different functionalities
 * can be included in a driver. Depending on the driver being implemented the
 * developer can organize the driver in the more convenient way. For example,
 * in this case the terminal and channel classes are quite independent and the
 * exampleDrv class adds them independently. For a different device it may be
 * of interest that each channel class adds its own child terminal class.
 */
class exampleDrv {

 public:

  /**
   * @brief Constructor of the class.
   *
   *   A vector of two channels and a terminal is added to the control system.
   * There is a firmware node which allows the user to know the firmware details
   * of the device and to change it if necessary.
   *
   */
  exampleDrv(nds::Factory& factory, const std::string& device,
	       const nds::namedParameters_t& parameters);
  /**
   * @brief Destructor of the class.
   */
  ~exampleDrv();

#ifndef EPICS
  /**
   * If this device is not going to be used in EPICS then functions for
   * allocating and deallocating the class are given.
   *
   */

  /*
   * Allocation function
   *********************/
  void* allocateDevice(nds::Factory& factory, const std::string& DeviceName,
		       const nds::namedParameters_t& parameters);

  /*
   * Deallocation function
   ***********************/
  void deallocateDevice(void* DeviceName);
#endif

  //////////////////////////////////////////////////////////////////////////////
  /// Firmware Node ////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Firmware node to be implemented.
   */
  nds::Firmware m_firmware;

  /**
   * Methods for the firmware state machine. A more detailed description of how
   * these methods describe a state machine can be found in the terminal.h file.
   *
   *   The firmware node allows the user to know which is the firmware installed
   * in the device but also the state machine allows to prepare the installation
   * of a new firmware in the device.
   *
   */
  void switchon_firmware();
  void switchoff_firmware();
  void start_firmware();
  void stop_firmware();
  void recover_firmware();
  bool allow_firmware_change(const nds::state_t, const nds::state_t,
			     const nds::state_t);

  void firmware_thread_body();
  nds::Thread m_firmware_thread;
  volatile bool m_stop_firmware;


  /**
   * @brief This method specifies how to obtain the path to the new firmware.
   */
  void pv_path_writer(const timespec& timestamp, const std::string& value);

  ////////////////////////
  /// End Firmware Node //
  ////////////////////////

 protected:

 private:

  /**
   * @brief Vector of shared pointers to class channel.
   *
   *   Each channel of the vector would correlate with a real channel of the
   *   device.
   *
   */
  std::vector<std::shared_ptr<channel>> m_channels;
  /**
   * @brief Vector of shared pointers to class channel.
   *
   *   For the moment only one terminal is added.
   *
   */
  std::vector<std::shared_ptr<terminal>> m_terminals;

};
