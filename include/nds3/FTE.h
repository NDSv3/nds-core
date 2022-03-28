/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFTE_H
#define NDSFTE_H

/**
 * @file FTE.h
 * @brief Defines the nds::FTE node, which provides basic services for Future Time Event scheduling
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
 * @brief Type defined to gather the arguments required by @ref FTE constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct FTEArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to set parameters related with FTE generation.
     * This function applies the selected configuration to schedule FTE, pulses
     * or wake-up events to the device. @n
     * It may take into account the following member methods to obtain the
     * last parameters that have been configured:
     * <ul>
     *      <li>@ref FTE.getTerminalSet()</li>
     *      <li>@ref FTE.getModeSet()</li>
     *      <li>@ref FTE.getStartTimeSet()</li>
     *      <li>@ref FTE.getStopTimeSet()</li>
     *      <li>@ref FTE.getLevelSet()</li>
     *      <li>@ref FTE.getPeriodNsecSet()</li>
     *      <li>@ref FTE.getDutyCycleSet()</li>
     * </ul>
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Set_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_Set_Writer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Set_Initializer;

    /**
     * @brief Function to set parameters related with aborting FTE.
     * This function applies the selected configuration to abort FTE, pulses
     * or wake-up events to the device. @n
     * It may take into account the following member methods to obtain the
     * last parameters that have been configured.
     * <ul>
     *      <li>@ref FTE.getTerminalSuppress</li>
     *      <li>@ref FTE.getModeSuppress</li>
     *      <li>@ref FTE.getAllSuppress</li>
     *      <li>@ref FTE.getStartTimeSuppress</li>
     * </ul>
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Suppress_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_Suppress_Writer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Suppress_Initializer;

    /**
     * @brief Function to set the parameters related to change the period.
     * This function applies the selected configuration to change the period. @n
     * It may take into account the following member methods to obtain the
     * last parameters that have been configured.
     * <ul>
     *      <li>@ref FTE.getTerminalChgPeriod</li>
     *      <li>@ref FTE.getPeriodChgPeriod</li>
     * </ul>
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_ChgPeriod_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_ChgPeriod_Writer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_ChgPeriod_Initializer;

    /**
     * @brief Function to obtain the parameters related to the pending events.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_PendingValue_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_PendingValue_Writer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_PendingValue_Initializer;

    /*
     * -----------------------------------------------------------------------------------------
     * List of default values that are used to initialize the PVVariableOutput member
     * variables. They can be edited by the user.
     * -----------------------------------------------------------------------------------------
     */

    // Set FTE initial values
    /**
     * @brief Initial value for the index of the terminal to schedule FTE, pulses or wake-up events.
     */
    std::int32_t m_TerminalSet_Init;
    /**
     * @brief Initial value for the mode to schedule FTE, pulses or wake-up events.
     * The next table shows the typical values:
     *  <table>
     *  <tr> <td><b><i>Value</i></b></td> <td><b>Meaning</b></td>  </tr>
     *  <tr> <td><i>0</i></td> <td>Single</td> </tr>
     *  <tr> <td><i>1</i></td> <td>Pulse</td> </tr>
     *  <tr> <td><i>2</i></td> <td>CLK</td> </tr>
     *  <tr> <td><i>3</i></td> <td>InmLVL</td> </tr>
     * </table>
     */
    std::int32_t m_ModeSet_Init;
    /**
     * @brief Initial value for the start time to schedule FTE, pulses or wake-up events.
     */
    timespec m_StartTimeSet_Init;
    /**
     * @brief Initial value for the stop time to schedule FTE, pulses or wake-up events.
     */
    timespec m_StopTimeSet_Init;
    /**
     * @brief Initial value for the level to schedule FTE, pulses or wake-up events.
     */
    std::int32_t m_LevelSet_Init;
    /**
     * @brief Initial value for the period (in nanoseconds) to generate CLK signals.
     */
    std::int32_t m_PeriodNsecSet_Init;
    /**
     * @brief Initial value for the duty cycle to generate CLK signals.
     */
    std::int32_t m_DutyCycleSet_Init;

    // Set initial values to abort FTE
    /**
     * @brief Initial value for the index of the terminal to abort FTE, pulses or wake-up events.
     */
    std::int32_t m_TerminalSuppress_Init;
    /**
     * @brief Initial value for the mode to abort FTE, pulses or wake-up events.
     */
    std::int32_t m_ModeSuppress_Init;
    /**
     * @brief Initial value for the flag to abort all FTE scheduled (@c 1) or a single one (@c 0).
     */
    std::int32_t m_AllSuppress_Init;
    /**
     * @brief Initial value for the stop time to abort FTE, pulses or wake-up events.
     */
    timespec m_StartTimeSuppress_Init;

    // Set initial values to change period of FTE
    /**
     * @brief Initial value for the index of the terminal to change FTE period.
     */
    std::int32_t m_TerminalChgPeriod_Init;
    /**
     * @brief Initial value for the period (in nanoseconds) to change on FTE.
     */
    std::int32_t m_PeriodChgPeriod_Init;

    /*
     * -----------------------------------------------------------------------------------------
     * End of default values
     * -----------------------------------------------------------------------------------------
     */

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref FTEArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref FTEArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref FTEArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref FTEArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref FTEArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref FTEArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_Set_Writer Function to apply FTE configuration. To be set in @ref FTEArgs_t.PV_Set_Writer.
     * @param _PV_Suppress_Writer Function to apply suppress configuration. To be set in @ref FTEArgs_t.PV_Suppress_Writer.
     * @param _PV_ChgPeriod_Writer Function to apply the configuration for changing the period. To be set in @ref FTEArgs_t.PV_ChgPeriod_Writer.
     * @param _PV_PendingValue_Writer Function to obtain the pending values. To be set in @ref FTEArgs_t.PV_PendingValue_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    FTEArgs_t(stateChange_t switchOnFunction,
            stateChange_t switchOffFunction,
            stateChange_t startFunction,
            stateChange_t stopFunction,
            stateChange_t recoverFunction,
            allowChange_t allowStateChangeFunction,
            writerInt32_t _PV_Set_Writer,
            writerInt32_t _PV_Suppress_Writer,
            writerInt32_t _PV_ChgPeriod_Writer,
            writerInt32_t _PV_PendingValue_Writer,
            autoEnable_t autoEnable=autoEnable_t::none) :
        handlerSTM( true, ///Asynchronous state transitions.
                                switchOnFunction,
                                switchOffFunction,
                                startFunction,
                                stopFunction,
                                recoverFunction,
                                allowStateChangeFunction,
                                autoEnable),
        PV_Set_Writer(_PV_Set_Writer),
        PV_Suppress_Writer(_PV_Suppress_Writer),
        PV_ChgPeriod_Writer(_PV_ChgPeriod_Writer),
        PV_PendingValue_Writer(_PV_PendingValue_Writer),
        m_TerminalSet_Init(0),
        m_ModeSet_Init(0),
        m_StartTimeSet_Init({0,0}),
        m_StopTimeSet_Init({0,0}),
        m_LevelSet_Init(0),
        m_PeriodNsecSet_Init(0),
        m_DutyCycleSet_Init(0),
        m_TerminalSuppress_Init(0),
        m_ModeSuppress_Init(0),
        m_AllSuppress_Init(0),
        m_StartTimeSuppress_Init({0,0}),
        m_TerminalChgPeriod_Init(0),
        m_PeriodChgPeriod_Init(50)
        {}
};

