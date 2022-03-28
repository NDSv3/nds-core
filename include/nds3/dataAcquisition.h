/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSDATAACQUISITION_H
#define NDSDATAACQUISITION_H

/**
 * @file dataAcquisition.h
 * @brief Defines the nds::DataAcquisition node, which provides basic services for data
 *        acquisition.
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
 * @brief Type defined to gather the arguments required by @ref DataAcquisition constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct DataAcquisitionArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to write the gain for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Gain_Writer;

    /**
     * @brief Function to load the initial value of gain.
     * It reads the gain configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Gain_Initializer;

    /**
     * @brief Function to write the offset for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Offset_Writer;

    /**
     * @brief Function to load the initial value of offset.
     * It reads the offset configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Offset_Initializer;

    /**
     * @brief Function to write the bandwidth for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Bandwidth_Writer;

    /**
     * @brief Function to load the initial value of bandwidth.
     * It reads the bandwidth configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Bandwidth_Initializer;

    /**
     * @brief Function to write the resolution for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Resolution_Writer;

    /**
     * @brief Function to load the initial value of resolution.
     * It reads the resolution configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Resolution_Initializer;

    /**
     * @brief Function to write the impedance for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Impedance_Writer;

    /**
     * @brief Function to load the initial value of impedance.
     * It reads the impedance configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Impedance_Initializer;

    /**
     * @brief Function to write the coupling for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Coupling_Writer;

    /**
     * @brief Function to load the initial value of coupling.
     * It reads the coupling configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Coupling_Initializer;

    /**
     * @brief Function to write the type of signal reference for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SignalRefType_Writer;

    /**
     * @brief Function to load the initial type of signal reference .
     * It reads the type of signal reference configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SignalRefType_Initializer;

    /**
     * @brief Function to write the ground for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Ground_Writer;

    /**
     * @brief Function to load the initial ground.
     * It reads the ground configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Ground_Initializer;

    /**
     * @brief Function to write (enable/disable) the DMA for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_DMAEnable_Writer;

    /**
     * @brief Function to load the initial value of the flag for enabling DMA.
     * It reads the status of the flag to enable DMA in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_DMAEnable_Initializer;

    /**
     * @brief Function to write the sampling rate for data acquisition.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_SamplingRate_Writer;

    /**
     * @brief Function to load the initial value of the sampling rate.
     * It reads the sampling rate configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_SamplingRate_Initializer;

    /*
     * -----------------------------------------------------------------------------------------
     * List of default values that are used to initialize the PVVariableOutput member
     * variables. They can be edited by the user.
     * -----------------------------------------------------------------------------------------
     */

    /**
     * @brief Initial value of the decimation factor for data acquisition.
     */
    std::int32_t m_Decimation_Init;

    /**
     * @brief Initial value of the type of decimation for data acquisition.
     */
    std::int32_t m_DecimationType_Init;

    /*
     * -----------------------------------------------------------------------------------------
     * End of default values
     * -----------------------------------------------------------------------------------------
     */

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction Switch-on function to be set in @ref DataAcquisitionArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref DataAcquisitionArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref DataAcquisitionArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref DataAcquisitionArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref DataAcquisitionArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref DataAcquisitionArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_Gain_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Gain_Writer.
     * @param _PV_Offset_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Offset_Writer.
     * @param _PV_Bandwidth_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Bandwidth_Writer.
     * @param _PV_Resolution_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Resolution_Writer.
     * @param _PV_Impedance_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Impedance_Writer.
     * @param _PV_Coupling_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Coupling_Writer
     * @param _PV_SignalRefType_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_SignalRefType_Writer.
     * @param _PV_Ground_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_Ground_Writer.
     * @param _PV_DMAEnable_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_DMAEnable_Writer
     * @param _PV_SamplingRate_Writer Function to write the gain for data acquisition. To be set in @ref DataAcquisitionArgs_t.PV_SamplingRate_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    DataAcquisitionArgs_t(stateChange_t switchOnFunction,
                          stateChange_t switchOffFunction,
                          stateChange_t startFunction,
                          stateChange_t stopFunction,
                          stateChange_t recoverFunction,
                          allowChange_t allowStateChangeFunction,
                          writerDouble_t _PV_Gain_Writer,
                          writerDouble_t _PV_Offset_Writer,
                          writerDouble_t _PV_Bandwidth_Writer,
                          writerDouble_t _PV_Resolution_Writer,
                          writerDouble_t _PV_Impedance_Writer,
                          writerInt32_t _PV_Coupling_Writer,
                          writerInt32_t _PV_SignalRefType_Writer,
                          writerInt32_t _PV_Ground_Writer,
                          writerInt32_t _PV_DMAEnable_Writer,
                          writerDouble_t _PV_SamplingRate_Writer,
                          autoEnable_t autoEnable=autoEnable_t::none):
                   handlerSTM (true, ///Asynchronous state transitions.
                           switchOnFunction,
                           switchOffFunction,
                           startFunction,
                           stopFunction,
                           recoverFunction,
                           allowStateChangeFunction,
                           autoEnable),
                   PV_Gain_Writer(_PV_Gain_Writer),
                   PV_Offset_Writer(_PV_Offset_Writer),
                   PV_Bandwidth_Writer(_PV_Bandwidth_Writer),
                   PV_Resolution_Writer(_PV_Resolution_Writer),
                   PV_Impedance_Writer(_PV_Impedance_Writer),
                   PV_Coupling_Writer(_PV_Coupling_Writer),
                   PV_SignalRefType_Writer(_PV_SignalRefType_Writer),
                   PV_Ground_Writer(_PV_Ground_Writer),
                   PV_DMAEnable_Writer(_PV_DMAEnable_Writer),
                   PV_SamplingRate_Writer(_PV_SamplingRate_Writer),
                   m_Decimation_Init(1),
                   m_DecimationType_Init(0){}
};

