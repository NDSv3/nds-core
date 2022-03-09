#include <mutex>
#include <functional>
#include <iostream>
#include <limits>

#include <nds3/nds.h>

#include "DevicePVs.h"


#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DevicePVs*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

DevicePVs::DevicePVs(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t & parameters):
                        m_Name(DeviceName),
                        maxArrayElements(5),
                        m_int_PV(nds::PVDelegateOut<std::int32_t>("Integer", std::bind(&DevicePVs::setInt, this, std::placeholders::_1, std::placeholders::_2))),
                        m_int64_PV(nds::PVDelegateOut<std::int64_t>("Integer64", std::bind(&DevicePVs::setInt64, this, std::placeholders::_1, std::placeholders::_2))),
                        m_float_PV(nds::PVDelegateOut<float>("Float", std::bind(&DevicePVs::setFloat, this, std::placeholders::_1, std::placeholders::_2))),
                        m_double_PV(nds::PVDelegateOut<double>("Double", std::bind(&DevicePVs::setDouble, this, std::placeholders::_1, std::placeholders::_2))),
                        m_boolArray_PV(nds::PVDelegateOut<std::vector<bool>>("BoolArray", std::bind(&DevicePVs::setBoolArray, this, std::placeholders::_1, std::placeholders::_2))),
                        m_uInt8Array_PV(nds::PVDelegateOut<std::vector<std::uint8_t>>("UInt8Array", std::bind(&DevicePVs::setUInt8Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_uInt16Array_PV(nds::PVDelegateOut<std::vector<std::uint16_t>>("UInt16Array", std::bind(&DevicePVs::setUInt16Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_uInt32Array_PV(nds::PVDelegateOut<std::vector<std::uint32_t>>("UInt32Array", std::bind(&DevicePVs::setUInt32Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_int8Array_PV(nds::PVDelegateOut<std::vector<std::int8_t>>("Int8Array", std::bind(&DevicePVs::setInt8Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_int16Array_PV(nds::PVDelegateOut<std::vector<std::int16_t>>("Int16Array", std::bind(&DevicePVs::setInt16Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_int32Array_PV(nds::PVDelegateOut<std::vector<std::int32_t>>("Int32Array", std::bind(&DevicePVs::setInt32Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_int64Array_PV(nds::PVDelegateOut<std::vector<std::int64_t>>("Int64Array", std::bind(&DevicePVs::setInt64Array, this, std::placeholders::_1, std::placeholders::_2))),
                        m_float32Array_PV(nds::PVDelegateOut<std::vector<float>>("FloatArray", std::bind(&DevicePVs::setFloatArray, this, std::placeholders::_1, std::placeholders::_2))),
                        m_float64Array_PV(nds::PVDelegateOut<std::vector<double>>("DoubleArray", std::bind(&DevicePVs::setDoubleArray, this, std::placeholders::_1, std::placeholders::_2))),
                        m_string_PV(nds::PVDelegateOut<std::string>("String", std::bind(&DevicePVs::setString, this, std::placeholders::_1, std::placeholders::_2))),
                        m_timespec_PV(nds::PVDelegateOut<timespec>("Timespec", std::bind(&DevicePVs::setTimespec, this, std::placeholders::_1, std::placeholders::_2))),
                        m_timespecArray_PV(nds::PVDelegateOut<std::vector<timespec>>("TimespecArray", std::bind(&DevicePVs::setTimespecArray, this, std::placeholders::_1, std::placeholders::_2))),
                        m_timestamp_PV(nds::PVDelegateOut<nds::timestamp_t>("Timestamp", std::bind(&DevicePVs::setTimestamp, this, std::placeholders::_1, std::placeholders::_2))),
                        m_dataSharingHandler_PV(nds::PVDelegateOut<std::int32_t>("ShareData", std::bind(&DevicePVs::increaseDataSouce, this, std::placeholders::_1, std::placeholders::_2))),
                        m_delegateOutWithName_PV(nds::PVDelegateOut<std::int32_t>("delegateOutWithName",NULL, std::bind(&DevicePVs::delegateOutWithName, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3))),
                        m_unsubscribe_PV(nds::PVDelegateOut<std::int32_t>("delegateUnsubscribe", NULL, std::bind(&DevicePVs::setUnsubscribe, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))),
                        m_factory(factory)
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
     * control system thread
     */

    nds::Port rootNode(DeviceName);

    nds::namedParameters_t::const_iterator findParam =  parameters.find("INIT");


    if (findParam != parameters.end() && findParam->second=="YES") {
        m_int_PV = nds::PVDelegateOut<std::int32_t>("Integer",
                std::bind(&DevicePVs::setInt, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initInt, this, std::placeholders::_1, std::placeholders::_2));
        m_int64_PV = nds::PVDelegateOut<std::int64_t>("Integer64",
                std::bind(&DevicePVs::setInt64, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initInt64, this, std::placeholders::_1, std::placeholders::_2));
        m_float_PV = nds::PVDelegateOut<float>("Float",
                std::bind(&DevicePVs::setFloat, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initFloat, this, std::placeholders::_1, std::placeholders::_2));
        m_double_PV = nds::PVDelegateOut<double>("Double",
                std::bind(&DevicePVs::setDouble, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initDouble, this, std::placeholders::_1, std::placeholders::_2));
        m_boolArray_PV = nds::PVDelegateOut<std::vector<bool>>("BoolArray",
                std::bind(&DevicePVs::setBoolArray, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initBoolArray, this, std::placeholders::_1, std::placeholders::_2));
        m_uInt8Array_PV = nds::PVDelegateOut<std::vector<std::uint8_t>>("UInt8Array",
                std::bind(&DevicePVs::setUInt8Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initUInt8Array, this, std::placeholders::_1, std::placeholders::_2));
        m_uInt16Array_PV = nds::PVDelegateOut<std::vector<std::uint16_t>>("UInt16Array",
                std::bind(&DevicePVs::setUInt16Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initUInt16Array, this, std::placeholders::_1, std::placeholders::_2));
        m_uInt32Array_PV = nds::PVDelegateOut<std::vector<std::uint32_t>>("UInt32Array",
                std::bind(&DevicePVs::setUInt32Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initUInt32Array, this, std::placeholders::_1, std::placeholders::_2));
        m_int8Array_PV = nds::PVDelegateOut<std::vector<std::int8_t>>("Int8Array",
                std::bind(&DevicePVs::setInt8Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initInt8Array, this, std::placeholders::_1, std::placeholders::_2));
        m_int16Array_PV = nds::PVDelegateOut<std::vector<std::int16_t>>("Int16Array",
                std::bind(&DevicePVs::setInt16Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initInt16Array, this, std::placeholders::_1, std::placeholders::_2));
        m_int32Array_PV = nds::PVDelegateOut<std::vector<std::int32_t>>("Int32Array",
                std::bind(&DevicePVs::setInt32Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initInt32Array, this, std::placeholders::_1, std::placeholders::_2));
        m_int64Array_PV = nds::PVDelegateOut<std::vector<std::int64_t>>("Int64Array",
                std::bind(&DevicePVs::setInt64Array, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initInt64Array, this, std::placeholders::_1, std::placeholders::_2));
        m_float32Array_PV = nds::PVDelegateOut<std::vector<float>>("FloatArray",
                std::bind(&DevicePVs::setFloatArray, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initFloatArray, this, std::placeholders::_1, std::placeholders::_2));
        m_float64Array_PV = nds::PVDelegateOut<std::vector<double>>("DoubleArray",
                std::bind(&DevicePVs::setDoubleArray, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initDoubleArray, this, std::placeholders::_1, std::placeholders::_2));
        m_string_PV = nds::PVDelegateOut<std::string>("String",
                std::bind(&DevicePVs::setString, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initString, this, std::placeholders::_1, std::placeholders::_2));
        m_timespec_PV = nds::PVDelegateOut<timespec>("Timespec",
                std::bind(&DevicePVs::setTimespec, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initTimespec, this, std::placeholders::_1, std::placeholders::_2));
        m_timespecArray_PV = nds::PVDelegateOut<std::vector<timespec>>("TimespecArray",
                std::bind(&DevicePVs::setTimespecArray, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initTimespecArray, this, std::placeholders::_1, std::placeholders::_2));
        m_timestamp_PV = nds::PVDelegateOut<nds::timestamp_t>("Timestamp",
                std::bind(&DevicePVs::setTimestamp, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&DevicePVs::initTimestamp, this, std::placeholders::_1, std::placeholders::_2));

        m_dataSharingHandler_PV = nds::PVDelegateOut<std::int32_t>("ShareData",
                                std::bind(&DevicePVs::increaseDataSouce, this, std::placeholders::_1, std::placeholders::_2),
                                std::bind(&DevicePVs::initHandler, this, std::placeholders::_1, std::placeholders::_2));

    }

    //Add the children PVs
    rootNode.addChild(m_unsubscribe_PV);
    m_unsubscribe_PV.setDescription("PV to unsubscribe itself");
    m_unsubscribe_PV.setScanType(nds::scanType_t::passive);

    m_unsubscribeValue_RBVPV = rootNode.addChild(nds::PVVariableIn<std::int32_t>("unsubscribeValue_RBVPV"));
    m_unsubscribeValue_RBVPV.setDescription("Check that the PV has been executed (RBV)");
    m_unsubscribeValue_RBVPV.setScanType(nds::scanType_t::interrupt);

    rootNode.addChild(m_int_PV);
    m_int_PV.setDescription("Integer to be set");
    m_int_PV.setScanType(nds::scanType_t::passive);

    m_int_RBVPV = rootNode.addChild(nds::PVVariableIn<std::int32_t>("Integer_RBV"));
    m_int_RBVPV.setDescription("Integer that has been set (RBV)");
    m_int_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_int_RBVPV.processAtInit(true);

    rootNode.addChild(m_int64_PV);
    m_int64_PV.setDescription("Integer64 to be set");
    m_int64_PV.setScanType(nds::scanType_t::passive);

    m_int64_RBVPV = rootNode.addChild(nds::PVVariableIn<std::int64_t>("Integer64_RBV"));
    m_int64_RBVPV.setDescription("Integer64 that has been set (RBV)");
    m_int64_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_int64_RBVPV.processAtInit(true);

    rootNode.addChild(m_float_PV);
    m_float_PV.setDescription("Float to be set");
    m_float_PV.setScanType(nds::scanType_t::passive);

    m_float_RBVPV = rootNode.addChild(nds::PVVariableIn<float>("Float_RBV"));
    m_float_RBVPV.setDescription("Float that has been set (RBV)");
    m_float_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_float_RBVPV.processAtInit(true);

    rootNode.addChild(m_double_PV);
    m_double_PV.setDescription("Double to be set");
    m_double_PV.setScanType(nds::scanType_t::passive);

    m_double_RBVPV = rootNode.addChild(nds::PVVariableIn<double>("Double_RBV"));
    m_double_RBVPV.setDescription("Double that has been set (RBV)");
    m_double_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_double_RBVPV.processAtInit(true);

    rootNode.addChild(m_boolArray_PV);
    m_boolArray_PV.setDescription("Bool array to be set");
    m_boolArray_PV.setScanType(nds::scanType_t::passive);
    m_boolArray_PV.setMaxElements(maxArrayElements);

    m_boolArray_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<bool>>("BoolArray_RBV"));
    m_boolArray_RBVPV.setDescription("Bool that has been set (RBV)");
    m_boolArray_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_boolArray_RBVPV.setMaxElements(maxArrayElements);
    m_boolArray_RBVPV.processAtInit(true);

    rootNode.addChild(m_uInt8Array_PV);
    m_uInt8Array_PV.setDescription("UInt8Array to be set");
    m_uInt8Array_PV.setScanType(nds::scanType_t::passive);
    m_uInt8Array_PV.setMaxElements(maxArrayElements);

    m_uInt8Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::uint8_t>>("UInt8Array_RBV"));
    m_uInt8Array_RBVPV.setDescription("UInt8Array that has been set (RBV)");
    m_uInt8Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_uInt8Array_RBVPV.setMaxElements(maxArrayElements);
    m_uInt8Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_uInt16Array_PV);
    m_uInt16Array_PV.setDescription("UInt16Array to be set");
    m_uInt16Array_PV.setScanType(nds::scanType_t::passive);
    m_uInt16Array_PV.setMaxElements(maxArrayElements);

    m_uInt16Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::uint16_t>>("UInt16Array_RBV"));
    m_uInt16Array_RBVPV.setDescription("UInt16Array that has been set (RBV)");
    m_uInt16Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_uInt16Array_RBVPV.setMaxElements(maxArrayElements);
    m_uInt16Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_uInt32Array_PV);
    m_uInt32Array_PV.setDescription("UInt32Array to be set");
    m_uInt32Array_PV.setScanType(nds::scanType_t::passive);
    m_uInt32Array_PV.setMaxElements(maxArrayElements);

    m_uInt32Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::uint32_t>>("UInt32Array_RBV"));
    m_uInt32Array_RBVPV.setDescription("UInt32Array that has been set (RBV)");
    m_uInt32Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_uInt32Array_RBVPV.setMaxElements(maxArrayElements);
    m_uInt32Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_int8Array_PV);
    m_int8Array_PV.setDescription("Int8Array to be set");
    m_int8Array_PV.setScanType(nds::scanType_t::passive);
    m_int8Array_PV.setMaxElements(maxArrayElements);

    m_int8Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::int8_t>>("Int8Array_RBV"));
    m_int8Array_RBVPV.setDescription("Int8Array that has been set (RBV)");
    m_int8Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_int8Array_RBVPV.setMaxElements(maxArrayElements);
    m_int8Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_int16Array_PV);
    m_int16Array_PV.setDescription("Int16Array to be set");
    m_int16Array_PV.setScanType(nds::scanType_t::passive);
    m_int16Array_PV.setMaxElements(maxArrayElements);

    m_int16Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::int16_t>>("Int16Array_RBV"));
    m_int16Array_RBVPV.setDescription("Int16Array that has been set (RBV)");
    m_int16Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_int16Array_RBVPV.setMaxElements(maxArrayElements);
    m_int16Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_int32Array_PV);
    m_int32Array_PV.setDescription("Int32Array to be set");
    m_int32Array_PV.setScanType(nds::scanType_t::passive);
    m_int32Array_PV.setMaxElements(maxArrayElements);

    m_int32Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::int32_t>>("Int32Array_RBV"));
    m_int32Array_RBVPV.setDescription("Int32Array that has been set (RBV)");
    m_int32Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_int32Array_RBVPV.setMaxElements(maxArrayElements);
    m_int32Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_int64Array_PV);
    m_int64Array_PV.setDescription("Int64Array to be set");
    m_int64Array_PV.setScanType(nds::scanType_t::passive);
    m_int64Array_PV.setMaxElements(maxArrayElements);

    m_int64Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<std::int64_t>>("Int64Array_RBV"));
    m_int64Array_RBVPV.setDescription("Int64Array that has been set (RBV)");
    m_int64Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_int64Array_RBVPV.setMaxElements(maxArrayElements);
    m_int64Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_float32Array_PV);
    m_float32Array_PV.setDescription("FloatArray to be set");
    m_float32Array_PV.setScanType(nds::scanType_t::passive);
    m_float32Array_PV.setMaxElements(maxArrayElements);

    m_float32Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<float>>("FloatArray_RBV"));
    m_float32Array_RBVPV.setDescription("FloatArray that has been set (RBV)");
    m_float32Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_float32Array_RBVPV.setMaxElements(maxArrayElements);
    m_float32Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_float64Array_PV);
    m_float64Array_PV.setDescription("DoubleArray to be set");
    m_float64Array_PV.setScanType(nds::scanType_t::passive);
    m_float64Array_PV.setMaxElements(maxArrayElements);

    m_float64Array_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<double>>("DoubleArray_RBV"));
    m_float64Array_RBVPV.setDescription("DoubleArray that has been set (RBV)");
    m_float64Array_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_float64Array_RBVPV.setMaxElements(maxArrayElements);
    m_float64Array_RBVPV.processAtInit(true);

    rootNode.addChild(m_string_PV);
    m_string_PV.setDescription("String to be set");
    m_string_PV.setScanType(nds::scanType_t::passive);
    m_string_PV.setMaxElements(maxArrayElements);

    m_string_RBVPV = rootNode.addChild(nds::PVVariableIn<std::string>("String_RBV"));
    m_string_RBVPV.setDescription("String that has been set (RBV)");
    m_string_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_string_RBVPV.setMaxElements(maxArrayElements);
    m_string_RBVPV.processAtInit(true);

    rootNode.addChild(m_timespec_PV);
    m_timespec_PV.setDescription("Timespec to be set");
    m_timespec_PV.setScanType(nds::scanType_t::passive);
    m_timespec_PV.setMaxElements(2);

    m_timespec_RBVPV = rootNode.addChild(nds::PVVariableIn<timespec>("Timespec_RBV"));
    m_timespec_RBVPV.setDescription("Timespec that has been set (RBV)");
    m_timespec_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_timespec_RBVPV.setMaxElements(2);
    m_timespec_RBVPV.processAtInit(true);

    rootNode.addChild(m_timespecArray_PV);
    m_timespecArray_PV.setDescription("TimespecArray to be set");
    m_timespecArray_PV.setScanType(nds::scanType_t::passive);
    m_timespecArray_PV.setMaxElements(2*maxArrayElements);

    m_timespecArray_RBVPV = rootNode.addChild(nds::PVVariableIn<std::vector<timespec>>("TimespecArray_RBV"));
    m_timespecArray_RBVPV.setDescription("TimespecArray that has been set (RBV)");
    m_timespecArray_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_timespecArray_RBVPV.setMaxElements(4*maxArrayElements);
    m_timespecArray_RBVPV.processAtInit(true);


    rootNode.addChild(m_timestamp_PV);
    m_timestamp_PV.setDescription("Timestamp to be set");
    m_timestamp_PV.setScanType(nds::scanType_t::passive);
    m_timestamp_PV.setMaxElements(4);

    m_timestamp_RBVPV = rootNode.addChild(nds::PVVariableIn<nds::timestamp_t>("Timestamp_RBV"));
    m_timestamp_RBVPV.setDescription("Timestamp that has been set (RBV)");
    m_timestamp_RBVPV.setScanType(nds::scanType_t::interrupt);
    m_timestamp_RBVPV.setMaxElements(4);


    //Pvs for testing data subscription and replication

        rootNode.addChild(m_dataSharingHandler_PV);
        m_dataSharingHandler_PV.setDescription("Increase selected source PVs");
        m_dataSharingHandler_PV.setScanType(nds::scanType_t::passive);

        rootNode.addChild(m_delegateOutWithName_PV);

    m_sourceInt_PV = rootNode.addChild(nds::PVVariableIn<std::int32_t>("SourceInt"));
    m_sourceInt_PV.setDescription("Input PV to send integer data");
    m_sourceInt_PV.setScanType(nds::scanType_t::interrupt);
    m_sourceInt_PV.processAtInit(true);

        m_sourceDouble_PV = rootNode.addChild(nds::PVVariableIn<double>("SourceDouble"));
        m_sourceDouble_PV.setDescription("Input PV to send double data");
        m_sourceDouble_PV.setScanType(nds::scanType_t::interrupt);
        m_sourceDouble_PV.processAtInit(true);

        m_targetReplicationInt_PV = rootNode.addChild(nds::PVVariableIn<std::int32_t>("ReplicatedInt"));
        m_targetReplicationInt_PV.setDescription("Input PV to receive integer data");
        m_targetReplicationInt_PV.setScanType(nds::scanType_t::interrupt);
        m_targetReplicationInt_PV.processAtInit(true);

        m_targetReplicationDouble_PV = rootNode.addChild(nds::PVVariableIn<double>("ReplicatedDouble"));
        m_targetReplicationDouble_PV.setDescription("Input PV to receive double data");
        m_targetReplicationDouble_PV.setScanType(nds::scanType_t::interrupt);
        m_targetReplicationDouble_PV.processAtInit(true);

        m_targetSubscriptionInt_PV = rootNode.addChild(nds::PVVariableOut<std::int32_t>("SubscribedInt"));
        m_targetSubscriptionInt_PV.setDescription("Output PV to receive integer data");
        m_targetSubscriptionInt_PV.setScanType(nds::scanType_t::passive);
        m_targetSubscriptionInt_PV.processAtInit(true);

        m_targetSubscriptionDouble_PV = rootNode.addChild(nds::PVVariableOut<double>("SubscribedDouble"));
        m_targetSubscriptionDouble_PV.setDescription("Output PV to receive double data");
        m_targetSubscriptionDouble_PV.setScanType(nds::scanType_t::passive);
        m_targetSubscriptionDouble_PV.processAtInit(true);

    // We have declared all the nodes and PVs in our Device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);

    //Stream information for debugging purposes
    rootNode.setLogLevel(nds::logLevel_t::debug);
    rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger:The DevicePVs is created" << std::endl;
    ndsDebugStream(rootNode) << "This is the ndsDebugStream: The DevicePVs named " << rootNode.getFullName() << " is created" << std::endl;

    initializePVs();

}


