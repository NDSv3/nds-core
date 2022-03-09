#ifndef DEVICE_H_
#define DEVICE_H_

#include <nds3/nds.h>

//Class that declares and implement a fictional
class Device
{

public:
    //Constructor
    Device(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
    //Destructor
    ~Device();



    //Allocation method
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    //Deallocation method
    static void deallocateDevice(void* deviceName);


    /*
     * For test purposes we make it possible to retrieve running instances of
     *  the device
     */
    static Device* getInstance(const std::string& deviceName);

private:

    //Device name
    std::string m_name;

    // Device State Machine
    nds::StateMachine m_Device_stateMachine;

    //Methods to control Device state machine.
    void switchOn_Device();  //Called to switch on the Device
    void switchOff_Device(); //Called to switch off the Device
    void start_Device();     //Called to start the Device
    void stop_Device();      //Called to stop the Device 
    void recover_Device();   //Called to recover the Device from
    //a failure.
    bool allow_Device_Change(const nds::state_t, const nds::state_t, const nds::state_t); //Called to verify if a state change is allowed

    //DataAcquisition Node
    nds::DataAcquisition<std::vector<double> > m_DataAcquisition;

    //Methods to control DataAcquisition Node state machine.
    void switchOn_DataAcquisition();  //Called to switch on the DataAcquisition node.
    void switchOff_DataAcquisition(); //Called to switch off the DataAcquisition node.
    void start_DataAcquisition();     //Called to start the DataAcquisition node.
    void stop_DataAcquisition();      //Called to stop the DataAcquisition node.
    void recover_DataAcquisition();   //Called to recover the DataAcquisition node from a failure.
    bool allow_DataAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t); //Called to verify if a state change is allowed

    //DataAcquisition setters
    void PV_DataAcquisition_Gain_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Offset_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Bandwidth_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Resolution_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Impedance_Writer(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Coupling_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SignalRefType_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_Ground_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_DMAEnable_Writer(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SamplingRate_Writer(const timespec& timestamp, const double& value);

    /**
     * DataAcquisition initializers
     */
    void PV_DataAcquisition_Gain_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Offset_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Bandwidth_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Resolution_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Impedance_Initializer(timespec* timestamp, double* value);
    void PV_DataAcquisition_Coupling_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SignalRefType_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_Ground_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_DMAEnable_Initializer(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SamplingRate_Initializer(timespec* timestamp, double* value);


    //Function that continuously acquires data. It is launched by
    //start_DataAcquisition() in a separate thread.
    void DataAcquisition_thread_body();

    //A thread that runs DataAcquisition_thread_body().
    nds::Thread m_DataAcquisition_Thread;

    //A boolean flag that stop the DataAcquisition loop in
    //DataAcquisition_thread_body() when true.
    volatile bool m_bStop_DataAcquisition;

    // PVs
    nds::PVVariableIn<std::vector<double>> m_VarIn_vDBL;
    nds::PVVariableOut<std::vector<double>> m_VarOut_vDBL;



};
#endif
