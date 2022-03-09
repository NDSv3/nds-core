#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "DeviceBasic.h"
#include "ndsTestInterface.h"
#include "ndsTestFactory.h"


TEST(testPVs, testVariable)
{
    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    std::int32_t value_I32(0);
    std::int64_t value_I64(0);
    double  value_DBL(0);
    std::vector<std::int8_t> vector_I8(2,0);
    std::vector<std::uint8_t> vector_UI8(2,0);
    std::vector<std::int32_t> vector_I32(2,0);
    std::vector<std::int64_t> vector_I64(2,0);
    std::vector<double> vector_DBL(2,0);
    std::string value_string;

    timespec readTimestamp{0,0}, timestamp{0,0};

    //read initial value of PVVariablesIn
    pInterface->readCSValue("/rootNode-int32_VariableIn", &readTimestamp, &value_I32);
    EXPECT_EQ(0, value_I32);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-int64_VariableIn", &readTimestamp, &value_I64);
    EXPECT_EQ(0, value_I64);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-double_VariableIn", &readTimestamp, &value_DBL);
    EXPECT_EQ(0, value_DBL);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI8_VariableIn", &readTimestamp, &vector_I8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorUI8_VariableIn", &readTimestamp, &vector_UI8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI32_VariableIn", &readTimestamp, &vector_I32);
    EXPECT_EQ(0, vector_I32[0]);
    EXPECT_EQ(0, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI64_VariableIn", &readTimestamp, &vector_I64);
    EXPECT_EQ(0, vector_I64[0]);
    EXPECT_EQ(0, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorDBL_VariableIn", &readTimestamp, &vector_DBL);
    EXPECT_EQ(0, vector_DBL[0]);
    EXPECT_EQ(0, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-string_VariableIn", &readTimestamp, &value_string);
    EXPECT_EQ("", value_string);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-int32_VariableOut
    pInterface->readCSValue("/rootNode-int32_VariableOut", &readTimestamp, &value_I32);
    EXPECT_EQ(0, value_I32);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =1; timestamp.tv_nsec=1;
    pInterface->writeCSValue("/rootNode-int32_VariableOut", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-int32_VariableOut", &readTimestamp, &value_I32);
    EXPECT_EQ(1, value_I32);
    EXPECT_EQ(1, readTimestamp.tv_sec);
    EXPECT_EQ(1, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-int64_VariableOut
    pInterface->readCSValue("/rootNode-int64_VariableOut", &readTimestamp, &value_I64);
    EXPECT_EQ(0, value_I64);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =1; timestamp.tv_nsec=1;
    pInterface->writeCSValue("/rootNode-int64_VariableOut", timestamp, (std::int64_t)1);
    pInterface->readCSValue("/rootNode-int64_VariableOut", &readTimestamp, &value_I64);
    EXPECT_EQ(1, value_I64);
    EXPECT_EQ(1, readTimestamp.tv_sec);
    EXPECT_EQ(1, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-double_VariableOut
    pInterface->readCSValue("/rootNode-double_VariableOut", &readTimestamp, &value_DBL);
    EXPECT_EQ(0, value_DBL);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =2; timestamp.tv_nsec=2;
    pInterface->writeCSValue("/rootNode-double_VariableOut", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-double_VariableOut", &readTimestamp, &value_DBL);
    EXPECT_EQ(1, value_DBL);
    EXPECT_EQ(2, readTimestamp.tv_sec);
    EXPECT_EQ(2, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-vectorI8_VariableOut
    pInterface->readCSValue("/rootNode-vectorI8_VariableOut", &readTimestamp, &vector_I8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =3; timestamp.tv_nsec=3;
    pInterface->writeCSValue("/rootNode-vectorI8_VariableOut", timestamp, std::vector<int8_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI8_VariableOut", &readTimestamp, &vector_I8);
    EXPECT_EQ(1, vector_I8[0]);
    EXPECT_EQ(1, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(3, readTimestamp.tv_sec);
    EXPECT_EQ(3, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-vectorUI8_VariableOut
    pInterface->readCSValue("/rootNode-vectorUI8_VariableOut", &readTimestamp, &vector_UI8);
    EXPECT_EQ(0, vector_UI8[0]);
    EXPECT_EQ(0, vector_UI8[1]);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =4; timestamp.tv_nsec=4;
    pInterface->writeCSValue("/rootNode-vectorUI8_VariableOut", timestamp,  std::vector<uint8_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorUI8_VariableOut", &readTimestamp, &vector_UI8);
    EXPECT_EQ(1, vector_UI8[0]);
    EXPECT_EQ(1, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(4, readTimestamp.tv_sec);
    EXPECT_EQ(4, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-vectorI32_VariableOut
    pInterface->readCSValue("/rootNode-vectorI32_VariableOut", &readTimestamp, &vector_I32);
    EXPECT_EQ(0, vector_I32[0]);
    EXPECT_EQ(0, vector_I32[1]);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =5; timestamp.tv_nsec=5;
    pInterface->writeCSValue("/rootNode-vectorI32_VariableOut", timestamp, std::vector<int32_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI32_VariableOut", &readTimestamp, &vector_I32);
    EXPECT_EQ(1, vector_I32[0]);
    EXPECT_EQ(1, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(5, readTimestamp.tv_sec);
    EXPECT_EQ(5, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-vectorI64_VariableOut
    pInterface->readCSValue("/rootNode-vectorI64_VariableOut", &readTimestamp, &vector_I64);
    EXPECT_EQ(0, vector_I64[0]);
    EXPECT_EQ(0, vector_I64[1]);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =5; timestamp.tv_nsec=5;
    pInterface->writeCSValue("/rootNode-vectorI64_VariableOut", timestamp, std::vector<int64_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI64_VariableOut", &readTimestamp, &vector_I64);
    EXPECT_EQ(1, vector_I64[0]);
    EXPECT_EQ(1, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(5, readTimestamp.tv_sec);
    EXPECT_EQ(5, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-vectorDBL_VariableOut
    pInterface->readCSValue("/rootNode-vectorDBL_VariableOut", &readTimestamp, &vector_DBL);
    EXPECT_EQ(0, vector_DBL[0]);
    EXPECT_EQ(0, vector_DBL[1]);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =6; timestamp.tv_nsec=6;
    pInterface->writeCSValue("/rootNode-vectorDBL_VariableOut", timestamp, std::vector<double> (2,1));
    pInterface->readCSValue("/rootNode-vectorDBL_VariableOut", &readTimestamp, &vector_DBL);
    EXPECT_EQ(1, vector_DBL[0]);
    EXPECT_EQ(1, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(6, readTimestamp.tv_sec);
    EXPECT_EQ(6, readTimestamp.tv_nsec);

    //Testing write/read of PVVariableOut: /rootNode-string_VariableOut
    pInterface->readCSValue("/rootNode-string_VariableOut", &readTimestamp, &value_string);
    EXPECT_EQ("", value_string);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);
    timestamp.tv_sec =7; timestamp.tv_nsec=7;
    pInterface->writeCSValue("/rootNode-string_VariableOut", timestamp, std::string("testing /rootNode-string_VariableOut output variable"));
    pInterface->readCSValue("/rootNode-string_VariableOut", &readTimestamp, &value_string);
    EXPECT_EQ("testing /rootNode-string_VariableOut output variable", value_string);
    EXPECT_EQ(7, readTimestamp.tv_sec);
    EXPECT_EQ(7, readTimestamp.tv_nsec);

    factory.destroyDevice("rootNode");

}

TEST(testPVs, testDelegate)
{
    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    std::int32_t value_I32(0);
    std::int64_t value_I64(0);
    double  value_DBL(0);
    std::vector<std::int8_t> vector_I8(2,0);
    std::vector<std::uint8_t> vector_UI8(2,0);
    std::vector<std::int32_t> vector_I32(2,0);
    std::vector<std::int64_t> vector_I64(2,0);
    std::vector<double> vector_DBL(2,0);
    std::string value_string;

    timespec readTimestamp{0,0}, timestamp{0,0};

    //read initial value of PVDelegateIn without initializer function
    pInterface->readCSValue("/rootNode-int32_DelegateIn", &readTimestamp, &value_I32);
    EXPECT_EQ(0, value_I32);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-int64_DelegateIn", &readTimestamp, &value_I64);
    EXPECT_EQ(0, value_I64);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-double_DelegateIn", &readTimestamp, &value_DBL);
    EXPECT_EQ(0, value_DBL);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI8_DelegateIn", &readTimestamp, &vector_I8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorUI8_DelegateIn", &readTimestamp, &vector_UI8);
    EXPECT_EQ(0, vector_UI8[0]);
    EXPECT_EQ(0, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI32_DelegateIn", &readTimestamp, &vector_I32);
    EXPECT_EQ(0, vector_I32[0]);
    EXPECT_EQ(0, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI64_DelegateIn", &readTimestamp, &vector_I64);
    EXPECT_EQ(0, vector_I64[0]);
    EXPECT_EQ(0, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorDBL_DelegateIn", &readTimestamp, &vector_DBL);
    EXPECT_EQ(0, vector_DBL[0]);
    EXPECT_EQ(0, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-string_DelegateIn", &readTimestamp, &value_string);
    EXPECT_EQ("", value_string);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    //read initial value of PVDelegateOut without initializer to test dontInitialize method
    pInterface->readCSValue("/rootNode-int32_DelegateOut", &readTimestamp, &value_I32);
    EXPECT_EQ(0, value_I32);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-int64_DelegateOut", &readTimestamp, &value_I64);
    EXPECT_EQ(0, value_I64);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-double_DelegateOut", &readTimestamp, &value_DBL);
    EXPECT_EQ(0, value_DBL);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI8_DelegateOut", &readTimestamp, &vector_I8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorUI8_DelegateOut", &readTimestamp, &vector_UI8);
    EXPECT_EQ(0, vector_UI8[0]);
    EXPECT_EQ(0, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI32_DelegateOut", &readTimestamp, &vector_I32);
    EXPECT_EQ(0, vector_I32[0]);
    EXPECT_EQ(0, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI64_DelegateOut", &readTimestamp, &vector_I64);
    EXPECT_EQ(0, vector_I64[0]);
    EXPECT_EQ(0, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorDBL_DelegateOut", &readTimestamp, &vector_DBL);
    EXPECT_EQ(0, vector_DBL[0]);
    EXPECT_EQ(0, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-string_DelegateOut", &readTimestamp, &value_string);
    EXPECT_EQ("", value_string);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    //Test PVDelegateOut write methods and PVDelegateIn read methods. As the device support is implemented, this values must be equal.

    timestamp.tv_sec =1; timestamp.tv_nsec=1;
    pInterface->writeCSValue("/rootNode-int32_DelegateOut", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-int32_DelegateIn", &readTimestamp, &value_I32);
    EXPECT_EQ(1, value_I32);
    EXPECT_EQ(1, readTimestamp.tv_sec);
    EXPECT_EQ(1, readTimestamp.tv_nsec);

    timestamp.tv_sec =1; timestamp.tv_nsec=1;
    pInterface->writeCSValue("/rootNode-int64_DelegateOut", timestamp, (std::int64_t)1);
    pInterface->readCSValue("/rootNode-int64_DelegateIn", &readTimestamp, &value_I64);
    EXPECT_EQ(1, value_I64);
    EXPECT_EQ(1, readTimestamp.tv_sec);
    EXPECT_EQ(1, readTimestamp.tv_nsec);

    timestamp.tv_sec =2; timestamp.tv_nsec=2;
    pInterface->writeCSValue("/rootNode-double_DelegateOut", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-double_DelegateIn", &readTimestamp, &value_DBL);
    EXPECT_EQ(1, value_DBL);
    EXPECT_EQ(2, readTimestamp.tv_sec);
    EXPECT_EQ(2, readTimestamp.tv_nsec);

    timestamp.tv_sec =3; timestamp.tv_nsec=3;
    pInterface->writeCSValue("/rootNode-vectorI8_DelegateOut", timestamp, std::vector<int8_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI8_DelegateIn", &readTimestamp, &vector_I8);
    EXPECT_EQ(1, vector_I8[0]);
    EXPECT_EQ(1, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(3, readTimestamp.tv_sec);
    EXPECT_EQ(3, readTimestamp.tv_nsec);

    timestamp.tv_sec =4; timestamp.tv_nsec=4;
    pInterface->writeCSValue("/rootNode-vectorUI8_DelegateOut", timestamp,  std::vector<uint8_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorUI8_DelegateIn", &readTimestamp, &vector_UI8);
    EXPECT_EQ(1, vector_UI8[0]);
    EXPECT_EQ(1, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(4, readTimestamp.tv_sec);
    EXPECT_EQ(4, readTimestamp.tv_nsec);

    timestamp.tv_sec =5; timestamp.tv_nsec=5;
    pInterface->writeCSValue("/rootNode-vectorI32_DelegateOut", timestamp, std::vector<int32_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI32_DelegateIn", &readTimestamp, &vector_I32);
    EXPECT_EQ(1, vector_I32[0]);
    EXPECT_EQ(1, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(5, readTimestamp.tv_sec);
    EXPECT_EQ(5, readTimestamp.tv_nsec);

    timestamp.tv_sec =5; timestamp.tv_nsec=5;
    pInterface->writeCSValue("/rootNode-vectorI64_DelegateOut", timestamp, std::vector<int64_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI64_DelegateIn", &readTimestamp, &vector_I64);
    EXPECT_EQ(1, vector_I64[0]);
    EXPECT_EQ(1, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(5, readTimestamp.tv_sec);
    EXPECT_EQ(5, readTimestamp.tv_nsec);

    timestamp.tv_sec =6; timestamp.tv_nsec=6;
    pInterface->writeCSValue("/rootNode-vectorDBL_DelegateOut", timestamp, std::vector<double> (2,1));
    pInterface->readCSValue("/rootNode-vectorDBL_DelegateIn", &readTimestamp, &vector_DBL);
    EXPECT_EQ(1, vector_DBL[0]);
    EXPECT_EQ(1, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(6, readTimestamp.tv_sec);
    EXPECT_EQ(6, readTimestamp.tv_nsec);

    timestamp.tv_sec =7; timestamp.tv_nsec=7;
    pInterface->writeCSValue("/rootNode-string_DelegateOut", timestamp, std::string("testing variable"));
    pInterface->readCSValue("/rootNode-string_DelegateIn", &readTimestamp, &value_string);
    EXPECT_EQ("testing variable", value_string);
    EXPECT_EQ(7, readTimestamp.tv_sec);
    EXPECT_EQ(7, readTimestamp.tv_nsec);

    factory.destroyDevice("rootNode");

}

TEST(testPVs, testDelegateInitialized)
{
    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    std::int32_t value_I32(0);
    std::int64_t value_I64(0);
    double  value_DBL(0);
    std::vector<std::int8_t> vector_I8(2,0);
    std::vector<std::uint8_t> vector_UI8(2,0);
    std::vector<std::int32_t> vector_I32(2,0);
    std::vector<std::int64_t> vector_I64(2,0);
    std::vector<double> vector_DBL(2,0);
    std::string value_string;

    timespec readTimestamp{0,0}, timestamp{0,0};

    //read initial value of PVDelegateIn without initializer function
    pInterface->readCSValue("/rootNode-int32_DelegateIn", &readTimestamp, &value_I32);
    EXPECT_EQ(0, value_I32);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-int64_DelegateIn", &readTimestamp, &value_I64);
    EXPECT_EQ(0, value_I64);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-double_DelegateIn", &readTimestamp, &value_DBL);
    EXPECT_EQ(0, value_DBL);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI8_DelegateIn", &readTimestamp, &vector_I8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorUI8_DelegateIn", &readTimestamp, &vector_UI8);
    EXPECT_EQ(0, vector_UI8[0]);
    EXPECT_EQ(0, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI32_DelegateIn", &readTimestamp, &vector_I32);
    EXPECT_EQ(0, vector_I32[0]);
    EXPECT_EQ(0, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI64_DelegateIn", &readTimestamp, &vector_I64);
    EXPECT_EQ(0, vector_I64[0]);
    EXPECT_EQ(0, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorDBL_DelegateIn", &readTimestamp, &vector_DBL);
    EXPECT_EQ(0, vector_DBL[0]);
    EXPECT_EQ(0, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-string_DelegateIn", &readTimestamp, &value_string);
    EXPECT_EQ("", value_string);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    //read initial value of PVDelegateOut with initializer function
    pInterface->readCSValue("/rootNode-int32_DelegateOut_init", &readTimestamp, &value_I32);
    EXPECT_EQ(0, value_I32);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-int64_DelegateOut_init", &readTimestamp, &value_I64);
    EXPECT_EQ(0, value_I64);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-double_DelegateOut_init", &readTimestamp, &value_DBL);
    EXPECT_EQ(0, value_DBL);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI8_DelegateOut_init", &readTimestamp, &vector_I8);
    EXPECT_EQ(0, vector_I8[0]);
    EXPECT_EQ(0, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorUI8_DelegateOut_init", &readTimestamp, &vector_UI8);
    EXPECT_EQ(0, vector_UI8[0]);
    EXPECT_EQ(0, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI32_DelegateOut_init", &readTimestamp, &vector_I32);
    EXPECT_EQ(0, vector_I32[0]);
    EXPECT_EQ(0, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorI64_DelegateOut_init", &readTimestamp, &vector_I64);
    EXPECT_EQ(0, vector_I64[0]);
    EXPECT_EQ(0, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-vectorDBL_DelegateOut_init", &readTimestamp, &vector_DBL);
    EXPECT_EQ(0, vector_DBL[0]);
    EXPECT_EQ(0, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    pInterface->readCSValue("/rootNode-string_DelegateOut_init", &readTimestamp, &value_string);
    EXPECT_EQ("", value_string);
    EXPECT_EQ(0, readTimestamp.tv_sec);
    EXPECT_EQ(0, readTimestamp.tv_nsec);

    //Test PVDelegateOut write methods and PVDelegateIn read methods. As the device support is implemented, this values must be equal.

    timestamp.tv_sec =1; timestamp.tv_nsec=1;
    pInterface->writeCSValue("/rootNode-int32_DelegateOut_init", timestamp, (std::int32_t)1);
    pInterface->readCSValue("/rootNode-int32_DelegateIn", &readTimestamp, &value_I32);
    EXPECT_EQ(1, value_I32);
    EXPECT_EQ(1, readTimestamp.tv_sec);
    EXPECT_EQ(1, readTimestamp.tv_nsec);

    timestamp.tv_sec =1; timestamp.tv_nsec=1;
    pInterface->writeCSValue("/rootNode-int64_DelegateOut_init", timestamp, (std::int64_t)1);
    pInterface->readCSValue("/rootNode-int64_DelegateIn", &readTimestamp, &value_I64);
    EXPECT_EQ(1, value_I64);
    EXPECT_EQ(1, readTimestamp.tv_sec);
    EXPECT_EQ(1, readTimestamp.tv_nsec);

    timestamp.tv_sec =2; timestamp.tv_nsec=2;
    pInterface->writeCSValue("/rootNode-double_DelegateOut_init", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-double_DelegateIn", &readTimestamp, &value_DBL);
    EXPECT_EQ(1, value_DBL);
    EXPECT_EQ(2, readTimestamp.tv_sec);
    EXPECT_EQ(2, readTimestamp.tv_nsec);

    timestamp.tv_sec =3; timestamp.tv_nsec=3;
    pInterface->writeCSValue("/rootNode-vectorI8_DelegateOut_init", timestamp, std::vector<int8_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI8_DelegateIn", &readTimestamp, &vector_I8);
    EXPECT_EQ(1, vector_I8[0]);
    EXPECT_EQ(1, vector_I8[1]);
    EXPECT_EQ((unsigned int)2, vector_I8.size());
    EXPECT_EQ(3, readTimestamp.tv_sec);
    EXPECT_EQ(3, readTimestamp.tv_nsec);

    timestamp.tv_sec =4; timestamp.tv_nsec=4;
    pInterface->writeCSValue("/rootNode-vectorUI8_DelegateOut_init", timestamp,  std::vector<uint8_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorUI8_DelegateIn", &readTimestamp, &vector_UI8);
    EXPECT_EQ(1, vector_UI8[0]);
    EXPECT_EQ(1, vector_UI8[1]);
    EXPECT_EQ((unsigned int)2, vector_UI8.size());
    EXPECT_EQ(4, readTimestamp.tv_sec);
    EXPECT_EQ(4, readTimestamp.tv_nsec);

    timestamp.tv_sec =5; timestamp.tv_nsec=5;
    pInterface->writeCSValue("/rootNode-vectorI32_DelegateOut_init", timestamp, std::vector<int32_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI32_DelegateIn", &readTimestamp, &vector_I32);
    EXPECT_EQ(1, vector_I32[0]);
    EXPECT_EQ(1, vector_I32[1]);
    EXPECT_EQ((unsigned int)2, vector_I32.size());
    EXPECT_EQ(5, readTimestamp.tv_sec);
    EXPECT_EQ(5, readTimestamp.tv_nsec);

    timestamp.tv_sec =5; timestamp.tv_nsec=5;
    pInterface->writeCSValue("/rootNode-vectorI64_DelegateOut_init", timestamp, std::vector<int64_t> (2,1));
    pInterface->readCSValue("/rootNode-vectorI64_DelegateIn", &readTimestamp, &vector_I64);
    EXPECT_EQ(1, vector_I64[0]);
    EXPECT_EQ(1, vector_I64[1]);
    EXPECT_EQ((unsigned int)2, vector_I64.size());
    EXPECT_EQ(5, readTimestamp.tv_sec);
    EXPECT_EQ(5, readTimestamp.tv_nsec);

    timestamp.tv_sec =6; timestamp.tv_nsec=6;
    pInterface->writeCSValue("/rootNode-vectorDBL_DelegateOut_init", timestamp, std::vector<double> (2,1));
    pInterface->readCSValue("/rootNode-vectorDBL_DelegateIn", &readTimestamp, &vector_DBL);
    EXPECT_EQ(1, vector_DBL[0]);
    EXPECT_EQ(1, vector_DBL[1]);
    EXPECT_EQ((unsigned int)2, vector_DBL.size());
    EXPECT_EQ(6, readTimestamp.tv_sec);
    EXPECT_EQ(6, readTimestamp.tv_nsec);

    timestamp.tv_sec =7; timestamp.tv_nsec=7;
    pInterface->writeCSValue("/rootNode-string_DelegateOut_init", timestamp, std::string("testing variable"));
    pInterface->readCSValue("/rootNode-string_DelegateIn", &readTimestamp, &value_string);
    EXPECT_EQ("testing variable", value_string);
    EXPECT_EQ(7, readTimestamp.tv_sec);
    EXPECT_EQ(7, readTimestamp.tv_nsec);

    factory.destroyDevice("rootNode");

}

TEST(testPVs, testSubscription0)
{
    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::parameters_t parameters;
    parameters.push_back("rootNode-testVariableIn");
    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("subscribe", "rootNode-testVariableOut", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-writeTestVariableIn", timestamp, value.str());

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-readTestVariableOut", &readTimestamp, &readValue);
        EXPECT_EQ(value.str(), readValue);
        EXPECT_EQ(count, readTimestamp.tv_sec);
        EXPECT_EQ(count + 10, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");
}


TEST(testPVs, testSubscription1)
{
    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    {
        nds::parameters_t parameters;
        parameters.push_back("rootNode-testVariableIn");
        nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("subscribe", "rootNode-testVariableOut", parameters);
    }

    {
        nds::parameters_t parameters;
        parameters.push_back("0");
        nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("decimation", "rootNode-testVariableIn", parameters);
    }

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-pushTestVariableIn", timestamp, value.str());

        {
            const std::string* readValue;
            const timespec* readTimestamp;
            EXPECT_THROW(pInterface->getPushedString("/rootNode-pushTestVariableIn", readTimestamp, readValue), std::runtime_error);
        }

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-readTestVariableOut", &readTimestamp, &readValue);
        EXPECT_EQ(value.str(), readValue);
        EXPECT_EQ(count, readTimestamp.tv_sec);
        EXPECT_EQ(count + 10, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");
}

TEST(testPVs, testReplication)
{
    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::parameters_t parameters;
    parameters.push_back("rootNode-testVariableIn");
    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("replicate", "rootNode-delegateIn", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-pushTestVariableIn", timestamp, value.str());

        const std::string* pReadValue;
        const timespec* pReadTimestamp;
        pInterface->getPushedString("/rootNode-delegateIn", pReadTimestamp, pReadValue);
        EXPECT_EQ(value.str(), *pReadValue);
        EXPECT_EQ(count, pReadTimestamp->tv_sec);
        EXPECT_EQ(count + 10, pReadTimestamp->tv_nsec);
    }

    factory.destroyDevice("rootNode");
}

