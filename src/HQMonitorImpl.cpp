/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "../include/nds3/impl/HQMonitorImpl.h"

#include "nds3/definitions.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

HQMonitorImpl::HQMonitorImpl(  const std::string& name,
                     stateChange_t switchOnFunction,
                     stateChange_t switchOffFunction,
                     stateChange_t startFunction,
                     stateChange_t stopFunction,
                     stateChange_t recoverFunction,
                     allowChange_t allowStateChangeFunction,
                     readerDouble_t PV_DevicePower_Reader,
                     readerDouble_t PV_DeviceTemperature_Reader,
                     readerDouble_t PV_DeviceVoltage_Reader,
                     readerDouble_t PV_DeviceCurrent_Reader,
                     writerInt32_t PV_SEUEnable_Writer,
                     writerInt32_t PV_DAQEnable_Writer,
                     writerInt32_t PV_SelfTestEnable_Writer,
                     writerInt32_t PV_SelfTestType_Writer,
                     writerInt32_t PV_SelfTestVerboseEnable_Writer,
                     writerInt32_t PV_SelfTestIDEnable_Writer,
                     writerInt32_t PV_SelfTestTxtEnable_Writer,
                     writerInt32_t PV_SelfTestCodeResultEnable_Writer,
                     readerString_t PV_SelfTestTextResult_Reader,
                     readerInt32_t PV_SignalQualityFlag_Reader,
                     writerDouble_t PV_SignalQualityFlagLevel_Writer,
                     autoEnable_t autoEnable):
  NodeImpl(name, nodeType_t::dataSourceChannel),
  m_onStartDelegate(startFunction),
  m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    HQMonitorArgs_t handlerHQM = HQMonitorArgs_t(switchOnFunction,
                                                        switchOffFunction,
                                                        startFunction,
                                                        stopFunction,
                                                        recoverFunction,
                                                        allowStateChangeFunction,
                                                        PV_DevicePower_Reader,
                                                        PV_DeviceTemperature_Reader,
                                                        PV_DeviceVoltage_Reader,
                                                        PV_DeviceCurrent_Reader,
                                                        PV_SEUEnable_Writer,
                                                        PV_DAQEnable_Writer,
                                                        PV_SelfTestEnable_Writer,
                                                        PV_SelfTestType_Writer,
                                                        PV_SelfTestVerboseEnable_Writer,
                                                        PV_SelfTestIDEnable_Writer,
                                                        PV_SelfTestTxtEnable_Writer,
                                                        PV_SelfTestCodeResultEnable_Writer,
                                                        PV_SelfTestTextResult_Reader,
                                                        PV_SignalQualityFlag_Reader,
                                                        PV_SignalQualityFlagLevel_Writer,
                                                        autoEnable);
    constructorBody(handlerHQM);
}

HQMonitorImpl::HQMonitorImpl(const std::string& name, const HQMonitorArgs_t& handlerHQM) :
          NodeImpl(name, nodeType_t::dataSourceChannel),
          m_onStartDelegate(handlerHQM.handlerSTM.startFunction),
          m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    constructorBody(handlerHQM);
}
  
