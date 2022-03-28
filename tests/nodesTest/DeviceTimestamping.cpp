#include <mutex>
#ifdef _WIN32
static const double M_PI = 3.14159265358979323846;
#else
#include <unistd.h>
#endif
#include <functional>

#include <nds3/nds.h>

#include "DeviceTimestamping.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceTimestamping*> m_DevicesMap;
static std::mutex m_lockDevicesMap;


DeviceTimestamping::DeviceTimestamping(nds::Factory &factory,
                       const std::string &DeviceName,
                       const nds::namedParameters_t & parameters):
  m_Name(DeviceName),
  m_Ntimestamps(0), /* Number of timestamps in stack. */
  m_bStop_Timestamping(true){

  //Verify that there is no devices of this type with the same name
  {
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end()) {

      throw std::logic_error("Device with the same name already allocated. "
                 "This should not happen");
    }
    m_DevicesMap[DeviceName] = this;
  }

  /**
   * Here we declare the root node.
   * It is a good practice to name it with the Device name.
   *
   * Also, for simplicity we declare it as a "Port": this means that
   * the root node will be responsible for the communication with
   * the underlying control system.
   *
   * It is possible to have the root node as a simple Node and promote one or
   * more of its children to "Port": each port will interface with a different
   * control system thread.
   */
  nds::Port rootNode(DeviceName);

  //Add a PV to set the current time
  m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
  m_setCurrentTime.setDescription("Set timestamp (in seconds)");
  // For testing purposes the current time is set to a constant bigger
  // than 1 of January of 1990 which is the EPICS epoch.
  timespec timestamp = {0, 0};
  m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

  nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");

  // Add Timestamping node
  if (findParam != parameters.end() && findParam->second=="YES") {
      //Set compulsory methods
      nds::TimestampingArgs_t handlerTMS =  nds::TimestampingArgs_t(
                std::bind(&DeviceTimestamping::switchOn_timestamping, this),
                std::bind(&DeviceTimestamping::switchOff_timestamping, this),
                std::bind(&DeviceTimestamping::start_timestamping, this),
                std::bind(&DeviceTimestamping::stop_timestamping, this),
                std::bind(&DeviceTimestamping::recover_timestamping, this),
                std::bind(&DeviceTimestamping::allow_timestamping_change, this,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(&DeviceTimestamping::pv_enable_writer, this,
                std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceTimestamping::pv_edge_writer, this,
                std::placeholders::_1, std::placeholders::_2),
                std::bind(&DeviceTimestamping::pv_clearoverflow_writer, this,
                std::placeholders::_1, std::placeholders::_2));
      //Set optional methods
      handlerTMS.PV_Enable_Initializer = std::bind(&DeviceTimestamping::PV_Enable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerTMS.PV_Edge_Initializer = std::bind(&DeviceTimestamping::PV_Edge_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerTMS.PV_ClearOverflow_Initializer = std::bind(&DeviceTimestamping::PV_ClearOverflow_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      m_Timestamping = rootNode.addChild(nds::Timestamping<nds::timestamp_t>(
                                                                    "Timestamping",
                                                                    handlerTMS));
  } else {
      m_Timestamping = rootNode.addChild(nds::Timestamping<nds::timestamp_t>(
            "Timestamping", 4,
            std::bind(&DeviceTimestamping::switchOn_timestamping, this),
            std::bind(&DeviceTimestamping::switchOff_timestamping, this),
            std::bind(&DeviceTimestamping::start_timestamping, this),
            std::bind(&DeviceTimestamping::stop_timestamping, this),
            std::bind(&DeviceTimestamping::recover_timestamping, this),
            std::bind(&DeviceTimestamping::allow_timestamping_change, this,
              std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&DeviceTimestamping::pv_enable_writer, this,
              std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTimestamping::pv_edge_writer, this,
              std::placeholders::_1, std::placeholders::_2),
            std::bind(&DeviceTimestamping::pv_clearoverflow_writer, this,
              std::placeholders::_1, std::placeholders::_2)));
  }

  m_Timestamping.setStartTimestampDelegate(std::bind(&DeviceTimestamping::getCurrentTime, this));

  // We have declared all the nodes and PVs in our Device: now we register them
  // with the control system that called this constructor.
  //////////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setTimestampDelegate(std::bind(&DeviceTimestamping::getCurrentTime, this));

  //Stream information for debugging purposes
  rootNode.setLogLevel(nds::logLevel_t::debug);
  rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger: "
                         << "The DeviceTimestamping is created"
                         << std::endl;
  ndsDebugStream(rootNode) << "This is the ndsDebugStream: "
               << "The DeviceTimestamping named "
               << rootNode.getFullName() << " is created"
               << std::endl;

}

DeviceTimestamping::~DeviceTimestamping() {

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  m_DevicesMap.erase(m_Name);

}

DeviceTimestamping* DeviceTimestamping::getInstance(const std::string& DeviceName) {

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  std::map<std::string, DeviceTimestamping*>::const_iterator findDevice =
    m_DevicesMap.find(DeviceName);
  if(findDevice == m_DevicesMap.end()){

    return 0;
  }

  return findDevice->second;
}

////////////////////////////////////////////////////////////////////////////////
// TIMESTAMPING STATE MACHINE FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief called when the Timestamping node has to be switched on
 */
void  DeviceTimestamping::switchOn_timestamping() {

  m_Timestamping.setEnable(getCurrentTime(), 0 /* OFF */);
  m_Timestamping.setEdge(getCurrentTime(), 1 /* FALLING */);
  m_Timestamping.setMaxTimestamps(getCurrentTime(), 5);
  m_Timestamping.setOverflow(getCurrentTime(), 0 /* NO */);
}


/**
 * @brief called when the Timestamping node has to be switched off
 */
void DeviceTimestamping::switchOff_timestamping() {

  m_Timestamping.setEnable(getCurrentTime(), 0 /* OFF */);
}

/**
 * @brief Called when the Timestamping node has to start working.
 *        We start the Timestamping thread.
 */
void DeviceTimestamping::start_timestamping() {

  m_bStop_Timestamping = false; //< We will set to true to stop the Timestamping
                //  thread.

  /**
   *  Start the Timestamping thread. This function is called when the state
   *  machine goes from on to running. We don't need to check if the thread was
   *  already started because the state machine guarantees that the start handler
   *  is called only while the state is ON.
   */
  m_timestamping_thread =
    m_Timestamping.runInThread("TS",
                   std::bind(&DeviceTimestamping::timestamping_thread_body, this));

}

// Stop the Timestamping node thread
void DeviceTimestamping::stop_timestamping(){

  m_bStop_Timestamping = true;
  m_timestamping_thread.join();
}

// A failure during a state transition will cause the state machine to switch to
// the failure state. For now we don't plan for this and every time the
// state machine wants to recover we throw StateMachineRollBack to force the
// state machine to stay on the failure state.
void DeviceTimestamping::recover_timestamping(){

  throw nds::StateMachineRollBack("Cannot recover");
}

// Always allow for change state.
bool DeviceTimestamping::allow_timestamping_change(nds::state_t, nds::state_t, nds::state_t){

  return true;
}

void DeviceTimestamping::timestamping_thread_body() {

  // Get status.
  std::int32_t enable = m_Timestamping.getEnable();
  // Get edge status.
  std::int32_t edge = m_Timestamping.getEdge();
  // Get maximum number of timestamps.
  std::int32_t max_tstamps = m_Timestamping.getMaxTimestamps();
  // Get overflow.
  std::int32_t overflow = m_Timestamping.getOverflow();

  std::cout << "Timestamping support information:" << std::endl;
  std::cout << "\tEnable = " << enable << std::endl;
  std::cout << "\tEdge = " << edge << std::endl;
  std::cout << "\tMaximum number of timestamps = " << max_tstamps << std::endl;
  std::cout << "\tOverflow state = " << overflow << std::endl;

  std::int32_t once_flag = 0; // The pushes are done only once.
  // Run until the state machine stops us
  while(!m_bStop_Timestamping){

    enable = m_Timestamping.getEnable();

    if (enable == 1 /* Enabled */ && once_flag++ == 0 /* Enter only once. */) {

      // Six timestamps are  going to be pushed:
      // First timestamp
      nds::timestamp_t pushed_timestamp = {{0, 0}, ++m_Ntimestamps /* ID */, true /* RISING */};
      push_timestamp(max_tstamps, pushed_timestamp);
      ::usleep(500000);

      // Second timestamp
      pushed_timestamp.timestamp.tv_nsec = 10; /* nsec */
      pushed_timestamp.id = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);
      ::usleep(500000);

      // Third Timestamp
      pushed_timestamp.id  = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);
      ::usleep(500000);

      // Fourth Timestamp
      pushed_timestamp.edge = false; /* FALLING */
      pushed_timestamp.id = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);
      ::usleep(500000);

      // Fifth Timestamp
      pushed_timestamp.id = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);
      ::usleep(500000);

      // Sixth Timestamp
      pushed_timestamp.edge = true; /* RISING */
      pushed_timestamp.id = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);

      ::usleep(1000000);
    }
  }
}

