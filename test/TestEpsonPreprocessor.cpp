#include <boost/test/unit_test.hpp>
#include <boost/fakeit.hpp>

#include "preprocessors/EpsonPreprocessor.h"

using fakeit::Mock;
using fakeit::Fake;
using fakeit::Verify;
using fakeit::VerifyNoOtherInvocations;

namespace
{
}

BOOST_AUTO_TEST_CASE(EpsonPreprocessor_plainText)
{
    EpsonPreprocessor preprocessor;

    fakeit::Mock<ICairoTTYProtected> cttyMock;
    Fake(Method(cttyMock, append));

    preprocessor.process(cttyMock.get(), 'a');
    preprocessor.process(cttyMock.get(), 'b');
    preprocessor.process(cttyMock.get(), 'c');
    Verify(Method(cttyMock, append).Using('a')).Once();
    Verify(Method(cttyMock, append).Using('b')).Once();
    Verify(Method(cttyMock, append).Using('c')).Once();
    VerifyNoOtherInvocations(cttyMock);
}

BOOST_AUTO_TEST_CASE(EpsonPreprocessor_CR)
{
    EpsonPreprocessor preprocessor;

    fakeit::Mock<ICairoTTYProtected> cttyMock;
    Fake(Method(cttyMock, carriageReturn));

    preprocessor.process(cttyMock.get(), '\r');
    Verify(Method(cttyMock, carriageReturn)).Once();
    VerifyNoOtherInvocations(cttyMock);
}

BOOST_AUTO_TEST_CASE(EpsonPreprocessor_LF)
{
    EpsonPreprocessor preprocessor;

    fakeit::Mock<ICairoTTYProtected> cttyMock;
    Fake(Method(cttyMock, lineFeed));

    preprocessor.process(cttyMock.get(), '\n');
    Verify(Method(cttyMock, lineFeed)).Once();
    VerifyNoOtherInvocations(cttyMock);
}

BOOST_AUTO_TEST_CASE(EpsonPreprocessor_FF)
{
    EpsonPreprocessor preprocessor;

    fakeit::Mock<ICairoTTYProtected> cttyMock;
    Fake(Method(cttyMock, newPage));

    preprocessor.process(cttyMock.get(), 0xc);
    Verify(Method(cttyMock, newPage)).Once();
    VerifyNoOtherInvocations(cttyMock);
}

BOOST_AUTO_TEST_CASE(EpsonPreprocessor_condensed)
{
    EpsonPreprocessor preprocessor;

    fakeit::Mock<ICairoTTYProtected> cttyMock;
    Fake(Method(cttyMock, append));
    Fake(Method(cttyMock, stretchFont));

    preprocessor.process(cttyMock.get(), 0x0f); // start condensed printing
    preprocessor.process(cttyMock.get(), 'a'); // an ordinary character
    preprocessor.process(cttyMock.get(), 0x12); // cancel condensed printing

    constexpr double CONDENSED_X_SCALE = static_cast<double>(EpsonPreprocessor::CONDENSED_CPI)/EpsonPreprocessor::STANDARD_CPI;
    Verify(Method(cttyMock, stretchFont).Using(CONDENSED_X_SCALE, 1.0)).Once();
    Verify(Method(cttyMock, append).Using('a')).Once();
    Verify(Method(cttyMock, stretchFont).Using(1.0, 1.0)).Once();
    VerifyNoOtherInvocations(cttyMock);
}
