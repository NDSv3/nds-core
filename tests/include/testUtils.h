
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

#include <nds3/nds.h>

#define RST  "\x1B[0m"
#define FRED  "\x1B[31m"
#define FGRN  "\x1B[32m"
#define FYEL  "\x1B[33m"
#define FBLU  "\x1B[34m"
#define FMAG  "\x1B[35m"
#define FCYN  "\x1B[36m"
#define FWHT  "\x1B[37m"
#define FGRY "\x1B[90m"

#define PrintHeader(text) { \
    std::cout << FGRY << text << RST << std::endl <<std::flush; \
}

#define PrintWarning(text) { \
    std::cout << FYEL << text << RST << std::endl <<std::flush; \
}

#define Print(text) { \
    std::cout << FWHT  << text << RST << std::endl <<std::flush; \
}

#define PrintAndParse(start, text) { \
    std::cout << FWHT <<start << TestUtils::getString(text) << RST << std::endl <<std::flush; \
}

class TestUtils
{
public:

    /**
     * @brief Get a string by identifying the type of data (std::int32_t) and its value
     */
    static std::string getString(const std::int32_t & data);

    /**
     * @brief Get a string by identifying the type of data (std::int64_t) and its value
     */
    static std::string getString(const std::int64_t & data);

    /**
     * @brief Get a string by identifying the type of data (std::uint64_t) and its value
     */
    static std::string getString(const std::uint64_t & data);

    /**
     * @brief Get a string by identifying the type of data (float) and its value
     */
    static std::string getString(const float & data);

    /**
     * @brief Get a string by identifying the type of data (double) and its value
     */
    static std::string getString(const double & data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<bool> data) and its value
     */
    static std::string getString(std::vector<bool> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::uint8_t>) and its value
     */
    static std::string getString(std::vector<std::uint8_t> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::uint16_t>) and its value
     */
    static std::string getString(std::vector<std::uint16_t> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::uint32_t>) and its value
     */
    static std::string getString(std::vector<std::uint32_t> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::int8_t>) and its value
     */
    static std::string getString(std::vector<std::int8_t> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::int16_t>) and its value
     */
    static std::string getString(std::vector<std::int16_t> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::int32_t>) and its value
     */
    static std::string getString(std::vector<std::int32_t> data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<std::int64_t>) and its value
     */
    static std::string getString(std::vector<std::int64_t> data);

    /**
     * @brief Get a string by identifying the type of data (float) and its value
     */
    static std::string getString(std::vector<float> data);

    /**
     * @brief Get a string by identifying the type of data (double) and its value
     */
    static std::string getString(std::vector<double> data);

    /**
     * @brief Get a string by identifying the type of data (std::string) and its value
     */
    static std::string getString(std:: string data);

    /**
     * @brief Get a string by identifying the type of data (timespec data) and its value
     */
    static std::string getString(timespec data);

    /**
     * @brief Get a string by identifying the type of data (std::vector<timespec> data) and its value
     */
    static std::string getString(std::vector<timespec> data);

    /**
     * @brief Get a string by identifying the type of data (nds::timestamp_t) and its value
     */
    static std::string getString(nds::timestamp_t data);

    /**
     * @brief Get the absolute path of the file given by its relative path.
     * The current directory for the relative paths is the one where the main method
     * is being executed.
     * @param relativePath Relative path to the target file.
     * @return Absolute path to the target path (if found), or relative path (if not found).
    */
    static std::string getAbsolutePath(const std::string& relativePath);

    static void displayTitle(std::string msg){
        std::cout<<"+-----------------------------------------------------------------------------"<<std::endl;
        std::cout<<msg<<std::endl;
        std::cout<<"+-----------------------------------------------------------------------------"<<std::endl<<std::endl;
    }

    static void waitEnterKey(std::string msg){
        std::cout<<"+-----------------------------------------------------------------------------"<<std::endl;
        std::cout<<msg<<std::endl;
        std::cout<<"+-----------------------------------------------------------------------------"<<std::endl;

        while(std::cin.get()!='\n');
    }
};




#endif /* TESTUTILS_H_ */