/**
 * This is a node that supplies FTE scheduling tools. Set, suppress and change FTEs.
 *
 * It also provides a state machine that allows to start/stop the node.
 *
 * The user of FTE class must declare few delegate functions that specify
 * the actions to perform when the FTE node's state changes.
 *
 * In particular, the transition from the state off to on should get
 * the hardware parameters.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::string
 *
 */
template <typename T>
class NDS3_API FTE: public Node
{
public:
    /**
     * @brief Initializes an empty FTE node.
     *
     * You must assign a valid FTE node before calling initialize().
     */
    FTE();

    /**
     * @brief Copies a FTE reference from another object.
     *
     * @param right a FTE holder from which the reference to
     *        the object implementation is copied
     */
    FTE(const FTE<T>& right);

    FTE& operator=(const FTE<T>& right);

    /**
     * @brief Constructs the FTE node.
     *
     * @param name                          Name (unique) to identify the instance of the node.
     * @param switchOnFunction              Delegate function that performs the actions to switch the node on
     * @param switchOffFunction             Delegate function that performs the actions to switch the node off
     * @param startFunction                 Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
     * @param stopFunction                  Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
     * @param recoverFunction               Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction      Delegate function that can deny a state change. Usually just returns true
     * @param PV_Set_Writer                 Delegate function setter/getter to interact to the Low Level Driver API
     * @param PV_Suppress_Writer            Delegate function setter/getter to interact to the Low Level Driver API
     * @param PV_ChgPeriod_Writer           Delegate function setter/getter to interact to the Low Level Driver API
     * @param PV_PendingValue_Writer        Delegate function setter/getter to interact to the Low Level Driver API
     * @param autoEnable                    See @ref autoEnable_t for further details
     */
    FTE(const std::string& name,
            stateChange_t switchOnFunction,
            stateChange_t switchOffFunction,
            stateChange_t startFunction,
            stateChange_t stopFunction,
            stateChange_t recoverFunction,
            allowChange_t allowStateChangeFunction,
            writerInt32_t PV_Set_Writer,
            writerInt32_t PV_Suppress_Writer,
            writerInt32_t PV_ChgPeriod_Writer,
            writerInt32_t PV_PendingValue_Writer,
            autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the FTE node.
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerFTE Structure with the arguments required by the FTE node.
     * See @ref FTEArgs_t for further details.
     */
    FTE(const std::string&name, const FTEArgs_t& handlerFTE);


    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate function that returns the exact starting time
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Set functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal
     *
     * @return TerminalSet value
     */
    std::int32_t getTerminalSet();
    /**
     * @brief Retrieve the Mode (Single, Pulse, Clk, InmLVL)
     *
     * @return ModeSet value
     */
    std::int32_t getModeSet();
    /**
     * @brief Retrieve the StartTime
     *
     * @return SignalRefSet value
     */
    timespec getStartTimeSet();
    /**
     * @brief Retrieve the StopTime
     *
     * @return StopTimeSet value
     */
    timespec getStopTimeSet();
    /**
     * @brief Retrieve the Level
     *
     * @return LevelSet value
     */
    std::int32_t getLevelSet();
    /**
     * @brief Retrieve the Period in Nanoseconds
     *
     * @return PeriodNsecSet value
     */
    std::int32_t getPeriodNsecSet();
    /**
     * @brief Retrieve the SignalRef
     *
     * @return DutyCycleSet value
     */
    std::int32_t getDutyCycleSet();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Suppress functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal Suppress
     *
     * @return TerminalSuppress value
     */
    std::int32_t getTerminalSuppress();
    /**
     * @brief Retrieve the Mode Suppress (FTE/Clock)
     *
     * @return ModeSuppress value
     */
    std::int32_t getModeSuppress();
    /**
     * @brief Retrieve the AllSuppress signal (Suppess All/Suppress One)
     *
     * @return AllSuppress value
     */
    std::int32_t getAllSuppress();
    /**
     * @brief Retrieve the Start Time of FTE to be suppressed
     *
     * @return StartTimeSuppress value
     */
    timespec getStartTimeSuppress();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Change Period functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal to change the period
     *
     * @return TerminalChgPeriod value
     */
    std::int32_t getTerminalChgPeriod();
    /**
     * @brief Retrieve the Period to change the period
     *
     * @return PeriodChgPeriod value
     */
    std::int32_t getPeriodChgPeriod();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Set functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_SetStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
    void setSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_SetCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error setting
     */
    void setSetCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Suppress functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_SuppressStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
    void setSuppressStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_SuppressCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error suppressing
     */
    void setSuppressCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Change Period functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_ChgPeriodStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
    void setChgPeriodStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_ChgPeriodCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error changing clock period
     */
    void setChgPeriodCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setter of Pending functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_PendingValue_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value number of pending FTEs
     */
    void setPendingValue(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_PendingStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
    void setPendingStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_PendingCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error retrieving pending FTEs
     */
    void setPendingCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setter of Maximum functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_Maximum_RBVPV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Maximum FTEs that can be scheduled. Size of the FTE FIFO.
     */
    void setMaximum(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

};

}
#endif // NDSFTE_H

