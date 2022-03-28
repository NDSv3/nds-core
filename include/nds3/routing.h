/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSROUTING_H
#define NDSROUTING_H

/**
 * @file routing.h
 * @brief Defines the nds::routing node, which provides basic services for routing
 * clocks and synchronization signals
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
 * @brief Type defined to gather the arguments required by @ref Routing constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct RoutingArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to set parameters related with clock routing.
     * This function applies the selected configuration to route the CLK. @n
     * It may take into account the following member methods to obtain the
     * last parameters that have been configured:
     * <ul>
     *  <li>@ref Routing.getClkSrc()</li>
     *  <li>@ref Routing.getClkDst()</li>
     * </ul>
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_ClkSet_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_ClkSet_Writer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_ClkSet_Initializer;

    /**
     * @brief Function to set the %terminal to read its clock destination connection.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_ClkDstRead_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_ClkDstRead_Writer.
     * This is an optional field of the structure
     */
    readerInt32_t PV_ClkDstRead_Initializer;

    /**
     * @brief Function to set parameters related with the %terminal routing configuration.
     * This function applies the selected routing configuration to the %terminal. @n
     * It may take into account the following member methods to obtain the
     * last parameters that have been configured:
     * <ul>
     *  <li>@ref Routing.getTermSrc()</li>
     *  <li>@ref Routing.getTermDst()</li>
     *  <li>@ref Routing.getTermSyncSet()</li>
     *  <li>@ref Routing.getTermInvertSet()</li>
     * </ul>
     */
    const writerInt32_t PV_TermSet_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_TermSet_Initializer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_TermSet_Initializer;

    /**
     * @brief Function to set the %terminal to read its destination routing parameters.
     * This function sets the %terminal to read the routing parameters. @n
     * It may call the following member methods to update the last parameters
     * that are configured for the given %terminal.
     * <ul>
     *  <li>@ref Routing.setTermSrcRead()</li>
     *  <li>@ref Routing.setTermSyncRead()</li>
     *  <li>@ref Routing.setTermInvertRead()</li>
     * </ul>
     */
    const writerInt32_t PV_TermDstRead_Writer;

    /**
     * @brief Function to load the initial value of the @ref PV_TermDstRead_Initializer.
     * This is an optional field of the structure.
     */
    readerInt32_t PV_TermDstRead_Initializer;

    /*
     * -----------------------------------------------------------------------------------------
     * List of default values that are used to initialize the PVVariableOutput member
     * variables. They can be edited by the user.
     * -----------------------------------------------------------------------------------------
     */

    // Set clock routing initial values
    /**
     * @brief Initial value for the clock source.
     */
    std::int32_t m_ClkSrc_Init;
    /**
     * @brief Initial value for the clock destination.
     */
    std::int32_t m_ClkDst_Init;

    // Set routing %terminals initial values
    /**
     * @brief Initial value for the %terminal source.
     */
    std::int32_t m_TermSrc_Init;
    /**
     * @brief Initial value for the destination %terminal.
     */
    std::int32_t m_TermDst_Init;
    /**
     * @brief Initial value for the %terminal synchronization mode.
     */
    std::int32_t m_TermSyncSet_Init;
    /**
     * @brief Initial value for the %terminal inversion status.
     */
    std::int32_t m_TermInvertSet_Init;
    /*
     * -----------------------------------------------------------------------------------------
     * End of default values
     * -----------------------------------------------------------------------------------------
     */

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref RoutingArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref RoutingArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref RoutingArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref RoutingArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref RoutingArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref RoutingArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_ClkSet_Writer Function to apply the CLK routing configuration. To be set in @ref RoutingArgs_t.PV_ClkSet_Writer.
     * @param _PV_ClkDstRead_Writer Function to set a %terminal and read its clock destination. To be set in @ref RoutingArgs_t.PV_ClkDstRead_Writer.
     * @param _PV_TermSet_Writer Function to set the routing configuration in a %terminal. To be set in @ref RoutingArgs_t.PV_TermSet_Writer.
     * @param _PV_TermDstRead_Writer Function to set a %terminal and read its clock %terminal configuration. To be set in @ref RoutingArgs_t.PV_TermDstRead_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    RoutingArgs_t(stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerInt32_t _PV_ClkSet_Writer,
                    writerInt32_t _PV_ClkDstRead_Writer,
                    writerInt32_t _PV_TermSet_Writer,
                    writerInt32_t _PV_TermDstRead_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none) :
            handlerSTM( true, ///Asynchronous state transitions.
                                    switchOnFunction,
                                    switchOffFunction,
                                    startFunction,
                                    stopFunction,
                                    recoverFunction,
                                    allowStateChangeFunction,
                                    autoEnable),
            PV_ClkSet_Writer(_PV_ClkSet_Writer),
            PV_ClkDstRead_Writer(_PV_ClkDstRead_Writer),
            PV_TermSet_Writer(_PV_TermSet_Writer),
            PV_TermDstRead_Writer(_PV_TermDstRead_Writer),
            m_ClkSrc_Init(0),
            m_ClkDst_Init(0),
            m_TermSrc_Init(0),
            m_TermDst_Init(0),
            m_TermSyncSet_Init(0),
            m_TermInvertSet_Init(0) {}
};

