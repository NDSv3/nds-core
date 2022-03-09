#include <functional>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

#include <nds3/nds.h>
#include "terminal.h"


terminal::terminal(const std::string& name, nds::Node& parent):
  m_Ntimestamps(0) /* Initial number of timestamps is zero. */{


  /**
   * This class will be a new child node of the given parent node, in this case
   * the exampleDrv class that holds the Root Node.
   */
  nds::Node terminal = parent.addChild(nds::Node(name));


  /**
   * @brief Calling the Timestamping node constructor.
   *
   *   Adding Timestamping node to the control system. Note that in this case is
   * added as a child of the parent node. See the exampleDrv cpp file for more
   * information.
   *
   *   All the delegate methods and the state machine functions must be passed
   * to the constructor of the node.
   *
   */
  m_timestamping = terminal.addChild(nds::Timestamping<nds::timestamp_t>
       ("TS", /* Name of the node which will be used by the CS. */
        4, /* This may be deprecated in the future. */
        std::bind(&terminal::switchon_timestamping, this),
        std::bind(&terminal::switchoff_timestamping, this),
        std::bind(&terminal::start_timestamping, this),
        std::bind(&terminal::stop_timestamping, this),
        std::bind(&terminal::recover_timestamping, this),
        std::bind(&terminal::allow_timestamping_change, this,
              std::placeholders::_1,
              std::placeholders::_2,
              std::placeholders::_3),
        std::bind(&terminal::pv_enable_writer, this,
              std::placeholders::_1, std::placeholders::_2),
        std::bind(&terminal::pv_edge_writer, this,
              std::placeholders::_1, std::placeholders::_2),
        std::bind(&terminal::pv_clearoverflow_writer, this,
              std::placeholders::_1, std::placeholders::_2)));


  /**
   * @brief Calling the Timing node constructor.
   *
   *   Timing node is also added to the CS via the addChild function.
   */
  m_timing = terminal.addChild(nds::Timing(
     "TM", /* Name of the node used by the CS. */
     std::bind(&terminal::switchon_timing, this),
     std::bind(&terminal::switchoff_timing, this),
     std::bind(&terminal::start_timing, this),
     std::bind(&terminal::stop_timing, this),
     std::bind(&terminal::recover_timing, this),
     std::bind(&terminal::allow_timing_change, this,
           std::placeholders::_1,
           std::placeholders::_2,
           std::placeholders::_3),
     std::bind(&terminal::pv_time_reader, this,
           std::placeholders::_1,
           std::placeholders::_2)));

}



terminal::~terminal() {

}

/**
 * Timing methods.
 *
 *   This are the methods the driver developer should implement to build the
 * driver. This is the space left for the developer to interact with the HW API
 * and write most of the driver code.
 *
 */

/**
 *   Timing state machine. NDS makes sure these functions are called only once
 * during the state change.
 */
void terminal::switchon_timing() {

  // This function is called when turning the node from OFF to ON. Here all the
  // PVs, flags may be setted for other dependant nodes (and this node as well)
  // to see and prepare to use the timing functionalities.
  // For example :
  // Call API HW to set a clock frequency and then set in the CS the final value
  // writen in the HW (this case and the followings are set in the CS straight
  // away).
  m_timing.setClkFrequency(m_timing.getTimestamp(), 100.001);
  // Call API HW to set a clock multiplier.
  m_timing.setClkMultiplier(m_timing.getTimestamp(), 2);
  // Call API HW to set a synching status.
  m_timing.setSyncStatus(m_timing.getTimestamp(), 1 /* SYNCING */);
  // Call API HW to set a reference time base.
  m_timing.setRefTimeBase(m_timing.getTimestamp(), m_timing.getTimestamp());
}

void terminal::switchoff_timing() {

  // This function is called when turning the node from ON to OFF. It can be
  // useful to deallocate variables, set different PVs accordingly as well as
  // notifying dependent processes. For example:
  // Call API HW to set the synching status and then send the result to the CS.
  m_timing.setSyncStatus(m_timing.getTimestamp(), 0 /* NOT_SYNC */);
  // Call API HW to get seconds since last sync. Maybe a thread could update this
  // value accordingly.
  m_timing.setSecsLastSync(m_timing.getTimestamp(), 10);
}

void terminal::start_timing() {

  // Function called when turning the node from  ON to RUNNING. This function
  // initializes the timing functionalities. In this case a thread is launched
  // which gets the unix time and publishes it to the CS. For example:
  // Call API HW to set synching status.
  m_timing.setSyncStatus(m_timing.getTimestamp(),2 /* SYNCED */);

  m_stop_timing = false;
  /**
   *   Start the Timing thread.
   *   We don't need to check if the thread was already started because the state
   *   machine guarantees that the start handler is called only once while the
   *   state is ON.
   *
   */
  m_timing_thread = m_timing.runInThread("Timing", 
                        std::bind(&terminal::timing_thread_body, this));
}

void terminal::stop_timing() {

  // Function called when turning the node from RUNNING to ON. In this case the
  // thread is closed and linux time is not pushed anymore to the CS.
  m_stop_timing = true;
  m_timing_thread.join();
}

