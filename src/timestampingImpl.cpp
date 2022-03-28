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
#include "nds3/impl/timestampingImpl.h"
#include "nds3/impl/stateMachineImpl.h"

#include<iostream>


namespace nds {

  template<typename T>
  TimestampingImpl<T>::TimestampingImpl(const std::string& name,
      size_t /*maxElements*/,
      stateChange_t switchOnFunction,
      stateChange_t switchOffFunction,
      stateChange_t startFunction,
      stateChange_t stopFunction,
      stateChange_t recoverFunction,
      allowChange_t allowStateChangeFunction,
      writerInt32_t PV_Enable_Writer,
      writerInt32_t PV_Edge_Writer,
      writerInt32_t PV_ClearOverflow_Writer,
      autoEnable_t autoEnable):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
  {
      TimestampingArgs_t handlerTMS = TimestampingArgs_t(switchOnFunction,
                                                         switchOffFunction,
                                                         startFunction,
                                                         stopFunction,
                                                         recoverFunction,
                                                         allowStateChangeFunction,
                                                         PV_Enable_Writer,
                                                         PV_Edge_Writer,
                                                         PV_ClearOverflow_Writer,
                                                         autoEnable);
      constructorBody(handlerTMS);
  }
  template <typename T>
  TimestampingImpl<T>::TimestampingImpl(const std::string& name, const TimestampingArgs_t& handlerTMS) :
      NodeImpl(name, nodeType_t::dataSourceChannel),
      m_OnStartDelegate(handlerTMS.handlerSTM.startFunction),
      m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
  {
      constructorBody(handlerTMS);
  }

