#ifndef DEVICEERROR_H_
#define DEVICEERROR_H_

#include <memory>
#include <functional>
#include <math.h>
#ifndef _WIN32
#endif
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a NDSError throwing function for testing purposes of nds-core V3.
 *
 *
 *  The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */

class DeviceError
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DeviceError(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DeviceError();

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
    static DeviceError* getInstance(const std::string& deviceName);
    /**
     * @brief function that throws the different types of NdsErrors
     * @param value NdsError type value
     */
    void throwError(std::int32_t value);


private:

    /*
     * @brief name of the device
     */
    std::string m_name;
    nds::PVDelegateOut<std::int32_t> m_delegateOutError;
    nds::PVDelegateIn<std::int32_t> m_delegateInError;
    nds::PVVariableOut<std::int32_t> m_variableOutError;
/**
 *
 * @param timespec
 * @param value NdsError type value
 */
    void delegateError(const timespec& timespec, const std::int32_t& value);
/**
 *
 * @param timespec
 * @param value NdsError type value
 */
    void readError(timespec* timespec, std::int32_t* value);

};

#endif // DEVICEERROR_H_
