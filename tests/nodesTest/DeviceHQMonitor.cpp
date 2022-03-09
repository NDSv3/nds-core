#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <functional>

#include <nds3/nds.h>

#include "DeviceHQMonitor.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */


static std::map<std::string, DeviceHQMonitor*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DeviceHQMonitor::DeviceHQMonitor(nds::Factory &factory,
                 const std::string &DeviceName,
                 const nds::namedParameters_t & parameters):
  m_Name(DeviceName),
  m_bStop_HQMonitor(true){

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
   * control system thread
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

  // Add Health Monitor node
  if (findParam != parameters.end() && findParam->second=="YES") {
      //Set compulsory methods
      nds::HQMonitorArgs_t handlerHQM = nds::HQMonitorArgs_t(
                      std::bind(&DeviceHQMonitor::switchOn_HQMonitor, this),
                      std::bind(&DeviceHQMonitor::switchOff_HQMonitor, this),
                      std::bind(&DeviceHQMonitor::start_HQMonitor, this),
                      std::bind(&DeviceHQMonitor::stop_HQMonitor, this),
                      std::bind(&DeviceHQMonitor::recover_HQMonitor, this),
                      std::bind(&DeviceHQMonitor::allow_HQMonitor_Change, this,
                        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_Power_Reader, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_Temperature_Reader, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_Voltage_Reader, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_Current_Reader, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SEUEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_DAQEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestType_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestVerboseEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestIDEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestTxtEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestCodeResultEnable_Writer, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestTxt_Reader, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SignalQualityFlag_Reader, this,
                        std::placeholders::_1, std::placeholders::_2),
                      std::bind(&DeviceHQMonitor::PV_HQMonitor_SignalQualityFlagLevel_Writer, this,
                        std::placeholders::_1, std::placeholders::_2));
      //Set optional methods
      handlerHQM.PV_SEUEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SEUEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_DAQEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_DAQEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SelfTestEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SelfTestType_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestType_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SelfTestVerboseEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestVerboseEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SelfTestIDEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestIDEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SelfTestTxtEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestTxtEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SelfTestCodeResultEnable_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestCodeResultEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      handlerHQM.PV_SignalQualityFlagLevel_Initializer = std::bind(&DeviceHQMonitor::PV_HQMonitor_SignalQualityFlagLevel_Initializer, this, std::placeholders::_1, std::placeholders::_2);
      //Add HQMonitor node to the rootNode
      m_HQMonitor = rootNode.addChild(nds::HQMonitor("HQMonitor", handlerHQM));
  } else {
      m_HQMonitor = rootNode.addChild(nds::HQMonitor("HQMonitor",
              std::bind(&DeviceHQMonitor::switchOn_HQMonitor, this),
              std::bind(&DeviceHQMonitor::switchOff_HQMonitor, this),
              std::bind(&DeviceHQMonitor::start_HQMonitor, this),
              std::bind(&DeviceHQMonitor::stop_HQMonitor, this),
              std::bind(&DeviceHQMonitor::recover_HQMonitor, this),
              std::bind(&DeviceHQMonitor::allow_HQMonitor_Change, this,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_Power_Reader, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_Temperature_Reader, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_Voltage_Reader, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_Current_Reader, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SEUEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_DAQEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestType_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestVerboseEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestIDEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestTxtEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestCodeResultEnable_Writer, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SelfTestTxt_Reader, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SignalQualityFlag_Reader, this,
                std::placeholders::_1, std::placeholders::_2),
              std::bind(&DeviceHQMonitor::PV_HQMonitor_SignalQualityFlagLevel_Writer, this,
                std::placeholders::_1, std::placeholders::_2)));
  }
  m_HQMonitor.setTimestampDelegate(std::bind(&DeviceHQMonitor::getCurrentTime,this));
  m_HQMonitor.getStartTimestamp();

  // We have declared all the nodes and PVs in our Device: now we register them
  //  with the control system that called this constructor.
  //////////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setTimestampDelegate(std::bind(&DeviceHQMonitor::getCurrentTime,this));

  /* For PVDelegateIn is necessary to instantiate them in the constructor. */
  m_HQMonitor.getSelfTestTextResult();

  //Stream information for debugging purposes
  rootNode.setLogLevel(nds::logLevel_t::debug);
  rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger: "
                         << "The DeviceFirmware is created."
                         << std::endl;
  ndsDebugStream(rootNode) << "This is the ndsDebugStream: "
               << "The DeviceFirmware named "
               << rootNode.getFullName() << " is created" << std::endl;


}



