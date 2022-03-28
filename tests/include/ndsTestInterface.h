#ifndef NDSTESTINTERFACE_H
#define NDSTESTINTERFACE_H

#include <vector>

#include <nds3/impl/interfaceBaseImpl.h>
#include <nds3/definitions.h>
#include <iostream>
#ifdef _WIN32
#include <nds3/timing.h>
#else
#include <unistd.h>
#endif
#include <chrono>
#include <mutex>

namespace nds
{

namespace tests
{

class TestControlSystemInterfaceImpl: public InterfaceBaseImpl
{
public:
    TestControlSystemInterfaceImpl(const std::string& fullName);
    ~TestControlSystemInterfaceImpl();

    /*
     * For testing purposes we give access to the control system from
     *  the test units
     */
    static TestControlSystemInterfaceImpl* getInstance(const std::string& fullName);

    virtual void registerPV(std::shared_ptr<PVBaseImpl> pv);

    virtual void deregisterPV(std::shared_ptr<PVBaseImpl> pv);

    virtual void registrationTerminated();

    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::int32_t& value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::int64_t& value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const float& value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const double& value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<bool> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint8_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint16_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint32_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int8_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int16_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int32_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int64_t> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<float> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<double> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::string & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const timespec & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<timespec> & value, const statusPV_t& status = statusPV_t::success);
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const timestamp_t & value, const statusPV_t& status = statusPV_t::success);


    template<typename T>
    void readCSValue(const std::string& pvName, timespec* pTimestamp, T* pValue);

    template<typename T>
    void writeCSValue(const std::string& pvName, const timespec& timestamp, const T& value);

