#ifndef DEVICE_TIMING_H_
#define DEVICE_TIMING_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Timing node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceTiming
{
 public:
  /**
   * @brief Constructor.
   *
   * @param factory    the control system factory that requested the creation of the device
   * @param deviceName     the name given to the device
   * @param parameters optional parameters passed to the device
   */
  DeviceTiming(nds::Factory& factory,
	       const std::string& deviceName,
	       const nds::namedParameters_t&);
  ~DeviceTiming();

  /*
   * Allocation/deallocation
   *
   *******************************************************/
  static void* allocateDevice(nds::Factory& factory,
			      const std::string& deviceName,
			      const nds::namedParameters_t& parameters);
  static void deallocateDevice(void* deviceName);

  /*
   * For test purposes we make it possible to retrieve running instances of
   *  the device
   */
  static DeviceTiming* getInstance(const std::string& deviceName);


 private:

  /*
   * @brief name of the device
   */
  std::string m_Name;

  //////////////////////////////////////////////////////////////////////////////
  // TEST TIMING NODE
  //////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Timing node
   */
  nds::Timing m_Timing;

  /**
   * Methods to control the Timing state machine
   */
  void switchOn_timing();  ///< Called to switch on the Timing node.
  void switchOff_timing(); ///< Called to switch off the Timing node.
  void start_timing();     ///< Called to start the Timing node.
  void stop_timing();      ///< Called to stop the Timing node.
  void recover_timing();   ///< Called to recover the Timing node from a failure.

  bool allow_timing_change(const nds::state_t,
			   const nds::state_t,
			   const nds::state_t); ///< Called to verify if a state
						///  change is allowed

  /**
   * Timing setters
   */
  void pv_timing_reader(timespec * timestamp, timespec * value);

  /**
   * @brief Function that emulates the changes of status of the Timing node.
   *        It is launched by start_Timing() in a separate thread.
   */
  void timing_thread_body();

  /**
   * @brief A thread that runs Timing_thread_body().
   */
  nds::Thread m_timing_thread;

  /**
   * @brief A boolean flag that stop the Timing loop in Timing_thread_body()
   *        when true.
   */
  volatile bool m_bStop_timing;


  //////////////////////////////////////////////////////////////////////////////
  // TIMESTAMP HANDLING
  //////////////////////////////////////////////////////////////////////////////
  /**
   * @brief PV to set the timestamp of the device, in seconds
   */
  nds::PVVariableOut<std::int32_t> m_setCurrentTime;

  /**
   * @brief Function to get the timestamp of the device
   */
  timespec getCurrentTime();

};

#endif // DEVICE_TIMING_H_
