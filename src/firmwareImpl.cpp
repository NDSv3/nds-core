/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * Modified by GMV & UPM
 *
 */


#include "nds3/definitions.h"
#include "nds3/impl/firmwareImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

namespace nds
{

FirmwareImpl::FirmwareImpl(const std::string& name,
                    size_t maxElements,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerString_t PV_FirmwarePath_Writer,
                    autoEnable_t autoEnable):
    NodeImpl(name, nodeType_t::inputChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    nds::FirmwareArgs_t handlerFIRM = FirmwareArgs_t(
                                    switchOnFunction,
                                    switchOffFunction,
                                    startFunction,
                                    stopFunction,
                                    recoverFunction,
                                    allowStateChangeFunction,
                                    PV_FirmwarePath_Writer,
                                    autoEnable);
    constructorBody(maxElements, handlerFIRM);
}

FirmwareImpl::FirmwareImpl(const std::string& name,
               size_t maxElements,
               const FirmwareArgs_t& handlerFIRM):
    NodeImpl(name, nodeType_t::inputChannel),
    m_OnStartDelegate(handlerFIRM.handlerSTM.startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    constructorBody(maxElements, handlerFIRM);
}

inline void FirmwareImpl::constructorBody(const size_t maxElements, const FirmwareArgs_t& handlerFIRM) {

    // Add the children PVs
    m_FirmwareVersion_PV.reset(new PVVariableInImpl<std::string>("Version"));
    m_FirmwareVersion_PV->setDescription("Firmware version");
    m_FirmwareVersion_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_FirmwareVersion_PV);

    enumerationStrings_t firmwareStatusEnumerationStrings;
    firmwareStatusEnumerationStrings.push_back("NO_ERROR");
    firmwareStatusEnumerationStrings.push_back("INITIALIZING");
    firmwareStatusEnumerationStrings.push_back("RESETTING");
    firmwareStatusEnumerationStrings.push_back("HARDWARE/FIRMWARE_ERROR");
    firmwareStatusEnumerationStrings.push_back("NO_BOARD_ACCESS");
    firmwareStatusEnumerationStrings.push_back("STATIC_CONF_ERROR");
    firmwareStatusEnumerationStrings.push_back("DYNAMIC_CONF_ERROR");
    firmwareStatusEnumerationStrings.push_back("RESERVED");


    m_FirmwareStatus_PV.reset(new PVVariableInImpl<std::int32_t>("Status"));
    m_FirmwareStatus_PV->setDescription("Firmware status");
    m_FirmwareStatus_PV->setScanType(scanType_t::interrupt, 0);
    m_FirmwareStatus_PV->setEnumeration(firmwareStatusEnumerationStrings);
    addChild(m_FirmwareStatus_PV);

    m_HWRevision_PV.reset(new PVVariableInImpl<std::string>("HWRevision"));
    m_HWRevision_PV->setDescription("Hardware revision");
    m_HWRevision_PV->setMaxElements(maxElements);
    m_HWRevision_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_HWRevision_PV);

    m_SerialNumber_PV.reset(new PVVariableInImpl<std::string>("SerialNumber"));
    m_SerialNumber_PV->setDescription("Serial number");
    m_SerialNumber_PV->setMaxElements(maxElements);
    m_SerialNumber_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SerialNumber_PV);

    m_DeviceModel_PV.reset(new PVVariableInImpl<std::string>("Model"));
    m_DeviceModel_PV->setDescription("Device model");
    m_DeviceModel_PV->setMaxElements(maxElements);
    m_DeviceModel_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DeviceModel_PV);

    m_DeviceType_PV.reset(new PVVariableInImpl<std::string>("Type"));
    m_DeviceType_PV->setDescription("Device type");
    m_DeviceType_PV->setMaxElements(maxElements);
    m_DeviceType_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DeviceType_PV);

    m_DriverVersion_PV.reset(new PVVariableInImpl<std::string>("DriverVersion"));
    m_DriverVersion_PV->setDescription("Driver Version");
    m_DriverVersion_PV->setMaxElements(maxElements);
    m_DriverVersion_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DriverVersion_PV);

    m_ChassisNumber_PV.reset(new PVVariableInImpl<int32_t>("ChassisNumber"));
    m_ChassisNumber_PV->setDescription("Chassis Number");
    m_ChassisNumber_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ChassisNumber_PV);

    m_SlotNumber_PV.reset(new PVVariableInImpl<int32_t>("SlotNumber"));
    m_SlotNumber_PV->setDescription("Slot Number");
    m_SlotNumber_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SlotNumber_PV);

    if (handlerFIRM.PV_FirmwarePath_Initializer) {
        m_FirmwarePath_PV.reset(new PVDelegateOutImpl<std::string>("FilePath",
                                                                    handlerFIRM.PV_FirmwarePath_Writer,
                                                                    handlerFIRM.PV_FirmwarePath_Initializer));
    } else {
        m_FirmwarePath_PV.reset(new PVDelegateOutImpl<std::string>("FilePath", handlerFIRM.PV_FirmwarePath_Writer));
    }
    m_FirmwarePath_PV->setDescription("Path to the firmware file to load");
    m_FirmwarePath_PV->setMaxElements(maxElements);
    m_FirmwarePath_PV->setScanType(scanType_t::passive, 0);
    addChild(m_FirmwarePath_PV);

    m_FirmwarePath_RBVPV.reset(new PVVariableInImpl<std::string>("FilePath_RBV"));
    m_FirmwarePath_RBVPV->setDescription("Readback PV of the firmware file");
    m_FirmwarePath_RBVPV->setMaxElements(maxElements);
    m_FirmwarePath_RBVPV-> setScanType(scanType_t::interrupt,0);
    addChild(m_FirmwarePath_RBVPV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(false,
                                   handlerFIRM.handlerSTM.switchOnFunction,
                                   handlerFIRM.handlerSTM.switchOffFunction,
                                   std::bind(&FirmwareImpl::onStart, this),
                                   handlerFIRM.handlerSTM.stopFunction,
                                   handlerFIRM.handlerSTM.recoverFunction,
                                   handlerFIRM.handlerSTM.allowStateChangeFunction,
                                   handlerFIRM.handlerSTM.autoEnable));
    addChild(m_StateMachine);
}