/**
 * This is a node that supplies a data acquisition PV and few control
 * PVs that specifies how the acquisition should be performed.
 *
 * It also provides a state machine that allows to start/stop the acquisition.
 *
 * The user of a DataAcquisition class must declare few delegate functions that
 *  specify the actions to perform when the acquisition node's state changes.
 *
 * In particular, the transition from the state off to running should launch
 *  the data acquisition thread which pushes the acquired data via push(),
 *  while the transition from running to on should stop the data acquisition thread.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::int64_t
 *            - float
 *            - double
 *            - std::vector<std::uint8_t>
 *            - std::vector<std::int8_t>
 *            - std::vector<std::int16_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<std::int64_t>
 *            - std::vector<float>
 *            - std::vector<double>
 *
 */
template <typename T>
class NDS3_API DataAcquisition: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid DataAcquisition node before calling initialize().
     */
    DataAcquisition();

    /**
     * @brief Copies a data acquisition reference from another object.
     *
     * @param right data acquisition holder from which the reference to
     *        the acquisition object implementation is copied
     */
    DataAcquisition(const DataAcquisition<T>& right);

    DataAcquisition& operator=(const DataAcquisition<T>& right);

    /**
     * @brief Constructs the data acquisition node.
     *
     */
    /**
     *
     * @param name
     * @param maxElements
     * @param switchOnFunction
     * @param switchOffFunction
     * @param startFunction
     * @param stopFunction
     * @param recoverFunction
     * @param allowStateChangeFunction
     * @param PV_Gain_Writer
     * @param PV_Offset_Writer
     * @param PV_Bandwidth_Writer
     * @param PV_Resolution_Writer
     * @param PV_Impedance_Writer
     * @param PV_Coupling_Writer
     * @param PV_SignalRefType_Writer
     * @param PV_Ground_Writer
     * @param PV_DMAEnable_Writer
     * @param PV_SamplingRate_Writer
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    DataAcquisition(const std::string& name,                ///< The node's name
                    size_t maxElements,                     ///< Maximum size of the acquired array. Set to 1 for scalar values
                    stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on
                    stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off
                    stateChange_t startFunction,            ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
                    stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
                    stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state
                    allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true
                    writerDouble_t PV_Gain_Writer,          ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerDouble_t PV_Offset_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerDouble_t PV_Bandwidth_Writer,     ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerDouble_t PV_Resolution_Writer,    ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerDouble_t PV_Impedance_Writer,     ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerInt32_t PV_Coupling_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerInt32_t PV_SignalRefType_Writer,  ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerInt32_t PV_Ground_Writer,         ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerInt32_t PV_DMAEnable_Writer,      ///< Delegate function setter/getter to interact to the Low Level Driver API
                    writerDouble_t PV_SamplingRate_Writer,   ///< Delegate function to interact to the low level driver API
                    autoEnable_t autoEnable=autoEnable_t::none ///< Parameter value
    );

    /**
     * @brief Simplified constructor of the DataAcquisition node.
     * @param name Name (unique) to identify the instance of the node.
     * @param maxElements Maximum size of the acquired array. Set to @c 1 for scalar values.
     * @param handlerDAQ Structure with the arguments required by the DataAcquisition node.
     * See @ref DataAcquisitionArgs_t for further details.
     */
    DataAcquisition(const std::string& name,
                    size_t maxElements,
                    const DataAcquisitionArgs_t& handlerDAQ);

    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the data acquisition starts.
     *
     * @param timestampDelegate function that returns the exact starting time of the
     *                           data acquisition
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    /**
     * @ingroup datareadwrite
     * @brief Push acquired data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data acquisition thread in order to push the acquired data.
     *
     * @param timestamp timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Retrieve the Gain
     *
     * @return the Gain value
     */
    double getGain();

    /**
     * @brief Retrieve the Offset
     *
     * @return the Offset value
     */
    double getOffset();

    /**
     * @brief Retrieve the Bandwidth
     *
     * @return the Bandwidth value
     */
    double getBandwidth();

    /**
     * @brief Retrieve the Resolution
     *
     * @return the Resolution value
     */
    double getResolution();

    /**
     * @brief Retrieve the Impedance
     *
     * @return the Impedance value
     */
    double getImpedance();

    /**
     * @brief Retrieve the Coupling
     *
     * @return the Coupling value
     */
    int32_t getCoupling();

    /**
     * @brief Retrieve the SignalRef
     *
     * @return the SignalRef value
     */
    int32_t getSignalRefType();

    /**
     * @brief Retrieve the Ground
     *
     * @return the Ground value
     */
    int32_t getGround();
    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the DataAcquisition constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();
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
     * @brief Retrieve the Number Of Pushed Data Blocks by the acquisition thread to the control system
     *
     * @return the Number Of Pushed Data Blocks
     */
    int32_t getNumberOfPushedDataBlocks();
    /**
     * @brief Retrieve the DMA Buffer size value
     *
     * @return the m_DMABufferSize_PV value
     */
    int32_t getDMABufferSize();
    /**
     * @brief Retrieve the DMAEnable status
     *
     * @return the m_DMAEnable_PV value
     */
    int32_t getDMAEnable();
     /**
     * @brief Retrieve the Number of DMA channels used by the DAQ node
     *
     * @return the m_DMANumChannels_PV value
     */
    int32_t getDMANumChannels();
    /**
    * @brief Retrieve the DMA Frame Type
    *
    * @return the m_DMAFrameType_PV value
    */
    int32_t getDMAFrameType();
    /**
    * @brief Retrieve the DMA sample size
    *
    * @return the m_DMASampleSize_PV value
    */
    int32_t getDMASampleSize();
    /**
    * @brief Retrieve the DMA sampling rate
    *
    * @return the m_DMASamplingRate_PV value
    */
    double getSamplingRate();

    /**
     * @brief Sets the value of the m_Gain_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Gain value in data acquisition
     */
    void setGain(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Offset_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Offset value in data acquisition
     */
    void setOffset(const timespec& timestamp, const double& value);

    /**
     * @brief Sets the value of the m_Bandwidth_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Bandwidth value in data acquisition
     */
    void setBandwidth(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Resolution_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Resolution value in data acquisition
     */
    void setResolution(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Impedance_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Impedance value in data acquisition
     */
    void setImpedance(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Coupling_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Coupling value in data acquisition
     */
    void setCoupling(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_SignalRef_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value SignalRefType value in data acquisition
     */
    void setSignalRefType(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_Ground_RBV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Ground value in data acquisition
     */
    void setGround(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_NumberOfPushedDataBocks and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Number of pushed data blocks during the data acquisition
     */
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMABufferSize_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value DMA buffer size value in data acquisition
     */
    void setDMABufferSize(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMAEnable_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value DMA enable/disable in data acquisition
     */
    void setDMAEnable(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMANumChannels_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value number of DMA channels
     */
    void setDMANumChannels(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMAFrameType_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value DMA frame type
     */
    void setDMAFrameType(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMASampleSize_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value DMA sample size
     */
    void setDMASampleSize(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMASamplingRate_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Sampling rate value during data acquisition
     */
    void setSamplingRate(const timespec& timestamp, const double& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);
};

}
#endif // NDSDATAACQUISITION_H

