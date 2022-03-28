#include <nds3/nds.h>
#include <mutex>
#include <functional>
#include <numeric>
#include <iostream>

#include "DeviceDataMultiplexing.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */


static std::map<std::string, DeviceDataMultiplexing*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DeviceDataMultiplexing::DeviceDataMultiplexing(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t & /*parameters*/):
                          m_Name(DeviceName),
                          m_PV_IncreaseSources(nds::PVDelegateOut<std::int32_t>("IncreaseSources", std::bind(&DeviceDataMultiplexing::increaseSourcePVs, this, std::placeholders::_1, std::placeholders::_2)))
        {
        //Verify that there is no devices of this type with the same name
        {
                std::lock_guard<std::mutex> lock(m_lockDevicesMap);
                if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end())
                {
                        throw std::logic_error("Device with the same name already allocated. This should not happen");
                }
                m_DevicesMap[DeviceName] = this;
        }

        /**
         * Here we declare the root node.
         * It is a good practice to name it with the Device name.
         *
         * Also, for simplicity we declare it as a "Port": this means that
         * the root node will be responsible for the communication with
         * the underlying control system.
         *
         * It is possible to have the root node as a simple Node and promote one or
         * more of its children to "Port": each port will interface with a different
         * control system thread.
         */
        nds::Port rootNode(DeviceName);

        //Add a PV to set the current time
        m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
        m_setCurrentTime.setDescription("Set timestamp (in secodns)");
        // For testing purposes the current time is set to a constant bigger
        // than 1 of January of 1990 which is the EPICS epoch.
        timespec timestamp = {0, 0};
        m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);

        // Add the multiplexing PV.
        std::string nodeName = "DataMultiplexing_" + std::to_string(nChannels) + "_" + dataTypeTxt;
        m_DataMultiplexing = rootNode.addChild(nds::DataMultiplexing<std::vector<float>>(nodeName, nChannels));
        m_DataMultiplexing.setTimestampDelegate(std::bind(&DeviceDataMultiplexing::getCurrentTime,this));

        // This section adds the source PVs that will not be required in a real applciation.
        m_PV_Source_0 = rootNode.addChild(nds::PVVariableIn<std::vector<float>>("FloatArray_Source_0"));
        m_PV_Source_0.setDescription("FloatArray in channel 0");
        m_PV_Source_0.setScanType(nds::scanType_t::interrupt);
        m_PV_Source_0.setMaxElements(maxElements);
        m_PV_Source_0.processAtInit(true);

        m_PV_Source_1 = rootNode.addChild(nds::PVVariableIn<std::vector<float>>("FloatArray_Source_1"));
        m_PV_Source_1.setDescription("FloatArray in channel 1");
        m_PV_Source_1.setScanType(nds::scanType_t::interrupt);
        m_PV_Source_1.setMaxElements(maxElements);
        m_PV_Source_1.processAtInit(true);

        m_PV_Source_2 = rootNode.addChild(nds::PVVariableIn<std::vector<float>>("FloatArray_Source_2"));
        m_PV_Source_2.setDescription("FloatArray in channel 2");
        m_PV_Source_2.setScanType(nds::scanType_t::interrupt);
        m_PV_Source_2.setMaxElements(maxElements);
        m_PV_Source_2.processAtInit(true);

        m_PV_Source_3 = rootNode.addChild(nds::PVVariableIn<std::vector<float>>("FloatArray_Source_3"));
        m_PV_Source_3.setDescription("FloatArray in channel 3");
        m_PV_Source_3.setScanType(nds::scanType_t::interrupt);
        m_PV_Source_3.setMaxElements(maxElements);
        m_PV_Source_3.processAtInit(true);

        // Add the PV for modifying the source PVs that will not be required in a real application.
        rootNode.addChild(m_PV_IncreaseSources);
        m_PV_IncreaseSources.setDescription("Set 1 to increase sources");
        m_PV_IncreaseSources.setScanType(nds::scanType_t::passive);

        // We have declared all the nodes and PVs in our Device: now we register them
        // with the control system that called this constructor.
        ////////////////////////////////////////////////////////////////////////////////
        rootNode.initialize(this, factory);
        rootNode.setTimestampDelegate(std::bind(&DeviceDataMultiplexing::getCurrentTime,this));

        //Stream information for debugging purposes
        rootNode.setLogLevel(nds::logLevel_t::debug);
        rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger:The DeviceDataMultiplexing is created" << std::endl;
        ndsDebugStream(rootNode) << "This is the ndsDebugStream: The DeviceDataMultiplexing named " << rootNode.getFullName() << " is created" << std::endl;
}



DeviceDataMultiplexing::~DeviceDataMultiplexing()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_Name);

}

DeviceDataMultiplexing* DeviceDataMultiplexing::getInstance(const std::string& DeviceName)
{

    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceDataMultiplexing*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }

    return findDevice->second;
}

timespec DeviceDataMultiplexing::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

void DeviceDataMultiplexing::setFloatArrayPV(nds::PVVariableIn<std::vector<float>> &pv, const timespec& timestamp, const std::vector<float>& data){
  pv.setValue(timestamp, data);
  pv.push(timestamp, data);
}

void DeviceDataMultiplexing::increaseSourcePVs(const timespec &/*timestamp*/, const std::int32_t &doIt)
{
  if (doIt) {
    for (int i = 0; i < nChannels; i++) {

      std::vector<float> v(maxElements);
      std::iota (std::begin(v), std::end(v), lastValue);
      lastValue += maxElements;
      switch (i) {
        case(0):
          setFloatArrayPV(m_PV_Source_0, m_DataMultiplexing.getTimestamp(), v);
          break;
        case(1):
          setFloatArrayPV(m_PV_Source_1, m_DataMultiplexing.getTimestamp(), v);
          break;
        case(2):
          setFloatArrayPV(m_PV_Source_2, m_DataMultiplexing.getTimestamp(), v);
          break;
        case(3):
          setFloatArrayPV(m_PV_Source_3, m_DataMultiplexing.getTimestamp(), v);
          break;
      }
    }
  }
}


//NDS_DEFINE_DRIVER(DeviceDataMultiplexing, DeviceDataMultiplexing)
/*
 * Allocation function
 *********************/
void* DeviceDataMultiplexing::allocateDevice(nds::Factory& factory,
                                     const std::string& DeviceName,
                                     const nds::namedParameters_t& parameters){
    return new DeviceDataMultiplexing(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceDataMultiplexing::deallocateDevice(void* DeviceName)
{
    delete (DeviceDataMultiplexing*)DeviceName;
}
