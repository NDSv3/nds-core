/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFIRMWARE_H
#define NDSFIRMWARE_H

/**
 * @file firmware.h
 * @brief Defines the nds::Firmware node, which provides support to obtain
 *          basic information about the device
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
 * @brief Type defined to gather the arguments required by @ref Firmware constructors.
 * It can be used in auxiliary methods or additional nodes to simplify the number
 * of arguments required.
 */
struct FirmwareArgs_t {

    /**
     * @brief State Machine handler structure.
     * It contains all methods required by the state machine.
     * This is a compulsory field of the structure. @n
     * See @ref StateMachineArgs_t for further details.
     */
    const StateMachineArgs_t handlerSTM;

    /**
     * @brief Function to set the path of the new firmware file.
     * It sets the path to the firmware file to be loaded.
     * This is a compulsory field of the structure.
     */
    const writerString_t PV_FirmwarePath_Writer;

    /**
     * @brief Function to load the initial path of the firmware file.
     * It reads the path of the firmware file from the hardware and it
     * is loaded as initial value in its corresponding PV.
     * This is an optional field of the structure.
     */
    readerString_t PV_FirmwarePath_Initializer;

    /**
     * @brief Constructor to create an instance of the given structure.
     * It must be used to ensure that compulsory fields are always provided in compilation time.
     * @param switchOnFunction              Switch-on function to be set in @ref FirmwareArgs_t.handlerSTM switchOnFunction.
     * @param switchOffFunction             Switch-off function to be set in @ref FirmwareArgs_t.handlerSTM switchOffFunction.
     * @param startFunction                 Start function to be set in @ref FirmwareArgs_t.handlerSTM startFunction
     * @param stopFunction                  Stop function to be set in @ref FirmwareArgs_t.handlerSTM stopFunction.
     * @param recoverFunction               Recover function to be set in @ref FirmwareArgs_t.handlerSTM recoverFunction.
     * @param allowStateChangeFunction      Checking state transitions function to be set in @ref FirmwareArgs_t.handlerSTM allowStateChangeFunction.
     * @param PV_FirmwarePath_Writer        Function to write the path of the firmware file to be set in @ref FirmwareArgs_t.PV_FirmwarePath_Writer.
     * @param autoEnable                    See @ref autoEnable_t for further details.
     */
    FirmwareArgs_t(stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerString_t PV_FirmwarePath_Writer,
                    autoEnable_t autoEnable=autoEnable_t::none):
            handlerSTM( true, ///Asynchronous state transitions.
                        switchOnFunction,
                        switchOffFunction,
                        startFunction,
                        stopFunction,
                        recoverFunction,
                        allowStateChangeFunction,
                        autoEnable),
            PV_FirmwarePath_Writer(PV_FirmwarePath_Writer) {}
};


/**
 * This is a node that supplies a firmware support  with a few control
 * PVs that provides to the Control System information about the device.
 *
 * The user of a Firmware class must declare few delegate functions that
 * provide some information about the device.
 *
 * In particular, the information to be provided should be: the firmware version,
 * the firmware status, the hardware version id, the device serial number, the
 * device model, and the device type DAQ/IMAQ
 *
 */

class NDS3_API Firmware: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid Firmware node before calling initialize().
     */
    Firmware();

    /**
     * @brief Copies a firmware support reference from another object.
     *
     * @param right a firmware support holder from which the reference to
     *        the firmware object implementation is copied
     */
    Firmware(const Firmware& right);

    Firmware& operator=(const Firmware& right);

    /**
     * @brief Constructs the firmware support device node.
     *
     * @param name                      Name (unique) to identify the instance of the node.
     * @param maxElements               Maximum number of elements to allocate vectors (if any).
     * @param switchOnFunction          Switch-on function.
     * @param switchOffFunction         Switch-off function.
     * @param startFunction             Start function.
     * @param stopFunction              Stop function.
     * @param recoverFunction           Recover function.
     * @param allowStateChangeFunction  Checking state transitions function.
     * @param PV_FirmwarePath_Writer    Function to write the path of the firmware file.
     * @param autoEnable                See @ref autoEnable_t for further details
     */
    Firmware(const std::string& name,
         size_t maxElements,
    stateChange_t switchOnFunction,
    stateChange_t switchOffFunction,
    stateChange_t startFunction,
    stateChange_t stopFunction,
    stateChange_t recoverFunction,
    allowChange_t allowStateChangeFunction,
    writerString_t PV_FirmwarePath_Writer,
    autoEnable_t autoEnable=autoEnable_t::none);

    /**
     * @brief Simplified constructor of the firmware node.
     * @param name Name (unique) to identify the instance of the node.
     * @param maxElements Maximum number of elements to allocate vectors (if any).
     * @param handlerFIRM Structure with the arguments required by the firmware node.
     * See @ref FirmwareArgs_t for further details.
     */
    Firmware(const std::string& name,
                size_t maxElements,
                const FirmwareArgs_t& handlerFIRM);

    /**
     *
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate
     *
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
     *
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const std::string& data);

    /**
     *
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @brief Retrieve the Firmware Version
     *
     * @return the Firmware Version
     */
    std::string getFirmwareVersion();
    /**
     * @brief Retrieve the Firmware Status
     *
     * @return the Firmware Status
     */
    std::int32_t  getFirmwareStatus();
    /**
     * @brief Retrieve the Hardware Revision
     *
     * @return the Hardware Revision
     */
    std::string getHardwareRevision();
    /**
     * @brief Retrieve the Serial Number
     *
     * @return the Serial Number
     */
    std::string getSerialNumber();
    /**
     * @brief Retrieve the Device Model
     *
     * @return the Device Model
     */
    std::string getDeviceModel();
    /**
     * @brief Retrieve the Device Type
     *
     * @return the Device Type
     */
    std::string getDeviceType();
    /**
     * @brief Retrieve the Driver Version
     *
     * @return the Driver Version
     */
    std::string getDriverVersion();
    /**
     * @brief Retrieve the Chassis Number
     *
     * @return the Chassis Number
     */
    std::int32_t getChassisNumber();
    /**
     * @brief Retrieve the Slot Number
     *
     * @return the Slot Number
     */
    std::int32_t getSlotNumber();

    /**
     * @brief Retrieve the Firmware Path
     *
     * @return the Firmware Path
     */
    std::string getFirmwarePath();
    /**
     * @brief Sets the value of the Firmware Version
     *
     */
    void setFirmwareVersion(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Firmware Status
     *
     */
    void setFirmwareStatus(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Hardware Revision
     *
     */
    void setHardwareRevision(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Serial Number
     *
     */
    void setSerialNumber(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Device Model
     *
     */
    void setDeviceModel(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Device Type
     *
     */
    void setDeviceType(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Driver Version
     *
     */
    void setDriverVersion(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Chassis Number
     *
     */
    void setChassisNumber(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Slot Number
     *
     */
    void setSlotNumber(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Firmware Path
     *
     */
    void setFirmwarePath(const timespec& timestamp, const std::string& value);
    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);
};


}
#endif // NDSFIRMWARESUP_H

