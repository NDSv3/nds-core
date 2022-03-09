#include <cstdint>
#include <vector>
#include <string>


/**
 * @brief Stucture that contains firmware information of a simulated device.
 */
typedef struct {

  std::string version;
  std::int32_t status;
  std::string revision;
  std::string serialNumber;
  std::string deviceModel;
  std::string deviceType;
  std::string driverVersion;
  std::int32_t chassisNum;
  std::int32_t slotNum;
  std::string firmPath;
} hw_firm;


/**
 * @brief function that returns a DataBlock with nElements of a sinusoidal wave.
 *
 */
std::vector<double> getDataBlock_sin(double amplitude, double frequency,
                     std::int32_t nElements, std::int32_t ClkFrequency,
                     std::int32_t edge, double SamplingRate);

/**
 * @brief function that returns a structure containing the firmware data.
 *
 */
hw_firm firmware_description();

/**
 * @brief function that returns a structure containing the new firmware data.
 *
 */
hw_firm change_hw_firmware();
