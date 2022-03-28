#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "ndsTestInterface.h"
#include "ndsTestFactory.h"
#include "testUtils.h"
#include "DeviceError.h"

/**
 * @brief Internal function to test the correct operation of throwing the different NdsErrors
 */
static void commonPVsTest();

TEST(testDeviceError, statusError) {
    commonPVsTest();
}

static void commonPVsTest() {

    //Create factory
    nds::Factory factory("test");
    nds::namedParameters_t parameters;

    factory.createDevice("DeviceError", "deviceError", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl *pInterface =
            nds::tests::TestControlSystemInterfaceImpl::getInstance(
                    "deviceError");

    std::int32_t value_I32;
    timespec readTimestamp{0,0}, timestamp{0,0};

    EXPECT_NO_THROW(DeviceError::getInstance("rootNode")->throwError(0));

    //NdsError Timeout
    try {
        pInterface->writeCSValue("/deviceError-delegateOutError", { 0, 0 }, 1);
        FAIL() << "No error thrown" << std::endl;
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::timeout);
        std::cout << "PVDelegateOut: "<< e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }
    try {
        pInterface->writeCSValue("/deviceError-variableOutError", timestamp,
                (std::int32_t) 1);
        pInterface->readCSValue("/deviceError-variableOutError", &readTimestamp,
                &value_I32);
        EXPECT_EQ(1, value_I32);
        pInterface->readCSValue("/deviceError-delegateInError", &readTimestamp,
                        &value_I32);
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::timeout);
        std::cout << "PVDelegateIn:" << e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }

    //NdsError Overflow
    try {
        pInterface->writeCSValue("/deviceError-delegateOutError", { 0, 0 }, 2);
        FAIL() << "No error thrown" << std::endl;
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::overflow);
        std::cout << "PVDelegateOut: "<< e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }
    try {
        pInterface->writeCSValue("/deviceError-variableOutError", timestamp,
                (std::int32_t) 2);
        pInterface->readCSValue("/deviceError-variableOutError", &readTimestamp,
                &value_I32);
        EXPECT_EQ(2, value_I32);
        pInterface->readCSValue("/deviceError-delegateInError", &readTimestamp,
                        &value_I32);
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::overflow);
        std::cout << "PVDelegateIn:" << e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }

    //NdsError disconnected
    try {
        pInterface->writeCSValue("/deviceError-delegateOutError", { 0, 0 }, 3);
        FAIL() << "No error thrown" << std::endl;
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::disconnected);
        std::cout << "PVDelegateOut: "<< e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }
    try {
        pInterface->writeCSValue("/deviceError-variableOutError", timestamp,
                (std::int32_t) 3);
        pInterface->readCSValue("/deviceError-variableOutError", &readTimestamp,
                &value_I32);
        EXPECT_EQ(3, value_I32);
        pInterface->readCSValue("/deviceError-delegateInError", &readTimestamp,
                        &value_I32);
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::disconnected);
        std::cout << "PVDelegateIn:" << e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }

    //NdsError disabled
    try {
        pInterface->writeCSValue("/deviceError-delegateOutError", { 0, 0 }, 4);
        FAIL() << "No error thrown" << std::endl;
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::disabled);
        std::cout << "PVDelegateOut: "<< e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl << std::endl;
    }
    try {
        pInterface->writeCSValue("/deviceError-variableOutError", timestamp,
                (std::int32_t) 4);
        pInterface->readCSValue("/deviceError-variableOutError", &readTimestamp,
                &value_I32);
        EXPECT_EQ(4, value_I32);
        pInterface->readCSValue("/deviceError-delegateInError", &readTimestamp,
                        &value_I32);
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::disabled);
        std::cout << "PVDelegateIn:" << e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }

    //NdsError other error
    try {
        pInterface->writeCSValue("/deviceError-delegateOutError", { 0, 0 }, 5);
        FAIL() << "No error thrown" << std::endl;
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::error);
        std::cout << "PVDelegateOut: "<< e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl << std::endl;
    }
    try {
        pInterface->writeCSValue("/deviceError-variableOutError", timestamp,
                (std::int32_t) 5);
        pInterface->readCSValue("/deviceError-variableOutError", &readTimestamp,
                &value_I32);
        EXPECT_EQ(5, value_I32);
        pInterface->readCSValue("/deviceError-delegateInError", &readTimestamp,
                        &value_I32);
    } catch (nds::NdsError &e) {
        EXPECT_EQ(e.status, nds::statusPV_t::error);
        std::cout << "PVDelegateIn:" << e.what() << std::endl;
    } catch (...) {
        FAIL() << "Error not expected" << std::endl;
    }

    // Destroy test device
    factory.destroyDevice("deviceError");
}

