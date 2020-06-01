/*
 * Copyright (C) 2020 Peter Kessen <p.kessen at kessen-peter.de>
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

void CodepageTranslator::loadTable(std::string const& tableName)
{
    std::fstream f(tableName, std::fstream::in);
    std::string line;

    m_table.clear();

    if (!f.is_open())
    {
        std::cerr << "Unable to load translation file: " << tableName << std::endl;
        exit(1);
    }

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
                std::cerr << "Could not parse line. Character value to high: " << text << std::endl;
                exit(1);
            }
            uint8_t ch = (uint8_t) c;
            if (uni.substr(0, 2) != "U+")
            {
                std::cerr << "Could not parse line. Unicode value not correctly formatted: " << text << std::endl;
                exit(1);
            }
            std::stringstream ss2(uni.substr(2));
            gunichar unichar;
            ss2 >> std::hex >> unichar;
            m_table.insert({ch, unichar});
        }
    }
}

bool CodepageTranslator::translate(uint8_t in, gunichar &out)
{
    bool ret = false;
    auto search = m_table.find(in);

    if (search != m_table.end())
    {
        out = search->second;
        ret = true;
    }
    else
    {
        int i = in;
        ret = false;
        std::cerr << "CodepageTranslator::translate(): Droppping unknown char 0x"
            << std::setfill('0') << std::setw(2) << std::hex << i << std::endl;
    }

    return ret;
}

