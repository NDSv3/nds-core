#include "nds3/definitions.h"
#include "nds3/impl/dataMultiplexingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include <iostream>

namespace nds
{

template<typename T>
DataMultiplexingImpl<T>::DataMultiplexingImpl(const std::string& name,  ///< The node's name.
               size_t numberInputs ///< Node's number of PV inputs.
               ):
  DataSchedulingImpl<T>(
                     name,
                     numberInputs,
                     1, //numberOutputs,
                     std::bind(&DataMultiplexingImpl<T>::switchOn, this),
                     std::bind(&DataMultiplexingImpl<T>::switchOff, this),
                     std::bind(&DataMultiplexingImpl<T>::start, this),
                     std::bind(&DataMultiplexingImpl<T>::stop, this),
                     std::bind(&DataMultiplexingImpl<T>::recover, this),
                     std::bind(&DataMultiplexingImpl<T>::allowStateChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                     std::bind(&DataMultiplexingImpl<T>::multiplex, this, std::placeholders::_1, std::placeholders::_2)
                    )
{


  m_SamplesPerChannel_PV.reset(new PVDelegateOutImpl<std::int32_t> ("SamplesPerChannel",
                                                                   std::bind(&DataMultiplexingImpl<T>::setSamplesPerChannel,
                                                                   this,
                                                                   std::placeholders::_1,
                                                                   std::placeholders::_2
                                                                   )));

  m_SamplesPerChannel_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SamplesPerChannel_RBV"));
  m_SamplesPerChannel_RBVPV->setDescription("Current Samples per channel");
  m_SamplesPerChannel_RBVPV->setScanType(scanType_t::interrupt, 0);
  this->addChild(m_SamplesPerChannel_RBVPV);

  m_SamplesPerChannel_PV->setDescription("Samples per channel");
  this->addChild(m_SamplesPerChannel_PV);

}

template<typename T>
void DataMultiplexingImpl<T>::switchOn(void) {

}

template<typename T>
void DataMultiplexingImpl<T>::switchOff(void) {

}

template<typename T>
void DataMultiplexingImpl<T>::start(void) {

}

template<typename T>
void DataMultiplexingImpl<T>::stop(void) {

}

template<typename T>
void DataMultiplexingImpl<T>::recover(void) {

}

template<typename T>
bool DataMultiplexingImpl<T>::allowStateChange(const state_t currentState, const state_t /*currentGlobalState*/, const state_t newState) {
  if (currentState == state_t::on && newState == state_t::running) {
      timespec timestamp;
      std::int32_t samplesPerChannel;
      m_SamplesPerChannel_RBVPV->read(&timestamp, &samplesPerChannel);
      if (samplesPerChannel <= 0) {
          std::cout << "DataMultiplexing: State cannot be changed. SamplesPerChannel must be greater than 0." << std::endl;
          return false;
      }
  }
  return true;
}

template<typename T>
void DataMultiplexingImpl<T>::multiplex(const timespec &/*time*/, const std::int32_t &doIt) {
  if (doIt && std::static_pointer_cast<StateMachineImpl>(this->m_StateMachine)->getLocalState() == state_t::running) {
      timespec timestamp;
      std::int32_t samplesPerChannel;
      m_SamplesPerChannel_RBVPV->read(&timestamp, &samplesPerChannel);
      std::uint64_t offset = 0;
      T dataOut(samplesPerChannel*this->nInputs);
      for (int i = 0; i < this->nInputs; i++) {
         T value;
         timespec time;
         this->m_DataIn_PV[i]->read(&time, &value);
         if (value.size() < samplesPerChannel) {
             throw std::runtime_error(m_SamplesPerChannel_RBVPV->getFullExternalName() + " greater than " +
                                      this->m_DataIn_PV[i]->getFullExternalName() +
                                      " (" + std::to_string(samplesPerChannel) + " > " +
                                      std::to_string(value.size()) + ")");
         }
         std::move(value.begin(), value.begin() + samplesPerChannel, dataOut.begin() + offset);
         offset += samplesPerChannel;
      }
      this->m_DataOut_PV[0]->setValue(this->getTimestamp(), dataOut);
      this->m_DataOut_PV[0]->push(this->getTimestamp(), dataOut);
  }
}

template<typename T>
void DataMultiplexingImpl<T>::setSamplesPerChannel(const timespec &time, const std::int32_t &maxSamples) {
  state_t currentState = this->m_StateMachine->getLocalState();
  if (currentState == state_t::on ) {
      m_SamplesPerChannel_RBVPV->setValue(time, maxSamples);
      m_SamplesPerChannel_RBVPV->push(time, maxSamples);
  } else {
      ndsErrorStream(*this) << "The number of samples per channel can only be modified in ON state." << std::endl;
  }
}

template<typename T>
std::int32_t DataMultiplexingImpl<T>::getSamplesPerChannel()
{
  std::int32_t samplesPerChannel;
  timespec timestamp;
  m_SamplesPerChannel_RBVPV->read(&timestamp, &samplesPerChannel);
  return samplesPerChannel;
}

template class DataMultiplexingImpl<std::vector<std::int8_t>>;
template class DataMultiplexingImpl<std::vector<std::uint8_t>>;
template class DataMultiplexingImpl<std::vector<std::int16_t>>;
template class DataMultiplexingImpl<std::vector<std::int32_t>>;
template class DataMultiplexingImpl<std::vector<std::int64_t>>;
template class DataMultiplexingImpl<std::vector<float>>;
template class DataMultiplexingImpl<std::vector<double>>;

}
