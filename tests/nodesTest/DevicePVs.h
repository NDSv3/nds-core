#ifndef DEVICE_PVS_H_
#define DEVICE_PVS_H_

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
class DevicePVs
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param deviceName     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    DevicePVs(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    ~DevicePVs();

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
    static DevicePVs* getInstance(const std::string& deviceName);


private:

    /**
     * @brief name of the device
     */
    std::string m_Name;

  nds::Thread m_thread;
  bool m_terminate;

    /**
     * @brief maximum number of elements allowed for array structures
     */
    const std::int32_t maxArrayElements;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIST OF SUPPORTED PVs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //dataType_t: dataInt32 -> std::int32_t
    /**
     * @brief PV for testing an Output PV to set an integer
     */
    nds::PVDelegateOut<std::int32_t> m_int_PV;
    /**
     * @brief PV for testing an Input PV to get an integer
     */
    nds::PVVariableIn<std::int32_t> m_int_RBVPV;

    //dataType_t: dataInt64 -> std::int64_t
    /**
     * @brief PV for testing an Output PV to set an integer
     */
    nds::PVDelegateOut<std::int64_t> m_int64_PV;
    /**
     * @brief PV for testing an Input PV to get an integer
     */
    nds::PVVariableIn<std::int64_t> m_int64_RBVPV;

    /*
     * @brief PV for testing an Output PV to set a double
     */
    nds::PVDelegateOut<float> m_float_PV;
    /*
     * @brief PV for testing an Input PV to get a double
     */
    nds::PVVariableIn<float> m_float_RBVPV;

    //dataType_t: dataFloat64 -> double
    /*
     * @brief PV for testing an Output PV to set a double
     */
    nds::PVDelegateOut<double> m_double_PV;
    /*
     * @brief PV for testing an Input PV to get a double
     */
    nds::PVVariableIn<double> m_double_RBVPV;

    //dataType_t: dataBoolArray -> std::vector<bool>
    /*
     * @brief PV for testing an Output PV to set an array of booleans
     */
    nds::PVDelegateOut<std::vector<bool>> m_boolArray_PV;
    /*
     * @brief PV for testing an Input PV to get an array of booleans
     */
    nds::PVVariableIn<std::vector<bool>> m_boolArray_RBVPV;

    //dataType_t: dataUint8Array -> std::vector<std::uint8_t>
    /*
     * @brief PV for testing an Output PV to set an array of unsigned integers (8)
     */

    nds::PVDelegateOut<std::vector<std::uint8_t>> m_uInt8Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of unsigned integers (8)
     */
    nds::PVVariableIn<std::vector<std::uint8_t>> m_uInt8Array_RBVPV;

    //dataType_t: dataUint16Array -> std::vector<std::uint16_t>
    /*
     * @brief PV for testing an Output PV to set an array of unsigned integers (16)
     */

    nds::PVDelegateOut<std::vector<std::uint16_t>> m_uInt16Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of unsigned integers (16)
     */
    nds::PVVariableIn<std::vector<std::uint16_t>> m_uInt16Array_RBVPV;

    //dataType_t: dataUint32Array -> std::vector<std::uint32_t>
    /*
     * @brief PV for testing an Output PV to set an array of unsigned integers (32)
     */
    nds::PVDelegateOut<std::vector<std::uint32_t>> m_uInt32Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of unsigned integers (36)
     */
    nds::PVVariableIn<std::vector<std::uint32_t>> m_uInt32Array_RBVPV;

    //dataType_t: dataInt8Array -> std::vector<std::int8_t>
    /*
     * @brief PV for testing an Output PV to set an array of integers (8)
     */
    nds::PVDelegateOut<std::vector<std::int8_t>> m_int8Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of integers (8)
     */
    nds::PVVariableIn<std::vector<std::int8_t>> m_int8Array_RBVPV;

    //dataType_t: dataInt16Array -> std::vector<std::int16_t>
    /*
     * @brief PV for testing an Output PV to set an array of integers (16)
     */
    nds::PVDelegateOut<std::vector<std::int16_t>> m_int16Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of integers (16)
     */
    nds::PVVariableIn<std::vector<std::int16_t>> m_int16Array_RBVPV;

    //dataType_t: dataInt32Array -> std::vector<std::int32_t>
    /*
     * @brief PV for testing an Output PV to set an array of integers (32)
     */
    nds::PVDelegateOut<std::vector<std::int32_t>> m_int32Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of integers (32)
     */
    nds::PVVariableIn<std::vector<std::int32_t>> m_int32Array_RBVPV;

    //dataType_t: dataInt64Array -> std::vector<std::int64_t>
    /*
     * @brief PV for testing an Output PV to set an array of integers (64)
     */
    nds::PVDelegateOut<std::vector<std::int64_t>> m_int64Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of integers (64)
     */
    nds::PVVariableIn<std::vector<std::int64_t>> m_int64Array_RBVPV;
    /*
     * @brief PV for testing an Output PV to set an array of doubles
     */

    nds::PVDelegateOut<std::vector<float>> m_float32Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of doubles
     */
    nds::PVVariableIn<std::vector<float>> m_float32Array_RBVPV;

    //dataType_t: dataFloat64Array -> std::vector<double>
    /*
     * @brief PV for testing an Output PV to set an array of doubles
     */
    nds::PVDelegateOut<std::vector<double>> m_float64Array_PV;
    /*
     * @brief PV for testing an Input PV to get an array of doubles
     */
    nds::PVVariableIn<std::vector<double>> m_float64Array_RBVPV;

    //dataType_t: dataString -> std::string
    /*
     * @brief PV for testing an Output PV to set a string
     */
    nds::PVDelegateOut<std::string> m_string_PV;
    /*
     * @brief PV for testing an Input PV to get a string
     */
    nds::PVVariableIn<std::string> m_string_RBVPV;

    //dataType_t: dataTimespec -> timespec
    /*
     * @brief PV for testing an Output PV to set a timespec
     */
    nds::PVDelegateOut<timespec> m_timespec_PV;
    /*
     * @brief PV for testing an Input PV to get a timespec
     */
    nds::PVVariableIn<timespec> m_timespec_RBVPV;

    //dataType_t: dataTimespecArray -> std::vector<timespec>
    /*
     * @brief PV for testing an Input PV to get an array of timespec
     */
    nds::PVDelegateOut<std::vector<timespec>> m_timespecArray_PV;
    /*
     * @brief PV for testing an Input PV to get an array of timespec
     */
    nds::PVVariableIn<std::vector<timespec>> m_timespecArray_RBVPV;

    //dataType_t: dataTimestamp -> timestamp_t
    /*
     * @brief PV for testing an Output PV to set a timestamp
     */
    nds::PVDelegateOut<nds::timestamp_t> m_timestamp_PV;
    /*
     * @brief PV for testing an Input PV to get a timestamp
     */
    nds::PVVariableIn<nds::timestamp_t> m_timestamp_RBVPV;

    /* @brief Output PV to increase Source PVs for data sharing purpose */
        nds::PVDelegateOut<std::int32_t> m_dataSharingHandler_PV;

        /* @brief Output PV to increase Source PVs for data sharing purpose */
        nds::PVDelegateOut<std::int32_t> m_delegateOutWithName_PV;

    /* @brief Reference PV for sending data to other PVs with integer data. */
    nds::PVVariableIn<std::int32_t> m_sourceInt_PV;

        /* @brief Reference PV for sending data to other PVs with double data. */
        nds::PVVariableIn<double> m_sourceDouble_PV;

        /* @brief Reference PV for replication from PVs with integer data. */
        nds::PVVariableIn<std::int32_t> m_targetReplicationInt_PV;

        /* @brief Reference PV for replication from PVs with double data. */
        nds::PVVariableIn<double> m_targetReplicationDouble_PV;

        /* @brief Reference PV for subscription from PVs with integer data. */
        nds::PVVariableOut<std::int32_t> m_targetSubscriptionInt_PV;

        /* @brief Reference PV for subscription from PVs with double data. */
        nds::PVVariableOut<double> m_targetSubscriptionDouble_PV;

    /**
     * @brief PV for testing unsubscription from a Delegate PV
     */
    nds::PVDelegateOut<std::int32_t> m_unsubscribe_PV;
    nds::PVVariableIn<std::int32_t> m_unsubscribeValue_RBVPV;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MEMBER METHODS THAT SUPPORT SPECIFIC OPERATIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Set the PVs with a default value
     */
    void initializePVs(void);

    /**
     * Delegate Function to test data sharing between PVs.
     * It increases the stored value in the source in one.
     * The received integer argument enables to select which
     * source PV is considered.
     */
    void increaseDataSouce(const timespec&, const std::int32_t&);

    void delegateOutWithName(const timespec& timestamp, const std::int32_t& data, const std::string& name);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIST OF WRITERS METHODS FOR ALL SUPPORTED TYPES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Method to be used when an write action is required on the integer PV
     */
    void setInt(const timespec&, const std::int32_t&);
    /**
     * @brief Method to be used when an write action is required on the integer64 PV
     */
    void setInt64(const timespec&, const std::int64_t&);
    /**
     * @brief Method to be used when an write action is required on the double PV
     */
    void setFloat(const timespec&, const float&);

    /**
     * @brief Method to be used when an write action is required on the double PV
     */
    void setDouble(const timespec&, const double&);

    /**
     * @brief Method to be used when an write action is required on the booleans array PV
     */
    void setBoolArray(const timespec&, const std::vector<bool>&);

    /**
     * @brief Method to be used when an write action is required on the unsigned integers (8) array PV
     */
    void setUInt8Array(const timespec&, const std::vector<std::uint8_t>&);

    /**
     * @brief Method to be used when an write action is required on the unsigned integers (16) array PV
     */
    void setUInt16Array(const timespec&, const std::vector<std::uint16_t>&);

    /**
     * @brief Method to be used when an write action is required on the unsigned integers (32) array PV
     */
    void setUInt32Array(const timespec&, const std::vector<std::uint32_t>&);

    /**
     * @brief Method to be used when an write action is required on the integers (8) array PV
     */
    void setInt8Array(const timespec&, const std::vector<std::int8_t>&);

    /**
     * @brief Method to be used when an write action is required on the integers (16) array PV
     */
    void setInt16Array(const timespec&, const std::vector<std::int16_t>&);

    /**
     * @brief Method to be used when an write action is required on the integers (32) array PV
     */
    void setInt32Array(const timespec&, const std::vector<std::int32_t>&);
    /**
     * @brief Method to be used when an write action is required on the integers (64) array PV
     */
    void setInt64Array(const timespec&, const std::vector<std::int64_t>&);
    /**
     * @brief Method to be used when an write action is required on the doubles array PV
     */
    void setFloatArray(const timespec&, const std::vector<float>&);

    /**
     * @brief Method to be used when an write action is required on the doubles array PV
     */
    void setDoubleArray(const timespec&, const std::vector<double>&);

    /**
     * @brief Method to be used when an write action is required on the string PV
     */
    void setString(const timespec&, const std::string&);

    /**
     * @brief Method to be used when an write action is required on the timespec PV
     */
    void setTimespec(const timespec&, const timespec&);

    /**
     * @brief Method to be used when an write action is required on the timespecs array PV
     */
    void setTimespecArray(const timespec&, const std::vector<timespec>&);

    /**
     * @brief Method to be used when an write action is required on the timestamp PV
     */
    void setTimestamp(const timespec&, const nds::timestamp_t&);

    /**
     * @brief Method to be used when an write action is required on the unsubscribe PV
     */
    void setUnsubscribe(const timespec&, const std::int32_t&, const std::string& name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIST OF INITIALIZERS METHODS FOR ALL SUPPORTED TYPES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Method to be used at initialization time for the integer PV
     */
    void initInt(timespec* timestamp, std::int32_t* value);
    /**
     * @brief Method to be used at initialization time for the integer64 PV
     */
    void initInt64(timespec* timestamp, std::int64_t* value);
    /**
     * @brief Method to be used at initialization time for the float PV
     */
    void initFloat(timespec* timestamp, float* value);

    /**
     * @brief Method to be used at initialization time for the double PV
     */
    void initDouble(timespec* timestamp, double* value);

    /**
     * @brief Method to be used at initialization time for the booleans array PV
     */
    void initBoolArray(timespec* timestamp, std::vector<bool>* values);

    /**
     * @brief Method to be used at initialization time for the unsigned integers (8) array PV
     */
    void initUInt8Array(timespec* timestamp, std::vector<std::uint8_t>* values);

    /**
     * @brief Method to be used at initialization time for the unsigned integers (16) array PV
     */
    void initUInt16Array(timespec* timestamp, std::vector<std::uint16_t>* values);

    /**
     * @brief Method to be used at initialization time for the unsigned integers (32) array PV
     */
    void initUInt32Array(timespec* timestamp, std::vector<std::uint32_t>* values);
    /**
     * @brief Method to be used at initialization time for the integers (64) array PV
     */
    void initInt64Array(timespec* timestamp, std::vector<std::int64_t>* values);
    /**
     * @brief Method to be used at initialization time for the integers (8) array PV
     */
    void initInt8Array(timespec* timestamp, std::vector<std::int8_t>* values);

    /**
     * @brief Method to be used at initialization time for the integers (16) array PV
     */
    void initInt16Array(timespec* timestamp, std::vector<std::int16_t>* values);

    /**
     * @brief Method to be used at initialization time for the integers (32) array PV
     */
    void initInt32Array(timespec* timestamp, std::vector<std::int32_t>* values);

    /**
     * @brief Method to be used at initialization time for the doubles array PV
     */
    void initFloatArray(timespec* timestamp, std::vector<float>* values);

    /**
     * @brief Method to be used at initialization time for the doubles array PV
     */
    void initDoubleArray(timespec* timestamp, std::vector<double>* values);

    /**
     * @brief Method to be used at initialization time for the string PV
     */
    void initString(timespec* timestamp, std::string* value);

    /**
     * @brief Method to be used at initialization time for the timespec PV
     */
    void initTimespec(timespec* timestamp, timespec* value);

    /**
     * @brief Method to be used at initialization time for the timespecs array PV
     */
    void initTimespecArray(timespec* timestamp, std::vector<timespec>* values);

    /**
     * @brief Method to be used at initialization time for the timestamp PV
     */
    void initTimestamp(timespec* timestamp, nds::timestamp_t* value);

    /**
     * @brief Method to be used at initialization time for the handler of data sharing
     */
    void initHandler(timespec* timestamp, std::int32_t* value);
    
    /**
     * @brief Reference to factory
     */
    nds::Factory& m_factory;

};

#endif // DEVICE_PVS_H_