DevicePVs::~DevicePVs()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_Name);

}

DevicePVs* DevicePVs::getInstance(const std::string& DeviceName)
{

    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DevicePVs*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }

    return findDevice->second;
}


void DevicePVs::initializePVs(void){
    int ns = 0;
    timespec timestamp = {NDS_EPOCH, ns};
    setInt(timestamp, 1);

    timestamp = {NDS_EPOCH, ns++};
    setInt64(timestamp, 1);

    timestamp = {NDS_EPOCH, ns++};
    setFloat(timestamp, 1.5);

    timestamp = {NDS_EPOCH, ns++};
    setDouble(timestamp, 1.1);

    std::vector<bool> boolArray = {true, true, false, true};
    timestamp = {NDS_EPOCH, ns++};
    setBoolArray(timestamp, boolArray);

    std::vector<std::uint8_t> uInt8Array = {0,1,2};
    timestamp = {NDS_EPOCH, ns++};
    setUInt8Array(timestamp, uInt8Array);

    std::vector<std::uint16_t> uInt16Array = {3,4};
    timestamp = {NDS_EPOCH, ns++};
    setUInt16Array(timestamp, uInt16Array);

    std::vector<std::uint32_t> uInt32Array = {5,6,7};
    timestamp = {NDS_EPOCH, ns++};
    setUInt32Array(timestamp, uInt32Array);

    std::vector<std::int8_t> int8Array = {0,-1,2};
    timestamp = {NDS_EPOCH, ns++};
    setInt8Array(timestamp, int8Array);

    std::vector<std::int16_t> int16Array = {3,-4};
    timestamp = {NDS_EPOCH, ns++};
    setInt16Array(timestamp, int16Array);

    std::vector<std::int32_t> int32Array = {-5,6,-7};
    timestamp = {NDS_EPOCH, ns++};
    setInt32Array(timestamp, int32Array);

    std::vector<std::int64_t> int64Array = {-8,6,-9};
    timestamp = {NDS_EPOCH, ns++};
    setInt64Array(timestamp, int64Array);

    std::vector<float> floatArray = {-0.5,0.5,2.5};
    timestamp = {NDS_EPOCH, ns++};
    setFloatArray(timestamp, floatArray);

    std::vector<double> doubleArray = {-0.1,0.2,1.5};
    timestamp = {NDS_EPOCH, ns++};
    setDoubleArray(timestamp, doubleArray);

    std::string string = "text";
    timestamp = {NDS_EPOCH, ns++};
    setString(timestamp, string);

    timespec timeTest = {11111111,2243354};
    timestamp = {NDS_EPOCH, ns++};
    setTimespec(timestamp, timeTest);

    std::vector<timespec> timesTest ={{1,2}, {3,4}};
    timestamp = {NDS_EPOCH, ns++};
    setTimespecArray(timestamp, timesTest);

    nds::timestamp_t timestampData = {{10,1}, 0, true};
    timestamp = {NDS_EPOCH, ns++};
    setTimestamp(timestamp, timestampData);

        timestamp = {NDS_EPOCH, ns++};
        std::int32_t sourceInt = -1;
        m_sourceInt_PV.setValue(timestamp, sourceInt);
        m_sourceInt_PV.push(timestamp, sourceInt);

        double sourceDouble = -1.1;
        timestamp = {NDS_EPOCH, ns++};
        m_sourceDouble_PV.setValue(timestamp, sourceDouble);
        m_sourceDouble_PV.push(timestamp, sourceDouble);

        timestamp = {NDS_EPOCH, ns++};
        std::int32_t replicatedInt = -2;
        m_targetReplicationInt_PV.setValue(timestamp, replicatedInt);
        m_targetReplicationInt_PV.push(timestamp, replicatedInt);

        double replicatedDouble = -2.2;
        timestamp = {NDS_EPOCH, ns++};
        m_targetReplicationDouble_PV.setValue(timestamp, replicatedDouble);
        m_targetReplicationDouble_PV.push(timestamp, replicatedDouble);

        timestamp = {NDS_EPOCH, ns++};
        std::int32_t subscribedInt = -3;
        m_targetSubscriptionInt_PV.write(timestamp, subscribedInt);

        double subscribedDouble = -3.3;
        timestamp = {NDS_EPOCH, ns++};
        m_targetSubscriptionDouble_PV.write(timestamp, subscribedDouble);

}