  template <typename T>
  inline void TimestampingImpl<T>::constructorBody(const TimestampingArgs_t& handlerTMS) {

        // Add the children PVs
        // PV: Enable
        if (handlerTMS.PV_Enable_Initializer) {
            m_Enable_PV.reset(new PVDelegateOutImpl<std::int32_t>("Enable",
                                            handlerTMS.PV_Enable_Writer,
                                            handlerTMS.PV_Enable_Initializer));
        } else {
            m_Enable_PV.reset(new PVDelegateOutImpl<std::int32_t>("Enable", handlerTMS.PV_Enable_Writer));
        }
        m_Enable_PV->setDescription("Enable/Disable timestamping");
        m_Enable_PV->setScanType(scanType_t::passive, 0);
        addChild(m_Enable_PV);

        // PV: Enable read back value
        m_Enable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Enable_RBV"));
        m_Enable_RBVPV->setDescription("Timestamping status");
        m_Enable_RBVPV->setScanType(scanType_t::interrupt, 0);
        addChild(m_Enable_RBVPV);

        // PV: Edge
        enumerationStrings_t edgeEnumeratorString;
        edgeEnumeratorString.push_back("RISING");
        edgeEnumeratorString.push_back("FALLING");
        edgeEnumeratorString.push_back("ANY");

        if (handlerTMS.PV_Edge_Initializer) {
            m_Edge_PV.reset(new PVDelegateOutImpl<std::int32_t>("Edge",
                                            handlerTMS.PV_Edge_Writer,
                                            handlerTMS.PV_Edge_Initializer));
        } else {
            m_Edge_PV.reset(new PVDelegateOutImpl<std::int32_t>("Edge", handlerTMS.PV_Edge_Writer));
        }
        m_Edge_PV->setDescription("Set timestamp edge detection");
        m_Edge_PV->setEnumeration(edgeEnumeratorString);
        m_Edge_PV->setScanType(scanType_t::passive, 0);
        addChild(m_Edge_PV);

        // PV: Edge read back value
        m_Edge_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Edge_RBV"));
        m_Edge_RBVPV->setDescription("Get timestamping edge detection");
        m_Edge_RBVPV->setScanType(scanType_t::interrupt, 0);
        m_Edge_RBVPV->setEnumeration(edgeEnumeratorString);
        addChild(m_Edge_RBVPV);

        // PV: Decimation
        m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
        m_Decimation_PV->setDescription("Decimation");
        m_Decimation_PV->setScanType(scanType_t::passive, 0);
        m_Decimation_PV->write(getTimestamp(),(std::int32_t)1);
        addChild(m_Decimation_PV);

        // PV: ClearOverflow
        enumerationStrings_t clearEnumeratorString;
        edgeEnumeratorString.push_back("YES");
        edgeEnumeratorString.push_back("NO");

        if (handlerTMS.PV_ClearOverflow_Initializer) {
            m_ClearOverflow_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClearOverflow",
                                            handlerTMS.PV_ClearOverflow_Writer,
                                            handlerTMS.PV_ClearOverflow_Initializer));
        } else {
            m_ClearOverflow_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClearOverflow",
                                            handlerTMS.PV_ClearOverflow_Writer));
        }

        m_ClearOverflow_PV->setDescription("Clear timestamp overflow");
        m_ClearOverflow_PV->setScanType(scanType_t::passive, 0);
        m_Edge_PV->setEnumeration(clearEnumeratorString);
        addChild(m_ClearOverflow_PV);


        // PV: Timestamps
        m_Timestamps_PV.reset(new PVVariableInImpl<T>("Timestamps"));
        m_Timestamps_PV->setDescription("Timestamps on terminal");
        m_Timestamps_PV->setScanType(scanType_t::interrupt, 0);
        m_Timestamps_PV->setMaxElements(4);
        addChild(m_Timestamps_PV);

        // PV: Maxtimestamps
        m_MaxTimestamps_PV.reset(new PVVariableInImpl<std::int32_t>("MaxTimestamps"));
        m_MaxTimestamps_PV->setDescription("Max. number of timestamps");
        m_MaxTimestamps_PV->setScanType(scanType_t::interrupt, 0);
        addChild(m_MaxTimestamps_PV);

        // PV: Overflow
        enumerationStrings_t overflowEnumeratorString; //Move to definitions.h
        overflowEnumeratorString.push_back("NO");
        overflowEnumeratorString.push_back("OVERFLOWED");
        overflowEnumeratorString.push_back("FULL");
        overflowEnumeratorString.push_back("ERROR");

        m_Overflow_PV.reset(new PVVariableInImpl<std::int32_t>("Overflow"));
        m_Overflow_PV->setDescription("Get overflow status");
        m_Overflow_PV->setScanType(scanType_t::interrupt, 0);
        m_Overflow_PV->setEnumeration(overflowEnumeratorString);
        addChild(m_Overflow_PV);

        // Add state machine
        m_StateMachine.reset(new StateMachineImpl(false,
                                                  handlerTMS.handlerSTM.switchOnFunction,
                                                  handlerTMS.handlerSTM.switchOffFunction,
                                                  std::bind(&TimestampingImpl::onStart, this),
                                                  handlerTMS.handlerSTM.stopFunction,
                                                  handlerTMS.handlerSTM.recoverFunction,
                                                  handlerTMS.handlerSTM.allowStateChangeFunction,
                                                  handlerTMS.handlerSTM.autoEnable));
        addChild(m_StateMachine);
  }

  // ----------------------- Common functions ----------------------------- //
  template<typename T>
  timespec TimestampingImpl<T>::getStartTimestamp() const
  {
      return m_StartTime;
  }

  template<typename T>
  void TimestampingImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
  {
      m_StartTimestampFunction = timestampDelegate;
  }

  template<typename T>
  void TimestampingImpl<T>::push(const timespec& timestamp, const T& data)
  {
    m_Timestamps_PV->push(timestamp, data);
  }

  template<typename T>
  void TimestampingImpl<T>::onStart()
  {
      m_StartTime = m_StartTimestampFunction();
      m_Timestamps_PV->setDecimation((std::uint32_t)(m_Decimation_PV->getValue()));
      m_OnStartDelegate();
  }

  template<typename T>
  nds::state_t TimestampingImpl<T>::getState()
  {
    return m_StateMachine->getLocalState();
  }

  // ---------------------------- Getters ---------------------------------- //
  template<typename T>
  std::int32_t TimestampingImpl<T>::getEnable()
  {
    std::int32_t enable;
    timespec timestamp;
    m_Enable_RBVPV->read(&timestamp, &enable);
    return enable;
  }

  template<typename T>
  std::int32_t TimestampingImpl<T>::getEdge()
  {
    std::int32_t edge;
    timespec timestamp;
    m_Edge_RBVPV->read(&timestamp, &edge);
    return edge;
  }

  template<typename T>
  std::int32_t TimestampingImpl<T>::getMaxTimestamps()
  {
    std::int32_t MaxTimestamps;
    timespec timestamp;
    m_MaxTimestamps_PV->read(&timestamp, &MaxTimestamps);
    return MaxTimestamps;
  }

  template<typename T>
  std::int32_t TimestampingImpl<T>::getOverflow()
  {
    std::int32_t overflow;
    timespec timestamp;
    m_Overflow_PV->read(&timestamp, &overflow);
    return overflow;
  }

  template<typename T>
  std::int32_t TimestampingImpl<T>::getDecimation()
  {
    std::int32_t decimation;
    timespec timestamp;
    m_Decimation_PV->read(&timestamp, &decimation);
    return decimation;
  }

  // --------------------------- Setters ----------------------------------- //
  template<typename T>
  void TimestampingImpl<T>::setEnable(const timespec& timestamp, const std::int32_t& value)
  {
    std::int32_t enable = value;
    if (enable != 0 && enable != 1) {
      ndsWarningStream(*this) << "Warning: " <<
                      "Enable must take values 0 or 1. " <<
                      "Value " << enable << " entered. " <<
                      "Setting enable value to 1." << std::endl;
      enable = 1;
    }
    m_Enable_RBVPV->setValue(timestamp, enable);
    m_Enable_RBVPV->push(timestamp, enable);
  }

  template<typename T>
  void TimestampingImpl<T>::setEdge(const timespec& timestamp, const std::int32_t& value)
  {
    std::int32_t edge = value;
    if (edge < 0 || edge > 2) {
      ndsWarningStream(*this) << "Warning: " <<
                      "Edge must take one of the following values: " <<
                      "0(RAISING), 1(FALLING), 2(ANY)." <<
                      "Value " << edge << " entered. " <<
                      "Setting edge value to 2." << std::endl;
      edge = 2; //ANY
    }
    m_Edge_RBVPV->setValue(timestamp, edge);
    m_Edge_RBVPV->push(timestamp, edge);
  }

  template<typename T>
  void TimestampingImpl<T>::setMaxTimestamps(const timespec& timestamp, const std::int32_t& value)
  {
    m_MaxTimestamps_PV->setValue(timestamp, value);
    m_MaxTimestamps_PV->push(timestamp, value);
  }

  template<typename T>
  void TimestampingImpl<T>::setOverflow(const timespec& timestamp, const std::int32_t& value)
  {
    std::int32_t overflow = value;
    if (overflow < 0 || overflow > 3) {
      ndsWarningStream(*this) << "Warning: " <<
                      "Overflow must take one of the following values: 0,1,2. "
                       << "Value " << overflow << " entered. " <<
                      "Setting overflow value to 0." << std::endl;
      overflow = 0;
    }
    m_Overflow_PV->setValue(timestamp, overflow);
    m_Overflow_PV->push(timestamp, overflow);
  }

  template<typename T>
  void TimestampingImpl<T>::setDecimation(const timespec& /*timestamp*/, const std::int32_t& value)
  {
    std::int32_t decimation = value;
    if (decimation < 1) {
      ndsWarningStream(*this) << "Warning: Decimation must take values greater 1. "
              << "Value "<< decimation << " entered."<<
              "Setting decimation value to 1." << std::endl;
      decimation = 1;
    }
    m_Timestamps_PV->setDecimation(decimation);
  }

  template<typename T>
  void TimestampingImpl<T>::setState(const nds::state_t& newState)
  {
    m_StateMachine->setState(newState);
  }

template class TimestampingImpl<timestamp_t>;
}
