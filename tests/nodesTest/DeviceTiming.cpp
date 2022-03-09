#include <mutex>
#ifdef _WIN32
static const double M_PI = 3.14159265358979323846;
#else
#include <unistd.h>
#endif
#include <functional>
#include <ctime>

#include <nds3/nds.h>

#include "DeviceTiming.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */


static std::map<std::string, DeviceTiming*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DeviceTiming::DeviceTiming(nds::Factory &factory,
         const std::string &DeviceName,
         const nds::namedParameters_t & parameters):
  m_Name(DeviceName),
  m_bStop_timing(true) {

  //Verify that there is no devices of this type with the same name
  {
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end())
      {
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
  m_setCurrentTime.setDescription("Set timestamp (in secodns)");
  // For testing purposes the current time is set to a constant bigger
  // than 1 of January of 1990 which is the EPICS epoch.
  timespec timestamp = {0, 0};
  m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);


  // Add Timing node
  nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");
  if (findParam != parameters.end() && findParam->second=="YES") {
      nds::TimingArgs_t handlerTM = nds::TimingArgs_t(
                 std::bind(&DeviceTiming::switchOn_timing, this),
                 std::bind(&DeviceTiming::switchOff_timing, this),
                 std::bind(&DeviceTiming::start_timing, this),
                 std::bind(&DeviceTiming::stop_timing, this),
                 std::bind(&DeviceTiming::recover_timing, this),
                 std::bind(&DeviceTiming::allow_timing_change, this,
                     std::placeholders::_1,
                     std::placeholders::_2,
                     std::placeholders::_3),
                 std::bind(&DeviceTiming::pv_timing_reader, this,
                     std::placeholders::_1,
                     std::placeholders::_2));
      m_Timing = rootNode.addChild(nds::Timing("Timing", handlerTM));
  } else {
      m_Timing = rootNode.addChild(nds::Timing("Timing",
                 std::bind(&DeviceTiming::switchOn_timing, this),
                 std::bind(&DeviceTiming::switchOff_timing, this),
                 std::bind(&DeviceTiming::start_timing, this),
                 std::bind(&DeviceTiming::stop_timing, this),
                 std::bind(&DeviceTiming::recover_timing, this),
                 std::bind(&DeviceTiming::allow_timing_change, this,
                     std::placeholders::_1,
                     std::placeholders::_2,
                     std::placeholders::_3),
                 std::bind(&DeviceTiming::pv_timing_reader, this,
                     std::placeholders::_1,
                     std::placeholders::_2)));
  }

  m_Timing.setStartTimestampDelegate(std::bind(&DeviceTiming::getCurrentTime,this));

  // We have declared all the nodes and PVs in our Device: now we register them
  //  with the control system that called this constructor.
  //////////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setTimestampDelegate(std::bind(&DeviceTiming::getCurrentTime,this));

  //Stream information for debugging purposes
  rootNode.setLogLevel(nds::logLevel_t::debug);
  rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger: "
               << "the DeviceTiming is created."
               << std::endl;
  ndsDebugStream(rootNode) << "This is the ndsDebugStream: "
         << "the DeviceTiming named "
         << rootNode.getFullName()
         << " is created." << std::endl;

}



DeviceTiming::~DeviceTiming() {

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  m_DevicesMap.erase(m_Name);
}



DeviceTiming* DeviceTiming::getInstance(const std::string& DeviceName) {

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);

  std::map<std::string, DeviceTiming*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
  if(findDevice == m_DevicesMap.end()) return 0;

  return findDevice->second;
}


////////////////////////////////////////////////////////////////////////////////
// TEST TIMING STATE MACHINE
////////////////////////////////////////////////////////////////////////////////

/*
 * Methods to control Timing state machine
 */

