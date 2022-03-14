/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMING_H
#define NDSTIMING_H

/**
 * @file timing.h
 * @brief Defines the nds::timing node.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"
#include "nds3/stateMachine.h"

namespace nds
{


/**
 * @brief Type defined to gather the arguments required by @ref Timing constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct TimingArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to read the time from the device.
     * This is a compulsory field of the structure.
     */
    const readerTime_t PV_Time_Reader;

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref TimingArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref TimingArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref TimingArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref TimingArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref TimingArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref TimingArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_Time_Reader Time reader to be set in @ref TimingArgs_t.PV_Time_Reader.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    TimingArgs_t(stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction,
                 readerTime_t _PV_Time_Reader,
                 autoEnable_t autoEnable=autoEnable_t::none):
            handlerSTM( true, ///Asynchronous state transitions.
                        switchOnFunction,
                        switchOffFunction,
                        startFunction,
                        stopFunction,
                        recoverFunction,
                        allowStateChangeFunction,
                        autoEnable),
                        PV_Time_Reader(_PV_Time_Reader)
    {
    }
};


class NDS3_API Timing: public Node  {
  public:
    // ---------------- Constructors and assignment operator -------------------//
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
    Timing();

    /**
     * @brief Copy constructor: copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
    Timing(const Timing& right);


    /**
     * @brief overloading of assignment operator
     * */
    Timing& operator=(const Timing& right);

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
     * @param PV_Time_Reader Delegate function, reads  Time PV
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    Timing( const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        readerTime_t PV_Time_Reader,
        autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the Timing node.
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerTM Structure with the arguments required by the Timing node.
     * See @ref TimingArgs_t for further details.
     */
    Timing(const std::string& name, const TimingArgs_t& handlerTM);

    // ------------------ Functions common to all nodes ---------------------//
    /**
     *
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate
     *
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     *
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call th`is function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const timespec& data);

    /**
     *
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

   // ----------------------------- Getters -------------------------------- //
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
    * @brief Retrieve the clock frequency
    *
    * @return the  clock frequency value
    */
   double getClkFrequency();
   /**
    * @brief Retrieve the clock multiplier
    *
    * @return clock multiplier value
    */
   std::int32_t getClkMultiplier();
   /**
    * @brief Retrieve synchronization status: NOT_SYNCED(0),
    *   SYNCING(1), SYNCED(2), LOST_SYNC(3)
    *
    * @return synchronization status
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

   // ----------------------------- Setters -------------------------------- //
   /**
    * @brief  sets the UNIX Time (PV is Delegate,
    *         push is called) It updates both Time and Htime
    * @param  timestamp timestamp
    * @param  value Time
    */
   void setTime(const timespec& timestamp, const timespec& value);
   /**
    * @brief Sets the value of the clock frequency
    *
    * @param  timestamp timestamp
    * @param  value clock frequency
    */
   void setClkFrequency(const timespec& timestamp, const double& value);
   /**
    * @brief Sets the value of the clock multiplier
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
    * @brief Sets the value of seconds since last sync
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
};

} // namespace nds

#ifdef _MSC_VER
#define CLOCK_REALTIME 0
int NDS3_API clock_gettime(int, struct timespec *spec);
#endif

#ifdef _WIN32
void NDS3_API usleep(int microsecs);
void NDS3_API sleep(int secs);
#endif

#endif // NDSTIMING_H
