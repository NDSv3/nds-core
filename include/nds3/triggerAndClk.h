/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTRIGGERANDCLK_H
#define NDSTRIGGERANDCLK_H

/**
 * @file
 * @brief TBD
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"
#include "nds3/routing.h"
#include "nds3/stateMachine.h"


namespace nds
{

/**
 * @brief Type defined to gather the arguments required by @ref TriggerAndClkImpl constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct TriggerAndClkArgs_t{

  /**
   * @brief State Machine handler structure.
   * It contains all methods required by the state machine.
   * This is a compulsory field of the structure.
   */
  const StateMachineArgs_t handlerSTM;

  /**
   * @brief Function to generate a trigger by software.
   * This is a compulsory field of the structure.
   */
  const writerInt32_t PV_SetSW_Writer;

  /**
   * @brief Function to load the initial value of the @ref PV_SetSW_Writer.
   * This is an optional field of the structure.
   */
  readerInt32_t PV_SetSW_Initializer;

  /**
   * @brief Function to set the parameters related with the trigger configuration.
   * This function applies the selected configurations of triggering to the hardware. @n
   * It may take into account the following member methods to obtain the last
   * parameters that have been configured;
   * <ul>
   *    <li>@ref TriggerAndClk.getDAQStartTimeDelay</li>
   *    <li>@ref TriggerAndClk.getTriggPeriod</li>
   *    <li>@ref TriggerAndClk.getLevel</li>
   *    <li>@ref TriggerAndClk.getEdge</li>
   *    <li>@ref TriggerAndClk.getChange</li>
   *    <li>@ref TriggerAndClk.getMode</li>
   *    <li>@ref TriggerAndClk.getPostTrigSamples</li>
   *    <li>@ref TriggerAndClk.getPreTrigSamples</li>
   * </ul>
   * This is a compulsory field of the structure.
   */
  const writerInt32_t PV_LoadTrigConf_Writer;

  /**
   * @brief Function to load the initial value of the @ref PV_LoadTrigConf_Writer.
   * This is an optional field of the structure.
   */
  readerInt32_t PV_LoadTrigConf_Initializer;

  /**
   * @brief Function to reset the triggering configuration.
   * This is a compulsory field of the structure.
   */
  const writerInt32_t PV_ResetTrigConf_Writer;

  /**
   * @brief Function to load the initial value of the @ref PV_ResetTrigConf_Writer.
   * This is an optional field of the structure.
   */
  readerInt32_t PV_ResetTrigConf_Initializer;

  /**
   * @brief Function to set the parameters related with PLL configuration.
   * This functions applies the selected PLL configuration. @n
   * It may take into account the following member methods to obtain the last
   * parameters that have been configured;
   * <ul>
   *    <li>@ref TriggerAndClk.getSyncMode</li>
   *    <li>@ref TriggerAndClk.getPLLRefFreq</li>
   *    <li>@ref TriggerAndClk.getPLLRefDiv</li>
   *    <li>@ref TriggerAndClk.getPLLRefMult</li>
   *    <li>@ref TriggerAndClk.getPLLRefDivALL</li>
   * </ul>
   * This is a compulsory field of the structure.
   */
  const writerInt32_t PV_PLLSyncSet_Writer;

  /**
   * @brief Function to load the initial value of the @ref PV_PLLSyncSet_Writer.
   * This is an optional field of the structure.
   */
  readerInt32_t PV_PLLSyncSet_Initializer;

  /**
   * @brief Function to enable or disable DLL.
   * This is a compulsory field of the structure.
   */
  const writerInt32_t PV_EnableDisablePLL_Writer;

  /**
   * @brief Function to load the initial value of the @ref PV_EnableDisablePLL_Writer.
   * This is an optional field of the structure.
   */
  readerInt32_t PV_EnableDisablePLL_Initializer;

  /**
   *@brief Reference to the routing node required for triggering purposes.
   * This is a compulsory field of the structure.
   */
  const nds::Routing<std::string>& routingNode;

  /*
   * -----------------------------------------------------------------------------------------
   * List of default values that are used to initialize the PVVariableOutput member
   * variables. They can be edited by the user.
   * -----------------------------------------------------------------------------------------
   */

  /**
   * @brief Initial value of the identifier number of the HW-block to program the triggering features.
   */
  std::int32_t m_HWBlock_Init;

  /**
   * @brief Initial value of the time to delay the data acquisition after a trigger.
   */
  std::int32_t m_DAQStartTimeDelay_Init;

  /**
   * @brief Initial value of the period of trigger.
   */
  std::int32_t m_TriggPeriod_Init;

  /**
   * @brief Initial value of the level of trigger (@b 0 =@e LOW, @b 1 =@e HIGH).
   */
  std::int32_t m_Level_Init;

  /**
   * @brief Initial value of the edge for trigger detection (@b 0 =@e FALLING, @b 1 =@e RISING).
   */
  std::int32_t m_Edge_Init;

  /**
   * @brief Initial value of the type of trigger (@b 0 =@e LEVEL, @b 1 =@e EDGE).
   */
  std::int32_t m_Change_Init;

  /**
   * @brief Initial value of the action to be executed when a trigger is received.
   * The next table shows the typical values:
   * <table>
   *    <tr> <td><b><i>Value</i></b></td> <td><b>Meaning</b></td>  </tr>
   *    <tr> <td><i>0</i></td> <td>FINITE</td> </tr>
   *    <tr> <td><i>1</i></td> <td>FINITE-RETRIGGER</td> </tr>
   *    <tr> <td><i>2</i></td> <td>CONTINIOUS</td> </tr>
   *    <tr> <td><i>3</i></td> <td>REF-RETRIGGER</td> </tr>
   *    <tr> <td><i>4</i></td> <td>PERIODIC-RETRIGGER</td> </tr>
   * </table>
   */
  std::int32_t m_Mode_Init;

  /**
   * @brief Initial value of the clock Divider for the trigger clock source.
   */
  std::int32_t m_ClkDivider_Init;

  /**
   * @brief Initial value of the number of samples to be acquired before a trigger is met.
   * It is only taken into account when the @e Mode is set to @e REF-RETRIGGER.
   */
  std::int32_t m_PreTrigSamples_Init;

  /**
   * @brief Initial value of the number of samples to be acquired after a trigger condition is met.
   * It is only taken into account when the @e Mode is set to @e REF-RETRIGGER.
   */
  std::int32_t m_PostTrigSamples_Init;

  /**
   * @brief Initial value of the PLL synchronisation mode (@b 0 =@e DEFAULT, @b 1 =@e MASTER, @b 2 =@e SLAVE).
   * It is only taken into account when the @e EnableDisablePVV is set to @e ENABLE.
   */
  std::int32_t m_SyncMode_Init;

  /**
   * @brief Initial value of the PLL reference frequency.
   */
  std::int32_t m_PLLRefFreq_Init;

  /**
   * @brief Initial value of the PLL reference divisor.
   */
  std::int32_t m_PLLRefDiv_Init;

  /**
   * @brief Initial value of the PLL reference multiplier.
   */
  std::int32_t m_PLLRefMult_Init;

  /**
   * @brief Initial value of the PLL reference divisor.
   */
  std::int32_t m_PLLRefDivAll_Init;

  /*
   * -----------------------------------------------------------------------------------------
   * End of default values
   * -----------------------------------------------------------------------------------------
   */

  /**
   * @brief Constructor to create an instance of the given structure.
   * It must be used to ensure that compulsory fields are always provided in compilation time.
   * @param switchOnFunction Switch-on function to be set in @ref TriggerAndClkArgs_t.handlerSTM switchOnFunction.
   * @param switchOffFunction Switch-off function to be set in @ref TriggerAndClkArgs_t.handlerSTM switchOffFunction.
   * @param startFunction Start function to be set in @ref TriggerAndClkArgs_t.handlerSTM startFunction.
   * @param stopFunction Stop function to be set in @ref TriggerAndClkArgs_t.handlerSTM stopFunction.
   * @param recoverFunction Recover function to be set in @ref TriggerAndClkArgs_t.handlerSTM recoverFunction.
   * @param allowStateChangeFunction Checking state transitions function to be set in @ref TriggerAndClkArgs_t.handlerSTM allowStateChangeFunction.
   * @param _PV_SetSW_Writer Function to generate a SW trigger. To be set in @ref TriggerAndClkArgs_t.PV_SetSW_Writer.
   * @param _PV_LoadTrigConf_Writer Function to apply the triggering configuration. To be set in @ref TriggerAndClkArgs_t..
   * @param _PV_ResetTrigConf_Writer Function to reset the triggering configuration. To be set in @ref TriggerAndClkArgs_t.PV_ResetTrigConf_Writer.
   * @param _PV_PLLSyncSet_Writer Function to apply PLL configuration.
   * @param _PV_EnableDisablePLL_Writer Function to enable or disable PLL. To be set in @ref TriggerAndClkArgs_t.PV_EnableDisablePLL_Writer.
   * @param _routingNode Routing node to be used for triggering purposes. To be set in @ref TriggerAndClkArgs_t.routingNode.
   * @param autoEnable See @ref autoEnable_t for further details.
   *
   */
  TriggerAndClkArgs_t(stateChange_t switchOnFunction,
                      stateChange_t switchOffFunction,
                      stateChange_t startFunction,
                      stateChange_t stopFunction,
                      stateChange_t recoverFunction,
                      allowChange_t allowStateChangeFunction,
                      writerInt32_t _PV_SetSW_Writer,
                      writerInt32_t _PV_LoadTrigConf_Writer,
                      writerInt32_t _PV_ResetTrigConf_Writer,
                      writerInt32_t _PV_PLLSyncSet_Writer,
                      writerInt32_t _PV_EnableDisablePLL_Writer,
                      const nds::Routing<std::string>& _routingNode,
                      autoEnable_t autoEnable=autoEnable_t::none) :
      handlerSTM(true, ///Asynchronous state transitions.
                 switchOnFunction,
                 switchOffFunction,
                 startFunction,
                 stopFunction,
                 recoverFunction,
                 allowStateChangeFunction,
                 autoEnable),
      PV_SetSW_Writer(_PV_SetSW_Writer),
      PV_LoadTrigConf_Writer(_PV_LoadTrigConf_Writer),
      PV_ResetTrigConf_Writer(_PV_ResetTrigConf_Writer),
      PV_PLLSyncSet_Writer(_PV_PLLSyncSet_Writer),
      PV_EnableDisablePLL_Writer(_PV_EnableDisablePLL_Writer),
      routingNode(_routingNode),
      m_HWBlock_Init(0),
      m_DAQStartTimeDelay_Init(0),
      m_TriggPeriod_Init(0),
      m_Level_Init(0),
      m_Edge_Init(0),
      m_Change_Init(0),
      m_Mode_Init(0),
      m_ClkDivider_Init(0),
      m_PreTrigSamples_Init(0),
      m_PostTrigSamples_Init(0),
      m_SyncMode_Init(0),
      m_PLLRefFreq_Init(0),
      m_PLLRefDiv_Init(0),
      m_PLLRefMult_Init(0),
      m_PLLRefDivAll_Init(0)
      {}
};

