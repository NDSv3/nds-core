/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2017 GMV
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 */

#include "nds3/firmware.h"
#include "../include/nds3/impl/firmwareImpl.h"

namespace nds
{

Firmware::Firmware(): Node()
{
}

/**
 * @brief Constructs the firmware support node
 *
 */
Firmware::Firmware(const std::string& name,
           size_t maxElements,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        writerString_t PV_FirmwarePath_Writer,
        autoEnable_t autoEnable):
    Node(std::shared_ptr<FirmwareImpl>(new FirmwareImpl(name,
                                maxElements,
        switchOnFunction,
        switchOffFunction,
        startFunction,
        stopFunction,
        recoverFunction,
        allowStateChangeFunction,
        PV_FirmwarePath_Writer,
        autoEnable)))
{
}

Firmware::Firmware(const std::string& name,
                   size_t maxElements,
                   const FirmwareArgs_t& handlerFIRM):
            Node(std::shared_ptr<FirmwareImpl>(new FirmwareImpl(name,
                                        maxElements,
                                        handlerFIRM)))
{
}

Firmware::Firmware(const Firmware& right): 
     Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

Firmware& Firmware::operator=(const Firmware& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

void Firmware::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

void Firmware::push(const timespec& timestamp, const std::string& data)
{
    std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->push(timestamp, data);
}

timespec Firmware::getStartTimestamp() const
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getStartTimestamp();
}

std::string Firmware::getFirmwareVersion()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getFirmwareVersion();
}

//std::string Firmware::getFirmwareStatus()
std::int32_t Firmware::getFirmwareStatus()
{
    return std::static_pointer_cast<FirmwareImpl>(m_pImplementation)->getFirmwareStatus();
}

std::string Firmware::getHardwareRevision()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getHardwareRevision();
}

std::string Firmware::getSerialNumber()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getSerialNumber();
}

std::string Firmware::getDeviceModel()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getDeviceModel();
}

std::string Firmware::getDeviceType()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getDeviceType();
}

std::string Firmware::getDriverVersion()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getDriverVersion();
}

int32_t Firmware::getChassisNumber()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getChassisNumber();
}

int32_t Firmware::getSlotNumber()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getSlotNumber();
}

std::string Firmware::getFirmwarePath()
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->getFirmwarePath();
}

void Firmware::setFirmwareVersion(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setFirmwareVersion(timestamp, value);
}

//void Firmware::setFirmwareStatus(const timespec& timestamp, const std::string& value)
void Firmware::setFirmwareStatus(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setFirmwareStatus(timestamp, value);
}

void Firmware::setHardwareRevision(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setHardwareRevision(timestamp, value);
}

void Firmware::setSerialNumber(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setSerialNumber(timestamp, value);
}

void Firmware::setDeviceModel(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setDeviceModel(timestamp, value);
}

void Firmware::setDeviceType(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setDeviceType(timestamp, value);
}

void Firmware::setDriverVersion(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setDriverVersion(timestamp, value);
}

void Firmware::setChassisNumber(const timespec& timestamp, const int32_t& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setChassisNumber(timestamp, value);
}

void Firmware::setSlotNumber(const timespec& timestamp, const int32_t& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setSlotNumber(timestamp, value);
}


void Firmware::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setFirmwarePath(timestamp, value);
}

void Firmware::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<FirmwareImpl >(m_pImplementation)->setState(newState);
}

}