timespec FirmwareImpl::getStartTimestamp() const
{
    return m_StartTime;
}

void FirmwareImpl::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

void FirmwareImpl::push(const timespec& /*timestamp*/, const std::string& /*data*/)
{
    //TODO
}

void FirmwareImpl::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    //Set Decimation when push method is defined
    m_OnStartDelegate();
}

std::string FirmwareImpl::getFirmwareVersion()
{
    std::string FirmwareVersion;
    timespec timestamp;
    m_FirmwareVersion_PV->read(&timestamp, &FirmwareVersion);
    return (std::string)FirmwareVersion;
}

std::int32_t FirmwareImpl::getFirmwareStatus()
{
    std::int32_t FirmwareStatus;
    timespec timestamp;
    m_FirmwareStatus_PV->read(&timestamp, &FirmwareStatus);
    return FirmwareStatus;
}

std::string FirmwareImpl::getHardwareRevision()
{
    std::string HardwareRevision;
    timespec timestamp;
    m_HWRevision_PV->read(&timestamp, &HardwareRevision);
    return (std::string)HardwareRevision;
}

std::string FirmwareImpl::getSerialNumber()
{
    std::string SerialNumber;
    timespec timestamp;
    m_SerialNumber_PV->read(&timestamp, &SerialNumber);
    return (std::string)SerialNumber;
}

std::string FirmwareImpl::getDeviceModel()
{
    std::string DeviceModel;
    timespec timestamp;
    m_DeviceModel_PV->read(&timestamp, &DeviceModel);
    return (std::string)DeviceModel;
}

std::string FirmwareImpl::getDeviceType()
{
    std::string DeviceType;
    timespec timestamp;
    m_DeviceType_PV->read(&timestamp, &DeviceType);
    return (std::string)DeviceType;
}

std::string FirmwareImpl::getDriverVersion() 
{
    std::string DriverVersion;
    timespec timestamp; 
    m_DriverVersion_PV->read(&timestamp, &DriverVersion);
    return DriverVersion; 
}

int32_t FirmwareImpl::getChassisNumber() 
{
    int32_t ChassisNumber;
    timespec timestamp; 
    m_ChassisNumber_PV->read(&timestamp, &ChassisNumber);
    return ChassisNumber; 
}

int32_t FirmwareImpl::getSlotNumber() 
{
    int32_t SlotNumber;
    timespec timestamp; 
    m_SlotNumber_PV->read(&timestamp, &SlotNumber);
    return SlotNumber; 
}

std::string FirmwareImpl::getFirmwarePath()
{
    std::string FirmwarePath;
    timespec timestamp;
    m_FirmwarePath_RBVPV->read(&timestamp, &FirmwarePath);
    return (std::string)FirmwarePath;
}

void FirmwareImpl::setFirmwareVersion(const timespec& timestamp, const std::string& value)
{
    m_FirmwareVersion_PV->setValue(timestamp, value);
    m_FirmwareVersion_PV->push(timestamp, value);
}

void FirmwareImpl::setFirmwareStatus(const timespec& timestamp, const std::int32_t& value)
{
    m_FirmwareStatus_PV->setValue(timestamp, value);
    m_FirmwareStatus_PV->push(timestamp, value);
}

void FirmwareImpl::setHardwareRevision(const timespec& timestamp, const std::string& value)
{
    m_HWRevision_PV->setValue(timestamp, value);
    m_HWRevision_PV->push(timestamp, value);
}

void FirmwareImpl::setSerialNumber(const timespec& timestamp, const std::string& value)
{
    m_SerialNumber_PV->setValue(timestamp, value);
    m_SerialNumber_PV->push(timestamp, value);
}

void FirmwareImpl::setDeviceModel(const timespec& timestamp, const std::string& value)
{
    m_DeviceModel_PV->setValue(timestamp, value);
    m_DeviceModel_PV->push(timestamp, value);
}

void FirmwareImpl::setDeviceType(const timespec& timestamp, const std::string& value)
{
    m_DeviceType_PV->setValue(timestamp, value);
    m_DeviceType_PV->push(timestamp, value);
}

void FirmwareImpl::setDriverVersion(const timespec& timestamp, const std::string& value)
{
    m_DriverVersion_PV->setValue(timestamp, value);
    m_DriverVersion_PV->push(timestamp, value);
}

void FirmwareImpl::setChassisNumber(const timespec& timestamp, const int32_t& value)
{
    m_ChassisNumber_PV->setValue(timestamp, value);
    m_ChassisNumber_PV->push(timestamp, value);
}

void FirmwareImpl::setSlotNumber(const timespec& timestamp, const int32_t& value)
{
    m_SlotNumber_PV->setValue(timestamp, value);
    m_SlotNumber_PV->push(timestamp, value);
}

void FirmwareImpl::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    m_FirmwarePath_RBVPV->setValue(timestamp, value);
    m_FirmwarePath_RBVPV->push(timestamp, value);
}

void FirmwareImpl::setState(const nds::state_t& newState)
{
    m_StateMachine->setState(newState);
}


}