/*
 * Enable writer.
 */
void DeviceTimestamping::pv_enable_writer(const timespec& timestamp,
                      const std::int32_t& value){

  // This function shall interact with hardware api to enable or disbale.

  m_Timestamping.setEnable(timestamp, value);

}

/*
 * Edge writer.
 */
void DeviceTimestamping::pv_edge_writer(const timespec& timestamp,
                    const std::int32_t& value){

  // This function shall interact with api hardware.
  m_Timestamping.setEdge(timestamp, value);

}

/*
 * Clearoverflow writer.
 */
void DeviceTimestamping::pv_clearoverflow_writer(const timespec& /*timestamp*/,
                    const std::int32_t& /*value*/){

  // This function may have to interact with api hardware.

  m_Ntimestamps = 0;
  m_Timestamping.setOverflow(getCurrentTime(), 0 /* NO */);
}

void DeviceTimestamping::push_timestamp(std::int32_t max_tstamps,
                    nds::timestamp_t pushed_tstamp){

  // Here we check manually the amount of timestamps in the queue.
  if(pushed_tstamp.id > max_tstamps) {

    m_Timestamping.setOverflow(getCurrentTime(), 1 /* OVERFLOWED */);
  } else if (pushed_tstamp.id == max_tstamps){

    m_Timestamping.setOverflow(getCurrentTime(), 2 /* FULL */);
  }

  /* The timestamp is always pushed, even if there is an overflow. */
  m_Timestamping.push(m_Timestamping.getTimestamp(), pushed_tstamp);

}

void DeviceTimestamping::PV_Enable_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = 1;
}

void DeviceTimestamping::PV_Edge_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 1; //ANY
}

void DeviceTimestamping::PV_ClearOverflow_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 30};
    *value = 1;
}

timespec DeviceTimestamping::getCurrentTime() {
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

//NDS_DEFINE_DRIVER(DeviceTimestamping, DeviceTimestamping)

/*
 * Allocation function
 *********************/
void* DeviceTimestamping::allocateDevice(nds::Factory& factory,
                   const std::string& DeviceName,
                   const nds::namedParameters_t& parameters){

  return new DeviceTimestamping(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceTimestamping::deallocateDevice(void* DeviceName){

  delete (DeviceTimestamping*)DeviceName;
}

