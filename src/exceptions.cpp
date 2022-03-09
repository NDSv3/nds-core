/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/exceptions.h"

namespace nds
{

NdsError::NdsError(const std::string &what, statusPV_t status): std::runtime_error(what), status(status)
{
}

StateMachineError::StateMachineError(const std::string& what, statusPV_t status): NdsError(what, status)
{
}

StateMachineRollBack::StateMachineRollBack(const std::string& what, statusPV_t status): StateMachineError(what, status)
{
}

StateMachineTransitionDenied::StateMachineTransitionDenied(const std::string& what, statusPV_t status): StateMachineError(what, status)
{
}

StateMachineNoSuchTransition::StateMachineNoSuchTransition(const std::string& what, statusPV_t status): StateMachineError(what, status)
{
}

TimeConversionError::TimeConversionError(const std::string &what, statusPV_t status): NdsError(what, status)
{
}

NoPortDefinedError::NoPortDefinedError(const std::string &what, statusPV_t status ): std::logic_error(what), pvStatus(status)
{
}

FactoryError::FactoryError(const std::string &what, statusPV_t status ): NdsError(what, status)
{
}

DirectoryNotFoundError::DirectoryNotFoundError(const std::string &what, statusPV_t status): FactoryError(what, status)
{
}

DriverNotFound::DriverNotFound(const std::string &what, statusPV_t status ): FactoryError(what, status)
{
}

DriverAlreadyRegistered::DriverAlreadyRegistered(const std::string &what, statusPV_t status): FactoryError(what, status)
{
}

DriverDoesNotExportRegistrationFunctions::DriverDoesNotExportRegistrationFunctions(const std::string &what, statusPV_t status ): FactoryError(what, status)
{
}

ControlSystemNotFound::ControlSystemNotFound(const std::string &what, statusPV_t status ): FactoryError(what, status)
{
}

DeviceNotAllocated::DeviceNotAllocated(const std::string &what, statusPV_t status ): FactoryError(what, status)
{
}

DeviceAlreadyCreated::DeviceAlreadyCreated(const std::string &what, statusPV_t status ): FactoryError(what, status)
{
}

PVAlreadyDeclared::PVAlreadyDeclared(const std::string& what, statusPV_t status ): FactoryError(what, status)
{
}

MissingInputPV::MissingInputPV(const std::string& what, statusPV_t status ): FactoryError(what, status)
{
}

MissingOutputPV::MissingOutputPV(const std::string& what, statusPV_t status ): FactoryError(what, status)
{
}

MissingDestinationPV::MissingDestinationPV(const std::string &what, statusPV_t status ): FactoryError(what, status)
{
}

INIParserError::INIParserError(const std::string& what, statusPV_t status ): NdsError(what, status)
{
}

INIParserMissingSection::INIParserMissingSection(const std::string& what, statusPV_t status ): INIParserError(what, status)
{
}

INIParserSyntaxError::INIParserSyntaxError(const std::string &what, statusPV_t status ): INIParserError(what, status)
{
}


}
