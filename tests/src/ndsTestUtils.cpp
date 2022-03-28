#include <string>
#include <sstream>

#include "testUtils.h"

std::string TestUtils::getString(const std::int32_t & data){
    std::ostringstream oss;
    oss << "Integer: " << data;
    return oss.str();
}
std::string TestUtils::getString(const std::int64_t & data){
    std::ostringstream oss;
    oss << "Integer64: " << data;
    return oss.str();
}
std::string TestUtils::getString(const std::uint64_t & data){
    std::ostringstream oss;
    oss << "Unsigned Integer 64: " << data;
    return oss.str();
}

std::string TestUtils::getString(const float & data){
    std::ostringstream oss;
    oss << "Float: " << data;
    return oss.str();
}

std::string TestUtils::getString(const double & data){
    std::ostringstream oss;
    oss << "Double: " << data;
    return oss.str();
}

std::string TestUtils::getString(std::vector<bool> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Bool Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<std::uint8_t> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "UInt8 Array: {";
    for (int i = 0; i <= last; i++) {
        std::int32_t value = (std::int32_t ) data.at(i);
        oss << value;
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<std::uint16_t> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "UInt16 Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<std::uint32_t> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "UInt32 Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<std::int8_t> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Int8 Array: {";
    for (int i = 0; i <= last; i++) {
        std::int32_t value = (std::int32_t ) data.at(i);
        oss << value;
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<std::int16_t> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Int16 Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<std::int32_t> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Int32 Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}
std::string TestUtils::getString(std::vector<std::int64_t> data){
    std::ostringstream oss;
    std::int64_t last = data.size() - 1;
    oss << "Int64 Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<float> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Float Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std::vector<double> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Double Array: {";
    for (int i = 0; i <= last; i++) {
        oss << data.at(i);
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(std:: string data){
    return "String: " + data;
}

std::string TestUtils::getString(timespec data){
    std::ostringstream oss;
    oss << "Timespec : {" << data.tv_sec << " s, " << data.tv_nsec << " ns}";
    return oss.str();
}

std::string TestUtils::getString(std::vector<timespec> data){
    std::ostringstream oss;
    std::int32_t last = data.size() - 1;
    oss << "Timespec Array: { ";
    for (int i = 0; i <= last; i++) {
        timespec t = data.at(i);
        oss << "{" << t.tv_sec << " s, " << t.tv_nsec << " ns}";
        if (i != last) {
            oss << ", ";
        } else {
            oss << "}";
        }
    }
    return oss.str();
}

std::string TestUtils::getString(nds::timestamp_t data){
    std::ostringstream oss;
    oss << "Timestamp: Timespec = {" << data.timestamp.tv_sec << " s, " << data.timestamp.tv_nsec << " ns}, ID = " << data.id << ", Edge = " << (data.edge?"Rising":"Falling");
    return oss.str();
}

#ifndef _WIN32
std::string TestUtils::getAbsolutePath (const std::string& relativePath)
{
    char* pAbsPath = realpath(relativePath.c_str(), NULL);
    std::string absolutePath(relativePath);
    if (pAbsPath != NULL) {
        absolutePath = std::string(pAbsPath);
        free(pAbsPath);
    }
    return absolutePath;
}
#endif

