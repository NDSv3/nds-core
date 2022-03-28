/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSDATAACQUISITIONIMPL_H
#define NDSDATAACQUISITIONIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/stateMachine.h"
#include "nds3/dataAcquisition.h"

#include <vector>

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class DataAcquisitionImpl: public NodeImpl
{
public:
    DataAcquisitionImpl(const std::string& name,
            size_t maxElements,
            stateChange_t switchOnFunction,
            stateChange_t switchOffFunction,
            stateChange_t startFunction,
            stateChange_t stopFunction,
            stateChange_t recoverFunction,
            allowChange_t allowStateChangeFunction,
            writerDouble_t PV_Gain_Writer,
            writerDouble_t PV_Offset_Writer,
            writerDouble_t PV_Bandwidth_Writer,
            writerDouble_t PV_Resolution_Writer,
            writerDouble_t PV_Impedance_Writer,
            writerInt32_t PV_Coupling_Writer,
            writerInt32_t PV_SignalRefType_Writer,
            writerInt32_t PV_Ground_Writer,
            writerInt32_t PV_DMAEnable_Writer,
            writerDouble_t PV_SamplingRate_Writer,
            autoEnable_t autoEnable
            );

    /**
     * @brief Constructs the DadaAcquisition node by means of its structure of functions.
     * @param name Name of the node.
     * @param maxElements Maximum number of data to be acquired.
     * @param handlerDAQ Structure with delegate functions that perform the required actions.
     */
    DataAcquisitionImpl(const std::string& name,
            size_t maxElements,
            const DataAcquisitionArgs_t& handlerDAQ);

    /**
     * @brief Specifies the function to call to get the acquisition start timestamp.
     *
     * The function is called only once at each start of the acquisition and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void push(const timespec& timestamp, const T& data);


    size_t getMaxElements();
    double getGain();
    double getOffset();
    double getBandwidth();
    double getResolution();
    double getImpedance();
    int32_t getCoupling();
    int32_t getSignalRefType();
    int32_t getGround();
    int32_t getNumberOfPushedDataBlocks();
    int32_t getDMABufferSize();
    int32_t getDMAEnable();
    int32_t getDMANumChannels();
    int32_t getDMAFrameType();
    int32_t getDMASampleSize();
    double getSamplingRate();

    void setGain(const timespec& timestamp, const double& value);
    void setOffset(const timespec& timestamp, const double& value);
    void setBandwidth(const timespec& timestamp, const double& value);
    void setResolution(const timespec& timestamp, const double& value);
    void setImpedance(const timespec& timestamp, const double& value);
    void setCoupling(const timespec& timestamp, const std::int32_t& value);
    void setSignalRefType(const timespec& timestamp, const std::int32_t& value);
    void setGround(const timespec& timestamp, const std::int32_t& value);
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    void setDMABufferSize(const timespec& timestamp, const std::int32_t& value);
    void setDMAEnable(const timespec& timestamp, const std::int32_t& value);
    void setDMANumChannels(const timespec& timestamp, const std::int32_t& value);
    void setDMAFrameType(const timespec& timestamp, const std::int32_t& value);
    void setDMASampleSize(const timespec& timestamp, const std::int32_t& value);
    void setSamplingRate(const timespec& timestamp, const double& value);


    /**
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();

    /**
     * @brief Called to obtain the actual state of the State Machine of the Node
     *
     * @return The actual state of the State Machine of the Node
     */
    nds::state_t getState();

    /**
     * @brief Called to change the actual state of the State Machine of the Node
     *
     * @param newState New state to set the State Machine of the node
     */
    void setState(const nds::state_t& newState);


protected:
    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Acquisition start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    // PVs

    std::shared_ptr<PVVariableInImpl<T> > m_Data_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_DecimationType_PV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Offset_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Offset_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Gain_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Gain_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Bandwidth_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Bandwidth_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Resolution_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Resolution_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Impedance_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Impedance_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Coupling_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Coupling_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SignalRefType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SignalRefType_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Ground_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Ground_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_NumberOfPushedDataBlocks;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DMABufferSize_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DMAEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DMAEnable_RBVPV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DMANumChannels_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DMAFrameType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DMASampleSize_PV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_SamplingRate_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_SamplingRate_RBVPV;


private:
    /**
    * @brief Common source code to define the body of any constructor.
    * @param maxElements Maximum number of data to be handled.
    * @param hanlderDAQ Structure containing both compulsory and optional functions.
    */
    void constructorBody(size_t maxElements, const DataAcquisitionArgs_t& hanlderDAQ);

};

}
#endif // NDSDATAACQUISITIONIMPL_H