template <typename T>
class NDS3_API TriggerAndClk: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
    TriggerAndClk();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
    TriggerAndClk(const TriggerAndClk<T>& right);

    TriggerAndClk& operator=(const TriggerAndClk<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
    TriggerAndClk(const std::string& name,
            stateChange_t switchOnFunction,
            stateChange_t switchOffFunction,
            stateChange_t startFunction,
            stateChange_t stopFunction,
            stateChange_t recoverFunction,
            allowChange_t allowStateChangeFunction,
            writerInt32_t PV_SetSW_Writer,
            writerInt32_t PV_LoadTrigConf_Writer,
            writerInt32_t PV_ResetTrigConf_Writer,
            writerInt32_t PV_PLLSyncSET_Writer,
            writerInt32_t PV_EnableDisablePLL_Writer,
            nds::Routing<std::string> routingNode,
            autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the TriggerAndClk node.
     * @param name Name (unique) to identify the instance of the node.
     * @param handlerTrig Structure with the arguments required by the TriggerAndClk node.
     * See @ref TriggerAndClkArgs_t for further details.
     */
        TriggerAndClk(const std::string& name, const TriggerAndClkArgs_t& handlerTrig);

    /**
     * @brief Routing node
     */
    nds::Routing<std::string> m_Routing;


    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the waveform Generation starts.
     *
     *
     * @param timestampDelegate
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

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
     * @brief Retrieve the HW Block set by the control system (AI,DI,AO,DO).
     *
     * @return HWBlock value
     */
    std::int32_t getHWBlock();

    /**
     * @brief Retrieve the time to delay the DAQ after trigger.
     *
     * @return DAQStartTimeDelay value
     */
    std::int32_t getDAQStartTimeDelay();

    /**
     * @brief Retrieve the Trigger Period.
     *
     * @return Trigger Period value
     */
    std::int32_t getTriggPeriod();

    /**
     * @brief Retrieve the level configuration for the trigger (HIGH or LOW).
     *
     * @return Level value
     */
    std::int32_t getLevel();

    /**
     * @brief Retrieve the detection edge for the trigger (RISING or FALLING).
     *
     * @return Edge value
     */
    std::int32_t getEdge();

    /**
     * @brief Retrieve the selected mode for the trigger (LEVEL or EDGE).
     *
     * @return Change value
     */
    std::int32_t getChange();

    /**
     * @brief Retrieve The action (operation) executed when the trigger is received.
     *
     * @return Mode value
     */
    std::int32_t getMode();

    /**
     * @brief Retrieve the selected divider value.
     *
     * @return Divider value
     */
    std::int32_t getClkDivider();
    /**
     * @brief Retrieve the number of samples to be acquired BEFORE the trigger condition is met(only if REF-RETRIGERABLE is configured).
     *
     * @return PreTrigSamples value
     */
    std::int32_t getPreTrigSamples();

    /**
     * @brief Retrieve the number of samples to be acquired AFTER the trigger condition is met.
     *
     * @return PostTrigSamples value
     */
    std::int32_t getPostTrigSamples();

    /**
     * @brief Retrieve if the device has been configured as master or slave. Should have a value.
     *
     * @return SyncMode value
     */
    std::int32_t getSyncMode();

    /**
     * @brief Retrieve the PLL Reference Frequency.
     *
     * @return PLLRefFreq value
     */
    std::int32_t getPLLRefFreq();

    /**
     * @brief Retrieve the PLL Reference Divisor.
     *
     * @return PLLRefDiv value
     */
    std::int32_t getPLLRefDiv();

    /**
     * @brief Retrieve the PLL Reference Multiplier.
     *
     * @return PLLRefMult value
     */
    std::int32_t getPLLRefMult();

    /**
     * @brief Retrieve the PLL Reference Divisor.
     *
     * @return PLLRefDiv value
     */
    std::int32_t getPLLRefDivALL();

    /**
    * @brief Sets the LoadTrigConf error status.
    *
    * @param timestamp timestamp for the value
    * @param value Status after the trigger configuration.
    */
    void setTrigLoadStatus(const timespec& timestamp, const std::string& value);

    /**
    * @brief Sets the LoadTrigConf error code.
    *
    * @param timestamp timestamp for the value
    * @param value Code of detected error.
    */
    void setTrigLoadCode(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the HW Block set by the control system (AI,DI,AO,DO) Readback Value.
    *
    * @param timestamp timestamp for the value
    * @param value HWBlockRBV
    */
    void setHWBlockRBV(const timespec& timestamp, const std::int32_t& value);
    /**
    * @brief Number ide
    *
    * @param timestamp timestamp for the value
    * @param value DAQStartTimeDelayRBV
    */
    void setDAQStartTimeDelayRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the trigger period readback value.
    *
    * @param timestamp timestamp for the value
    * @param value TriggPeriodRBV
    */
    void setTriggPeriodRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the level configuration for the trigger (HIGH or LOW) readback value.
    *
    * @param timestamp timestamp for the value
    * @param value LevelRBV
    */
    void setLevelRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the detection edge for the trigger (RISING or FALLING) readback value.
    *
    * @param timestamp timestamp for the value
    * @param value EdgeRBV
    */
    void setEdgeRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the selected mode for the trigger (LEVEL or EDGE) readback value.
    *
    * @param timestamp timestamp for the value
    * @param value ChangeRBV
    */
    void setChangeRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the action (operation) executed when the trigger is receive readback value.
    *
    * @param timestamp timestamp for the value
    * @param value ModeRBV
    */
    void setModeRBV(const timespec& timestamp, const std::int32_t& value);
    /**
    * @brief Sets the Divider value readback value.
    *
    * @param timestamp timestamp for the value
    * @param value ClkDividerRBV
    */
    void setClkDividerRBV(const timespec& timestamp, const std::int32_t& value);
    /**
    * @brief Sets the the number of samples to be acquired BEFORE the trigger condition is met readback value.
    *
    * @param timestamp timestamp for the value
    * @param value PreTrigSamplesRBV
    */
    void setPreTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the number of samples to be acquired AFTER the trigger condition is met readback value.
    *
    * @param timestamp timestamp for the value
    * @param value PostTrigSamplesRBV
    */
    void setPostTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets if the device has been configured as master or slave (Should have a value). Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value SyncModeRBV configured in the HW.
     */
    void setSyncModeRBV(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the PLL Reference Frequency Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefFreq configured in the HW.
     */
    void setPLLRefFreqRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the PLL Reference Divisor Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefDiv configured in the HW.
     */
    void setPLLRefDivRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the PLL Reference Multiplier Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefMult configured in the HW.
     */
    void setPLLRefMultRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the PLL Reference Divisor Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefDivAll configured in the HW.
     */
    void setPLLRefDivALLRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the PLL Load error status.
    *
    * @param timestamp timestamp for the value
    * @param value Status after the PLL configuration.
    */
    void setPLLLoadStatus(const timespec& timestamp, const std::string& value);

    /**
    * @brief Sets the PLL Load error code.
    *
    * @param timestamp timestamp for the value
    * @param value Code of detected error.
    */
    void setPLLLoadCode(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the enable or disable PLL readback value.
    *
    * @param timestamp timestamp for the value
    * @param value Code of detected error.
    */
    void setEnableDisablePLLRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);
};

}
#endif // NDSTRIGGERANDCLK_H

