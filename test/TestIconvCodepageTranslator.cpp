#include <string>
#include <system_error>

#include <glibmm.h>

#include <boost/test/unit_test.hpp>

#include "translators/IconvCodepageTranslator.h"

BOOST_AUTO_TEST_CASE(IconvCodepageTranslator_CP850)
{
    IconvCodepageTranslator translator("CP850");

    gunichar c;
    BOOST_TEST(translator.translate('a', c));
    BOOST_TEST(c == 'a');

    // ö
    BOOST_TEST(translator.translate(0x94, c));
    BOOST_TEST(c == 0xf6);
}

BOOST_AUTO_TEST_CASE(IconvCodepageTranslator_unknownEncoding)
{
    BOOST_CHECK_THROW(IconvCodepageTranslator("nonexisting"), std::system_error);
}