void DevicePVs::delegateOutWithName(const timespec& /*timestamp*/, const std::int32_t& /*data*/, const std::string& name) {

    std::cout<<"============================"<<std::endl;
    std::cout<<"Called PV Delegate with name: "<<name<<std::endl;
    std::cout<<"============================"<<std::endl;

}

void DevicePVs::increaseDataSouce(const timespec& timestamp, const std::int32_t& data) {
  timespec time;
  switch (data) {
    case 0:
      std::int32_t sourceInt;
      m_sourceInt_PV .read(&time, &sourceInt);
      sourceInt++;
      m_sourceInt_PV.setValue(timestamp, sourceInt);
      m_sourceInt_PV.push(timestamp, sourceInt);
      break;
    case 1:
      double sourceDouble;
      m_sourceDouble_PV.read(&time, &sourceDouble);
      sourceDouble++;
      m_sourceDouble_PV.setValue(timestamp, sourceDouble);
      m_sourceDouble_PV.push(timestamp, sourceDouble);
      break;
    default:
      break;
  }
}

void DevicePVs::setInt(const timespec& timestamp, const std::int32_t& data){
    m_int_RBVPV.setValue(timestamp, data);
    m_int_RBVPV.push(timestamp, data);
}

void DevicePVs::setInt64(const timespec& timestamp, const std::int64_t& data){
    m_int64_RBVPV.setValue(timestamp, data);
    m_int64_RBVPV.push(timestamp, data);
}

