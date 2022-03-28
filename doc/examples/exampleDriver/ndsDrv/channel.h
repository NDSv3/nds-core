#include <nds3/nds.h>


/**
 * @brief Class which groups data adquisition functionalities.
 *
 *   This class includes a Data Acquisition node which retrieves a vector of
 * values which represent a sinusoidal signal. This signal can be modified by
 * user by means of the Frequency and Amplitude PVs.
 *
 *   This class could also add a timing and/or timestamping nodes to give a
 * timestamp to the data retrieved.
 */
class channel {

 public:

  /**
   * @brief Constructor of the class.
   *
   *   It will add the Data Acquisition NDS node and as part of the control
   * system. These nodes will be automatically added as children of any parent
   * class of channel.
   *
   */
  channel(const std::string& name, nds::Node& parent);
  /**
   * @brief Destructor of the class.
   */
  ~channel();

  /**
   * @brief PV that will allow the user to set the desired signal amplitude.
   */
  nds::PVVariableOut<double> m_amplitude;
  /**
   * @brief PV that will allow the user to set the desired signal frequency.
   */
  nds::PVVariableOut<double> m_frequency;


  //////////////////////////////////////////////////////////////////////////////
  /// Data Acquisition Node ////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Methods to control Data Acquisition state machine.
   *
   *   A description of what these methods do is given in the terminal.h file.
   *
   */
  nds::DataAcquisition<std::vector<double>> m_acquisition;
  void switchon_acquisition();
  void switchoff_acquisition();
  void start_acquisition();
  void stop_acquisition();
  void recover_acquisition();
  bool allowChange_acquisition(const nds::state_t,
                   const nds::state_t,
                   const nds::state_t);


  /**
   * Data acquisition setters. These setters allow the user to modify different
   * characteristics of the data being acquired.
   *
   */

  /**
   * @brief The gain of the signal.
   */
  void pv_gain_writer(const timespec& timestamp, const double& value);
  /**
   * @brief The offset of the signal.
   */
  void pv_offset_writer(const timespec& timestamp, const double& value);
  /**
   * @brief The bdanwidth of the signal.
   */
  void pv_bandwidth_writer(const timespec& timestamp, const double& value);
  /**
   * @brief The resolution of the signal.
   */
  void pv_resolution_writer(const timespec& timestamp, const double& value);
  /**
   * @brief The impedance.
   */
  void pv_impedance_writer(const timespec& timestamp, const double& value);
  /**
   * @brief The coupling.
   */
  void pv_coupling_writer(const timespec& timestamp, const std::int32_t& value);
  /**
   * @brief The  signal ref. value.
   */
  void pv_signalRefType_writer(const timespec& timestamp, const std::int32_t& value);
  /**
   * @brief Stablishes the ground value.
   */
  void pv_ground_writer(const timespec& timestamp, const std::int32_t& value);
  /**
   * @brief Sets the number of DMA channels been used by the DAQ node.
   */
  void pv_dmaEnable_writer(const timespec& timestamp, const std::int32_t& value);
  /**
   * @brief The desired sampling rate
   */
  void pv_samplingRate_writer(const timespec& timestamp, const double& value);

  /**
   * @brief This thread will be active while the timing state machine is in
   * RUNNING state.
   */
  void acquisition_thread_body();

  /**
   * @brief A standard thread that runs the acquisition_thread_body() function.
   */
  nds::Thread m_acquisition_thread;

  /**
   * @brief Variable to be set to false when the Data  state machine is set to
   * ON.
   *
   */
  volatile bool m_stop_acquisition;

  //////////////////////////
  // End Data Acquisition //
  //////////////////////////
 protected:

 private:

};
