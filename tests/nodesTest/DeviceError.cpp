#include <mutex>
#include <functional>
#include <iostream>

#include <nds3/nds.h>

#include "DeviceError.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceError*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceError::DeviceError(nds::Factory &factory, const std::string &deviceName,
        const nds::namedParameters_t &parameters) :
        m_name(deviceName),
        m_delegateOutError(nds::PVDelegateOut<std::int32_t>("delegateOutError",std::bind(&DeviceError::delegateError, this, std::placeholders::_1, std::placeholders::_2))),
        m_delegateInError(nds::PVDelegateIn<std::int32_t>("delegateInError",std::bind(&DeviceError::readError, this, std::placeholders::_1, std::placeholders::_2))),
        m_variableOutError(nds::PVVariableOut<std::int32_t>("variableOutError"))
{

    {
        std::lock_guard<std::mutex> lock(m_lockDevicesMap);
        if (m_devicesMap.find(deviceName) != m_devicesMap.end()) {
            throw std::logic_error(
                    "Device with the same name already allocated. This should not happen");
        }
        m_devicesMap[deviceName] = this;
    }

    /**
     * Here we declare the root node.
     * It is a good practice to name it with the device name.
     *
     * Also, for simplicity we declare it as a "Port": this means that
     * the root node will be responsible for the communication with
     * the underlying control system.
     *
     * It is possible to have the root node as a simple Node and promote one or
     * more of its children to "Port": each port will interface with a different
     * control system thread.
     */
    nds::Port rootNode(deviceName);
    nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");

    rootNode.addChild(m_delegateOutError);
    rootNode.addChild(m_delegateInError);
    rootNode.addChild(m_variableOutError);

    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);

}

DeviceError::~DeviceError() {
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

DeviceError* DeviceError::getInstance(const std::string &deviceName) {
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceError*>::const_iterator findDevice =
            m_devicesMap.find(deviceName);
    if (findDevice == m_devicesMap.end()) {
        return 0;
    }
    return findDevice->second;
}

void DeviceError::readError(timespec* timestamp, std::int32_t *value) {
    std::int32_t type= m_variableOutError.getValue();
    DeviceError::throwError(type);
    *value = type;
    *timestamp = {0,0};
}

void DeviceError::delegateError(const timespec& /*timestamp*/, const std::int32_t &value) {
    DeviceError::throwError(value);

}

void DeviceError::throwError(std::int32_t value) {
    switch (value) {
    case 0:
        break;
    case 1:
        throw nds::NdsError("Timeout Error", nds::statusPV_t::timeout);
        break;
    case 2:
        throw nds::NdsError("Overflow Error", nds::statusPV_t::overflow);
        break;
    case 3:
        throw nds::NdsError("Disconnected Error",
                nds::statusPV_t::disconnected);
        break;
    case 4:
        throw nds::NdsError("Disabled Error", nds::statusPV_t::disabled);
        break;
    case 5:
        throw nds::NdsError("Other error", nds::statusPV_t::error);
        break;
    }
}

//NDS_DEFINE_DRIVER(DeviceError, DeviceError)
/**
* Allocation function
*********************/
void* DeviceError::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
{
    return new DeviceError(factory, deviceName, parameters);
}

/**
* Deallocation function
***********************/
void DeviceError::deallocateDevice(void* deviceName)
{
    delete (DeviceError*)deviceName;
}
