/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */

#include <ctime>

#include "nds3/definitions.h"
#include "nds3/impl/timingImpl.h"
#include "nds3/impl/stateMachineImpl.h"


namespace nds {

  // TimingImpl Constructor
  TimingImpl::TimingImpl( const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        readerTime_t PV_Time_Reader,
        autoEnable_t autoEnable):
        NodeImpl(name, nodeType_t::dataSourceChannel),
        m_OnStartDelegate(startFunction),
        m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this)) {
      TimingArgs_t handlerTM = TimingArgs_t(switchOnFunction,
                                            switchOffFunction,
                                            startFunction,
                                            stopFunction,
                                            recoverFunction,
                                            allowStateChangeFunction,
                                            PV_Time_Reader,
                                            autoEnable);
      constructorBody(handlerTM);
  }

  TimingImpl::TimingImpl(const std::string& name, const TimingArgs_t& handlerTM):
          NodeImpl(name, nodeType_t::dataSourceChannel),
          m_OnStartDelegate(handlerTM.handlerSTM.startFunction),
          m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
  {
      constructorBody(handlerTM);
  }

  inline void TimingImpl::constructorBody(const TimingArgs_t& handlerTM) {

        // Add the children PVs

        // PV: time
        m_Time_PV.reset(new PVDelegateInImpl<timespec>("Time", handlerTM.PV_Time_Reader));
        m_Time_PV->setDescription("Get Time");
        m_Time_PV->setMaxElements(2);
        m_Time_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_Time_PV);

        // PV: human readable time
        m_HumanTime_PV.reset(new PVDelegateInImpl<std::string>("HTime",
                 std::bind(&TimingImpl::PV_HTime_Reader, this,
                       std::placeholders::_1, std::placeholders::_2 )));
        m_HumanTime_PV->setDescription("Get Time in UTC format");
        m_HumanTime_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_HumanTime_PV);

        // PV: Clock Frequency
        m_ClkFrequency_PV.reset(new PVVariableInImpl<double>("ClkFrequency"));
        m_ClkFrequency_PV->setDescription("Clock frequency");
        m_ClkFrequency_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_ClkFrequency_PV);

        // PV: Clock Multiplier
        m_ClkMultiplier_PV.reset(new PVVariableInImpl<std::int32_t>("ClkMultiplier"));
        m_ClkMultiplier_PV->setDescription("Clock multiplier");
        m_ClkMultiplier_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_ClkMultiplier_PV);

        // PV: Sync Status
        enumerationStrings_t SyncStatusEnumerationStrings;
        SyncStatusEnumerationStrings.push_back("NOT_SYNCED");
        SyncStatusEnumerationStrings.push_back("SYNCING");
        SyncStatusEnumerationStrings.push_back("SYNCED");
        SyncStatusEnumerationStrings.push_back("LOST_SYNC");

        m_SyncStatus_PV.reset(new PVVariableInImpl<std::int32_t>("SyncStatus"));
        m_SyncStatus_PV->setDescription("Synchronization status");
        m_SyncStatus_PV->setScanType(scanType_t::interrupt, 0);
        m_SyncStatus_PV->setEnumeration(SyncStatusEnumerationStrings);
        addChild(m_SyncStatus_PV);

        // PV: SecsLastSync
        m_SecsLastSync_PV.reset(new PVVariableInImpl<std::int32_t>("SecsLastSync"));
        m_SecsLastSync_PV->setDescription("Seconds since last synchronisation");
        m_SecsLastSync_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_SecsLastSync_PV);

        // PV: Reference Time Base
        m_RefTimeBase_PV.reset(new PVVariableInImpl<timespec>("RefTimeBase"));
        m_RefTimeBase_PV->setDescription("Reference time Base");
        m_Time_PV->setMaxElements(2);
        m_RefTimeBase_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_RefTimeBase_PV);

        // PV: Decimation
        m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
        m_Decimation_PV->setDescription("Decimation");
        m_Decimation_PV->setScanType(scanType_t::passive, 0);
        m_Decimation_PV->write(getTimestamp(),(std::int32_t)1);
        addChild(m_Decimation_PV);


        // Add state machine
        m_StateMachine.reset(new StateMachineImpl(false,
                                        handlerTM.handlerSTM.switchOnFunction,
                                        handlerTM.handlerSTM.switchOffFunction,
                                        std::bind(&TimingImpl::onStart, this),
                                        handlerTM.handlerSTM.stopFunction,
                                        handlerTM.handlerSTM.recoverFunction,
                                        handlerTM.handlerSTM.allowStateChangeFunction,
                                        handlerTM.handlerSTM.autoEnable));
         addChild(m_StateMachine);
  }

  // ------------------------ Common Functions ----------------------------- //
  timespec TimingImpl::getStartTimestamp() const {
    return m_StartTime;
  }

  void TimingImpl::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate) {
    m_StartTimestampFunction = timestampDelegate;
  }

  void TimingImpl::push(const timespec& timestamp, const timespec& data) {
    m_Time_PV -> push(timestamp, data);
  }


  void TimingImpl::onStart() {
    m_StartTime = m_StartTimestampFunction();
    m_Time_PV->setDecimation((std::uint32_t)(m_Decimation_PV->getValue()));
    m_OnStartDelegate();
  }

  nds::state_t TimingImpl::getState()
  {
    return m_StateMachine->getLocalState();
  }

  // ------------------------ Delegate Functions ---------------------------- //
  void TimingImpl::PV_HTime_Reader(timespec* /*timestamp*/, std::string *value) {
    tm *gmttm = gmtime(&m_CurrentTime.tv_sec);
    *value = asctime(gmttm);
  }


  // ---------------------------- Getters ----------------------------------- //
  timespec TimingImpl::getTime() {
    timespec timestamp;
    m_Time_PV -> read( &timestamp, &m_CurrentTime);
    return m_CurrentTime;
  }

  std::string TimingImpl::getHumanTime() {
    std::string UTCTime;
    timespec timestamp;
    m_HumanTime_PV -> read( &timestamp, &UTCTime);
    return UTCTime;
  }
  double TimingImpl::getClkFrequency() {
    double ClkFrequency;
    timespec timestamp;
    m_ClkFrequency_PV -> read(&timestamp, &ClkFrequency);
    return ClkFrequency;
  }

  std::int32_t TimingImpl::getClkMultiplier() {
    std::int32_t ClkMultiplier;
    timespec timestamp;
    m_ClkMultiplier_PV-> read(&timestamp, &ClkMultiplier);
    return ClkMultiplier;
  }

  std::int32_t TimingImpl::getSyncStatus() {
    std::int32_t SyncStatus;
    timespec timestamp;
    m_SyncStatus_PV -> read(&timestamp, &SyncStatus);
    return SyncStatus;
  }

  std::int32_t TimingImpl::getSecsLastSync() {
    std::int32_t SecsLastSync;
    timespec timestamp;
    m_SecsLastSync_PV -> read(&timestamp, &SecsLastSync);
    return SecsLastSync;
  }

  timespec TimingImpl::getRefTimeBase() {
    timespec getRefTimeBase;
    timespec timestamp;
    m_RefTimeBase_PV -> read(&timestamp, &getRefTimeBase);
    return getRefTimeBase;
  }

  // --------------------------- Setters ----------------------------------- //

  void TimingImpl::setTime(const timespec& timestamp, const timespec& value) {
     m_CurrentTime = value;
     m_Time_PV -> push(timestamp, value);
     std::string htime  = getHumanTime();
     m_HumanTime_PV -> push(timestamp, htime);
  }

  void TimingImpl::setClkFrequency(const timespec& timestamp, const double& value) {
     m_ClkFrequency_PV -> setValue(timestamp, value);
     m_ClkFrequency_PV -> push(timestamp, value);
  }

  void TimingImpl::setClkMultiplier(const timespec& timestamp, const std::int32_t& value) {
     m_ClkMultiplier_PV -> setValue(timestamp, value);
     m_ClkMultiplier_PV -> push(timestamp, value);
  }

  void TimingImpl::setSyncStatus(const timespec& timestamp, const std::int32_t& value) {
     m_SyncStatus_PV -> setValue(timestamp, value);
     m_SyncStatus_PV -> push(timestamp, value);
  }

  void TimingImpl::setSecsLastSync(const timespec& timestamp, const std::int32_t& value) {
     m_SecsLastSync_PV -> setValue(timestamp, value);
     m_SecsLastSync_PV -> push(timestamp, value);
  }

  void TimingImpl::setRefTimeBase(const timespec& timestamp, const timespec& value) {
     m_RefTimeBase_PV -> setValue(timestamp, value);
     m_RefTimeBase_PV -> push(timestamp, value);
  }

  void TimingImpl::setState(const nds::state_t& newState)
  {
    m_StateMachine->setState(newState);
  }
}
