/*
 * Copyright (C) 2020 Peter Kessen <p.kessen at kessen-peter.de>
 * Copyright (C) 2023 David Kozub <zub at linux.fjfi.cvut.cz>
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

#include "CodepageTranslator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <system_error>
#include <cerrno>

CodepageTranslator::CodepageTranslator(const std::string &tableName)
{
    std::ifstream f(tableName);
    if (!f.is_open())
    {
        const int e = errno;
        throw std::system_error(e, std::generic_category(), "can't open " + tableName);
    }

    std::string line;
    while (std::getline(f, line))
    {
        auto startText = line.find_first_not_of(" \t");
        if (startText == std::string::npos)
        {
            // Skip empty line (only comment)
            continue;
        }
        auto startComment = line.find_first_of("#", startText);

        auto text = line.substr(startText, startComment - startText);
        if (text.length() > 0)
        {
            std::stringstream ss1(text);
            unsigned int c;
            std::string uni;

            ss1 >> std::hex >> c >> uni;
            if (c > 0xFF)
            {
                throw CodepageTableParseException("Could not parse line. Character value too high: " + text);
            }

            const uint8_t ch = static_cast<uint8_t>(c);
            if (uni.substr(0, 2) != "U+")
            {
                throw CodepageTableParseException("Could not parse line. Unicode value not correctly formatted: " + text);
            }

            std::stringstream ss2(uni.substr(2));
            gunichar unichar;
            ss2 >> std::hex >> unichar;
            m_table.insert({ch, unichar});
        }
    }

    if (f.bad())
    {
        const int e = errno;
        throw std::system_error(e, std::generic_category(), "can't read " + tableName);
    }
}

bool CodepageTranslator::translate(uint8_t in, gunichar &out)
{
    const auto it = m_table.find(in);

    if (it != m_table.end())
    {
        out = it->second;
        return true;
    }

    std::cerr << "CodepageTranslator::translate(): Dropping unknown char 0x"
        << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(in) << std::endl;
    return false;
}