DeviceHQMonitor::~DeviceHQMonitor()
{
  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  m_DevicesMap.erase(m_Name);

}

DeviceHQMonitor* DeviceHQMonitor::getInstance(const std::string& DeviceName)
{

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);

  std::map<std::string, DeviceHQMonitor*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
  if(findDevice == m_DevicesMap.end())
    {
      return 0;
    }

  return findDevice->second;
}


////////////////////////////////////////////////////////////////////////////////
// TEST HEALTH MONITOR STATE MACHINE
////////////////////////////////////////////////////////////////////////////////

/*
 * Methods to control Health Monitor state machine
 */

// Called when the HQMonitor node has to be switched on.
void DeviceHQMonitor::switchOn_HQMonitor(){

  //Call API HW to set the default parameters
  m_HQMonitor.setDevicePower(getCurrentTime(), 0.42);
  m_HQMonitor.setDeviceTemperature(getCurrentTime(), 296.5);
  m_HQMonitor.setDeviceVoltage(getCurrentTime(), 2.6);
  m_HQMonitor.setDeviceCurrent(getCurrentTime(), 0.2);
  m_HQMonitor.setSEUEnable(getCurrentTime(),0);
  m_HQMonitor.setDAQMonitorEnable(getCurrentTime(),0);
  m_HQMonitor.setSelfTestEnable(getCurrentTime(),0);
  m_HQMonitor.setSelfTestType(getCurrentTime(),0);
  m_HQMonitor.setSelfTestVerboseEnable(getCurrentTime(), 0);
  m_HQMonitor.setSelfTestIDEnable(getCurrentTime(), 0);
  m_HQMonitor.setSelfTestTextEnable(getCurrentTime(), 0);
  m_HQMonitor.setSelfTestCodeResultEnable(getCurrentTime(), 0);
  m_HQMonitor.setSignalQualityFlag(getCurrentTime(), 0);
  m_HQMonitor.setSignalQualityFlagLevel(getCurrentTime(), 0.5);
}

// Called when the HQMonitor node has to be switched off.
void DeviceHQMonitor::switchOff_HQMonitor(){

}

// Called when the HQMonitor node has to start working. We start the HQMonitor
// thread.
void DeviceHQMonitor::start_HQMonitor(){

  m_bStop_HQMonitor = false; //< We will set to true to stop the HQMonitor thread
  /**
   *   Start the HQMonitor thread.
   *   We don't need to check if the thread was already started because the state
   *   machine guarantees that the start handler is called only while the state
   *   is ON.
   */
  m_HQMonitor_Thread =
    m_HQMonitor.runInThread("HQmon",
                   std::bind(&DeviceHQMonitor::HQMonitor_thread_body, this));
}

