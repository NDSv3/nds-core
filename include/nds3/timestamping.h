/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESTAMPING_H
#define NDSTIMESTAMPING_H

/**
 * @file timestamping.h
 * @brief Defines the nds::timestamping node.
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
 * @brief Type defined to gather the arguments required by @ref Timestamping constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct TimestampingArgs_t {
    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to write (enable/disable) timestamps generation.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Enable_Writer;

    /**
     * @brief Function to load the initial value of the timestamps generation.
     * It reads the status of the flag to generate timestamps and it
     * is loaded as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Enable_Initializer;

    /**
     * @brief Function to write the Edge to retrieve timestamps.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Edge_Writer;

    /**
     * @brief Function to load the initial value of the Edge.
     * It reads the Edge that is configured to generate timestamps and it
     * is loaded as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Edge_Initializer;

    /**
     * @brief Function to clear the overflow status.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_ClearOverflow_Writer;

    /**
     * @brief Function to load the initial value of the ClearOverflow flag.
     * It reads the ClearOverflow status  and it
     * is loaded as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_ClearOverflow_Initializer;

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref TimestampingArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref TimestampingArgs_t.handlerSTM switchOffFunction.
     * @param startFunction function to be set in @ref TimestampingArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref TimestampingArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref TimestampingArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref TimestampingArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_Enable_Writer Function to enable/disable the generation of timestamps. To be set in @ref TimestampingArgs_t.PV_Enable_Writer.
     * @param _PV_Edge_Writer Function to write the type of Edge to generate timestamps. To be set in @ref TimestampingArgs_t.PV_Edge_Writer.
     * @param _PV_ClearOverflow_Writer Function to clear the overflow error. To be set in @ref TimestampingArgs_t.PV_ClearOverflow_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    TimestampingArgs_t(stateChange_t switchOnFunction,
                       stateChange_t switchOffFunction,
                       stateChange_t startFunction,
                       stateChange_t stopFunction,
                       stateChange_t recoverFunction,
                       allowChange_t allowStateChangeFunction,
                       writerInt32_t _PV_Enable_Writer,
                       writerInt32_t _PV_Edge_Writer,
                       writerInt32_t _PV_ClearOverflow_Writer,
                       autoEnable_t autoEnable=autoEnable_t::none) :
               handlerSTM (true, ///Asynchronous state transitions.
                       switchOnFunction,
                       switchOffFunction,
                       startFunction,
                       stopFunction,
                       recoverFunction,
                       allowStateChangeFunction,
                       autoEnable),
               PV_Enable_Writer(_PV_Enable_Writer),
               PV_Edge_Writer(_PV_Edge_Writer),
               PV_ClearOverflow_Writer(_PV_ClearOverflow_Writer) {}

};


template <typename T>
class NDS3_API Timestamping: public Node  {
  public:
    // ---------------- Constructors and assignment operator -------------------//
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
    Timestamping();

    /**
     * @brief Copy constructor: copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
    Timestamping(const Timestamping<T>& right);


    /**
     * @brief overloading of assignment operator
     * */
    Timestamping& operator=(const Timestamping<T>& right);

    /**
     *
     * @brief Constructs the Time stamping node
     *
     * @param name node name
     * @param maxElements used in this node
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
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    Timestamping(const std::string& name,
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
                 autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the Timestamping node.
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerTMS Structure with the arguments required by the Timestamping node.
     * See @ref TimestampingArgs_t for further details.
     */
    Timestamping(const std::string& name, const TimestampingArgs_t& handlerTMS);

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
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

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
     *
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    // ---------------------------- Getters ---------------------------------- //
    /**
     * @brief Retrieves timestamping status
     *
     * @return timestamping status
     */
    std::int32_t getEnable();

    /**
     * @brief Sets detection Edge: RISING(0), FALLING(1), ANY(2)
     *
     * @return detection edge
     */
    std::int32_t getEdge();

    /**
     * @brief Retrieves max number of timestamps
     *
     * @return max number of timestamps
     */
    std::int32_t getMaxTimestamps();

    /**
     * @brief Retrieves overflow error status
     *
     * @return overflow error status
     */
    std::int32_t getOverflow();

    /**
     * @brief Retrieves the decimation value set by the Control System
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
    * @brief Sets detection Edge
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

  };
}

#endif // NDSTIMESTAMPING_H

