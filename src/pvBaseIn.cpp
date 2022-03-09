/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/pvBaseIn.h"
#include "nds3/impl/pvBaseInImpl.h"

namespace nds
{

PVBaseIn::PVBaseIn()
{

}

PVBaseIn::PVBaseIn(std::shared_ptr<PVBaseInImpl> pvImpl): PVBase(std::static_pointer_cast<PVBaseImpl>(pvImpl))
{
}

template<typename T>
void PVBaseIn::read(timespec* pTimestamp, T* pValue) const
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->read(pTimestamp, pValue);
}

template<typename T>
void PVBaseIn::push(const timespec& timestamp, const T& value, const statusPV_t& status)
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->push(timestamp, value, status);
}

void PVBaseIn::setDecimation(const std::uint32_t decimation)
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->setDecimation(decimation);
}

void PVBaseIn::replicateFrom(const std::string &sourceInputPVName)
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->replicateFrom(sourceInputPVName);
}

template void NDS3_API PVBaseIn::read<std::int32_t>(timespec*, std::int32_t*) const;
template void NDS3_API PVBaseIn::read<std::int64_t>(timespec*, std::int64_t*) const;
template void NDS3_API PVBaseIn::read<float>(timespec*, float*) const;
template void NDS3_API PVBaseIn::read<double>(timespec*, double*) const;
template void NDS3_API PVBaseIn::read<std::vector<bool> >(timespec*, std::vector<bool>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::uint8_t> >(timespec*, std::vector<std::uint8_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::uint16_t> >(timespec*, std::vector<std::uint16_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::uint32_t> >(timespec*, std::vector<std::uint32_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::int8_t> >(timespec*, std::vector<std::int8_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::int16_t> >(timespec*, std::vector<std::int16_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::int32_t> >(timespec*, std::vector<std::int32_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<std::int64_t> >(timespec*, std::vector<std::int64_t>*) const;
template void NDS3_API PVBaseIn::read<std::vector<float> >(timespec*, std::vector<float>*) const;
template void NDS3_API PVBaseIn::read<std::vector<double> >(timespec*, std::vector<double>*) const;
template void NDS3_API PVBaseIn::read<std::string >(timespec*, std::string*) const;
template void NDS3_API PVBaseIn::read<timespec >(timespec*, timespec*) const;
template void NDS3_API PVBaseIn::read<std::vector<timespec> >(timespec*, std::vector<timespec>*) const;
template void NDS3_API PVBaseIn::read<timestamp_t>(timespec*, timestamp_t*) const;

template void NDS3_API PVBaseIn::push<std::int32_t>(const timespec&, const std::int32_t&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::int64_t>(const timespec&, const std::int64_t&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<float>(const timespec&, const float&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<double>(const timespec&, const double&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<bool> >(const timespec&, const std::vector<bool>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::uint16_t> >(const timespec&, const std::vector<std::uint16_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::uint32_t> >(const timespec&, const std::vector<std::uint32_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::int16_t> >(const timespec&, const std::vector<std::int16_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<std::int64_t> >(const timespec&, const std::vector<std::int64_t>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<float> >(const timespec&, const std::vector<float>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<double> >(const timespec&, const std::vector<double>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::string >(const timespec&, const std::string&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<timespec >(const timespec&, const timespec&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<std::vector<timespec> >(const timespec&, const std::vector<timespec>&, const statusPV_t&);
template void NDS3_API PVBaseIn::push<timestamp_t>(const timespec&, const timestamp_t&, const statusPV_t&);

}