inline void HQMonitorImpl::constructorBody(const HQMonitorArgs_t& handlerHQM) {

      // Add the children PVs
      m_DevPower_PV.reset(new PVDelegateInImpl<double>("DevPower",handlerHQM.PV_DevicePower_Reader));
      m_DevPower_PV->setDescription("Device Power");
      m_DevPower_PV-> setScanType(scanType_t::interrupt,0);
      addChild(m_DevPower_PV);

      m_DevTemperature_PV.reset(new PVDelegateInImpl<double>("DevTemperature",handlerHQM.PV_DeviceTemperature_Reader));
      m_DevTemperature_PV->setDescription("Device Temperature");
      m_DevTemperature_PV-> setScanType(scanType_t::interrupt,0);
      addChild(m_DevTemperature_PV);

      m_DevVoltage_PV.reset(new PVDelegateInImpl<double>("DevVoltage",handlerHQM.PV_DeviceVoltage_Reader));
      m_DevVoltage_PV->setDescription("Device Volt");
      m_DevVoltage_PV-> setScanType(scanType_t::interrupt,0);
      addChild(m_DevVoltage_PV);

      m_DevCurrent_PV.reset(new PVDelegateInImpl<double>("DevCurrent",handlerHQM.PV_DeviceCurrent_Reader));
      m_DevCurrent_PV->setDescription("Device Current");
      m_DevCurrent_PV-> setScanType(scanType_t::interrupt,0);
      addChild(m_DevCurrent_PV);

      if (handlerHQM.PV_SEUEnable_Initializer) {
          m_SEUEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("SEUEnable",
                                                                  handlerHQM.PV_SEUEnable_Writer,
                                                                  handlerHQM.PV_SEUEnable_Initializer));
      } else {
          m_SEUEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("SEUEnable",
                                                                  handlerHQM.PV_SEUEnable_Writer));
      }
      m_SEUEnable_PV->setDescription("Enable Detection of Single Events Upsets");
      addChild(m_SEUEnable_PV);

      m_SEUEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SEUEnable_RBV"));
      m_SEUEnable_RBVPV->setDescription("Enable Detection of SEU ReadBack");
      m_SEUEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_SEUEnable_RBVPV);

      if (handlerHQM.PV_DAQEnable_Initializer) {
          m_HQMonitorDAQEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("DAQEnable",
                                                                  handlerHQM.PV_DAQEnable_Writer,
                                                                  handlerHQM.PV_DAQEnable_Initializer));
      } else {
          m_HQMonitorDAQEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("DAQEnable",
                                                                  handlerHQM.PV_DAQEnable_Writer));
      }
      m_HQMonitorDAQEnable_PV->setDescription("Enable Monitoring of DAQ anomalies");
      //m_HQMonitorDAQEnable_PV->write(getTimestamp(), (std::int32_t)0);
      addChild(m_HQMonitorDAQEnable_PV);

      m_HQMonitorDAQEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DAQEnable_RBV"));
      m_HQMonitorDAQEnable_RBVPV->setDescription("Enable Monitor of DAQ anomalies ReadBack");
      m_HQMonitorDAQEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_HQMonitorDAQEnable_RBVPV);

      if (handlerHQM.PV_SelfTestEnable_Initializer) {
          m_TestEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestEnable",
                                                                  handlerHQM.PV_SelfTestEnable_Writer,
                                                                  handlerHQM.PV_SelfTestEnable_Initializer));
      } else {
          m_TestEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestEnable",
                                                                  handlerHQM.PV_SelfTestEnable_Writer));
      }
      m_TestEnable_PV->setDescription("Enable (start) the Self-Test");
      addChild(m_TestEnable_PV);

      m_TestEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestEnable_RBV"));
      m_TestEnable_RBVPV->setDescription("Enable the Self-Test ReadBack");
      m_TestEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestEnable_RBVPV);

      enumerationStrings_t SelfTestEnumeratorStrings;
      SelfTestEnumeratorStrings.push_back("Quick-Test");
      SelfTestEnumeratorStrings.push_back("Full-Test");

      if (handlerHQM.PV_SelfTestType_Initializer) {
          m_TestType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestType",
                                                                  handlerHQM.PV_SelfTestType_Writer,
                                                                  handlerHQM.PV_SelfTestType_Initializer));
      } else {
          m_TestType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestType",
                                                                  handlerHQM.PV_SelfTestType_Writer));
      }
      m_TestType_PV->setDescription("Type of Self-Test");
      m_TestType_PV->setEnumeration(SelfTestEnumeratorStrings);
      addChild(m_TestType_PV);

      m_TestType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestType_RBV"));
      m_TestType_RBVPV->setDescription("Type of Self-Test ReadBack");
      m_TestType_RBVPV->setEnumeration(SelfTestEnumeratorStrings);
      m_TestType_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestType_RBVPV);

      if (handlerHQM.PV_SelfTestVerboseEnable_Initializer) {
          m_TestVerboseEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestVerboseEnable",
                                                                  handlerHQM.PV_SelfTestVerboseEnable_Writer,
                                                                  handlerHQM.PV_SelfTestVerboseEnable_Initializer));
      } else {
          m_TestVerboseEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestVerboseEnable",
                                                                  handlerHQM.PV_SelfTestVerboseEnable_Writer));
      }
      m_TestVerboseEnable_PV->setDescription("Enable the Self-Test Verbose");
      addChild(m_TestVerboseEnable_PV);

      m_TestVerboseEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestVerboseEnable_RBV"));
      m_TestVerboseEnable_RBVPV->setDescription("Enable Verbose Self-Test Verbose RBV");
      m_TestVerboseEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestVerboseEnable_RBVPV);

      if (handlerHQM.PV_SelfTestIDEnable_Initializer) {
          m_TestIDEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestIDEnable",
                                                                  handlerHQM.PV_SelfTestIDEnable_Writer,
                                                                  handlerHQM.PV_SelfTestIDEnable_Initializer));
      } else {
          m_TestIDEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestIDEnable",
                                                                  handlerHQM.PV_SelfTestIDEnable_Writer));
      }
      m_TestIDEnable_PV->setDescription("Enable the Self-Test ID");
      addChild(m_TestIDEnable_PV);

      m_TestIDEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestIDEnable_RBV"));
      m_TestIDEnable_RBVPV->setDescription("Enable the Self-Test ID ReadBack");
      m_TestIDEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestIDEnable_RBVPV);

      if (handlerHQM.PV_SelfTestTxtEnable_Initializer) {
          m_TestTxtEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestTxtEnable",
                                                                  handlerHQM.PV_SelfTestTxtEnable_Writer,
                                                                  handlerHQM.PV_SelfTestTxtEnable_Initializer));
      } else {
          m_TestTxtEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestTxtEnable",
                                                                  handlerHQM.PV_SelfTestTxtEnable_Writer));
      }
      m_TestTxtEnable_PV->setDescription("Enable the Self-Test text description");
      addChild(m_TestTxtEnable_PV);

      m_TestTxtEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestTxtEnable_RBV"));
      m_TestTxtEnable_RBVPV->setDescription("Enable the Self-Test text description RBV");
      m_TestTxtEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestTxtEnable_RBVPV);

      if (handlerHQM.PV_SelfTestCodeResultEnable_Initializer) {
          m_TestCodeResultEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestCodeResultEnable",
                                                                  handlerHQM.PV_SelfTestCodeResultEnable_Writer,
                                                                  handlerHQM.PV_SelfTestCodeResultEnable_Initializer));
      } else {
          m_TestCodeResultEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestCodeResultEnable",
                                                                  handlerHQM.PV_SelfTestCodeResultEnable_Writer));
      }
      m_TestCodeResultEnable_PV->setDescription("Enable Self-Test result number");
      addChild(m_TestCodeResultEnable_PV);

      m_TestCodeResultEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestCodeResultEnable_RBV"));
      m_TestCodeResultEnable_RBVPV->setDescription("Enable Self-Test result number RBV");
      m_TestCodeResultEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestCodeResultEnable_RBVPV);

      m_TestTxtResult_PV.reset(new PVDelegateInImpl<std::string>("TestTxtResult",handlerHQM.PV_SelfTestTextResult_Reader));
      m_TestTxtResult_PV->setDescription("Text detailing the Self-Test result");
      m_TestTxtResult_PV-> setScanType(scanType_t::interrupt,0);
      addChild(m_TestTxtResult_PV);

      m_SignalQFlag_PV.reset(new PVDelegateInImpl<std::int32_t>("SignalQFlag",handlerHQM.PV_SignalQualityFlag_Reader));
      m_SignalQFlag_PV->setDescription("Read the flag of low quality signal");
      m_SignalQFlag_PV-> setScanType(scanType_t::interrupt,0);
      addChild(m_SignalQFlag_PV);

      if (handlerHQM.PV_SignalQualityFlagLevel_Initializer) {
          m_SignalQFlagTrigLevel_PV.reset(new PVDelegateOutImpl<double>("SignalQFlagTrigLevel",
                                                                  handlerHQM.PV_SignalQualityFlagLevel_Writer,
                                                                  handlerHQM.PV_SignalQualityFlagLevel_Initializer));
      } else {
          m_SignalQFlagTrigLevel_PV.reset(new PVDelegateOutImpl<double>("SignalQFlagTrigLevel",
                                                                  handlerHQM.PV_SignalQualityFlagLevel_Writer));
      }
      m_SignalQFlagTrigLevel_PV->setDescription("Quality Flag trigger level");
      m_SignalQFlagTrigLevel_PV-> setScanType(scanType_t::passive,0);
      addChild(m_SignalQFlagTrigLevel_PV);

      m_SignalQFlagTrigLevel_RBVPV.reset(new PVVariableInImpl<double>("SignalQFlagTrigLevel_RBV"));
      m_SignalQFlagTrigLevel_RBVPV->setDescription("Quality Flag trigger level ReadBack");
      m_SignalQFlagTrigLevel_RBVPV-> setScanType(scanType_t::interrupt,0);
      addChild(m_SignalQFlagTrigLevel_RBVPV);


      m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
      m_Decimation_PV->setDescription("Decimation");
      m_Decimation_PV->setScanType(scanType_t::passive, 0);
      m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
      addChild(m_Decimation_PV);

      // Add state machine
      m_StateMachine.reset(new StateMachineImpl(false,
                              handlerHQM.handlerSTM.switchOnFunction,
                              handlerHQM.handlerSTM.switchOffFunction,
                              std::bind(&HQMonitorImpl::onStart, this),
                              handlerHQM.handlerSTM.stopFunction,
                              handlerHQM.handlerSTM.recoverFunction,
                              handlerHQM.handlerSTM.allowStateChangeFunction,
                              handlerHQM.handlerSTM.autoEnable));
        addChild(m_StateMachine);

}


  timespec HQMonitorImpl::getStartTimestamp() const
  {
    return m_startTime;
  }
  
  void HQMonitorImpl::push(const timespec& /*timestamp*/, const std::int32_t& /*data*/)
  {
    //TODO
  }
  
  void HQMonitorImpl::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
  {
    m_startTimestampFunction = timestampDelegate;
  }
  
  void HQMonitorImpl::onStart()
  {
    m_startTime = m_startTimestampFunction();
    m_onStartDelegate();
  }
  
  nds::state_t HQMonitorImpl::getState()
  {
    return m_StateMachine->getLocalState();
  }

