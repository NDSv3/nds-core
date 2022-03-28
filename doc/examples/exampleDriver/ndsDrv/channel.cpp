#include <functional>
#include <sstream>
#include <iostream>
#include <unistd.h>

#include "simulated_signal.h"

#include "nds3/nds.h"
#include "channel.h"


channel::channel(const std::string& name, nds::Node& parent) {

  //TODO: Set value of the amplitude and frequency pv at start.

  /**
   * This class will be a new child node of the given parent node, in this case
   * the exampleDrv class that holds the Root Node.
   */
  nds::Node channel = parent.addChild(nds::Node(name));

  /**
   * @brief Calling the data acquisition node.
   *
   *   Adding Timestamping node to the control system. Note that in this case is
   * added as a child of the parent node. See the exampleDrv cpp file for more
   * information.
   *
   *   All the delegate methods and the state machine functions must be passed
   *   to the constructor of the node.
   *
   */
  m_acquisition = channel.addChild(nds::DataAcquisition<std::vector<double>>(
   "DAQ", /* Name of the Data Acquisition Node. */
    100, /* Maximum length of the buffer to be retrieved. */
    std::bind(&channel::switchon_acquisition, this),
    std::bind(&channel::switchoff_acquisition, this),
    std::bind(&channel::start_acquisition, this),
    std::bind(&channel::stop_acquisition, this),
    std::bind(&channel::recover_acquisition, this),
    std::bind(&channel::allowChange_acquisition, this,
          std::placeholders::_1, std::placeholders::_2,std::placeholders::_3),
    std::bind(&channel::pv_gain_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_offset_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_bandwidth_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_resolution_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_impedance_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_coupling_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_signalRefType_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_ground_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_dmaEnable_writer,
        this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&channel::pv_samplingRate_writer,
        this, std::placeholders::_1, std::placeholders::_2)));

  /*
   *   These PVs are an example of how a user can implement new PVs to cover
   * missing functionalities. In  this case the Amplitude PV allows the user to
   * change the amplitude of the sine signal and the PV Frequency allows to change
   * the frequency of the sine signal.
   *
   */
  m_amplitude = nds::PVVariableOut<double>("Amplitude");
  channel.addChild(m_amplitude);
  m_frequency = nds::PVVariableOut<double>("Frequency");
  channel.addChild(m_frequency);


}


channel::~channel() {

}

/**
 *   Data Acquisitions methods.
 *
 *   This are the methods the driver developer should implement to build the
 * driver. This is the space left for the developer to interact with the HW API
 * and write most of the driver code.
 *
 */

/**
 *   A description of the state machine functions is given in the terminal.cpp
 * file.
 *
 */
void channel::switchon_acquisition() {

}

void channel::switchoff_acquisition() {

}

void channel::start_acquisition() {

  m_stop_acquisition = false;
  /**
   *   On RUNNING state a thread is launched to start the DAQ functionalities.
   * In this thread an external function is called which gives back a vector
   * with the desired length containing a sinusoidal signal. Once the signal is
   * retrieved the vector is pushed to the CS.
   *
   */
  m_acquisition_thread = m_acquisition.runInThread("Aqcquisition",
                         std::bind(&channel::acquisition_thread_body, this));

}

void channel::stop_acquisition() {

  m_stop_acquisition = true;
  m_acquisition_thread.join();
}

void channel::recover_acquisition() {

  throw nds::StateMachineRollBack("Cannot recover");
}

bool channel::allowChange_acquisition(const nds::state_t, const nds::state_t,
                      const nds::state_t) {

  return true;
}


/*
 *   Data Acquisition setters.
 *
 */
void channel::pv_gain_writer(const timespec& timestamp, const double& value) {

  double HW_value;
  // Value has the Gain to be programmed on the hardware.
  // Call to function programming the hardware. This function should return the
  // real Gain programmed. This value has to be set to the readback attribute.
  // In the meantime, without real hardware value and HW_value are equal.
  HW_value=value;
  m_acquisition.setGain(timestamp,HW_value);

}

void channel::pv_offset_writer(const timespec& timestamp, const double& value) {

  double HW_value;
  // Value has the Offset to be programmed on the hardware.
  // Call to function programming the hardware. This function should return the
  // real Offset programmed. This value has to be set to the readback attribute.
  // In the meantime, without real hardware value and HW_value are equal.
  HW_value=value;
  m_acquisition.setOffset(timestamp,HW_value);

}

void channel::pv_bandwidth_writer(const timespec& timestamp,
                  const double& value) {

  double HW_value;
  // Value has the Bandwidth to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real Bandwidth programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are
  // equal.
  HW_value=value;
  m_acquisition.setBandwidth(timestamp,HW_value);
}

void channel::pv_resolution_writer(const timespec& timestamp,
                   const double& value) {

  double HW_value;
  // Value has the Resolution to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real Resolution programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are
  // equal.
  HW_value=value;
  m_acquisition.setResolution(timestamp,HW_value);
}

void channel::pv_impedance_writer(const timespec& timestamp, const double& value) {

  double HW_value;
  // Value has the Impedance to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real Impedance programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are
  // equal.
  HW_value=value;
  m_acquisition.setImpedance(timestamp,HW_value);
}

