#include <nds3/nds.h>

/**
 * @brief Class which groups timing and timestamping functionalities.
 *
 *   This class shows how a generic time and timestamping node can be implemented
 * and then included by other class, in this case the exampleDrv, that wants to
 * use these functionalities.
 *
 */
class terminal {

 public:

  /**
   * @brief Constructor of the class.
   *
   *   It will add the Timing and Timestamping NDS nodes and as part of the
   * control system. These nodes will be automatically added as children of any
   * parent class of terminal.
   *
   */
  terminal(const std::string& name, nds::Node& parent);
  /**
   * @brief Destructor of the class
   *
   */
  ~terminal();

  //////////////////////////////////////////////////////////////////////////////
  /// Timing Node //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Timing node to be implemented.
   */
  nds::Timing m_timing;

  /**
   * The following are the functions that define the behaviour of the timing
   * state machine. These functions will be called only once when going from
   * one state to a different state. Take into account that only certain
   * transitions. For example, is not allowed the transition from state RUNNING
   * to state OFF.
   */

  /**
   * @brief Function called when going from OFF to ON state.
   */
  void switchon_timing();
  /**
   * @brief Function called when going from ON to OFF state.
   */
  void switchoff_timing();
  /**
   * @brief Function called when going from ON to RUNNING state.
   */
  void start_timing();
  /**
   * @brief Function called when going from RUNNING to ON state.
   */
  void stop_timing();
  /**
   * @brief Function called when going from FAULT to OFF state.
   */
  void recover_timing();
  /**
   * @brief This functions checks if the required conditions for changing from
   * one state to the other are met.
   */
  bool allow_timing_change(const nds::state_t,
			   const nds::state_t,
			   const nds::state_t);
  /**
   * @brief Function that specifies how time must be retrieved from hardware.
   *
   *   This function is called every time the user makes a caget in EPICS or the
   * setTime() method is called.
   *
   */
  void  pv_time_reader(timespec * timestamp, timespec * value);


  /**
   * @brief This thread will be active while the timing state machine is in
   * RUNNING state.
   *
   */
  void timing_thread_body();
  /**
   * @brief Standard thread for runnning the timing_thread_body() function.
   *
   */
  nds::Thread m_timing_thread;
  /**
   * @brief Variable to be set to false when the timing state machine is set to
   * ON.
   *
   */
  volatile bool m_stop_timing;

  //////////////////////////
  // End Timing ////////////
  //////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  /// Timestamping Node ////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Timestamping node to be implemented.
   */
  nds::Timestamping<nds::timestamp_t> m_timestamping;

  /**
   *   Methods to control timestamping state machine. See above description for
   * timing node for a more detailed description.
   *
   */
  void switchon_timestamping();
  void switchoff_timestamping();
  void start_timestamping();
  void stop_timestamping();
  void recover_timestamping();
  bool allow_timestamping_change(const nds::state_t, const nds::state_t,
				 const nds::state_t);

  void timestamping_thread_body();
  nds::Thread m_timestamping_thread;
  volatile bool m_stop_timestamping;

  /**
   *   Timestamping setters. This are the delegate function used to write on the
   * hardware.
   */

  /**
   * @brief This function will be called when the timestamping functionalities
   * need to be enabled.
   *
   */
  void pv_enable_writer(const timespec& timestamp, const std::int32_t& value);
  /**
   * @brief Function that specifies in which edge of the clock signal shall the
   * samples be acquired.
   *
   *   The possibles are either in the raising or falling edge of the clock or in
   * both edges.
   *
   */
  void pv_edge_writer(const timespec& timestamp, const std::int32_t& value);
  /**
   * @brief Functions that clears the stack of timestamps pushed by the hardware
   * in case there is an overflow.
   *
   */
  void pv_clearoverflow_writer(const timespec& timestamp, const std::int32_t& value);


  //////////////////////////
  // End Timestamping //////
  //////////////////////////

 protected:

 private:

  /**
   * @brief Attribute used by the terminal class to keep track of the number of
   * the timestamps pushed.
   *
   */
  std::int32_t m_Ntimestamps;

};
