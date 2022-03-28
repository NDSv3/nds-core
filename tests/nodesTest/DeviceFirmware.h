#ifndef DEVICE_FIRM_H_
#define DEVICE_FIRM_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Firmware node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceFirmware
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceFirmware(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceFirmware();

    /*
     * Allocation/deallocation
     *
     *******************************************************/
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    static void deallocateDevice(void* deviceName);

    /*
     * For test purposes we make it possible to retrieve running instances of
     *  the device
     */
    static DeviceFirmware* getInstance(const std::string& deviceName);


private:

    /*
     * @brief name of the device
     */
    std::string m_Name;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // TEST FIRMWARE NODE
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Firmware node
     */
    nds::Firmware m_Firmware;

    /**
     * Methods to control the Firmware state machine
     */
    void switchOn_Firmware();  ///< Called to switch on the Firmware node.
    void switchOff_Firmware(); ///< Called to switch off the Firmware node.
    void start_Firmware();     ///< Called to start the Firmware node.
    void stop_Firmware();      ///< Called to stop the Firmware node.
    void recover_Firmware();   ///< Called to recover the Firmware node from a failure.

    bool allow_Firmware_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

    /**
     * Firmware setters
     */
    void PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value);

    /**
     * @brief Initializer method to read the firmware path from the hardware at start-up time.
     * @param time Timestamp to indicate when the function is called.
     * @param path Path of the firmware that is loaded in the hardware.
     */
    void PV_Firmware_Path_Initializer(timespec* time, std::string* path);

    /**
     * @brief Function that emulates the changes of status of the Firmware node.
     *        It is launched by start_Firmware() in a separate thread.
     */
    void Firmware_thread_body();

    /**
     * @brief A thread that runs Firmware_thread_body().
     */
    nds::Thread m_Firmware_Thread;

    /**
     * @brief A boolean flag that stop the Firmware loop in Firmware_thread_body()
     *        when true.
     */
    volatile bool m_bStop_Firmware;


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

};

#endif // DEVICE_FIRM_H_