/**
 * ---------------------------------------------------
 * Getter functions
 * ---------------------------------------------------
 */

double HQMonitorImpl::getDevicePower()
{
    double power;
    timespec timestamp;
    m_DevPower_PV->read(&timestamp, &power);
    return (double) power;
}

double HQMonitorImpl::getDeviceTemperature()
{
    double temperature;
    timespec timestamp;
    m_DevTemperature_PV->read(&timestamp, &temperature);
    return (double) temperature;
}

double HQMonitorImpl::getDeviceVoltage()
{
    double voltage;
    timespec timestamp;
    m_DevVoltage_PV->read(&timestamp, &voltage);
    return (double) voltage;
}


double HQMonitorImpl::getDeviceCurrent()
{
    double current;
    timespec timestamp;
    m_DevCurrent_PV->read(&timestamp, &current);
    return (double) current;
}


size_t HQMonitorImpl::getSEUEnable()
{
    std::int32_t SEUenable;
    timespec timestamp;
    m_SEUEnable_RBVPV->read(&timestamp, &SEUenable);
    return (std::int32_t) SEUenable;
}

size_t HQMonitorImpl::getDAQMonitorEnable()
{
    std::int32_t DAQMonEnable;
    timespec timestamp;
    m_HQMonitorDAQEnable_RBVPV->read(&timestamp, &DAQMonEnable);
    return (std::int32_t) DAQMonEnable;
}

