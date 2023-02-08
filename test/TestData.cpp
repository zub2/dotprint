#include "TestData.h"

#include <cstdlib>

namespace
{
    const std::filesystem::path & getTestDataPath()
    {
        static std::filesystem::path testDataPath;

        if (testDataPath.empty())
        {
            const char *root = getenv("TEST_DATA_ROOT");
            if (root)
            {
                testDataPath = root;
            }
            else
            {
                // try some fallbacks
                const char * FALLBACKS[] =
                {
                    "data",
                    "test/data"
                };
                for (size_t i = 0; i < std::size(FALLBACKS); i++)
                {
                    if (std::filesystem::is_directory(std::filesystem::status(FALLBACKS[i])))
                    {
                        testDataPath = FALLBACKS[i];
                    }
                }
                if (testDataPath.empty())
                {
                    throw std::runtime_error("can't find test data, specify path via TEST_DATA_ROOT environment variable");
                }
            }
        }

        return testDataPath;
    }
}

std::filesystem::path getTestFile(const std::string & testcase, const std::string & fileName)
{
    return getTestDataPath() / testcase / fileName;
}
