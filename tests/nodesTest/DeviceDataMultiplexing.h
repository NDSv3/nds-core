#ifndef DEVICE_DATAMUX_H_
#define DEVICE_DATAMUX_H_

#include <memory>

#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implements a fictional device with several channels for testing data multiplexing of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 * accept few mandatory parameters and should register the root node via Node::initialize().
 *
 * @ingroup Scheduling
 */
class DeviceDataMultiplexing
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceDataMultiplexing(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters );

    /**
     * @brief Destructor.
     */
    ~DeviceDataMultiplexing();

    /*
     * Allocation/deallocation
     *
     *******************************************************/
    /**
     * @brief Function required for allocating the resources of the device driver.
     * @param factory the control system factory that requested the creation of the device.
     * @param deviceName The name given to the device.
     * @param parameters Optional parameters passed to the device.
     */
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);

    /**
     * @brief Function required for deallocating the resource so fthe device driver.
     * @param deviceName The name given to the device.
     */
    static void deallocateDevice(void* deviceName);

    /**
     * @brief Test function for getting instances of the device.
     * For test purposes we make it possible to retrieve running instances of
     *  the device
     */
    static DeviceDataMultiplexing* getInstance(const std::string& deviceName);


private:

    std::string m_Name; ///< Device Name

    std::int32_t nChannels = 4; ///< Number of channels to be multiplexed.
        std::string dataTypeTxt = "float"; ///< Text indicating the data type to be multiplexed (in vectors).
        std::int32_t maxElements = 20; ///< Maximum number of samples to be multiplexed.
        std::int32_t lastValue = 0; ///< Internal variable to initialise the source PVs with different values.

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // TEST DATAMULTIPLEXING NODE
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief DataMultiplexing node
     */
    nds::DataMultiplexing<std::vector<float>> m_DataMultiplexing;

    /**
     * @brief Source PV for the multiplexing input channel number 0.
     *
     * In a real application, this PV will be provided from a different device driver.
     */
    nds::PVVariableIn<std::vector<float>> m_PV_Source_0;

        /**
         * @brief Source PV for the multiplexing input channel number 1.
         *
         * In a real application, this PV will be provided from a different device driver.
         */
        nds::PVVariableIn<std::vector<float>> m_PV_Source_1;

        /**
         * @brief Source PV for the multiplexing input channel number 2.
         *
         * In a real application, this PV will be provided from a different device driver.
         */
        nds::PVVariableIn<std::vector<float>> m_PV_Source_2;

        /**
         * @brief Source PV for the multiplexing channel number 3.
         *
         * In a real application, this PV will be provided from a different device driver.
         */
        nds::PVVariableIn<std::vector<float>> m_PV_Source_3;

        /**
         * @brief Auxiliary PV in order to increase the source PVs whenever it is written.
         *
         * It is just used for testing purpose and ina real application, this PV will not be required.
         */
        nds::PVDelegateOut<std::int32_t>  m_PV_IncreaseSources;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // TIMESTAMP HANDLING
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief PV to set the timestamp of the device, in seconds
     */
    nds::PVVariableOut<std::int32_t> m_setCurrentTime;

    /**
     * @brief Function to get the timestamp of the device
     */
    timespec getCurrentTime();

    /**
     * @brief Auxiliary function to set and push the value for different PVs.
     * @param pv PV to set its value.
     * @param timestamp Timestamp indicating when the function is called.
     * @param data Data to be set in the PV.
     */
    void setFloatArrayPV(nds::PVVariableIn<std::vector<float>> &pv, const timespec& timestamp, const std::vector<float>& data);

public:

    /**
     * @brief Delegate function to the @ref m_PV_IncreaseSources PV.
     * @param timestamp Timestamp indicating when the function is called.
     * @param doIt Flag to indicate if the source PVs shall be modified when it is different than zero.
     */
    void increaseSourcePVs(const timespec &timestamp, const std::int32_t &doIt);


};

#endif // DEVICE_DATAMUX_H_
