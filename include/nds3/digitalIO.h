/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSDIGITALIO_H
#define NDSDIGITALIO_H

/**
 * @file digitalIO.h
 * @brief Defines the nds::DigitalIO node, which provides basic services for Digital
 *        Input/Output.
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
 * @brief Type defined to gather the arguments required by @ref DigitalIO constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 * @tparam T  the PV data type.
 *      The following data types are supported:
 *      - std::vector<bool>
 *      - std::vector<std::int8_t>
 *      - std::vector<std::int16_t>
 *      - std::vector<std::int32_t>
 *      - std::vector<std::int64_t>
 */
template<typename T>
struct DigitalIOArgs_t{

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to write the mask for the output data.
     * This is a compulsory field of the structure
     */
    const writerVectorBool_t PV_dataOutMask_Writer;

    /**
     * @brief Function to load the initial mask for the output data.
     * It reads the mask configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerVectorBool_t PV_dataOutMask_Initializer;

    /**
     * @brief Function to write the voltage for high level.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_voltLevelHigh_Writer;

    /**
     * @brief Function to load the initial voltage for high level.
     * It reads the voltage configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_voltLevelHigh_Initializer;

    /**
     * @brief Function to write the voltage for low level.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_voltLevelLow_Writer;

    /**
     * @brief Function to load the initial voltage for low level.
     * It reads the voltage configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_voltLevelLow_Initializer;

    /**
     * @brief Function to write the direction of the channels.
     * This is a compulsory field of the structure.
     */
    const writerVectorBool_t PV_ChannelDir_Writer;

    /**
     * @brief Function to load the initial direction of the channels.
     * It reads the directions configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerVectorBool_t PV_ChannelDir_Initializer;

    /*
     * -----------------------------------------------------------------------------------------
     * List of default values that are used to initialize the PVVariableOutput member
     * variables. They can be edited by the user.
     * -----------------------------------------------------------------------------------------
     */
    /**
     * @brief Initial value of the output data.
     */
    T m_DataOut_Init;

    /**
     * @brief Initial value of the decimation factor.
     */
    std::int32_t m_Decimation_Init;
    /*
     * -----------------------------------------------------------------------------------------
     * End of default values
     * -----------------------------------------------------------------------------------------
     */

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref DigitalIOArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref DigitalIOArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref DigitalIOArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref DigitalIOArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref DigitalIOArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref DigitalIOArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_dataOutMask_Writer Function to write the mask for output data. To be set in @ref DigitalIOArgs_t.PV_dataOutMask_Writer.
     * @param _PV_voltLevelHigh_Writer Function to write the voltage for high level. To be set in @ref DigitalIOArgs_t.PV_voltLevelHigh_Writer.
     * @param _PV_voltLevelLow_Writer Function to write the voltage for low level. To be set in @ref DigitalIOArgs_t.PV_voltLevelLow_Writer.
     * @param _PV_ChannelDir_Writer Function to write the channels directions. To be set in @ref DigitalIOArgs_t.PV_ChannelDir_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    DigitalIOArgs_t(stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerVectorBool_t _PV_dataOutMask_Writer,
                    writerDouble_t _PV_voltLevelHigh_Writer,
                    writerDouble_t _PV_voltLevelLow_Writer,
                    writerVectorBool_t _PV_ChannelDir_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none) :
           handlerSTM (true, ///Asynchronous state transitions.
                   switchOnFunction,
                   switchOffFunction,
                   startFunction,
                   stopFunction,
                   recoverFunction,
                   allowStateChangeFunction,
                   autoEnable),
           PV_dataOutMask_Writer(_PV_dataOutMask_Writer),
           PV_voltLevelHigh_Writer(_PV_voltLevelHigh_Writer),
           PV_voltLevelLow_Writer(_PV_voltLevelLow_Writer),
           PV_ChannelDir_Writer(_PV_ChannelDir_Writer),
           m_DataOut_Init(T()), //Default value of the underlying data type
           m_Decimation_Init(1) {}
};
template struct DigitalIOArgs_t<std::vector<bool>>;
template struct DigitalIOArgs_t<std::vector<std::int8_t>>;
template struct DigitalIOArgs_t<std::vector<std::int16_t>>;
template struct DigitalIOArgs_t<std::vector<std::int32_t>>;
template struct DigitalIOArgs_t<std::vector<std::int64_t>>;

