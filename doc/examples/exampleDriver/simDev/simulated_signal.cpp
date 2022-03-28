#include <cmath>
#include <iostream>
#include "simulated_signal.h"

std::vector<double> getDataBlock_sin(double amplitude, double frequency,
				     std::int32_t nElements,
                                     std::int32_t ClkFrequency, std::int32_t edge,
                                     double SamplingRate) {
  static double t = 0.0;
  double increment;
  if (edge == 2) {
    increment = SamplingRate/((double)ClkFrequency * 2.0);
  } else if (edge == 0) {
    increment = SamplingRate/(double)ClkFrequency;
    t += increment/2.0;
  } else if (edge == 1) {
    increment = SamplingRate/(double)ClkFrequency;
  } else {
     std::cout  << "WARNING: edge should be 0(RAISING),1(FALLING),2(ANY)\n";
     std::cout  << "         Empty vector returned.\n";
     std::vector<double> data(nElements, 0);
     return data;
  }

  if (amplitude <= 0 || frequency <= 0 || nElements <= 0
      || ClkFrequency <= 0 || SamplingRate <= 0) {

     std::cout  << "WARNING: make sure the variables below take values >=0:\n";
     std::cout  << "         - Amplitude: " << amplitude << std::endl;
     std::cout  << "         - Frequency: " << frequency << std::endl; 
     std::cout  << "         - SamplingRate: " << SamplingRate << std::endl; 
     std::cout  << "         - nElements: " << nElements << std::endl;
     std::cout  << "         - ClkFrequency: " << ClkFrequency << std::endl;
     std::cout  << "         Empty vector returned.\n";
     std::vector<double> data(nElements, 0);
     return data;
  }

  std::vector<double> data(nElements);
  for (auto it = data.begin(); it != data.end(); ++it) {
     *it  = amplitude * sin(2.0 * M_PI * frequency * t);
     t += increment;
  }

  if (edge == 1) {
    t += increment/2.0;
  }

  return data;
}


hw_firm firmware_description() {

  hw_firm firmware = {
    "Firmware Version",
    1,
    "Firmware revision",
    "Firmware serial number",
    "Firmware device model",
    "Firmware device type",
    "Firmware driver version",
    1,
    32,
    "Firmware path"
  };

  return firmware;

}



hw_firm change_hw_firmware() {


  hw_firm new_firmware = {
  "New firmware Version",
  1,
  "New firmware revision",
  "New firmware serial number",
  "New firmware device model",
  "New firmware device type",
  "New firmware driver version",
  1,
  32,
  "New firmware path"
  };

  return new_firmware;
}