    void getPushedInt32(const std::string& pvName, const timespec*& pTime, const int32_t*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedInt64(const std::string& pvName, const timespec*& pTime, const int64_t*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedFloat(const std::string& pvName, const timespec*& pTime, const float*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedDouble(const std::string& pvName, const timespec*& pTime, const double*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorBool(const std::string& pvName, const timespec*& pTime, const std::vector<bool>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorUint8(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint8_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorUint16(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint16_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorUint32(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint32_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorInt8(const std::string& pvName, const timespec*& pTime, const std::vector<std::int8_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorInt16(const std::string& pvName, const timespec*& pTime, const std::vector<std::int16_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorInt32(const std::string& pvName, const timespec*& pTime, const std::vector<std::int32_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorInt64(const std::string& pvName, const timespec*& pTime, const std::vector<std::int64_t>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorFloat(const std::string& pvName, const timespec*& pTime, const std::vector<float>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorDouble(const std::string& pvName, const timespec*& pTime, const std::vector<double>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedString(const std::string& pvName, const timespec*& pTime, const std::string*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedTimespec(const std::string& pvName, const timespec*& pTime, const timespec*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedVectorTimespec(const std::string& pvName, const timespec*& pTime, const std::vector<timespec>*& pValue, const std::uint32_t& timeoutMs=1000);
    void getPushedTimestamp(const std::string& pvName, const timespec*& pTime, const timestamp_t*& pValue, const std::uint32_t& timeoutMs=1000);

    virtual void registerReporter(reporter_t reporter);
    virtual void report(FILE* file , int details);

private:

    const std::string m_name;

    typedef std::map<std::string, PVBaseImpl*> registeredPVs_t;
    registeredPVs_t m_registeredPVs;

    reporter_t reporter;
    std::mutex mtx;


    template <typename T>
    class PushedValues
    {
    public:
        PushedValues(): m_firstUsed(0), m_firstAvailable(0){}

        static const size_t m_historyBits = 10;
        static const size_t m_historyPositionMask = (0x1 << m_historyBits) - 1;

        std::array<T, (0x1 << m_historyBits)> m_values;
        std::array<timespec, (0x1 << m_historyBits)> m_timestamps;

        size_t m_firstUsed, m_firstAvailable;


        void storeValue(const timespec& timestamp, const T& value)
        {
            m_values[m_firstAvailable & m_historyPositionMask] = value;
            m_timestamps[m_firstAvailable++ & m_historyPositionMask] = timestamp;
            if((m_firstUsed & m_historyPositionMask) == (m_firstAvailable & m_historyPositionMask))
            {
                ++m_firstUsed;
            }
        }

        void getValue(const timespec*& pTimestamp, const T*& pValue)
        {
            if((m_firstUsed & m_historyPositionMask) == (m_firstAvailable & m_historyPositionMask))
            {
                throw std::runtime_error("No pushed data to be retrieved");
            }
            pTimestamp = &(m_timestamps[m_firstUsed & m_historyPositionMask]);
            pValue = &(m_values[m_firstUsed++ & m_historyPositionMask]);

        }
    };

    std::map<std::string, PushedValues<std::int32_t> >m_pushedInt32;
    std::map<std::string, PushedValues<std::int64_t> >m_pushedInt64;
    std::map<std::string, PushedValues<float> >m_pushedFloat;
    std::map<std::string, PushedValues<double> >m_pushedDouble;
    std::map<std::string, PushedValues<std::vector<bool> > >m_pushedVectorBool;
    std::map<std::string, PushedValues<std::vector<std::uint8_t> > >m_pushedVectorUint8;
    std::map<std::string, PushedValues<std::vector<std::uint16_t> > >m_pushedVectorUint16;
    std::map<std::string, PushedValues<std::vector<std::uint32_t> > >m_pushedVectorUint32;
    std::map<std::string, PushedValues<std::vector<std::int8_t> > >m_pushedVectorInt8;
    std::map<std::string, PushedValues<std::vector<std::int16_t> > >m_pushedVectorInt16;
    std::map<std::string, PushedValues<std::vector<std::int32_t> > >m_pushedVectorInt32;
    std::map<std::string, PushedValues<std::vector<std::int64_t> > >m_pushedVectorInt64;
    std::map<std::string, PushedValues<std::vector<float> > >m_pushedVectorFloat;
    std::map<std::string, PushedValues<std::vector<double> > >m_pushedVectorDouble;
    std::map<std::string, PushedValues<std::string> >m_pushedString;
    std::map<std::string, PushedValues<timespec> >m_pushedTimespec;
    std::map<std::string, PushedValues<std::vector<timespec>> >m_pushedVectorTimespec;
    std::map<std::string, PushedValues<timestamp_t> >m_pushedTimestamp;


    template <typename T>
    void storePushedData(const std::string& pvName,
                         std::map<std::string, PushedValues<T> >& storeInto,
                         const timespec& timestamp,
                         const T& value)
    {
        try{
            storeInto.at(pvName).storeValue(timestamp, value);
        }
        catch(const std::out_of_range& ex){
            mtx.lock();
            storeInto[pvName].storeValue(timestamp, value);
            mtx.unlock();
        }
    }

    template <typename T>
    void getPushedData(const std::string& pvName,
                           std::map<std::string, PushedValues<T> >& storeInto,
                           const timespec*& pTime,
                           const T*& pValue,
                           const std::uint32_t& timeoutMs=1000)
    {
        std::int64_t usTimeout = timeoutMs*1000; //Convert timeout from milliseconds to microseconds
        bool getData=false;
        std::chrono::system_clock::time_point begin = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point end;

        do{
            try {
                storeInto[pvName].getValue(pTime, pValue);
                getData=true; //This flag only turns true if data is available
            } catch (const std::runtime_error& ex) {
                end = std::chrono::system_clock::now();
                if(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() >=usTimeout){
                    std::string msg = ex.what();
                    msg += " (PV name: " + pvName + ")";
                    throw std::runtime_error(msg);
                }
                ::usleep(100); //rest for a while
            }
        }while(!getData);
    }

};

}//namespace tests

}//namespace nds
#endif // NDSTESTINTERFACE_H
