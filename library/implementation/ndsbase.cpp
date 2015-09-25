#include "../include/nds3/base.h"
#include "../include/nds3/port.h"
#include "../include/nds3/factory.h"
#include "baseImpl.h"
#include "factoryBaseImpl.h"

namespace nds
{

Base::Base()
{
}

Base::Base(std::shared_ptr<BaseImpl> impl): m_pImplementation(impl)
{
}

Base::Base(const Base& right): m_pImplementation(right.m_pImplementation)
{
}

Base& Base::operator=(const Base& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

Base::~Base()
{
}

Port Base::getPort()
{
    return Port(m_pImplementation->getPort());
}


std::string Base::getComponentName() const
{
    return m_pImplementation->getComponentName();
}


std::string Base::getFullName() const
{
    return m_pImplementation->getFullName();
}


std::string Base::getFullNameFromPort() const
{
    return m_pImplementation->getFullNameFromPort();
}

void Base::initialize(void* pDeviceObject, const controlSystem_t controlSystem /*  = controlSystem_t::default */)
{
    if(m_pImplementation->getParent().get() != 0)
    {
        throw std::logic_error("You can initialize only the root nodes");
    }

    FactoryBaseImpl* pFactory = Factory(controlSystem).m_pFactory;
    m_pImplementation->initialize(*pFactory);

    pFactory->holdNode(pDeviceObject, m_pImplementation);
}

timespec Base::getTimestamp() const
{
    return m_pImplementation->getTimestamp();
}

void Base::setTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_pImplementation->setTimestampDelegate(timestampDelegate);
}


}
