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

#ifndef NDS_DATAMULTIPLEXING_H
#define NDS_DATAMULTIPLEXING_H

/**
 * @file dataMultiplexing.h
 * @brief Defines the nds::DataMultiplexing which implements a type of nds::DataScheduling
 * for multiplexing data.
 *
 * This header file can be included by end user, as the class defined is public.
 *
 * @ingroup Scheduling
 */

#include "nds3/definitions.h"
#include "nds3/dataScheduling.h"

namespace nds
{

/**
 * @brief Class for multiplexing data from input PVs following the data sharing mechanism.
 *
 * This child class provides an implementation of the @ref nds::DataScheduling class that
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
 * <li><code></code><std::vector<std::int32_t>></li>
 * <li><code></code><std::vector<std::int64_t>></li>
 * <li><code></code><std::vector<float>></li>
 * <li><code></code><std::vector<double>></li>
 * </ul>
 *
 *
 * @ingroup Scheduling
 */
template <typename T>
class NDS3_API DataMultiplexing: public DataScheduling<T>
{

public:

  /**
   * @brief Empty constructor.
   */
  DataMultiplexing();

  /**
   * @brief Main constructor to create a multiplexing node according to its name and number of inputs.
   *
   * This constructor creates the node with its internal NDS PVs, that is, according to the
   * given name @a name:
   * <ul>
   *    <li>Output PV to set the number of data elements to be considered per input channel,
   *    to be accessed as <em>name.SamplesPerChannel</em>. It can only be set when the
   *    state machine is in ON state.</li>
   *    <li>Input PV to get the number of data element to be considered per input channel,
   *    to be accessed as <em>name.SamplesPerChannel_RBV</em>.</li>
   * </ul>
   *
   * @param name The node's name.
   * @param numberInputs Node's number of PV inputs for the node. They are defined as NDS Output PVs.
   */
  DataMultiplexing(const std::string& name,
               size_t numberInputs
               );

  /**
   * @brief Function to set the number of data elements to be considered per input channel.
   *
   * It can only be set when the state machine is in ON state. Before going to RUN state,
   * this variable must be set to a positive integer number.
   *
   * @param timestamp Timestamp to indicate when the function is called.
   * @param maxSamples Number of samples to be considered per input channel.
   */
  void setSamplesPerChannel(const timespec &timestamp, const std::int32_t &maxSamples);

  /**
   *
   * @return Function to get the number of samples that are considered per input channel.
   */
  std::int32_t getSamplesPerChannel();

};


}

#endif //NDS_DATAMULTIPLEXING_H