size_t HQMonitorImpl::getSelfTestEnable()
{
    std::int32_t selfTestEnable;
    timespec timestamp;
    m_TestEnable_RBVPV->read(&timestamp, &selfTestEnable);
    return (std::int32_t) selfTestEnable;
}

size_t HQMonitorImpl::getSelfTestType()
{
    std::int32_t selfTestType;
    timespec timestamp;
    m_TestType_RBVPV->read(&timestamp, &selfTestType);
    return (std::int32_t) selfTestType;
}

size_t HQMonitorImpl::getSelfTestVerboseEnable()
{
    std::int32_t selfTestVerbose;
    timespec timestamp;
    m_TestVerboseEnable_RBVPV->read(&timestamp, &selfTestVerbose);
    return (std::int32_t) selfTestVerbose;
}

size_t HQMonitorImpl::getSelfTestIDEnable()
{
    std::int32_t selfTestID;
    timespec timestamp;
    m_TestIDEnable_RBVPV->read(&timestamp, &selfTestID);
    return (std::int32_t) selfTestID;
}

size_t HQMonitorImpl::getSelfTestTextEnable()
{
    std::int32_t selfTestTxt;
    timespec timestamp;
    m_TestTxtEnable_RBVPV->read(&timestamp, &selfTestTxt);
    return (std::int32_t) selfTestTxt;
}

