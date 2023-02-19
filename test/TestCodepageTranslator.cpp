#include <string>

#include <boost/test/unit_test.hpp>

#include <glibmm.h>

#include "TestData.h"
#include "translators/CodepageTranslator.h"

namespace
{
    std::filesystem::path getTestFile(const std::string & s)
    {
        return ::getTestFile("CodepageTranslator", s);
    }
}

BOOST_AUTO_TEST_CASE(CodepageTranslator_fileNotFound)
{
    BOOST_CHECK_THROW(CodepageTranslator("non-existent-file"), std::system_error);
}

BOOST_AUTO_TEST_CASE(CodepageTranslator_parseError_wrongLine)
{
    BOOST_CHECK_THROW(CodepageTranslator(getTestFile("wrong-format-wrong-byte.trans")), CodepageTableParseException);
}

BOOST_AUTO_TEST_CASE(CodepageTranslator_parseError_wrongByte)
{
    BOOST_CHECK_THROW(CodepageTranslator(getTestFile("wrong-format-wrong-byte.trans")), CodepageTableParseException);
}

BOOST_AUTO_TEST_CASE(CodepageTranslator_parseError_wrongUnicodeCodepoint)
{
    BOOST_CHECK_THROW(CodepageTranslator(getTestFile("wrong-format-wrong-unicode-codepoint.trans")), CodepageTableParseException);
}

BOOST_AUTO_TEST_CASE(CodepageTranslator_simpleTable)
{
    CodepageTranslator translator(getTestFile("simple-table.trans"));

    gunichar c = 0;
    BOOST_TEST(translator.translate(0, c));
    BOOST_TEST(c == 0);

    BOOST_TEST(translator.translate(1, c));
    BOOST_TEST(c == 1);

    BOOST_TEST(!translator.translate(2, c));
}
