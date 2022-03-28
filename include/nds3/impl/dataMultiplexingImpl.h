/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDS_DATASMULTIPLEXINGIMPL_H
#define NDS_DATASMULTIPLEXINGIMPL_H

#include "nds3/definitions.h"
#include "nds3/impl/dataSchedulingImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"

/**
 * @file dataMultiplexingImpl.h
 *
 * @brief Defines the nds::DataMultiplexingImpl class which implements the internal behaviour of the
 * nds::DataMultiplexing class for multiplexing data.
 */

namespace nds
{

/**
 * @brief Children class of data scheduling implementing functionalities for data multiplexing.
 *
 * This child class provides the internal implementation of the @ref nds::DataMultiplexing class that
 * can be used to gather data from their input PVs and provide a multiplexed of them only
 * in one output PV.
 *
 * This class contains so many inputs as indicated in instantiation, but only one output.
 * The output PV is composed of all data of each input PV dumped in order. For instance,
 * let @em N be the size of the input PVs, the output is shaped as:
 * <table>
 *   <tr>
 *     <td>Input index</td> <td>Input data index</td> <td>Output index</td>
 *   </tr>
 *   <tr>
 *     <td>0</td> <td>0</td> <td>0</td>
 *   </tr>
 *   <tr>
 *     <td>0</td> <td>1</td> <td>1</td>
 *   </tr>
 *   <tr>
 *     <td>0</td> <td>...</td> <td>...</td>
 *   </tr>
 *   <tr>
 *     <td>0</td> <td>N-1</td> <td>N-1</td>
 *   </tr>
 *
 *   <tr>
 *     <td>1</td> <td>0</td> <td>N</td>
 *   </tr>
 *   <tr>
 *     <td>1</td> <td>1</td> <td>N+1</td>
 *   </tr>
 *   <tr>
 *     <td>1</td> <td>...</td> <td>...</td>
 *   </tr>
 *   <tr>
 *     <td>1</td> <td>N-1</td> <td>2*N-1</td>
 *   </tr>
 *
 *   <tr>
 *     <td>...</td> <td>...</td> <td>...</td>
 *   </tr>
 *
 *   <tr>
 *     <td>numberInputs-1</td> <td>0</td> <td>(numberInputs-1)*N</td>
 *   </tr>
 *   <tr>
 *     <td>numberInputs-1</td> <td>1</td> <td>(numberInputs-1)*N+1</td>
 *   </tr>
 *   <tr>
 *     <td>numberInputs-1</td> <td>...</td> <td>...</td>
 *   </tr>
 *   <tr>
 *     <td>numberInputs-1</td> <td>N-1</td> <td>numberInputs*N-1</td>
 *   </tr>
 * </table>
 *
 * @tparam T the PV data type to work with. This class is only focused on @em std::vector PVs.
 * The following data types are supported:
 * <ul>
 * <li><code><std::vector<std::int8_t>></code></li>
 * <li><code><std::vector<std::uint8_t>></code></li>
 * <li><code><std::vector<std::int16_t>></code></li>
 * <li><code><std::vector<std::int32_t>></code></li>
 * <li><code><std::vector<std::int64_t>></code></li>
 * <li><code><std::vector<float>></code></li>
 * <li><code><std::vector<double>></code></li>
 * </ul>
 *
 */
template<typename T>
class DataMultiplexingImpl: public DataSchedulingImpl<T>
{


public:

  /**
   * @brief Constructor to create a multiplexing node given its name and the number of inputs.
   *
   * This constructor creates the implementation of the node with its internal NDS elements.
   * It contains the PVs provided by its parent class @ref nds::DataSchedulingImpl, and the
   * specific PVs, according to the given @a name:
   * <ul>
   *    <li>Samples per channel: to be accessed as <em>name.SamplesPerChannel</em>. Output PV for setting
   *    the number of samples to be used per input channel. It can only be set when the state machine is in
   *    ON state.</li>
   *    <li>Samples per channel (readback): to be accessed as <em>name.SamplesPerChannel_RBV</em>. Input PV for getting
   *    the number of samples that is considered per input channel. This PV shows the actual number of
   *    samples that are taken into account for multiplexing data.</li>
   * </ul>
   *
   * @param name The node's name.
   * @param numberInputs Node's number of PV inputs for the node. They are defined as NDS Output PVs.
   */
  DataMultiplexingImpl(const std::string& name,
               size_t numberInputs
               );

protected:

  /**
   * @brief Delegate PV for setting the number of samples to be considered per input channel.
   *
   * The value provided to this PV will be valid only when the sate machine is in ON state.
   * A positive value must be set to this PV before going to RUNNING state.
   */
  std::shared_ptr< PVDelegateOutImpl<std::int32_t> > m_SamplesPerChannel_PV;

  /**
   * @brief PV providing the actual number of samples to be considered per input channel.
   *
   * It will be updated whenever a successful write operation is done on @ref m_SamplesPerChannel_PV.
   */
  std::shared_ptr< PVVariableInImpl<std::int32_t> > m_SamplesPerChannel_RBVPV;

protected:

  /**
   * @brief Switch on function required by the state machine.
   *
   * For this node, no actions are required in this function.
   */
  void switchOn(void);

  /**
   * @brief Switch off function required by the state machine.
   *
   * For this node, no actions are required in this function.
   */
  void switchOff(void);

  /**
   * @brief Start function required by the state machine.
   *
   * For this node, no actions are required in this function.
   */
  void start(void);

  /**
   * @brief Stop function required by the state machine.
   *
   * For this node, no actions are required in this function.
   */
  void stop(void);

  /**
   * @brief Recover function required by the state machine.
   *
   * For this node, no actions are required in this function.
   */
  void recover(void);

  /**
   * @brief Check function required by the state machine to allow state transitions.
   *
   * Transition from ON to RUNNING state is only allowed when @ref m_SamplesPerChannel_RBVPV
   * has been set to a positive value.
   * @param currentState Current state.
   * @param currentGlobalState Current global state.
   * @param newState New state to switch.
   * @return <code>true</code> if the state transition is allowed, <code>false</code> otherwise.
   */
  bool allowStateChange(const state_t currentState, const state_t currentGlobalState, const state_t newState);

  /**
   * @brief Multiplex function that is executed whenever the @ref nds::DataSchedulingImpl::m_Trigger_PV
   * is written.
   *
   * The length of the input PVs must be greater or equal than @ref m_SamplesPerChannel_RBVPV.
   * @param time Timestamp to indicate when the function is called.
   * @param doIt Parameter indicating if the action shall be executed when it is different to zero.
   */
  void multiplex(const timespec &time, const std::int32_t &doIt);

public:

  /**
   * @brief Delegate function to set the number of samples per channel.
   *
   * Only when the state machine is in ON state, this function sets the @ref
   * m_SamplesPerChannel_RBVPV to the given value.
   *
   * @param timestamp Timestamp to indicate when the function is called.
   * @param maxSamples Maximum number of samples to be taken per channel.
   */
  void setSamplesPerChannel(const timespec &timestamp, const std::int32_t &maxSamples);

  /**
   * @brief Get the number of samples per channel to be multiplexed.
   * @return Number of samples per channel configured.
   */
  std::int32_t getSamplesPerChannel();
};

}
#endif //NDS_DATASMULTIPLEXINGIMPL_H
