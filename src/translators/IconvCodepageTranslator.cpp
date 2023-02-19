/*
 * Copyright (C) 2023 Peter Kessen <p.kessen at kessen-peter.de>
 *
 * This file is part of dotprint.
 *
 * dotprint is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dotprint is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dotprint. If not, see <http://www.gnu.org/licenses/>.
 */

#include "IconvCodepageTranslator.h"

#include <system_error>
#include <cerrno>
#include <iostream>
#include <iomanip>

namespace
{
    // Unicode Byte order mark
    constexpr gunichar BOM = 0xfeff;
}

IconvCodepageTranslator::IconvCodepageTranslator(const std::string &sourceEncodingName):
    m_iconvDescriptor(iconv_open("UTF−32", sourceEncodingName.c_str()))
{
    if (m_iconvDescriptor == (iconv_t)-1)
    {
        const int e = errno;
        throw std::system_error(e, std::generic_category(), "can't create libiconv instance");
    }
}

IconvCodepageTranslator::~IconvCodepageTranslator()
{
    if (iconv_close(m_iconvDescriptor) == -1)
    {
        const int e = errno;
        std::error_condition errorCondition = std::generic_category().default_error_condition(e);
        std::cerr << "Can't close icov instance: " << errorCondition.message() << std::endl;
    }
}

bool IconvCodepageTranslator::translate(uint8_t in, gunichar &out)
{
    /*
     * Iconv want to put the BOM as the 1st output byte. And for that iconv demands output buffer
     * with size for 2 codepoints.
     */
    char * src = reinterpret_cast<char*>(&in);
    size_t inBytesLeft = sizeof(in);

    gunichar outputBuffer[2];
    char * dest = reinterpret_cast<char*>(&outputBuffer);
    size_t outBytesLeft = sizeof(outputBuffer);

    const size_t r = iconv(m_iconvDescriptor, &src, &inBytesLeft, &dest, &outBytesLeft);
    const int err = errno;

    if (r == static_cast<size_t>(-1))
    {
        std::cerr << "IconvCodePageTranslator: can't convert 0x" << std::setfill('0') << std::setw(2)
            << std::hex << static_cast<unsigned>(in) << ": ";

        /*
         * None of the below is expected: this code assumes a simple single-byte encoding.
         * Multi-byte encodings could be supported, but the ICodepageTranslator would need
         * to change. As this is all very old, it doesn't seem like adding this makes sense.
         * Let's keep this simple.
         */
        switch (err)
        {
        case EILSEQ:
            std::cerr << "invalid multi-byte sequence; multi-byte encodings are not suppoted";
            break;
        case EINVAL:
            std::cerr << "incomplete multi-byte sequence; multi-byte encodings are not supported";
            break;
        case E2BIG:
            std::cerr << "a single input byte maps to more than 1 unicode codepoint, this is not supported";
            break;
        default:
            std::cerr << "unknown reason " << err;
            break;
        }
        std::cerr << std::endl;
        return false;
    }
    if (inBytesLeft != 0)
    {
        std::cerr << "IconvCodePageTranslator: iconv() did not consume input byte, odd" << std::endl;
        return false;
    }
    if (outBytesLeft == sizeof(outputBuffer) - sizeof(outputBuffer[0]))
    {
        // 1 UTF-32 codepoint has been produced - this is the normal case
        out = outputBuffer[0];
    }
    else if (outBytesLeft == sizeof(outputBuffer) - 2*sizeof(outputBuffer[0]))
    {
        // 2 UTF-32 codepoints have been produced - this s expected to only happen
        // on the 1st call and the output is expected to consist of the BOM
        // which we discard followed by the codepoint we really want
        if (outputBuffer[0] != BOM)
        {
            std::cerr << "IconvCodePageTranslator: iconv produced 2 output units without the BOM being the first" << std::endl;
            return false;
        }
        out = outputBuffer[1];
    }
    else
    {
        // very odd
        std::cerr << "IconvCodePageTranslator: iconv produced output with size that is not a multiple of UTF-32 unit" << std::endl;
        return false;
    }

    return true;
}
