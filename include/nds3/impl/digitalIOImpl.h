/*
 * Nominal Device Support v.3 (NDS3)
 *
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSDIGITALIOIMPL_H
#define NDSDIGITALIOIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/digitalIO.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class DigitalIOImpl: public NodeImpl
{
public:
    DigitalIOImpl(const std::string& name,
                    size_t maxElements,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
                    writerVectorBool_t PV_dataOutMask_Writer,
                    writerDouble_t PV_voltLevelHigh_Writer,
                    writerDouble_t PV_voltLevelLow_Writer,
                    writerVectorBool_t PV_ChannelDir_Writer,
                    autoEnable_t autoEnable);
    /**
     * @brief Constructs the DigitalIO node by means of its structure of functions.
     * @param name Name of the node.
     * @param maxElements Maximum size of data to be generated.
     * @param handlerDIO Structure with delegate functiosn that perform the required actions.
     */
    DigitalIOImpl( const std::string& name,
               size_t maxElements,
               const DigitalIOArgs_t<T>& handlerDIO);


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

    std::vector<bool> getDataOutMask();
    size_t getMaxElements();
    double getVoltLevelHigh();
    double getVoltLevelLow();
    std::vector<bool> getChannelDir();

    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    void setDataOutMask(const timespec& timestamp, const std::vector<bool>& value);
    void setVoltLevelHigh(const timespec& timestamp, const double& value);
    void setVoltLevelLow(const timespec& timestamp, const double& value);
    void setChannelDir(const timespec& timestamp, const std::vector<bool>& value);

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
    std::shared_ptr<PVVariableInImpl<T> > m_DataIn_PV;
    std::shared_ptr<PVVariableOutImpl<T> > m_DataOut_PV;

    std::shared_ptr<PVDelegateOutImpl<std::vector<bool>>> m_DataOutMask_PV;
    std::shared_ptr<PVVariableInImpl<std::vector<bool>>> m_DataOutMask_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

    std::shared_ptr<PVDelegateOutImpl<double> > m_VoltLevelHigh_PV;
    std::shared_ptr<PVVariableInImpl <double> > m_VoltLevelHigh_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_VoltLevelLow_PV;
    std::shared_ptr<PVVariableInImpl <double> > m_VoltLevelLow_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::vector<bool>>> m_ChannelDir_PV;
    std::shared_ptr<PVVariableInImpl<std::vector<bool>>> m_ChannelDir_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_NumberOfPushedDataBlocks;

private:
    /**
    * @brief Common source code to define the body of any constructor.
    * @param maxElements Maximum size of data to be generated.
    * @param handlerDIO Structure containing both compulsory and optional functions.
    */
    void constructorBody(size_t maxElements, const DigitalIOArgs_t<T>& handlerDIO);


};

}
#endif // NDSDIGITALIOIMPL_H

