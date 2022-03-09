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

#ifndef NDS_DATASCHEDULINGIMPL_H
#define NDS_DATASCHEDULINGIMPL_H

#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

/**
 * @file dataSchedulingImpl.h
 *
 * @brief Defines the nds::DataSchedulingImpl class which implements the internal behaviour of the
 * nds::DataScheduling class for data sharing and scheduling.
 */

namespace nds
{

/**
 * @brief Base implementation class for data sharing and scheduling nodes.
 *
 * This base class contains all internal elements and methods that implement and
 * control the MIMO (Multiple Inputs, Multiple Outputs) nodes that can be
 * used in order to gather data from their inputs and to provide processed outputs.
 *
 * This class provides so many inputs and outputs as indicated in instantiation.
 *
 * @tparam T the PV data type to work with.
 * The following data types are supported:
 * <ul>
 * <li><code><std::int32_t></code></li>
 * <li><code><std::int64_t></code></li>
 * <li><code><std::float></code></li>
 * <li><code><std::double></code></li>
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
class DataSchedulingImpl: public NodeImpl
{

public:

  /**
   * @brief Main constructor called by children nodes providing the specific functionalities.
   *
   * It provides a common structure for all children nodes in the group.
   *
   * This constructor creates the implementation of the node with its internal NDS elements, that is,
   * according to the given @a name:
   * <ul>
   *    <li>State Machine: to be accessed as <em>name.StateMachine</em>. It allows controlling the node behaviour.</li>
   *    <li>Trigger: to be accessed as <em>name.Trigger</em>. Whenever this PV is set to <code>1</code>, the
   *    current node runs its specific action.</li>
   *    <li>Inputs: to be accessed as <em>DataIn_M</em>, being M the index of the given input,
   *    in the range of <em>[0, numberInputs-1]</em>. These are the data input PVs of the node that
   *    shall be subscribed to the corresponding data source PVs.</li>
   *    <li>Outputs: to be accessed as <em>DataOut_N</em>, being  N the index of the given input,
   *    in the range of <em>[0, numberOutputs-1]</em>. These are the data output PVs of the node that
   *    will provide the processed data whenever the Trigger PV is processed.</li>
   * </ul>
   *
   * @param name The node's name.
   * @param numberInputs Node's number of PV inputs for the node. They are defined as NDS Output PVs.
   * @param numberOutputs Node's number of PV outputs for the node. . They are defined as NDS Input PVs.
   * @param switchOnFunction Delegate function that performs the actions to switch the node on.
   * @param switchOffFunction Delegate function that performs the actions to switch the node off.
   * @param startFunction Delegate function that performs the actions to start the data scheduling.
   * @param stopFunction Delegate function that performs the actions to stop the data scheduling.
   * @param recoverFunction Delegate function to execute to recover from an error state.
   * @param allowStateChangeFunction Delegate function that can deny a state change. Usually just returns true.
   * @param triggerAction Delegate function that defines what the node does to generate its outputs.
   */
  DataSchedulingImpl(const std::string& name,
               size_t numberInputs,
               size_t numberOutputs,
               stateChange_t switchOnFunction,
               stateChange_t switchOffFunction,
               stateChange_t startFunction,
               stateChange_t stopFunction,
               stateChange_t recoverFunction,
               allowChange_t allowStateChangeFunction,
               writerInt32_t triggerAction
               );


  /**
   * @brief Set the function that retrieves the exact start time when the data processing starts.
   *
   * @param timestampDelegate function that returns the exact starting time of the
   *                           data scheduling
   */
  void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

  /**
   * @brief Returns the timestamp at start.
   *
   * @return the time when started.
   */
  timespec getStartTimestamp() const;

  /**
   * @brief Called by the state machine. Store the current timestamp and then calls the
   *        delegated onStart function.
   */
  void onStart();

protected:

  /**
   * @brief Number of input PVs to be handled by the instantiated node.
   */
  const size_t nInputs;


  /**
   * @brief Number of output PVs to be handled by the instantiated node.
   */
  const size_t nOutputs;


  /**
   * @brief In the state machine we set the start function to onStart(), so we
   *        remember here what to call from onStart().
   */
  stateChange_t m_OnStartDelegate;

  /**
   * @brief Delegate function that retrieves the start time. Executed
   *        by onStart().
   *
   * By default points to BaseImpl::getTimestamp().
   *
   * Use setStartTimestampDelegate() to change the delegate function.
   */
  getTimestampPlugin_t m_StartTimestampFunction;

  /**
   * @brief Generation start time. Retrieved during onStart() via the delegate
   *        function declared in  m_startTimestampFunction.
   */
  timespec m_StartTime;

  /**
   * @brief State machine controlling the general node behaviour.
   *
   * It defines what can be done by the node in each state.
   *
   * This PVs is named as <em>StateMachine</em>.
   *
   * Its logical shall be provided by the children nodes by calling to its
   * parent's constructor method.
   */
  std::shared_ptr<StateMachineImpl> m_StateMachine;

  /**
   * @brief Delegate PV defining the specific node behaviour.
   *
   * This is the key PV that contains the specific implementation of the node.
   * Whenever this PV receives a write operation, the node's action will be executed.
   *
   * This PVs is named as <em>Trigger</em>.
   *
   * Its logical shall be provided by the children nodes by calling to its
   * parent's constructor method.
   */
  std::shared_ptr< PVDelegateOutImpl<std::int32_t> > m_Trigger_PV;

  /**
   * @brief Vector containing the set of Input PVs to be considered by the node.
   *
   * This vector is allocated to @ref nInputs at instantiation time and its size
   * is not modified any more.
   *
   * These input PVs are named as <em>DataIn_M</em>, being M the index of the given input,
   * in the range of <em>[0, nInputs-1]</em>.
   *
   * Finally, it is expected that each input PV in this vector is subscribed to the corresponding
   * data source PV. In this way, whenever the source PV receives a new value, it is also sent
   * to the corresponding input PV of this node.
   */
  std::vector< std::shared_ptr< PVVariableOutImpl<T>>> m_DataIn_PV;

  /**
   * @brief Vector containing the set of Output PVs to be generated by the node.
   *
   * This vector is allocated to @ref nOutputs at instantiation time and its size
   * is not modified any more.
   *
   * These output PVs are named as <em>DataOut_N</em>, being N the index of the given input,
   * in the range of <em>[0, nOutputs-1]</em>.
   *
   * Finally, this PV is updated whenever the @ref m_Trigger_PV PV has executed its
   * delegated action.
   */
  std::vector< std::shared_ptr< PVVariableInImpl<T>>> m_DataOut_PV;


};

}

#endif //NDS_DATASCHEDULINGIMPL_H
