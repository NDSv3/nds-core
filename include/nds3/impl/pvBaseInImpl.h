/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVBASEINIMPL_H
#define NDSPVBASEINIMPL_H

#include <string>
#include <set>
#include <mutex>
#include <atomic>
#include <queue>

#include "nds3/definitions.h"
#include "nds3/impl/baseImpl.h"
#include "nds3/impl/pvBaseImpl.h"

namespace nds
{

class PVBase;
class PVBaseOutImpl;

/**
 * @brief Custom implementation of a recursive_mutex with a public counter.
 */
class recursive_mutex_counter
{
private:
    std::recursive_mutex _mutex;
    std::atomic<unsigned int> _counter;
public:
    /**
     * @brief Constructor
     */
    recursive_mutex_counter() : _mutex(), _counter(0) {};
    recursive_mutex_counter(recursive_mutex_counter&) = delete;
    void operator=(recursive_mutex_counter&) = delete;
    void lock() { _mutex.lock(); ++_counter; }
    bool try_lock() { bool result = _mutex.try_lock(); _counter += result; return result; }
    void unlock() { --_counter; _mutex.unlock(); }
    /**
     * @brief Retrieves the number of times the mutex has been taken recursively
     */
    unsigned get_count() { return _counter; }
};

/**
 * @brief Base class for all the PVs.
 */
class PVBaseInImpl: public PVBaseImpl
{
public:
    /**
     * @brief Constructor.
     *
     * @param name the PV's name
     * @param pvType
     */
    PVBaseInImpl(const std::string& name, const inputPvType_t pvType);

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;
    virtual void read(timespec* pTimestamp, std::int64_t* pValue) const;
    virtual void read(timespec* pTimestamp, float* pValue) const;
    virtual void read(timespec* pTimestamp, double* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<bool>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint16_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint32_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int16_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int32_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int64_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<float>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<double>* pValue) const;
    virtual void read(timespec* pTimestamp, std::string* pValue) const;
    virtual void read(timespec* pTimestamp, timespec* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<timespec>* pValue) const;
    virtual void read(timespec* pTimestamp, timestamp_t* pValue) const;

    /**
     * @brief Pushes data to the control system and to the subscribed PVs.
     *
     * @tparam T the data type
     * @param timestamp    the timestamp related to the data
     * @param value        the data to push
     * @param status       the status to be shown in the control system (if it is supported)
     */
    template<typename T>
    void push(const timespec& timestamp, const T& value, const statusPV_t& status = statusPV_t::success);

    /**
     * @brief Subscribe an output PV to this PV.
     *
     * The subscribed PV will receive all the data pushed or written into this
     *  PV.
     *
     * @param pReceiver the PV that will receive al the data
     */
    void subscribeReceiver(PVBaseOutImpl* pReceiver);

    /**
     * @brief Unsubscribe an output PV from this PV.
     *
     * @param pReceiver the output PV to unsubscribe
     */
    void unsubscribeReceiver(PVBaseOutImpl* pReceiver);

    /**
     * @brief Replicate the data written to this PV to another input PV.
     *
     * @param pDestination the PV into which the data must be copied
     */
    void replicateTo(PVBaseInImpl* pDestination);

    /**
     * @brief Stop the replication of data to the specified destination PV.
     *
     * @param pDestination the destination to unsubscribe from replication
     */
    void stopReplicationTo(PVBaseInImpl* pDestination);

    /**
     * @brief Set the decimation factor.
     *
     * The decimation factor is the ration pushed_data/data_passed_to_control_system.
     *
     * @param decimation  the decimation factor
     */
    void setDecimation(const std::uint32_t decimation);

    /**
     * @brief Specifies an input PV from which the data must be copied.
     *
     * Any push operation made on the source input PV will be replicated
     *  to this PV.
     *
     * @param sourceInputPVName the full name of the input PV from which the data
     *                           must be copied
     */
    void replicateFrom(const std::string& sourceInputPVName);

    virtual dataDirection_t getDataDirection() const;

    virtual std::string buildFullExternalName(const FactoryBaseImpl& controlSystem) const;
    virtual std::string buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const;

protected:
    std::string buildFullExternalName(const FactoryBaseImpl& controlSystem, const bool bStopAtPort) const;

    inputPvType_t m_pvType;

    /**
     * @brief List of subscribed PVs.
     */
    typedef std::set<PVBaseOutImpl*> subscribersList_t;

    /**
     * @brief List of subscribed PVs.
     */
    subscribersList_t m_subscriberOutputPVs;

    /**
     * @brief List of destination input PVs.
     */
    typedef std::set<PVBaseInImpl*> destinationList_t;

    /**
     * @brief List of PVs to which the data must be pushed or written
     */
    destinationList_t m_replicationDestinationPVs;

    /**
     * @brief List of PVs to unsubscribe
     */
    typedef std::queue<PVBaseOutImpl*> unsubscribeList_t;

    /**
     * @brief List of PVs to unsubscribe
     */
    unsubscribeList_t m_unsubscribeOutputPVs;

    /**
     * @brief List of PVs to stop replication
     */
    typedef std::queue<PVBaseInImpl*> stopReplicationList_t;

    /**
     * @brief List of PVs to stop replication
     */
    stopReplicationList_t m_stopReplicationDestinationPVs;

    //std::mutex m_lockSubscribersList; ///< Lock the access to m_subscriberOutputPVs.
    nds::recursive_mutex_counter m_lockSubscribersList; ///< Lock the access to m_subscriberOutputPVs.
    std::uint32_t m_decimationFactor;  ///< Decimation factor.
    std::uint32_t m_decimationCount;   ///< Keeps track of the received data/vs data pushed to the control system.

private:
    parameters_t commandReplicate(const parameters_t& parameters);
    parameters_t commandDecimation(const parameters_t& parameters);

};

}
#endif // NDSPVBASEINIMPL_H
