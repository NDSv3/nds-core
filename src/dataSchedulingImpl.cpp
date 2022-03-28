#include <string>

#include "nds3/definitions.h"
#include "nds3/impl/dataSchedulingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"

namespace nds
{

template<typename T>
DataSchedulingImpl<T>::DataSchedulingImpl(const std::string& name,  ///< The node's name.
               size_t numberInputs, ///< Node's number of PV inputs.
               size_t numberOutputs, ///< Node's number of PV outputs.
               stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on.
               stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off.
               stateChange_t startFunction,            ///< Delegate function that performs the actions to start the data scheduling.
               stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the data scheduling.
               stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state.
               allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true.
               writerInt32_t triggerAction ///< Delegate function that defines what the node does to generate its outputs.
               ):
                NodeImpl(name, nodeType_t::dataSourceChannel),
                nInputs(numberInputs),
                nOutputs(numberOutputs),
                m_OnStartDelegate(startFunction),
                m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this)),
                m_DataIn_PV(numberInputs),
                m_DataOut_PV(numberOutputs)
{


  // Add state machine
  m_StateMachine.reset(new StateMachineImpl(false,
                                            switchOnFunction,
                                            switchOffFunction,
                                            std::bind(&DataSchedulingImpl::onStart, this),
                                            stopFunction,
                                            recoverFunction,
                                            allowStateChangeFunction));
  addChild(m_StateMachine);

  //Add the PV that launch the node's action when triggered
  m_Trigger_PV.reset(new PVDelegateOutImpl<std::int32_t> ("Trigger", triggerAction));
  m_Trigger_PV->setDescription("Trigger the node");
  this->addChild(m_Trigger_PV);

  for (int i = 0; i < this->nInputs; i++) {
      std::string iTxt = std::to_string(i);
      std::shared_ptr< PVVariableOutImpl<T>> ptr(new PVVariableOutImpl<T>("DataIn_" + iTxt));
      ptr->setDescription("Data Input " + iTxt);
      ptr->setScanType(scanType_t::interrupt, 0);
      m_DataIn_PV[i] = ptr;
      this->addChild(ptr);
  }

  for (int i = 0; i < this->nOutputs; i++) {
      std::string iTxt = std::to_string(i);
      std::shared_ptr< PVVariableInImpl<T>> ptr(new PVVariableInImpl<T>("DataOut_" + iTxt));
      ptr->setDescription("Data Output " + iTxt);
      ptr->setScanType(scanType_t::interrupt, 0);
      m_DataOut_PV[i] = ptr;
      this->addChild(ptr);
  }
}

template<typename T>
void DataSchedulingImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
  m_StartTimestampFunction = timestampDelegate;
}
template<typename T>
timespec DataSchedulingImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void DataSchedulingImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    //Set Decimation when push method is defined
    m_OnStartDelegate();
}

template class DataSchedulingImpl<std::int32_t>;
template class DataSchedulingImpl<std::int64_t>;
template class DataSchedulingImpl<float>;
template class DataSchedulingImpl<double>;
template class DataSchedulingImpl<std::vector<std::int8_t> >;
template class DataSchedulingImpl<std::vector<std::uint8_t> >;
template class DataSchedulingImpl<std::vector<std::int16_t> >;
template class DataSchedulingImpl<std::vector<std::int32_t> >;
template class DataSchedulingImpl<std::vector<std::int64_t> >;
template class DataSchedulingImpl<std::vector<float> >;
template class DataSchedulingImpl<std::vector<double> >;


}