/**
 * This is a node that supplies PVs that specifies how the IO acquisition
 * generation should be performed.
 *
 * It also provides a state machine for these kind of channels
 *
 * The user of a DigitalIO class must declare few delegate functions that
 *  specify the actions to perform when the node's state changes.
 *
 * In particular, the transition from the state off to running should launch
 *  the DigitalIO thread which pushes the data via push(), while the transition
 *  from running to on should stop the DigitalIO thread.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::int64_t
 *            - std::double
 *            - std::vector<bool>
 *            - std::vector<std::int8_t>
 *            - std::vector<std::int16_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<std::int64_t>
 *
 */
template <typename T>
class NDS3_API DigitalIO: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid DigitalIO node before calling initialize().
     */
    DigitalIO();

    /**
     * @brief Copies a data reference from another object.
     *
     * @param right a digitalIO holder from which the reference to
     *        the digitalIO object implementation is copied
     */
    DigitalIO(const DigitalIO<T>& right);

    DigitalIO& operator=(const DigitalIO<T>& right);

    /**
     * @brief Constructor to create a DigitalIO node
     *
     * @param name Name (unique) to identify the instance of the node.
     * @param maxElements Maximum size of the acquired array. Set to 1 for scalar values
     * @param switchOnFunction Switch-on function.
     * @param switchOffFunction Switch-off function.
     * @param startFunction Start function.
     * @param stopFunction Stop function.
     * @param recoverFunction Recover function.
     * @param allowStateChangeFunction Checking state transitions function.
     * @param PV_dataOutMask_Writer Delegate function setter/getter to interact to the Low Level Driver API
     * @param PV_voltLevelHigh_Writer Delegate function setter/getter to interact to the Low Level Driver API
     * @param PV_voltLevelLow_Writer Delegate function setter/getter to interact to the Low Level Driver API
     * @param PV_ChannelDir_Writer Delegate function setter/getter to interact to the Low Level Driver API
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    DigitalIO( const std::string& name,
               size_t maxElements,
               stateChange_t switchOnFunction,
               stateChange_t switchOffFunction,
               stateChange_t startFunction,
               stateChange_t stopFunction,
               stateChange_t recoverFunction,
               allowChange_t allowStateChangeFunction,
               writerVectorBool_t PV_dataOutMask_Writer,
               writerDouble_t PV_voltLevelHigh_Writer,
               writerDouble_t PV_voltLevelLow_Writer,
               writerVectorBool_t PV_ChannelDir_Writer,
               autoEnable_t autoEnable = autoEnable_t::none);

    /**
     * @brief Simplified constructor of the DigitalIO node.
     * @param name Name (unique) to identify the instance of the node.
     * @param maxElements Maximum size of the data to be handled.
     * @param handlerDIO Structure with the arguments required by the DigitalIO node.
     * See @ref DigitalIOArgs_t for further details.
     */
    DigitalIO( const std::string& name,
               size_t maxElements,
               const DigitalIOArgs_t<T>& handlerDIO);

    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the digitalIO node starts.
     *
     * @param timestampDelegate the function that returns the exact starting time of the
     *                           digitalIO node
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    /**
     * @brief Push the data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  DigitalIO thread in order to push the data.
     *
     * @param timestamp timestamp for the data
     * @param data      data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the DigitalIO constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();
    /**
     * @brief Retrieve the dataOutMask
     *
     * @return the dataOutMask value
     */
    std::vector<bool> getDataOutMask();
    /**
     * @brief Retrieve the voltLevelHigh
     *
     * @return the voltLevelHigh value
     */
    double getVoltLevelHigh();
    /**
     * @brief Retrieve the voltLevelLow.
     *
     * @return the voltLevelLow value
     */
    double getVoltLevelLow();
    /**
     * @brief Retrieve the ChannelDir.
     *
     * @return the ChannelDir value
     */
    std::vector<bool> getChannelDir();
    /**
     * @brief Sets the value of the m_NumberOfPushedDataBocks.
     *
     * @param timestamp timestamp for the value
     * @param value Number of pushed data blocks during the data acquisition
     */
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_dataOutMask_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Data Out Mask value. Each position of the array should be set to true or false.
     */
    void setDataOutMask(const timespec& timestamp, const std::vector<bool>& value);
    /**
     * @brief Sets the value of the m_voltLevelHigh_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Voltage value for high voltage levels.
     */
    void setVoltLevelHigh(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_voltLevelLow_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Voltage value for low voltage levels.
     */
    void setVoltLevelLow(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_channelDir_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Channel direction. Each position of the array should be set to true or false.
     */
    void setChannelDir(const timespec& timestamp, const std::vector<bool>& value);
    /**
     * @ingroup timing
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;
    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

};

}
#endif // NDSDIGITALIO_H

