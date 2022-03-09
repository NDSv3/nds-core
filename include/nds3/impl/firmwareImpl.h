/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFIRMWAREIMPL_H
#define NDSFIRMWAREIMPL_H

#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/firmware.h"

namespace nds
{

class FirmwareImpl: public NodeImpl {
public:
    /**
     *
     * @param name The node's name
     * @param maxElements Maximum length of the PV strings.
     * @param switchOnFunction          Delegate function that performs the actions to switch the node on
     * @param switchOffFunction         Delegate function that performs the actions to switch the node off
     * @param startFunction             Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
     * @param stopFunction              Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
     * @param recoverFunction           Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction  Delegate function that can deny a state change. Usually just returns true
     * @param PV_FirmwarePath_Writer Delegate function that performs the actions to set the firmware path
     * @param autoEnable See @ref autoEnable_t for further details
     */
      FirmwareImpl(const std::string& name,
           size_t maxElements,
          stateChange_t switchOnFunction,
          stateChange_t switchOffFunction,
          stateChange_t startFunction,
          stateChange_t stopFunction,
          stateChange_t recoverFunction,
          allowChange_t allowStateChangeFunction,
          writerString_t PV_FirmwarePath_Writer,
          autoEnable_t autoEnable);

        /**
         * @param name The node's name
         * @param maxElements Maximum length of the PV strings.
         * @param handlerFIRM See @ref FirmwareArgs_t for futher details.
         */
      FirmwareImpl(const std::string& name,
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
       * @brief Called by the state machine. Store the current timestamp and then calls the
       *        delegated onStart function.
       */
      void onStart();

      /**
       * @brief Called to obtain the actual state of the State Machine of the Node
       *
       * @return The actual state of the State Machine of the Node
       */
      nds::state_t getState();

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
      std::int32_t getFirmwareStatus();
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
       * @brief Sets the value of the ChassisNumber
       *
       */
      void setChassisNumber(const timespec& timestamp, const int32_t& value);
      /**
       * @brief Sets the value of the Slot Number
       *
       */
      void setSlotNumber(const timespec& timestamp, const int32_t& value);
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

    protected:

      /**
       * @brief In the state machine we set the start function to onStart(), so we
       *        remember here what to call from onStart().
       */
      stateChange_t m_OnStartDelegate;

      /**
       * @brief Delegate function that retrieves the start time. Executed
       *        by onStart().
       *
       * By default points to BaseImpl::getTimestamp().
       *
       * Use setStartTimestampDelegate() to change the delegate function.
       */
      getTimestampPlugin_t m_StartTimestampFunction;

      /**
       * @brief Generation start time. Retrieved during onStart() via the delegate
       *        function declared in  m_startTimestampFunction.
       */
      timespec m_StartTime;

      // PVs

      /**
       * @brief Input PV that provides the version of the firmware with a string
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_FirmwareVersion_PV;

      /**
       * @brief Input PV that provides the status of the firmware with a string
       */
      std::shared_ptr<PVVariableInImpl<std::int32_t> > m_FirmwareStatus_PV;

      /**
       * @brief Input PV that provides the hardware revision with a string
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_HWRevision_PV;

      /**
       * @brief Input PV that provides the serial number of the device with a string
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_SerialNumber_PV;

      /**
       * @brief Input PV that provides the device model with a string
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_DeviceModel_PV;

      /**
       * @brief Input PV that provides the type of device with a string
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_DeviceType_PV;

      /**
       * @brief Input PV that provides the driver version
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_DriverVersion_PV;

      /**
       * @brief Input PV that provides the chassis number
       * */
      std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ChassisNumber_PV;

      /**
       * @brief Input PV that provides the slot number
       * */
      std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SlotNumber_PV;

      /**
       * @brief Output PV to set the path of the firmware to be loaded
       */
      std::shared_ptr<PVDelegateOutImpl<std::string> > m_FirmwarePath_PV;

      /**
       * @brief Readback PV to get the path to the firmware file
       */
      std::shared_ptr<PVVariableInImpl<std::string> > m_FirmwarePath_RBVPV;

      /**
       * @brief State machine to handle the status of the node
       */
      std::shared_ptr<StateMachineImpl> m_StateMachine;


    private:

      /**
       * Common source code to define the body of any constructor
       * @param maxElements Maximum number of elements to allocate vectors (if any).
       * @param handlerFIRM Structure containing both compulsory and optional functions.
       */
      void constructorBody(const size_t maxElements, const FirmwareArgs_t& handlerFIRM);
  };


}
#endif // NDSFIRMWARESUPIMP_H