// Stop the HQMonitor node thread
void DeviceHQMonitor::stop_HQMonitor(){
  m_bStop_HQMonitor = true;
  m_HQMonitor_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to
//  the failure state. For now we don't plan for this and every time the state
//  machine wants to recover we throw StateMachineRollBack to force the state
//  machine to stay on the failure state.
void DeviceHQMonitor::recover_HQMonitor(){
  throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this
// function the state machine has already verified that the requested state
// transition is legal.
bool DeviceHQMonitor::allow_HQMonitor_Change(const nds::state_t,
                         const nds::state_t,
                         const nds::state_t){
  return true;
}

/**
 * HQMonitor support getters
 */
void DeviceHQMonitor::PV_HQMonitor_Power_Reader(timespec* timestamp,
                        double* value) {

  *timestamp = getCurrentTime();

  *value = 0.25 ;
  m_HQMonitor.setDevicePower(*timestamp, *value );

}

void DeviceHQMonitor::PV_HQMonitor_Temperature_Reader(timespec* timestamp,
                              double* value){

  *timestamp = getCurrentTime();
  *value = 295.5;
  m_HQMonitor.setDeviceTemperature(*timestamp, *value );
}

void DeviceHQMonitor::PV_HQMonitor_Voltage_Reader(timespec* timestamp,
                          double* value){

  *timestamp = getCurrentTime();
  *value = 2.5;
  m_HQMonitor.setDeviceVoltage(*timestamp, *value );
}

void DeviceHQMonitor::PV_HQMonitor_Current_Reader(timespec* timestamp,
                          double* value){

  *timestamp = getCurrentTime();
  *value = 0.1;
  m_HQMonitor.setDeviceCurrent(*timestamp, *value );
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestTxt_Reader(timespec* timestamp,
                              std::string* value){

  std::string result = "";
  if (m_HQMonitor.getSelfTestEnable() == 1) {
    if (m_HQMonitor.getSelfTestVerboseEnable()==1) {
      result += "Verbose information\n";
    }
    if (m_HQMonitor.getSelfTestIDEnable()==1) {
      result += "Test ID: 123\n";
    }
    if (m_HQMonitor.getSelfTestTextEnable()==1) {
      result += "Test TxtID: Test-01\n";
    }
    if (m_HQMonitor.getSelfTestCodeResultEnable()==1) {
      result += "CodeResult: 1\n";
    }
  }
  *timestamp = getCurrentTime();
  *value = result;
  m_HQMonitor.setSelfTestTextResult(*timestamp, *value );
}

void DeviceHQMonitor::PV_HQMonitor_SignalQualityFlag_Reader(timespec* timestamp,
                                std::int32_t* value){

  *timestamp = getCurrentTime();
  *value = 0;
}

/*
 * HQMonitor support setters
 */
void DeviceHQMonitor::PV_HQMonitor_SEUEnable_Writer(const timespec& timestamp,
                            const std::int32_t& value){

  int32_t SEUEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSEUEnable(timestamp, SEUEnable);
}


void DeviceHQMonitor::PV_HQMonitor_DAQEnable_Writer(const timespec& timestamp,
                            const std::int32_t& value){

  int32_t DAQEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setDAQMonitorEnable(timestamp, DAQEnable);
}


void DeviceHQMonitor::PV_HQMonitor_SelfTestEnable_Writer(const timespec& timestamp,
                             const std::int32_t& value){

  int32_t selfTestEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSelfTestEnable(timestamp, selfTestEnable);
}


void DeviceHQMonitor::PV_HQMonitor_SelfTestType_Writer(const timespec& timestamp,
                               const std::int32_t& value){

  int32_t selfTestType = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSelfTestType(timestamp, selfTestType);
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestVerboseEnable_Writer(const timespec& timestamp,
                                const std::int32_t& value){

  int32_t selfTestVerboseEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSelfTestVerboseEnable(timestamp, selfTestVerboseEnable);
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestIDEnable_Writer(const timespec& timestamp,
                               const std::int32_t& value){

  int32_t SEUEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSelfTestIDEnable(timestamp, SEUEnable);
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestTxtEnable_Writer(const timespec& timestamp,
                                const std::int32_t& value){

  int32_t selfTestTxtEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSelfTestTextEnable(timestamp, selfTestTxtEnable);
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestCodeResultEnable_Writer(const timespec& timestamp,
                                   const std::int32_t& value){

  int32_t selfTestTxtEnable = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSelfTestCodeResultEnable(timestamp, selfTestTxtEnable);
}

void DeviceHQMonitor::PV_HQMonitor_SignalQualityFlagLevel_Writer(const timespec& timestamp,
                                 const double& value){

  double signalQualityFlagLevel = value;
  //TODO: Call to function programming the hardware
  m_HQMonitor.setSignalQualityFlagLevel(timestamp, signalQualityFlagLevel);
}


void DeviceHQMonitor::HQMonitor_thread_body(){

  //Get Device power
  double devicePower = m_HQMonitor.getDevicePower();
  //Get Device temperature
  double deviceTemperature = m_HQMonitor.getDeviceTemperature();
  //Get Device voltage
  double deviceVoltage = m_HQMonitor.getDeviceVoltage();
  //Get Device current
  double deviceCurrent = m_HQMonitor.getDeviceCurrent();
  //Get Self-Test enable
  std::int32_t selfTestEnableOld = m_HQMonitor.getSelfTestEnable();

  //Initial Default Values
  std::int32_t SelfTestVerboseEnableOld = m_HQMonitor.getSelfTestVerboseEnable();
  std::int32_t SelfTestTextEnableOld = m_HQMonitor.getSelfTestTextEnable();
  std::int32_t SelfTestCodeResultEnableOld = m_HQMonitor.getSelfTestCodeResultEnable();

  std::cout<<"HealthMonitor support information:"<<std::endl;
  std::cout<<"\tDevicePower = "<<devicePower<<std::endl;
  std::cout<<"\tDeviceTemperature = "<<deviceTemperature<<std::endl;
  std::cout<<"\tDeviceVoltage = "<<deviceVoltage<<std::endl;
  std::cout<<"\tDeviceCurrent = "<<deviceCurrent<<std::endl;
  std::cout<<"\tSelfTestEnable = "<<selfTestEnableOld<<std::endl;

  // Run until the state machine stops us
  while(!m_bStop_HQMonitor){

    devicePower = m_HQMonitor.getDevicePower();
    deviceVoltage = m_HQMonitor.getDeviceVoltage();
    deviceCurrent =  m_HQMonitor.getDeviceCurrent();
    deviceTemperature  =  m_HQMonitor.getDeviceTemperature();

    //Verbosity flags
    std::int32_t SelfTestVerboseEnable = m_HQMonitor.getSelfTestVerboseEnable();
    std::int32_t SelfTestTextEnable = m_HQMonitor.getSelfTestTextEnable();
    std::int32_t SelfTestCodeResultEnable = m_HQMonitor.getSelfTestCodeResultEnable();

    // Get Self-Test enable
    std::int32_t selfTestEnable =  m_HQMonitor.getSelfTestEnable();

    if(selfTestEnable != selfTestEnableOld){
      m_HQMonitor.push(getCurrentTime(), selfTestEnable);
      selfTestEnableOld = selfTestEnable;
    }

    if(selfTestEnable != 0){
      // Call to the function that print the Result string and update (push) the
      // string
      if((SelfTestVerboseEnable != SelfTestVerboseEnableOld) ||
     (SelfTestTextEnable != SelfTestTextEnableOld) ||
     (SelfTestCodeResultEnable !=  SelfTestCodeResultEnableOld)){
    std::string result = m_HQMonitor.getSelfTestTextResult();

      }
    }

    //update old values
    if(SelfTestVerboseEnable!= SelfTestVerboseEnableOld)
      SelfTestVerboseEnableOld = SelfTestVerboseEnable;

    if(SelfTestTextEnable!= SelfTestTextEnableOld)
      SelfTestTextEnableOld = SelfTestTextEnable;

    if (SelfTestCodeResultEnable !=  SelfTestCodeResultEnableOld)
      SelfTestCodeResultEnableOld = SelfTestCodeResultEnable;

    // Rest for a while
    ::usleep(1000000);
  }
}

/*
 * --------------------------------------------------------------------------------
 * Only for testing purpose and verifying different values in each
 * initializer method, the value returned by the below methods is increased.
 * --------------------------------------------------------------------------------
 */
void DeviceHQMonitor::PV_HQMonitor_SEUEnable_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = 101; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_DAQEnable_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 102; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestEnable_Initializer(
        timespec* timestamp, int32_t* value) {
    *timestamp = {NDS_EPOCH, 30};
    *value = 103; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestType_Initializer(timespec* timestamp,
        int32_t* value) {
    *timestamp = {NDS_EPOCH, 40};
    *value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestVerboseEnable_Initializer(
        timespec* timestamp, int32_t* value) {
    *timestamp = {NDS_EPOCH, 50};
    *value = 105; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestIDEnable_Initializer(
        timespec* timestamp, int32_t* value) {
    *timestamp = {NDS_EPOCH, 60};
    *value = 106; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestTxtEnable_Initializer(
        timespec* timestamp, int32_t* value) {
    *timestamp = {NDS_EPOCH, 70};
    *value = 107; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SelfTestCodeResultEnable_Initializer(
        timespec* timestamp, int32_t* value) {
    *timestamp = {NDS_EPOCH, 80};
    *value = 108; //Note that this value has no sense and it is fixed only for testing purposes.
}

void DeviceHQMonitor::PV_HQMonitor_SignalQualityFlagLevel_Initializer(
        timespec* timestamp, double* value) {
    *timestamp = {NDS_EPOCH, 90};
    *value = 1.65; //Note that this value has no sense and it is fixed only for testing purposes.
}

timespec DeviceHQMonitor::getCurrentTime(){
  timespec time;
  time.tv_sec = m_setCurrentTime.getValue();
  time.tv_nsec = time.tv_sec + 10;
  return time;
}

//NDS_DEFINE_DRIVER(DeviceHQMonitor, DeviceHQMonitor)
/*
 * Allocation function
 *********************/
void* DeviceHQMonitor::allocateDevice(nds::Factory& factory,
                      const std::string& DeviceName,
                      const nds::namedParameters_t& parameters){

  return new DeviceHQMonitor(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceHQMonitor::deallocateDevice(void* DeviceName){
  delete (DeviceHQMonitor*)DeviceName;
}