size_t HQMonitorImpl::getSelfTestCodeResultEnable()
{
    std::int32_t selfTestResult;
    timespec timestamp;
    m_TestCodeResultEnable_RBVPV->read(&timestamp, &selfTestResult);
    return (std::int32_t) selfTestResult;
}

std::string HQMonitorImpl::getSelfTextTxtResult()
{
    std::string selfTestTextResult;
    timespec timestamp;
    m_TestTxtResult_PV->read(&timestamp, &selfTestTextResult);
    return (std::string) selfTestTextResult;
}

size_t HQMonitorImpl::getSignalQualityFlag()
{
    std::int32_t signalQualityFlag;
    timespec timestamp;
    m_SignalQFlag_PV->read(&timestamp, &signalQualityFlag);
    return (std::int32_t) signalQualityFlag;
}

double HQMonitorImpl::getSignalQualityFlagLevel()
{
    double signalQualityFlagLevel;
    timespec timestamp;
    m_SignalQFlagTrigLevel_RBVPV->read(&timestamp, &signalQualityFlagLevel);
    return (double) signalQualityFlagLevel;
}


/**
 * ---------------------------------------------------
 * Setter functions
 * ---------------------------------------------------
 */

void HQMonitorImpl::setDevicePower(const timespec& timestamp, const double& value){
    m_DevPower_PV->push(timestamp, value);
}

void HQMonitorImpl::setDeviceTemperature(const timespec& timestamp, const double& value)
{
    m_DevTemperature_PV->push(timestamp, value);
}

void HQMonitorImpl::setDeviceVoltage(const timespec& timestamp, const double& value)
{
    m_DevVoltage_PV->push(timestamp, value);
}

void HQMonitorImpl::setDeviceCurrent(const timespec& timestamp, const double& value)
{
    m_DevCurrent_PV->push(timestamp, value);
}

void HQMonitorImpl::setSEUEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_SEUEnable_RBVPV->setValue(timestamp, value);
    m_SEUEnable_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setDAQMonitorEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_HQMonitorDAQEnable_RBVPV->setValue(timestamp, value);
    m_HQMonitorDAQEnable_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setSelfTestEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_TestEnable_RBVPV->setValue(timestamp, value);
    m_TestEnable_RBVPV->push(timestamp, value);
}


void HQMonitorImpl::setSelfTestType(const timespec& timestamp, const std::int32_t& value)
{
    m_TestType_RBVPV->setValue(timestamp, value);
    m_TestType_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setSelfTestVerboseEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_TestVerboseEnable_RBVPV->setValue(timestamp, value);
    m_TestVerboseEnable_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setSelfTestIDEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_TestIDEnable_RBVPV->setValue(timestamp, value);
    m_TestIDEnable_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setSelfTestTextEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_TestTxtEnable_RBVPV->setValue(timestamp, value);
    m_TestTxtEnable_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setSelfTextTxtResult(const timespec& timestamp, const std::string& value){
    m_TestTxtResult_PV->push(timestamp, value);
}

void HQMonitorImpl::setSelfTestCodeResultEnable(const timespec& timestamp, const std::int32_t& value)
{
    m_TestCodeResultEnable_RBVPV->setValue(timestamp, value);
    m_TestCodeResultEnable_RBVPV->push(timestamp, value);
}

void HQMonitorImpl::setSignalQualityFlag(const timespec& timestamp, const std::int32_t& value)
{
    m_SignalQFlag_PV->push(timestamp, value);
}

void HQMonitorImpl::setSignalQualityFlagLevel(const timespec& timestamp, const double& value)
{
    m_SignalQFlagTrigLevel_RBVPV->setValue(timestamp, value);
    m_SignalQFlagTrigLevel_RBVPV->push(timestamp, value);
}


void HQMonitorImpl::setState(const nds::state_t& newState)
{
    m_StateMachine->setState(newState);
}

}
