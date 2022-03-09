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

#ifndef NDS_DATASCHEDULING_H
#define NDS_DATASCHEDULING_H

/**
 * @file dataScheduling.h
 * @brief Defines the nds::DataScheduling which provides basic elements for data sharing and
 *        scheduling.
 *
 * This header file should not be included by end user, as the class defined is protected
 * and it is just a prototype for its children classes.
 *
 * @ingroup Scheduling
 *
 * @defgroup Scheduling Scheduling
 *
 * Group to deal with MIMO (Multiple Inputs, Multiple Outputs) nodes whose purpose
 * is gathering data from its inputs and process them specifically in order to
 * provide the corresponding outputs.

 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * @brief Base class for data sharing and scheduling nodes.
 *
 * This base class provides a common structure to MIMO (Multiple Inputs, Multiple
 * Outputs) nodes that can be implemented in order to gather data from their inputs
 * and provide the outputs after applying them its specific function.
 *
 * This class provides so many inputs and outputs as indicated in instantiation.
 *
 * It also contains  a @ref nds::StateMachine as well as a Trigger PV
 * whose behaviours are defined by the children nodes implementations.
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
 *
 * @ingroup Scheduling
 */
template <typename T>
class NDS3_API DataScheduling: virtual public Node
{

protected:

  /**
   * @brief Empty constructor that cannot be used by end user.
   *
   * It is a internal constructor required by its children nodes.
   */
  DataScheduling();

  /**
   * @brief Main constructor that cannot be used by end user.
   *
   * It is a internal constructor required by its children nodes.
   *
   * This constructor creates the node with its internal NDS elements, that is,
   * according to the given @a name:
   * <ul>
   *    <li>State Machine: to be accessed as <em>name.StateMachine</em>. It allows controlling the node behaviour.</li>
   *    <li>Trigger: to be accessed as <em>name.Trigger</em>. Whenever this PV is set to <code>1</code>, the
   *    current node runs its specific action.</li>
   *    <li>Inputs: to be accessed as <em>DataIn_M</em>, being M the index of the given input,
   *    in the range of <em>[0, numberInputs-1]</em>. These are the data input PVs of the node that
   *    shall be subscribed to the corresponding data source PVs.</li>
   *    <li>Outputs: to be accessed as <em>DataOut_N</em>, being N the index of the given input,
   *    in the range of <em>[0, numberOutputs-1]</em>. These are the data output PVs of the node that
   *    will provide the processed data whenever the Trigger PV is processed.</li>
   * </ul>
   */
  DataScheduling(const std::string& name,  ///< The node's name.
               size_t numberInputs, ///< Node's number of PV inputs.
               size_t numberOutputs, ///< Node's number of PV outputs.
               stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on.
               stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off.
               stateChange_t startFunction,            ///< Delegate function that performs the actions to start the data scheduling.
               stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the data scheduling.
               stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state.
               allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true.
               writerInt32_t triggerAction ///< Delegate function that defines what the node does to generate its outputs.
               );

  /**
   * @brief Copy constructor that cannot be used by end user.
   *
   * It copies a DataScheduling node reference from another object.
   * @param right data scheduling holder from which the reference to the
   *              data scheduling object implementation is copied.
   */
  DataScheduling(const DataScheduling<T>& right);

public:

  /**
   * @brief Set the function that retrieves the exact start time when the data processing starts.
   *
   * @param timestampDelegate function that returns the exact starting time of the
   *                           data scheduling
   */
  void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

  /**
   * @ingroup timing
   * @brief Returns the timestamp at the moment of the start of the data scheduling.
   *
   * This value is set by the state machine when the state switches to running.
   * If a timing plugin is active then the timestamp is taken from the plugin.
   *
   * @return the time when the data scheduling started.
   */
  timespec getStartTimestamp() const;

  /**
   * @brief Copies a data scheduling reference into other one.
   * @param right Right object to be copied into the left one.
   * @return Copied object reference.
   */
  DataScheduling& operator=(const DataScheduling& right);

};


}

#endif //NDS_DATASCHEDULING_H
