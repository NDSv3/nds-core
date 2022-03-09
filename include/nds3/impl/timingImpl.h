/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMINGIMPL_H
#define NDSTIMINGIMPL_H

/**
 * @file timingImpl.h
 * @brief Defines nds:Timing node.
 *
 */
#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/timing.h"

namespace nds {

class TimingImpl: public NodeImpl {
  public:
    /**
     *
     * @brief Constructs the Timing node
     *
     * @param name node name
     * @param switchOnFunction  Delegate function, performs the actions to switch the node on
     * @param switchOffFunction Delegate function, performs the actions to switch the node off
     * @param startFunction     Delegate function, performs the actions to start the timestamping
     * @param stopFunction      Delegate function, performs the actions to stop the timestamping
     * @param recoverFunction   Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction  Delegate function that can deny a state change.
     *                                  Usually just returns true
     * @param PV_Time_Reader Delegate function, reads Time PV
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    TimingImpl( const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        readerTime_t PV_Time_Reader,
        autoEnable_t autoEnable);

    /**
     * @brief Construct the Timing node by means of its structure of functions.
     * @param name Name of the node
     * @param handlerTM Structure with delegate functions that perform the required actions.
     */
    TimingImpl(const std::string& name, const TimingArgs_t& handlerTM);

   // Common functions
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
   void push(const timespec& timestamp, const timespec& data);
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
    * @brief Retrieve the UNIX time
    *
    * @return UNIX time
    */
   timespec getTime();
   /**
    * @brief Retrieve human readable time (UTC)
    *
    * @return human readable time (UTC)
    */
   std::string getHumanTime();
   /**
    * @brief Retrieve the Clock frequency
    *
    * @return the  Clock frequency value
    */
   double getClkFrequency();
   /**
    * @brief Retrieve the Clock multiplier
    *
    * @return the  Clock multiplier value
    */
   std::int32_t getClkMultiplier();
   /**
    * @brief Retrieve Synchronization status: NOT_SYNCED(0),
    * SYNCING(1), SYNCED(2), LOST_SYNC(3)
    *
    * @return Synchronization status
    *
    */
   std::int32_t getSyncStatus();
   /**
    * @brief Retrieve the seconds since last Sync
    *
    * @return the seconds since last Sync
    */
   std::int32_t getSecsLastSync();
   /**
    * @brief Retrieve the Reference base time
    *
    * @return the Reference base time value
    */
   timespec getRefTimeBase();

   // --------------------------- Setters ----------------------------------- //
   /**
    * @brief  sets the UNIX Time (PV is Delegate,
    *         push is called) It updates both Time and HTime
    *
    * @param  timestamp timestamp
    * @param  value Time
    */
   void setTime(const timespec& timestamp, const timespec& value);
   /**
    * @brief Sets the value of the Clock frequency
    *
    * @param  timestamp timestamp
    * @param  value clock frequency
    */
   void setClkFrequency(const timespec& timestamp, const double& value);
   /**
    * @brief Sets the value of the Clock multiplier
    *
    * @param  timestamp timestamp
    * @param  value clock multiplier
    */
   void setClkMultiplier(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of synchronization status: NOT_SYNCED(0),
    *   SYNCING(1), SYNCED(2), LOST_SYNCED(3)
    *
    * @param  timestamp timestamp
    * @param  value synchronization status
    */
   void setSyncStatus(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of the seconds since last Sync
    *
    * @param  timestamp timestamp
    * @param  value seconds since last sync
    */
   void setSecsLastSync(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of the Reference base time
    *
    * @param  timestamp timestamp
    * @param  value reference base time
    */
   void setRefTimeBase(const timespec& timestamp, const timespec& value);

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

    /**
     * @brief current time. Retrieved via the delegate
     *        function associated to m_TimeV
     */
    timespec m_CurrentTime;


    // PVs

    std::shared_ptr<PVDelegateInImpl<timespec>> m_Time_PV;

    std::shared_ptr<PVDelegateInImpl<std::string>> m_HumanTime_PV;

    std::shared_ptr<PVVariableInImpl<double> > m_ClkFrequency_PV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ClkMultiplier_PV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SyncStatus_PV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SecsLastSync_PV;

    std::shared_ptr<PVVariableInImpl<timespec> > m_RefTimeBase_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_Decimation_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

   void PV_HTime_Reader(timespec *timestamp, std::string *value);

private:

    /**
    * Common source code to define the body of any constructor
    * @param handlerTM Structure containing both compulsory and optional functions.
    */
    void constructorBody(const TimingArgs_t& handlerTM);


};
}

#endif //  NDSTIMINGIMPL_H