void terminal::recover_timing() {

  // Function called when there has been an error and the node needs to go back
  // from FAULT state to OFF. If the exception StateMachineTRollBack is thrown
  // the device will not be able to recover and will stay in FAULT state.
  // In this case the exception is thrown because is not planned to recover the
  // device from an error.
  throw nds::StateMachineRollBack("Cannot recover");
}

bool terminal::allow_timing_change(const nds::state_t, const nds::state_t,
                   const nds::state_t) {

  // Function called by NDS when changing from one state to the other. It allows
  // a change of state only if a given condition is fulfilled. In this case all
  // changes are always allowed.
  return true;

}

void terminal::pv_time_reader(timespec * /* time */, timespec* val) {

  // Delegate method for the time PV which reads the time from the HW. In this
  // case the Linux time is published:
  clock_gettime(CLOCK_REALTIME, val);
}

void terminal::timing_thread_body() {

  // This thread only runs while the state machine is in RUNNING state.
  // Get clock frequency.
  double clk_freq = m_timing.getClkFrequency();
  // Get Clock multiplier.
  std::int32_t clk_mult = m_timing.getClkMultiplier();
  // Get synching status.
  std::int32_t sync_status = m_timing.getSyncStatus();
  // Get seconds since last sync.
  std::int32_t secs = m_timing.getSecsLastSync();
  // Get reference time base.
  timespec time_base = m_timing.getRefTimeBase();

  std::cout << "Timing support information:" << std::endl;
  std::cout << "\tClock frequency = " << clk_freq << std::endl;
  std::cout << "\tClock multiplier = " << clk_mult << std::endl;
  std::cout << "\tSync. status = " << sync_status << std::endl;
  std::cout << "\tSeconds since last sync. = " << secs << std::endl;
  std::cout << "\tReference time base = {" << time_base.tv_sec
        << ", " << time_base.tv_nsec << "}" << std::endl;

  // Run until the state machine stops us.
  while(!m_stop_timing) {

    // For PV Delegates of type IN first a Get is done to read the PV value and
    // secondly a set is done to push it to the CS (is an IO interrupt PV).
    timespec time_val = m_timing.getTime();
    m_timing.setTime(m_timing.getTimestamp(), time_val);
    ::usleep(1000000);
  }
}

/** End Timing Methods. */


/**
 *   Timestamping methods.
 */

/**
 * For a detailed description of the state machine methods read the timing node
 * above.
 *
 */
void terminal::switchon_timestamping() {

  // In this case when the timestamping is switched to ON the following PVs are
  // set:
  // The timestamping is not enabled yet (needs to start running).
  m_timestamping.setEnable(m_timing.getTimestamp(), 0 /* OFF */);
  // The clock edge for the signal sampling is set to FALLING.
  m_timestamping.setEdge(m_timing.getTimestamp(), 1 /* FALLING */);
  // The HW device has a maximum timestamp stack of five.
  m_timestamping.setMaxTimestamps(m_timing.getTimestamp(), 5);
  // At start there any possibly overflow is cleared.
  m_timestamping.setOverflow(m_timing.getTimestamp(), 0 /* NO */);
}

void terminal::switchoff_timestamping() {

  m_timestamping.setEnable(m_timing.getTimestamp(), 0 /* OFF */);
}

void terminal::start_timestamping() {

  m_stop_timestamping = false;
  /**
   *  This thread will peridically push a timestamp to CS making an increment of
   *  the ID field and using the linux time retrieved by the timing node (via the
   *  Time PV) as a time value.
   */
  m_timestamping_thread = m_timestamping.runInThread("Timestamping", 
                           std::bind(&terminal::timestamping_thread_body, this));
}


void terminal::stop_timestamping() {

  m_stop_timestamping = true;
  m_timestamping_thread.join();
}

void terminal::recover_timestamping() {

  throw nds::StateMachineRollBack("Cannot recover");
}

bool terminal::allow_timestamping_change(const nds::state_t, const nds::state_t,
                     const nds::state_t) {

  return true;
}


/**
 *   Timestamping setters.
 *
 * These functions will write on the HW the passed value.
 */
void terminal::pv_enable_writer(const timespec& timestamp,
                const std::int32_t& value) {

  // This function may interact with the api hardware to enable the timestamping
  // functionalities.
  m_timestamping.setEnable(timestamp, value);
}

void terminal::pv_edge_writer(const timespec& timestamp,
                  const std::int32_t& value) {

  // This function may interact with the api hardware to set in which edge of the
  // clock frequency is the sampling done.
  m_timestamping.setEdge(timestamp, value);
}

void terminal::pv_clearoverflow_writer(const timespec& /* timestamp */,
                       const std::int32_t& /*value*/) {

  // This function may have to interact with the api hardware to clear the HW
  // stack of timestamps.
  m_Ntimestamps = 0;
  m_timestamping.setOverflow(m_timing.getTimestamp(), 0 /* NO */);
}



void terminal::timestamping_thread_body() {

  nds::timestamp_t timestamp;

  // This loop performs a periodic push of timestamps while the node is in
  // RUNNING state. No control of overflow is done.
  while(!m_stop_timestamping) {

    timestamp.timestamp = m_timing.getTime();
    timestamp.edge = false;
    timestamp.id = ++m_Ntimestamps;
    m_timestamping.push(m_timestamping.getTimestamp(), timestamp);
    ::usleep(1000000);
  }
}

/* End Timestmaping methods. */