/**
 *
 * This is a node that supplies routing PV and few control
 * PV that specifies how the connection should be performed.
 *
 * It also provides a state machine that allows to start/stop the node.
 *
 * The user of a routing class must declare few delegate functions that
 *  specify the actions to perform when connecting sources and destinations.
 *
 */

template <typename T>
class NDS3_API Routing: public Node
{
public:
    /**
     * @brief Initializes an empty routing node.
     *
     * You must assign a valid routing node before calling initialize().
     */
    Routing();

    /**
     * @brief Copies a routing reference from another object.
     *
     * @param right a routing holder from which the reference to
     *        the routing object implementation is copied
     */
    Routing(const Routing<T>& right);

    Routing& operator=(const Routing<T>& right);

    /**
     * @brief Constructs the routing node.
     * @param name Name (unique) to identify the instance of the node.
     * @param switchOnFunction Switch-on function to be set in @ref RoutingArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref RoutingArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref RoutingArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref RoutingArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref RoutingArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref RoutingArgs_t.handlerSTM allowStateChangeFunction.
     * @param PV_ClkSet_Writer Function to apply the CLK routing configuration. To be set in @ref RoutingArgs_t.PV_ClkSet_Writer.
     * @param PV_ClkDstRead_Writer Function to set a %terminal and read its clock destination. To be set in @ref RoutingArgs_t.PV_ClkDstRead_Writer.
     * @param PV_TermSet_Writer Function to set the routing configuration in a %terminal. To be set in @ref RoutingArgs_t.PV_TermSet_Writer.
     * @param PV_TermDstRead_Writer Function to set a %terminal and read its clock %terminal configuration. To be set in @ref RoutingArgs_t.PV_TermDstRead_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */

    Routing(const std::string& name,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerInt32_t PV_ClkSet_Writer,
                    writerInt32_t PV_ClkDstRead_Writer,
                    writerInt32_t PV_TermSet_Writer,
                    writerInt32_t PV_TermDstRead_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none);


    /**
     * @brief Simplified constructor of the Routing node.
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerRTN Structure with the arguments required by the Routing node.
     * See @ref RoutingArgs_t for further details.
     */
    Routing(const std::string& name, const RoutingArgs_t& handlerRTN);

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    // TODO Is it necessary this delegate function?
    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the node starts.
     *
     * @param timestampDelegate function that returns the exact starting time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at start
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the routing node started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Retrieve the clock source
     *
     * @return clock source value
     */
    size_t getClkSrc();
    /**
     * @brief Retrieve the clock destination
     *
     * @return clock destination value
     */
    size_t getClkDst();
    /**
     * @brief Retrieve the %terminal source
     *
     * @return %terminal source value
     */
    size_t getTermSrc();
    /**
     * @brief Retrieve the %terminal destination
     *
     * @return %terminal destination value
     */
    size_t getTermDst();
    /**
     * @brief Retrieve the %terminal sync mode
     * @return %terminal sync mode value
     */
    size_t getTermSyncSet();
    /**
     * @brief Retrieve the %terminal invert mode
     *
     * @return %terminal invert mode value
     */
    size_t getTermInvertSet();


    /**
     * @brief Sets the value of the m_ClkSetStatus_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the clock setting
     */
    void setClkSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_ClkSetCode_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error clock setting
     */
    void setClkSetCode(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Update the value of the m_ClkSrcRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value clock source to read
     */
    void setClkSrcRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSetStatus_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the %terminal setting
     */
    void setTermSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_TermSetCode_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error %terminal setting
     */
    void setTermSetCode(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSrcRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value %terminal source to be read
     */
    void setTermSrcRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSyncRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value %terminal sync
     */
    void setTermSyncRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermInvertRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value %terminal Invert
     */
    void setTermInvertRead(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);
};

}
#endif // NDSROUTING_H

