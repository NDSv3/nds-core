/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESTAMPINGIMPL_H
#define NDSTIMESTAMPINGIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/timestamping.h"

namespace nds {

  template <typename T> class PVVariableInImpl;
  template <typename T> class PVDelegateOutImpl;
  template <typename T> class PVVariableOutImpl;

  template<typename T>
  class TimestampingImpl: public NodeImpl {
   public:

    /**
     *
     * @brief Constructs the Time stamping node
     *
     * @param name node name
     * @param maxElements not used in this node
     * @param switchOnFunction  Delegate function, performs the actions to switch the node on
     * @param switchOffFunction Delegate function, performs the actions to switch the node off
     * @param startFunction     Delegate function, performs the actions to start the timestamping
     * @param stopFunction      Delegate function, performs the actions to stop the timestamping
     * @param recoverFunction   Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction  Delegate function that can deny a state change.
     *                                  Usually just returns true
     * @param PV_Enable_Writer Delegate function to enable/disable timestamping
     * @param PV_Edge_Writer   Delegate function, sets the edges at which the
     *                         timestamping must be retrieved: RISING, FALLING, ANY.
     * @param PV_ClearOverflow_Writer Delegate function, performs the actions to clear the overflow.
     * @param autoEnable See @ref autoEnable_t for further details
     */
     TimestampingImpl(const std::string& name,
                 size_t maxElements,
                 stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction,
                 writerInt32_t PV_Enable_Writer,
                 writerInt32_t PV_Edge_Writer,
                 writerInt32_t PV_ClearOverflow_Writer,
                 autoEnable_t autoEnable);

     /**
      * @brief Constructs the Timestamping node by means of its structure of functions.
      * @param name Name of the node
      * @param handlerTMS Structure with delegate functions that perform the required actions.
      */
     TimestampingImpl(const std::string& name, const TimestampingArgs_t& handlerTMS);

    // ----------------------- Common functions ----------------------------- //
    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     *
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Called by the state machine. Store the current timestamp and
     *        then calls the delegated onStart function.
     */
    void onStart();

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    // ---------------------------- Getters ---------------------------------- //
    /**
     * @brief Retrieves timestamping status
     *
     * @return timestamping status
     */
    std::int32_t getEnable();

    /**
     * @brief Retreives detection edge
     *
     * @return detection edge
     */
    std::int32_t  getEdge();

    /**
     * @brief Retrieves max number of timestamps
     *
     * @return max number of timestamps
     */
    std::int32_t getMaxTimestamps();

    /**
     * @brief Retreives overflow error status: NO(0), OVERFLOWED(1), FULL(2), ERROR(2)
     *
     * @return overflow error status
     */
    std::int32_t getOverflow();

    /**
     * @brief Retreives the decimation value set by the Control System
     *
     * @return decimation value
     */
    std::int32_t getDecimation();


    // --------------------------- Setters ----------------------------------- //
    /**
    * @brief Sets timestamping status
    *
    * @param timestamp timestamp
    * @param value timestamping status value
    */
    void setEnable(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets detection Edge: RISING(0), FALLING(1), ANY(2)
    *
    * @param timestamp timestamp
    * @param value detection edge
    */
    void setEdge(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Retrieves max number of timestamps
     *
     * @param  timestamp timestamp
     * @param  value  max number of timestamps
     */
    void setMaxTimestamps(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets overflow error status: NO(0), OVERFLOWED(1), FULL(2), ERROR(2)
    *
    * @param timestamp timestamp
    * @param value overflow error status
    */
    void setOverflow(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the decimation factor to the timestamps PV
     *
     * @param timestamp timestamp
     * @param value decimation factor
     */
    void setDecimation(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

   protected:
     /**
      * @brief In the state machine we set the start function to onStart(), so we
      *        remember here what to call from onStart().
      */
     stateChange_t m_OnStartDelegate;
     /**
      * @brief Delegate function that retrieves the start time.
      *
      * By default points to BaseImpl::getTimestamp().
      *
      * Use setStartTimestampDelegate() to change the delegate function.
      */
     getTimestampPlugin_t m_StartTimestampFunction;

     /**
      * @brief Start time. Retrieved via the delegate
      *        function declared in  m_startTimestampFunction.
      */
     timespec m_StartTime;

     // PVs
     std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Enable_PV;
     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_Enable_RBVPV;

     std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Edge_PV;
     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_Edge_RBVPV;

     std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_Decimation_PV;

     std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ClearOverflow_PV;

     std::shared_ptr<PVVariableInImpl<T>> m_Timestamps_PV;

     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_MaxTimestamps_PV;

     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_Overflow_PV;

     std::shared_ptr<StateMachineImpl> m_StateMachine;

   private:

    /**
    * @brief Common source code to define the body of any constructor.
    * @param hanlderTMS Structure containing both compulsory and optional functions.
    */
    void constructorBody(const TimestampingArgs_t& hanlderTMS);


  };
}

#endif // NDSTIMESTAMPINGIMPL_H
