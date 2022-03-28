/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSINTERFACEIMPL_H
#define NDSINTERFACEIMPL_H

#include <list>
#include <memory>
#include "nds3/impl/pvBaseImpl.h"

namespace nds
{

/**
 * @brief Represents the interface between NDS and the control system API.
 *
 * Classes derived from this one implement specific code for the control system they
 * interact with.
 */
class NDS3_API InterfaceBaseImpl
{
public:
    virtual ~InterfaceBaseImpl();

    /**
     * @brief Register a PV (attribute) with the control system.
     *
     * The interface will take care of calling the PV's read and write
     * functions when the control system requires it.
     *
     * In order to push values immediately to the control system use push().
     *
     * @param pv the PV to be registered
     */
    virtual void registerPV(std::shared_ptr<PVBaseImpl> pv) = 0;

    /**
     * @brief Deregister a PV (attribute) from the control system.
     *
     * @param pv the PV to be deregistered
     */
    virtual void deregisterPV(std::shared_ptr<PVBaseImpl> pv) = 0;

    /**
     * @brief Called by the nodes after all the PVs have been registered.
     *        The interface may commit all the registered PV at this point or
     *        perform additional tasks.
     */
    virtual void registrationTerminated() = 0;
    /**
     * @brief Register a Reporter function. Reporters allow the control system display
     * more complete information, e.g., with a file that the driver can write to.
     * @param reporter function object to be registered.
     */
    virtual void registerReporter(reporter_t reporter) =0;

    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::int32_t& value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::int64_t& value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const float& value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const double& value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<bool> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint8_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint16_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint32_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int8_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int16_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int32_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int64_t> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<float> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<double> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::string & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const timespec & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<timespec> & value, const statusPV_t& status = statusPV_t::success) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const timestamp_t & value, const statusPV_t& status = statusPV_t::success) = 0;


};

}
#endif // NDSINTERFACEIMPL_H