void DevicePVs::setFloat(const timespec& timestamp, const float& data){
    m_float_RBVPV.setValue(timestamp, data);
    m_float_RBVPV.push(timestamp, data);
}

void DevicePVs::setDouble(const timespec& timestamp, const double& data){
    m_double_RBVPV.setValue(timestamp, data);
    m_double_RBVPV.push(timestamp, data);
}

void DevicePVs::setBoolArray(const timespec& timestamp, const std::vector<bool>& data){
    m_boolArray_RBVPV.setValue(timestamp, data);
    m_boolArray_RBVPV.push(timestamp, data);
}

void DevicePVs::setUInt8Array(const timespec& timestamp, const std::vector<std::uint8_t>& data){
    m_uInt8Array_RBVPV.setValue(timestamp, data);
    m_uInt8Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setUInt16Array(const timespec& timestamp, const std::vector<std::uint16_t>& data){
    m_uInt16Array_RBVPV.setValue(timestamp, data);
    m_uInt16Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setUInt32Array(const timespec& timestamp, const std::vector<std::uint32_t>& data){
    m_uInt32Array_RBVPV.setValue(timestamp, data);
    m_uInt32Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setInt8Array(const timespec& timestamp, const std::vector<std::int8_t>& data){
    m_int8Array_RBVPV.setValue(timestamp, data);
    m_int8Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setInt16Array(const timespec& timestamp, const std::vector<std::int16_t>& data){
    m_int16Array_RBVPV.setValue(timestamp, data);
    m_int16Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setInt32Array(const timespec& timestamp, const std::vector<std::int32_t>& data){
    m_int32Array_RBVPV.setValue(timestamp, data);
    m_int32Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setInt64Array(const timespec& timestamp, const std::vector<std::int64_t>& data){
    m_int64Array_RBVPV.setValue(timestamp, data);
    m_int64Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setFloatArray(const timespec& timestamp, const std::vector<float>& data){
    m_float32Array_RBVPV.setValue(timestamp, data);
    m_float32Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setDoubleArray(const timespec& timestamp, const std::vector<double>& data){
    m_float64Array_RBVPV.setValue(timestamp, data);
    m_float64Array_RBVPV.push(timestamp, data);
}

void DevicePVs::setString(const timespec& timestamp, const std::string& data){
    m_string_RBVPV.setValue(timestamp, data);
    m_string_RBVPV.push(timestamp, data);
}

void DevicePVs::setTimespec(const timespec& timestamp, const timespec& data){
    m_timespec_RBVPV.setValue(timestamp, data);
    m_timespec_RBVPV.push(timestamp, data);
}

void DevicePVs::setTimespecArray(const timespec& timestamp, const std::vector<timespec>& data){
    m_timespecArray_RBVPV.setValue(timestamp, data);
    m_timespecArray_RBVPV.push(timestamp, data);
}

void DevicePVs::setTimestamp(const timespec& timestamp, const nds::timestamp_t& data){
    m_timestamp_RBVPV.setValue(timestamp, data);
    m_timestamp_RBVPV.push(timestamp, data);
}

void DevicePVs::setUnsubscribe(const timespec& timestamp, const std::int32_t& data, const std::string& name){
    m_unsubscribeValue_RBVPV.setValue(timestamp, data);
    m_unsubscribeValue_RBVPV.push(timestamp, data);
    //Unsubscribe this PV
    m_factory.unsubscribe(name.substr(1));
}

void DevicePVs::initInt(timespec* timestamp, std::int32_t* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = -2147483648;
}

void DevicePVs::initInt64(timespec* timestamp, std::int64_t* value) {
    *timestamp = {NDS_EPOCH, 10};
    *value = std::numeric_limits<std::int64_t>::min();
}

void DevicePVs::initFloat(timespec* timestamp, float* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 4e8;
}

void DevicePVs::initDouble(timespec* timestamp, double* value) {
    *timestamp = {NDS_EPOCH, 20};
    *value = 5e8;
}

void DevicePVs::initBoolArray(timespec* timestamp, std::vector<bool>* values) {
    *timestamp = {NDS_EPOCH, 30};
    *values = std::vector<bool>(2, true); //Vector with 2 data to true.
}

void DevicePVs::initUInt8Array(timespec* timestamp,
        std::vector<std::uint8_t>* values) {
    *timestamp = {NDS_EPOCH, 40};
    *values = std::vector<std::uint8_t>(5, 255); //Vector with 5 data to 255.
}

void DevicePVs::initUInt16Array(timespec* timestamp,
        std::vector<std::uint16_t>* values) {
    *timestamp = {NDS_EPOCH, 50};
    *values = std::vector<std::uint16_t>(4, 65535); //Vector with 4 data to 65535.
}

void DevicePVs::initUInt32Array(timespec* timestamp,
        std::vector<std::uint32_t>* values) {
    *timestamp = {NDS_EPOCH, 60};
    *values = std::vector<std::uint32_t>(3, 4294967295); //Vector with 3 data to 4294967295.
}

void DevicePVs::initInt8Array(timespec* timestamp,
        std::vector<std::int8_t>* values) {
    *timestamp = {NDS_EPOCH, 70};
    *values = std::vector<std::int8_t>(5, -128); //Vector with 5 data to -128.
}

void DevicePVs::initInt16Array(timespec* timestamp,
        std::vector<std::int16_t>* values) {
    *timestamp = {NDS_EPOCH, 80};
    *values = std::vector<std::int16_t>(1, -32768); //Vector with 1 data to -32768.
}

void DevicePVs::initInt32Array(timespec* timestamp,
        std::vector<std::int32_t>* values) {
    *timestamp = {NDS_EPOCH, 90};
    *values = std::vector<std::int32_t>(1, -2147483648); //Vector with 1 data to -2147483648.
}

void DevicePVs::initInt64Array(timespec* timestamp,
        std::vector<std::int64_t>* values) {
    *timestamp = {NDS_EPOCH, 100};
    *values = std::vector<std::int64_t>(1, std::numeric_limits<std::int64_t>::min()); //Vector with 1 data to -9223372036854775808.
}

void DevicePVs::initFloatArray(timespec* timestamp,
        std::vector<float>* values) {
    *timestamp = {NDS_EPOCH, 100};
    *values = std::vector<float>(4, 8e12); //Vector with 4 data to 8e12.
}

void DevicePVs::initDoubleArray(timespec* timestamp,
        std::vector<double>* values) {
    *timestamp = {NDS_EPOCH, 100};
    *values = std::vector<double>(4, 9e12); //Vector with 4 data to 9e12.
}

void DevicePVs::initString(timespec* timestamp, std::string* value) {
    *timestamp = {NDS_EPOCH, 110};
    *value = "string";
}

void DevicePVs::initTimespec(timespec* timestamp, timespec* value) {
    *timestamp = {NDS_EPOCH, 120};
    *value = {123, 456};
}

void DevicePVs::initTimespecArray(timespec* timestamp,
        std::vector<timespec>* values) {
    *timestamp = {NDS_EPOCH, 120};
    *values = std::vector<timespec>(5, {123, 456}); //Vector with 5 data to {123, 456}.
}

void DevicePVs::initTimestamp(timespec* timestamp, nds::timestamp_t* value) {
    *timestamp = {NDS_EPOCH, 120};
    *value = {{123, 456}, 0, false};
}

void DevicePVs::initHandler(timespec* timestamp, std::int32_t* value) {
        *timestamp = {NDS_EPOCH, 130};
        *value = 0;
}

//NDS_DEFINE_DRIVER(DevicePVs, DevicePVs)

/*
 * Allocation function
 ********************
*/
void* DevicePVs::allocateDevice(nds::Factory& factory,
                     const std::string& DeviceName,
                     const nds::namedParameters_t& parameters) {
  return new DevicePVs(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 **********************
*/
void DevicePVs::deallocateDevice(void* DeviceName) {
  delete (DevicePVs*)DeviceName;
}



