#include <mutex>
#include <functional>
#include <iostream>

#include "DeviceReporterAndParser.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceReporterAndParser*> m_DevicesMap;
static std::mutex m_lockDevicesMap;


DeviceReporterAndParser::DeviceReporterAndParser(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t & parameters):
                        m_Name(DeviceName),
                        m_parseDBandWriteToDriver(false),
                        m_reporterSuccess(false)
                        {

    parameters.find("INIT");
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
     * control system thread
     */

    nds::Port rootNode(DeviceName);


    //Add the children PVs


    // We have declared all the nodes and PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
    rootNode.registerReporter(std::bind(&DeviceReporterAndParser::report,this,std::placeholders::_1,std::placeholders::_2));
    factory.registerDBParser(std::bind(&DeviceReporterAndParser::parseDB,this));
}


DeviceReporterAndParser::~DeviceReporterAndParser()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_Name);

}

DeviceReporterAndParser* DeviceReporterAndParser::getInstance(const std::string& DeviceName)
{

    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceReporterAndParser*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }

    return findDevice->second;
}
/**
 * This is the reporter function. It shall be called by the control system when more information
 * that would otherwise be impractical to access is needed.
 *
 */
void DeviceReporterAndParser::report(FILE* file, int details){
    if(details==0){
        fprintf(file,"NDS Device driver %s reporting...\n",m_Name.c_str());
        m_reporterSuccess=true;
        std::cout<<"reporter success."<<std::endl;
    }else {
        //do nothing
    }
}
/**
 * This is the DB parser function. Registered on the factory to be Control System dependent.
 * The control system can access the DB to obtain useful parameters for the Device driver to know.
 * For example, in EPICS obtaining the waveform Number of Elements field is interesting.
 */
void DeviceReporterAndParser::parseDB(void){
m_parseDBandWriteToDriver=true;
    std::cout<<"parseDB success."<<std::endl;
}

//NDS_DEFINE_DRIVER(DeviceReporterAndParser, DeviceReporterAndParser)

/*
 * Allocation function
 ********************
*/
void* DeviceReporterAndParser::allocateDevice(nds::Factory& factory,
                     const std::string& DeviceName,
                     const nds::namedParameters_t& parameters) {
  return new DeviceReporterAndParser(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 **********************
*/
void DeviceReporterAndParser::deallocateDevice(void* DeviceName) {
  delete (DeviceReporterAndParser*)DeviceName;
}
