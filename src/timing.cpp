/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/timing.h"
#include "../include/nds3/impl/timingImpl.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace nds {

///////////////////////////////////////// TIME SUPPORT /////////////////////////////////////////

// ---------------- Constructors and assignment operator -------------------//
Timing::Timing(): Node()
{
}

Timing::Timing(const std::string& name,
    stateChange_t switchOnFunction,
    stateChange_t switchOffFunction,
    stateChange_t startFunction,
    stateChange_t stopFunction,
    stateChange_t recoverFunction,
    allowChange_t allowStateChangeFunction,
    readerTime_t PV_Time_Reader,
autoEnable_t autoEnable):
    Node(std::shared_ptr<TimingImpl>(new TimingImpl(name,
    switchOnFunction,
    switchOffFunction,
    startFunction,
    stopFunction,
    recoverFunction,
    allowStateChangeFunction,
    PV_Time_Reader,
    autoEnable)))
{
}

Timing::Timing(const std::string& name, const TimingArgs_t& handlerTM):
        Node(std::shared_ptr<TimingImpl>(new TimingImpl(name, handlerTM)))
{
}

Timing::Timing(const Timing& right):
     Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

Timing& Timing::operator=(const Timing& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

// -------------------- Functions common to all nodes ----------------------- //
void Timing::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<TimingImpl >(m_pImplementation)->
          setStartTimestampDelegate(timestampDelegate);
}

void Timing::push(const timespec& timestamp, const timespec& data)
{
    std::static_pointer_cast<TimingImpl>(m_pImplementation)->push(timestamp, data);
}

timespec Timing::getStartTimestamp() const
{
    return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getStartTimestamp();
}

nds::state_t Timing::getState()
{
    return std::static_pointer_cast<TimingImpl>(m_pImplementation)->getState();
}

// -------------------------------- Getters --------------------------------- //
timespec Timing::getTime() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getTime();
}

std::string Timing::getHumanTime() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getHumanTime();
}

double Timing::getClkFrequency() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getClkFrequency();
}

std::int32_t Timing::getClkMultiplier() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getClkMultiplier();
}

std::int32_t Timing::getSyncStatus() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getSyncStatus();
}

std::int32_t Timing::getSecsLastSync() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getSecsLastSync();
}

timespec Timing::getRefTimeBase() {
  return std::static_pointer_cast<TimingImpl >(m_pImplementation)->getRefTimeBase();
}

// -------------------------------- Setters --------------------------------- //
void Timing::setTime(const timespec& timestamp, const timespec& value) {
  std::static_pointer_cast<TimingImpl >(m_pImplementation)->setTime(timestamp, value);
}

void Timing::setClkFrequency(const timespec& timestamp, const double& value) {
  std::static_pointer_cast<TimingImpl >(m_pImplementation)->setClkFrequency(timestamp, value);
}

void Timing::setClkMultiplier(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimingImpl >(m_pImplementation)->setClkMultiplier(timestamp, value);
}

void Timing::setSyncStatus(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimingImpl >(m_pImplementation)->setSyncStatus(timestamp, value);
}

void Timing::setSecsLastSync(const timespec& timestamp, const std::int32_t& value) {
  std::static_pointer_cast<TimingImpl >(m_pImplementation)->setSecsLastSync(timestamp, value);
}

void Timing::setRefTimeBase(const timespec& timestamp, const timespec& value) {
  std::static_pointer_cast<TimingImpl >(m_pImplementation)->setRefTimeBase(timestamp, value);
}

void Timing::setState(const nds::state_t& newState)
{
    std::static_pointer_cast<TimingImpl>(m_pImplementation)->setState(newState);
}
} //namespace nds

#ifdef _MSC_VER
// https://github.com/esa/pykep/issues/47
//C-file part
int clock_gettime(int, struct timespec *spec)
{
    __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
    wintime -= 116444736000000000ull;  //1jan1601 to 1jan1970
    spec->tv_sec = wintime / 10000000ull;           //seconds
    spec->tv_nsec = wintime % 10000000ull * 100;      //nano-seconds
    return 0;
}
#endif

#ifdef _WIN32
void usleep(int microsecs)
{
	Sleep(microsecs / 1000);
}
void sleep(int secs)
{
	Sleep(secs * 1000);
}
#endif

