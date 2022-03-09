// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "DeviceBasic.h"
#include "DeviceFloat.h"
#include "DeviceDBL.h"
#include "DeviceDigitalIO.h"
#include "DeviceFTE.h"
#include "DeviceI32.h"
#include "DeviceI64.h"
#include "DeviceRouting.h"
#include "DeviceVectorI32.h"
#include "DeviceVectorI64.h"
#include "DeviceVectorI8.h"
#include "DeviceVectorUI8.h"
#include "DeviceVectorFloat.h"
#include "DeviceVectorDBL.h"
#include "DeviceHQMonitor.h"
#include "DeviceStateMachine.h"
#include "DeviceFirmware.h"
#include "DeviceTiming.h"
#include "DeviceTimestamping.h"
#include "DevicePVs.h"
#include "DeviceTrigAndClk.h"
#include "DeviceDataMultiplexing.h"
#include "DeviceError.h"

#include "ndsTestFactory.h"


int main(int argc, char **argv)
{
    nds::Factory::registerDriver("Device",
                           std::bind(&DeviceBasic::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceBasic::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceFloat",
                           std::bind(&DeviceFloat::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceFloat::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceDBL",
                           std::bind(&DeviceDBL::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceDBL::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceI32",
                           std::bind(&DeviceI32::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceI32::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorI32",
                           std::bind(&DeviceVectorI32::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorI32::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceI64",
                          std::bind(&DeviceI64::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                          std::bind(&DeviceI64::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorI64",
                           std::bind(&DeviceVectorI64::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorI64::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorI8",
                           std::bind(&DeviceVectorI8::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorI8::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorUI8",
                           std::bind(&DeviceVectorUI8::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorUI8::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorFloat",
                               std::bind(&DeviceVectorFloat::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                               std::bind(&DeviceVectorFloat::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorDBL",
                           std::bind(&DeviceVectorDBL::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorDBL::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceDigitalIO",
                           std::bind(&DeviceDigitalIO::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceDigitalIO::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceFTE",
                           std::bind(&DeviceFTE::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceFTE::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceRouting",
                               std::bind(&DeviceRouting::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                               std::bind(&DeviceRouting::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceHQMonitor",
                           std::bind(&DeviceHQMonitor::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceHQMonitor::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceTiming",
                           std::bind(&DeviceTiming::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceTiming::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceTimestamping",
                           std::bind(&DeviceTimestamping::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceTimestamping::deallocateDevice, std::placeholders::_1));


    nds::Factory::registerDriver("DeviceStateMachine",
                           std::bind(&DeviceStateMachine::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceStateMachine::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceFirmware",
                           std::bind(&DeviceFirmware::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceFirmware::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DevicePVs",
                           std::bind(&DevicePVs::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DevicePVs::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceTrigAndClk",
                           std::bind(&DeviceTrigAndClk::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceTrigAndClk::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceDataMultiplexing",
                           std::bind(&DeviceDataMultiplexing::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceDataMultiplexing::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceError",
                           std::bind(&DeviceError::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceError::deallocateDevice, std::placeholders::_1));

    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);


    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "testDeviceDataMultiplexing*";
    return RUN_ALL_TESTS();
}