void channel::pv_coupling_writer(const timespec& timestamp,
                 const std::int32_t& value) {

  std::int32_t HW_value;
  // Value has the Coupling to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real Coupling programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are
  // equal.
  HW_value=value;
  m_acquisition.setCoupling(timestamp,HW_value);
}


void channel::pv_signalRefType_writer(const timespec& timestamp,
                      const std::int32_t& value) {

  std::int32_t HW_value;
  // Value has the SignalRefType to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real SignalRefType programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are equal.
  HW_value=value;
  m_acquisition.setSignalRefType(timestamp,HW_value);
}

void channel::pv_ground_writer(const timespec& timestamp,
                   const std::int32_t& value) {

  std::int32_t HW_value;
  // Value has the Ground to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real Ground programmed. This value has to be set to the readback attribute.
  // In the meantime, without real hardware value and HW_value are equal.
  HW_value=value;
  m_acquisition.setGround(timestamp,HW_value);
}

void channel::pv_dmaEnable_writer(const timespec& timestamp,
                  const std::int32_t& value) {

  std::int32_t HW_value;
  // Value has the DMAEnable value to be programmed on the hardware.
  // Call to function programming the hardware. This function should return
  // the real DMAEnable value programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are
  // equal.
  HW_value=value;
  m_acquisition.setDMAEnable(timestamp,HW_value);

}

void channel::pv_samplingRate_writer(const timespec& timestamp,
                     const double& value) {

  double HW_value;
  // Value has the SamplingRate to be programmed on the hardware.
  // Call to function programming the hardware. This function should return the
  // real SamplingRate programmed. This value has to be set to the readback
  // attribute. In the meantime, without real hardware value and HW_value are
  // equal.
  HW_value = value;
  m_acquisition.setSamplingRate(timestamp,HW_value);

}

void channel::acquisition_thread_body() {

    //Counter for number of pushed data blocks.
    std::int32_t NumberOfPushedDataBlocks(0);

    // Get desired amplitude and frequency from control system.
    double amplitude = m_amplitude.getValue();
    double frequency = m_frequency.getValue();

    // Get Gain
    double Gain = m_acquisition.getGain();
    // Get Bandwidth
    double Bandwidth = m_acquisition.getBandwidth();
    // Get Resolution
    double Resolution = m_acquisition.getResolution();
    // Get Coupling
    double Coupling = m_acquisition.getCoupling();
    // Get SignalRefType
    double SignalRefType = m_acquisition.getSignalRefType();
    // Get Ground
    double Ground = m_acquisition.getGround();
    // Get offset
    double Offset = m_acquisition.getOffset();
    // Get impedance
    std::int32_t Impedance = m_acquisition.getImpedance();
    // Get DMABufferSize
    std::int32_t DMABufferSize = m_acquisition.getDMABufferSize();
    // Get DMANumChannels
    std::int32_t DMANumChannels = m_acquisition.getDMANumChannels();
    // Get DMAFrametype
    std::int32_t DMAFrameType = m_acquisition.getDMAFrameType();
    // Get DMASampleSize
    std::int32_t DMASampleSize = m_acquisition.getDMASampleSize();
    // Get DMAEnable
    std::int32_t DMAEnable = m_acquisition.getDMAEnable();
    // Get SamplingRate
    double SamplingRate = m_acquisition.getSamplingRate();

    std::cout << "\tAmplitude = " << amplitude << std::endl;
    std::cout << "\tFrequency = " << frequency << std::endl;
    std::cout << "\tGain = " << Gain<<std::endl;
    std::cout << "\tBandwidth = " << Bandwidth<<std::endl;
    std::cout << "\tResolution = " << Resolution<<std::endl;
    std::cout << "\tCoupling = " << Coupling<<std::endl;
    std::cout << "\tSignalRefType = " << SignalRefType<<std::endl;
    std::cout << "\tGround = " << Ground << std::endl;
    std::cout << "\tOffset = " << Offset << std::endl;
    std::cout << "\tImpedance = " << Impedance << std::endl;
    std::cout << "\tDMABufferSize = " << DMABufferSize << std::endl;
    std::cout << "\tDMANumChannels = " << DMANumChannels << std::endl;
    std::cout << "\tDMAFrameType = " << DMAFrameType << std::endl;
    std::cout << "\tDMASampleSize = " << DMASampleSize << std::endl;
    std::cout << "\tDMAEnable = " << DMAEnable << std::endl;
    std::cout << "\tSamplingRate = " << SamplingRate << std::endl;

    while(!m_stop_acquisition) {

      // Refresh desired amplitude, frequency and sampling rate from control system.
      amplitude = m_amplitude.getValue();
      frequency = m_frequency.getValue();
      SamplingRate = m_acquisition.getSamplingRate();
      // Push the vector to the control system. The signal is generated taking
      // into account different parameters related to the clock frequency which
      // now are set hardcoded.
      m_acquisition.push((const timespec)m_acquisition.getTimestamp(),
             (const std::vector<double>)getDataBlock_sin(
                              amplitude,
                              frequency,
                              100, /* Buffer length */
                              100, /* Clock freq. */
                              2, /* Edge*/
                              SamplingRate));

      ++NumberOfPushedDataBlocks;

      // Rest for a while
      ::usleep(1000000);
    }
    m_acquisition.setNumberOfPushedDataBlocks(m_acquisition.getTimestamp(),
                          NumberOfPushedDataBlocks);


}
/* End Data Acquisition methods. */
