/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */

#include "../include/nds3/HQMonitor.h"

#include "../include/nds3/impl/HQMonitorImpl.h"

namespace nds
{

HQMonitor::HQMonitor(): Node()
{
}

HQMonitor::HQMonitor(  const std::string& name,
                                    stateChange_t switchOnFunction,
                                    stateChange_t switchOffFunction,
                                    stateChange_t startFunction,
                                    stateChange_t stopFunction,
                                    stateChange_t recoverFunction,
                                    allowChange_t allowStateChangeFunction,
                                    readerDouble_t PV_DevicePower_Reader,
                                    readerDouble_t PV_DeviceTemperature_Reader,
                                    readerDouble_t PV_DeviceVoltage_Reader,
                                    readerDouble_t PV_DeviceCurrent_Reader,
                                    writerInt32_t PV_SEUEnable_Writer,
                                    writerInt32_t PV_DAQEnable_Writer,
                                    writerInt32_t PV_SelfTestEnable_Writer,
                                    writerInt32_t PV_SelfTestType_Writer,
                                    writerInt32_t PV_SelfTestVerboseEnable_Writer,
                                    writerInt32_t PV_SelfTestIDEnable_Writer,
                                    writerInt32_t PV_SelfTestTxtEnable_Writer,
                                    writerInt32_t PV_SelfTestCodeResultEnable_Writer,
                                    readerString_t PV_SelfTestTextResult_Reader,
                                    readerInt32_t PV_SignalQualityFlag_Reader,
                                    writerDouble_t PV_SignalQualityFlagLevel_Writer,
                                    autoEnable_t autoEnable):
    Node(std::shared_ptr<HQMonitorImpl >(new HQMonitorImpl( name,
                                    switchOnFunction,
                                    switchOffFunction,
                                    startFunction,
                                    stopFunction,
                                    recoverFunction,
                                    allowStateChangeFunction,
                                    PV_DevicePower_Reader,
                                    PV_DeviceTemperature_Reader,
                                    PV_DeviceVoltage_Reader,
                                    PV_DeviceCurrent_Reader,
                                    PV_SEUEnable_Writer,
                                    PV_DAQEnable_Writer,
                                    PV_SelfTestEnable_Writer,
                                    PV_SelfTestType_Writer,
                                    PV_SelfTestVerboseEnable_Writer,
                                    PV_SelfTestIDEnable_Writer,
                                    PV_SelfTestTxtEnable_Writer,
                                    PV_SelfTestCodeResultEnable_Writer,
                                    PV_SelfTestTextResult_Reader,
                                    PV_SignalQualityFlag_Reader,
                                    PV_SignalQualityFlagLevel_Writer,
                                    autoEnable)))
{
}


HQMonitor::HQMonitor(const std::string& name, const HQMonitorArgs_t& handlerHQM):
                Node(std::shared_ptr<HQMonitorImpl>(new HQMonitorImpl(name, handlerHQM)))
{
}

HQMonitor::HQMonitor(const HQMonitor& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

HQMonitor& HQMonitor::operator=(const HQMonitor& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

void HQMonitor::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<HQMonitorImpl >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

void HQMonitor::push(const timespec& timestamp, const std::int32_t& data)
{
    std::static_pointer_cast<HQMonitorImpl >(m_pImplementation)->push(timestamp, data);
}

timespec HQMonitor::getStartTimestamp() const
{
    return std::static_pointer_cast<HQMonitorImpl >(m_pImplementation)->getStartTimestamp();
}

nds::state_t HQMonitor::getState()
{
    return std::static_pointer_cast<HQMonitorImpl >(m_pImplementation)->getState();
}

/**
 * ---------------------------------------------------
 * Getter functions
 * ---------------------------------------------------
 */

double HQMonitor::getDevicePower()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) ->getDevicePower();
}

double HQMonitor::getDeviceTemperature()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) ->getDeviceTemperature();
}

double HQMonitor::getDeviceVoltage()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) ->getDeviceVoltage();
}


double HQMonitor::getDeviceCurrent()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) ->getDeviceCurrent();
}


size_t HQMonitor::getSEUEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSEUEnable();
}

size_t HQMonitor::getDAQMonitorEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getDAQMonitorEnable();
}

size_t HQMonitor::getSelfTestEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTestEnable();
}

size_t HQMonitor::getSelfTestType()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTestType();
}

size_t HQMonitor::getSelfTestVerboseEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTestVerboseEnable();
}

size_t HQMonitor::getSelfTestIDEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTestIDEnable();
}

size_t HQMonitor::getSelfTestTextEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTestTextEnable();
}

size_t HQMonitor::getSelfTestCodeResultEnable()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTestCodeResultEnable();
}

  std::string HQMonitor::getSelfTestTextResult()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSelfTextTxtResult();
}



size_t HQMonitor::getSignalQualityFlag()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSignalQualityFlag();
}

double HQMonitor::getSignalQualityFlagLevel()
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> getSignalQualityFlagLevel();
}

/**
 * ---------------------------------------------------
 * Setter functions
 * ---------------------------------------------------
 */

void HQMonitor::setDevicePower(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setDevicePower(timestamp, value);
}

void HQMonitor::setDeviceTemperature(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setDeviceTemperature(timestamp, value);
}

void HQMonitor::setDeviceVoltage(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setDeviceVoltage(timestamp, value);
}

void HQMonitor::setDeviceCurrent(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setDeviceCurrent(timestamp, value);
}

void HQMonitor::setSEUEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSEUEnable(timestamp, value);
}

void HQMonitor::setDAQMonitorEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setDAQMonitorEnable(timestamp, value);
}

void HQMonitor::setSelfTestEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTestEnable(timestamp, value);
}


void HQMonitor::setSelfTestType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTestType(timestamp, value);
}

void HQMonitor::setSelfTestVerboseEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTestVerboseEnable(timestamp, value);
}

void HQMonitor::setSelfTestIDEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTestIDEnable(timestamp, value);
}

void HQMonitor::setSelfTestTextEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTestTextEnable(timestamp, value);
}

void HQMonitor::setSelfTestCodeResultEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTestCodeResultEnable(timestamp, value);
}

void HQMonitor::setSignalQualityFlag(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSignalQualityFlag(timestamp, value);
}

void HQMonitor::setSignalQualityFlagLevel(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSignalQualityFlagLevel(timestamp, value);
}

void HQMonitor::setSelfTestTextResult(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<HQMonitorImpl> (m_pImplementation) -> setSelfTextTxtResult(timestamp, value);
}

void HQMonitor::setState(const nds::state_t& newState)
{
    return std::static_pointer_cast<HQMonitorImpl>(m_pImplementation)->setState(newState);
}

}
