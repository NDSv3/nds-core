/*
 * Nominal Device Support v.3 (NDS3)
 *
 * By GMV & UPM
 */

#ifndef NDSWAVEFORMGENERATION_H
#define NDSWAVEFORMGENERATION_H

/**
 * @file waveformGeneration.h
 * @brief Defines the nds::WaveformGeneration node, which provides basic services for
 *  AWG support.
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
 * @brief Type defined to gather the arguments required by @ref WaveformGeneration constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 * @tparam T  the PV data type.
 *      The following data types are supported:
 *      - std::int32_t
 *      - std::int64_t
 *      - float
 *      - double
 *      - std::vector<std::uint8_t>
 *      - std::vector<std::int8_t>
 *      - std::vector<std::int16_t>
 *      - std::vector<std::int32_t>
 *      - std::vector<std::int64_t>
 *      - std::vector<float>
 *      - std::vector<double>
 */
template<typename T>
struct WaveformGenerationArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to write the frequency for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Frequency_Writer;

    /**
     * @brief Function to load the initial value of frequency.
     * It reads the frequency configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Frequency_Initializer;

    /**
     * @brief Function to write the reference frequency for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_RefFrequency_Writer;

    /**
     * @brief Function to load the initial value of the reference frequency.
     * It reads the reference frequency configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_RefFrequency_Initializer;

    /**
     * @brief Function to write the amplitude for waveform generation.
     * This is a compulsory field of the structure
     */
    const writerDouble_t PV_Amp_Writer;

    /**
     * @brief Function to load the initial value of the amplitude.
     * It reads the amplitude configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Amp_Initializer;

    /**
     * @brief Function to write the phase for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Phase_Writer;

    /**
     * @brief Function to load the initial value of the phase.
     * It reads the phase configured in the device and it is loaded as
     * initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Phase_Initializer;

    /**
     * @brief Function to write the update rate for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_UpdateRate_Writer;

    /**
     * @brief Function to load the initial value of the update rate.
     * It reads the update rate configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_UpdateRate_Initializer;

    /**
     * @brief Function to write the duty cycle for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_DutyCycle_Writer;

    /**
     * @brief Function to load the initial value of the duty cycle.
     * It reads the duty cycle configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_DutyCycle_Initializer;

    /**
     * @brief Function to write the gain for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerDouble_t PV_Gain_Writer;

    /**
     * @brief Function to load the initial value of the gain.
     * It reads the gain configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerDouble_t PV_Gain_Initializer;

    /**
     * @brief Function to write the offset for waveform generation.
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
     * @brief Function to write the impedance for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_Impedance_Writer;

    /**
     * @brief Function to load the initial value of impedance.
     * It reads the impedance configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_Impedance_Initializer;

    /**
     * @brief Function to write the coupling for waveform generation.
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
     * @brief Function to write the signal reference for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SignalRef_Writer;

    /**
     * @brief Function to load the initial signal reference.
     * It reads the signal reference configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SignalRef_Initializer;

    /**
     * @brief Function to write the type of signal for waveform generation.
     * This is a compulsory field of the structure.
     */
    const writerInt32_t PV_SignalType_Writer;

    /**
     * @brief Function to load the initial type of signal .
     * It reads the type of signal configured in the device and it is loaded
     * as initial value in its corresponding PV. @n
     * This is an optional field of the structure.
     */
    readerInt32_t PV_SignalType_Initializer;

    /**
     * @brief Function to write the ground for waveform generation.
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

    /*
     * -----------------------------------------------------------------------------------------
     * List of default values that are used to initialize the PVVariableOutput member
     * variables. They can be edited by the user.
     * -----------------------------------------------------------------------------------------
     */
    /**
     * @brief Initial value of AWG Samples for waveform generation.
     */
    T m_DataAWG_Init;

    /**
     * @brief Initial value of the decimation factor for waveform generation.
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
     * @param switchOnFunction Switch-on function to be set in @ref WaveformGenerationArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction Switch-off function to be set in @ref WaveformGenerationArgs_t.handlerSTM switchOffFunction.
     * @param startFunction Start function to be set in @ref WaveformGenerationArgs_t.handlerSTM startFunction.
     * @param stopFunction Stop function to be set in @ref WaveformGenerationArgs_t.handlerSTM stopFunction.
     * @param recoverFunction Recover function to be set in @ref WaveformGenerationArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction Checking state transitions function to be set in @ref WaveformGenerationArgs_t.handlerSTM allowStateChangeFunction.
     * @param _PV_Frequency_Writer Function to write the frequency for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Frequency_Writer.
     * @param _PV_RefFrequency_Writer Function to write the reference frequency for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_RefFrequency_Writer.
     * @param _PV_Amp_Writer Function to write the amplitude for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Amp_Writer.
     * @param _PV_Phase_Writer Function to write the phase for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Phase_Writer.
     * @param _PV_UpdateRate_Writer Function to write the update rate for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_UpdateRate_Writer.
     * @param _PV_DutyCycle_Writer Function to write the duty cycle for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_DutyCycle_Writer.
     * @param _PV_Gain_Writer Function to write the gain for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Gain_Writer.
     * @param _PV_Offset_Writer Function to write the offset for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Offset_Writer.
     * @param _PV_Bandwidth_Writer Function to write the bandwidth for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Bandwidth_Writer.
     * @param _PV_Resolution_Writer Function to write the resolution for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Resolution_Writer.
     * @param _PV_Impedance_Writer Function to write the impedance for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Impedance_Writer.
     * @param _PV_Coupling_Writer Function to write the coupling for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Coupling_Writer.
     * @param _PV_SignalRef_Writer Function to write the signal reference for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_SignalRef_Writer.
     * @param _PV_SignalType_Writer Function to write the signal type for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_SignalType_Writer.
     * @param _PV_Ground_Writer Function to write the ground for waveform generation. To be set in @ref WaveformGenerationArgs_t.PV_Ground_Writer.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    WaveformGenerationArgs_t(stateChange_t switchOnFunction,
                             stateChange_t switchOffFunction,
                             stateChange_t startFunction,
                             stateChange_t stopFunction,
                             stateChange_t recoverFunction,
                             allowChange_t allowStateChangeFunction,
                             writerDouble_t _PV_Frequency_Writer,
                             writerDouble_t _PV_RefFrequency_Writer,
                             writerDouble_t _PV_Amp_Writer,
                             writerDouble_t _PV_Phase_Writer,
                             writerDouble_t _PV_UpdateRate_Writer,
                             writerDouble_t _PV_DutyCycle_Writer,
                             writerDouble_t _PV_Gain_Writer,
                             writerDouble_t _PV_Offset_Writer,
                             writerDouble_t _PV_Bandwidth_Writer,
                             writerDouble_t _PV_Resolution_Writer,
                             writerInt32_t _PV_Impedance_Writer,
                             writerInt32_t _PV_Coupling_Writer,
                             writerInt32_t _PV_SignalRef_Writer,
                             writerInt32_t _PV_SignalType_Writer,
                             writerInt32_t _PV_Ground_Writer,
                             autoEnable_t autoEnable=autoEnable_t::none) :
           handlerSTM (true, ///Asynchronous state transitions.
                   switchOnFunction,
                   switchOffFunction,
                   startFunction,
                   stopFunction,
                   recoverFunction,
                   allowStateChangeFunction,
                   autoEnable),
           PV_Frequency_Writer(_PV_Frequency_Writer),
           PV_RefFrequency_Writer(_PV_RefFrequency_Writer),
           PV_Amp_Writer(_PV_Amp_Writer),
           PV_Phase_Writer(_PV_Phase_Writer),
           PV_UpdateRate_Writer(_PV_UpdateRate_Writer),
           PV_DutyCycle_Writer(_PV_DutyCycle_Writer),
           PV_Gain_Writer(_PV_Gain_Writer),
           PV_Offset_Writer(_PV_Offset_Writer),
           PV_Bandwidth_Writer(_PV_Bandwidth_Writer),
           PV_Resolution_Writer(_PV_Resolution_Writer),
           PV_Impedance_Writer(_PV_Impedance_Writer),
           PV_Coupling_Writer(_PV_Coupling_Writer),
           PV_SignalRef_Writer(_PV_SignalRef_Writer),
           PV_SignalType_Writer(_PV_SignalType_Writer),
           PV_Ground_Writer(_PV_Ground_Writer),
           m_DataAWG_Init(T()), //Default value of the underlying data type
           m_Decimation_Init(1) {}
};
template struct WaveformGenerationArgs_t<std::int32_t>;
template struct WaveformGenerationArgs_t<std::int64_t>;
template struct WaveformGenerationArgs_t<float>;
template struct WaveformGenerationArgs_t<double>;
template struct WaveformGenerationArgs_t<std::vector<std::int8_t> >;
template struct WaveformGenerationArgs_t<std::vector<std::uint8_t> >;
template struct WaveformGenerationArgs_t<std::vector<std::int16_t> >;
template struct WaveformGenerationArgs_t<std::vector<std::int32_t> >;
template struct WaveformGenerationArgs_t<std::vector<std::int64_t> >;
template struct WaveformGenerationArgs_t<std::vector<float> >;
template struct WaveformGenerationArgs_t<std::vector<double> >;

/**
 * This is a node that supplies waveform generation PVs and few control
 * PVs configure and control an standard AWG.
 *
 * It also provides a state machine that allows to start/stop the waveform generation.
 *
 * The user of a WaveformGeneration class must declare few delegate functions that
 *  specify the actions to perform when the AWG node's state changes.
 *
 * In particular, the transition from the state off to running should launch
 *  the waveform generator thread which pushes the generated data via push(),
 *  while the transition from running to on should stop the waveform generator thread.
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
class NDS3_API WaveformGeneration: public Node
{
public:
    /**
     * @brief Initializes an empty waveform generation node.
     *
     * You must assign a valid WaveformGeneration node before calling initialize().
     */
    WaveformGeneration();

    /**
     * @brief Copies a waveform generation reference from another object.
     *
     * @param right a waveform generation holder from which the reference to
     *        the generation object implementation is copied
     */
    WaveformGeneration(const WaveformGeneration<T>& right);

    WaveformGeneration& operator=(const WaveformGeneration<T>& right);

    /**
     * @brief Constructs the waveform generation node which generates vector of values
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param name Name (unique) to identify the instance of the node.
     * @param maxElements Maximum size of the generated data array. Set to @c 1 for scalar values.
     * @param switchOnFunction Switch-on function
     * @param switchOffFunction Switch-off function
     * @param startFunction Start function
     * @param stopFunction Stop function
     * @param recoverFunction Recover function
     * @param allowStateChangeFunction Checking state transitions function
     * @param PV_Frequency_Writer Function to write the frequency for waveform generation.
     * @param PV_RefFrequency_Writer Function to write the reference frequency for waveform generation.
     * @param PV_Amp_Writer Function to write the amplitude for waveform generation.
     * @param PV_Phase_Writer Function to write the phase for waveform generation.
     * @param PV_UpdateRate_Writer Function to write the update rate for waveform generation.
     * @param PV_DutyCycle_Writer Function to write the duty cycle for waveform generation.
     * @param PV_Gain_Writer Function to write the gain for waveform generation.
     * @param PV_Offset_Writer Function to write the offset for waveform generation.
     * @param PV_Bandwidth_Writer Function to write the bandwidth for waveform generation.
     * @param PV_Resolution_Writer Function to write the resolution for waveform generation.
     * @param PV_Impedance_Writer Function to write the impedance for waveform generation.
     * @param PV_Coupling_Writer Function to write the coupling for waveform generation.
     * @param PV_SignalRef_Writer Function to write the signal reference for waveform generation.
     * @param PV_SignalType_Writer Function to write the signal type for waveform generation.
     * @param PV_Ground_Writer Function to write the ground for waveform generation.
     * @param autoEnable See @ref autoEnable_t for further details.
     */
    WaveformGeneration( const std::string& name,
                    size_t maxElements,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerDouble_t PV_Frequency_Writer,
                    writerDouble_t PV_RefFrequency_Writer,
                    writerDouble_t PV_Amp_Writer,
                    writerDouble_t PV_Phase_Writer,
                    writerDouble_t PV_UpdateRate_Writer,
                    writerDouble_t PV_DutyCycle_Writer,
                    writerDouble_t PV_Gain_Writer,
                    writerDouble_t PV_Offset_Writer,
                    writerDouble_t PV_Bandwidth_Writer,
                    writerDouble_t PV_Resolution_Writer,
                    writerInt32_t PV_Impedance_Writer,
                    writerInt32_t PV_Coupling_Writer,
                    writerInt32_t PV_SignalRef_Writer,
                    writerInt32_t PV_SignalType_Writer,
                    writerInt32_t PV_Ground_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the WaveformGeneration node.
     * @param name Name (unique) to identify the instance of the node.
     * @param maxElements Maximum size of the generated data array. Set to @c 1 for scalar values.
     * @param handlerWFG Structure with the arguments required by the WaveformGeneration node.
     * See @ref WaveformGenerationArgs_t for further details.
     */
    WaveformGeneration( const std::string& name,
                        size_t maxElements,
                        const WaveformGenerationArgs_t<T>& handlerWFG);

    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the waveform Generation starts.
     *
     * @param timestampDelegate 
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at the moment of the start of the generation.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the generation started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    /**
     * @ingroup datareadwrite
     * @brief Push waveform generated data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  waveform generation thread in order to push the generated data.
     *
     * @param timestamp timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Retrieve the waveform data from the control system
     *
     * @return waveform data from the control system
     */
    T getDataAWG();

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the WaveformGeneration constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();

    /**
     * @brief Retrieve the amplitude of the signal type generated.
     *
     * @return the amplitude value
     */
    double getAmplitude();
    /**
     * @brief Retrieve the desired signal type to be generated.
     *
     * @return the signalType value
     */
    int32_t getSignalType();
    /**
     * @brief Retrieve the frequency of the signal generated.
     *
     * @return the frequency value
     */
    double getFrequency();
    /**
     * @brief Retrieve the UpdateRate of the signal generated.
     *
     * @return the UpdateRate value
     */
    double getUpdateRate();
    /**
     * @brief Retrieve the Offset of the signal generated.
     *
     * @return the Offset value
     */
    double getOffset();
    /**
     * @brief Retrieve the Phase of the signal generated.
     *
     * @return the Phase value
     */
    double getPhase();
    /**
     * @brief Retrieve the Impedance of the signal generator.
     *
     * @return the Impedance value
     */
    int32_t getImpedance();
    /**
     * @brief Retrieve the RefFrequency of the signal generator.
     *
     * @return the RefFrequency value
     */
    double getRefFrequency();
    /**
     * @brief Retrieve the DutyCycle of the signal generator.
     *
     * @return the DutyCycle value
     */
    double getDutyCycle();
    /**
     * @brief Retrieve the Gain of the signal generator.
     *
     * @return the Gain value
     */
    double getGain();
    /**
     * @brief Retrieve the Bandwidth of the signal generator.
     *
     * @return the Bandwidth value
     */
    double getBandwidth();
    /**
     * @brief Retrieve the Resolution of the signal generator.
     *
     * @return the Resolution value
     */
    double getResolution();
    /**
     * @brief Retrieve the Coupling of the signal generator.
     *
     * @return the Coupling value
     */
    int32_t getCoupling();
    /**
     * @brief Retrieve the SignalRef of the signal generator.
     *
     * @return the SignalRef value
     */
    int32_t getSignalRef();
    /**
      * @brief Retrieve the Ground of the signal generator.
      *
      * @return the Ground value
      */
    int32_t getGround();
    /**
     * @brief Sets the value of the m_NumberOfPushedDataBocks.
     *
     * @param timestamp timestamp for the value
     * @param value Number of pushed data blocks during the data acquisition
     */
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_Amplitude_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Amplitude of the signal to be generated
     */
    void setAmplitude(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_SignalType_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Signal type of the signal to be generated
     */
    void setSignalType(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the value of the m_Frequency_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Frequency of the signal to be generated
     */
    void setFrequency(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_updateRate_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Waveform generator update rate value
     */
    void setUpdateRate(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_offSet_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Offset of the signal to be generated
     */
    void setOffset(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_phase_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Phase of the signal to be generated
     */
    void setPhase(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_impedance_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Waveform generator impedance value
     */
    void setImpedance(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_RefFrequency_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Reference frequency of the signal to be generated
     */
    void setRefFrequency(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_DutyCycle_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Duty cycle of the signal to be generated
     */
    void setDutyCycle(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Gain_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Waveform generator gain value
     */
    void setGain(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Bandwidth_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Bandwidth of the signal to be generated
     */
    void setBandwidth(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Resolution_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Resolution of the signal to be generated
     */
    void setResolution(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Coupling_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Signal coupled/decoupled
     */
    void setCoupling(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_SignalRef_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Reference signal
     */
    void setSignalRef(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_Ground_RBV.
     *
     * @param timestamp Timestamp for the value
     * @param value Waveform generator grounded/degrounded
     */
    void setGround(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);

};

}
#endif // NDSDATAGENERATION_H