// Called when the Timing node has to be switched on.
void DeviceTiming::switchOn_timing(){

  // Call API HW to set clock grequency.
  m_Timing.setClkFrequency(getCurrentTime(),100.001);
  // Call API HW to set Clock multiplier.
  m_Timing.setClkMultiplier(getCurrentTime(),2);
  // Call API HW to set synching status.
  m_Timing.setSyncStatus(getCurrentTime(),1 /* SYNCING */);
  // Call API HW to set seconds since last sync.
  m_Timing.setSecsLastSync(getCurrentTime(), 0);
  // Call API HW to set reference time base.
  m_Timing.setRefTimeBase(getCurrentTime(), getCurrentTime()/*Used  just for convenience.*/);
}

// Called when the Timing node has to be switched off.
void DeviceTiming::switchOff_timing(){

  // Call API HW to set synching status.
  m_Timing.setSyncStatus(getCurrentTime(),0 /* NOT_SYNC */);
  // Call API HW to set seconds since last sync.
  m_Timing.setSecsLastSync(getCurrentTime(), 10);
}

// Called when the Timing node has to start working. We start the Timing thread.
void DeviceTiming::start_timing(){

  // Call API HW to set synching status.
  m_Timing.setSyncStatus(getCurrentTime(),2 /* SYNCED */);

  m_bStop_timing = false; //< We will set to true to stop the Timing thread
  /**
   *   Start the Timing thread.
   *   We don't need to check if the thread was already started because the state
   *   machine guarantees that the start handler is called only while the state
   *   is ON.
   */
  m_timing_thread =
    m_Timing.runInThread("TM",
             std::bind(&DeviceTiming::timing_thread_body, this));
}

// Stop the Timing node thread
void DeviceTiming::stop_timing(){

  m_bStop_timing = true;
  m_timing_thread.join();
}

// A failure during a state transition will cause the state machine to switch to
// the failure state. For now we don't plan for this and every time the
// state machine wants to recover we throw StateMachineRollBack to force the
// state machine to stay on the failure state.
void DeviceTiming::recover_timing(){

  throw nds::StateMachineRollBack("Cannot recover");
}

// We always allow the state machine to switch state. Before calling this
// function the state machine has already verified that the requested state
// transition is legal.
bool DeviceTiming::allow_timing_change(const nds::state_t,
               const nds::state_t,
               const nds::state_t){
  return true;
}

void DeviceTiming::timing_thread_body() {

  // Get clock grequency.
  double clk_freq = m_Timing.getClkFrequency();
  // Get Clock multiplier.
  std::int32_t clk_mult = m_Timing.getClkMultiplier();
  // Get synching status.
  std::int32_t sync_status = m_Timing.getSyncStatus();
  // Get seconds since last sync.
  std::int32_t secs = m_Timing.getSecsLastSync();
  // Get reference time base.
  timespec time_base = m_Timing.getRefTimeBase();

  std::cout << "Timing support information:" << std::endl;
  std::cout << "\tClock frequency = " << clk_freq << std::endl;
  std::cout << "\tClock multiplier = " << clk_mult << std::endl;
  std::cout << "\tSync. status = " << sync_status << std::endl;
  std::cout << "\tSeconds since last sync. = " << secs << std::endl;
  std::cout << "\tReference time base = {" << time_base.tv_sec
      << ", " << time_base.tv_nsec << "}" << std::endl;

  // Run until the state machine stops us
  while(!m_bStop_timing){

    // Get Self-Test enable
    timespec time_val = m_Timing.getTime();
    m_Timing.setTime(getCurrentTime(), time_val);
    ::usleep(1000000);
  }
}

/*
 * Time getter
 */
void DeviceTiming::pv_timing_reader(timespec * timestamp, timespec * value){

  *timestamp = getCurrentTime();
  *value = {(time_t)NDS_EPOCH, (long int)20091982};
}


timespec DeviceTiming::getCurrentTime() {
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

//NDS_DEFINE_DRIVER(DeviceTiming, DeviceTiming)
/*
 * Allocation function
 *********************/
void* DeviceTiming::allocateDevice(nds::Factory& factory,
           const std::string& DeviceName,
           const nds::namedParameters_t& parameters){

  return new DeviceTiming(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceTiming::deallocateDevice(void* DeviceName){

  delete (DeviceTiming*)DeviceName;
}
