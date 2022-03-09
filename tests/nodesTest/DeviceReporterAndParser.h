#ifndef DEVICE_REPORTERANDPARSER_H_
#define DEVICE_DEVICE_REPORTERANDPARSER_H_

#include <memory>
#include <functional>
#include <math.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with two PVs (output and input)
 * of each supported type for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceReporterAndParser
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceReporterAndParser(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceReporterAndParser();

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
    static DeviceReporterAndParser* getInstance(const std::string& deviceName);


private:

    /**
     * @brief name of the device
     */
    std::string m_Name;
    nds::Thread m_thread;
    bool m_parseDBandWriteToDriver;
    bool m_reporterSuccess;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MEMBER METHODS THAT SUPPORT SPECIFIC OPERATIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIST OF WRITERS METHODS FOR ALL SUPPORTED TYPES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    void report(FILE*, int);
    void parseDB(void);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIST OF INITIALIZERS METHODS FOR ALL SUPPORTED TYPES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

};

#endif // DEVICE_DEVICE_REPORTERANDPARSER_H_
