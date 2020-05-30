/*
 * Copyright (C) 2020 Peter Kessen
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
#include <iomanip>

CodepageTranslator::CodepageTranslator()
{
}

CodepageTranslator::~CodepageTranslator()
{
}

void CodepageTranslator::loadTable(std::string const& tableName)
{
    m_table.clear();
    m_table.insert({(unsigned char)0x09, (gunichar)0x09});
    for (unsigned char i=21; i < 127; ++i)
    {
        m_table.insert({i, (gunichar)i});
    }
    m_table.insert({(unsigned char)0x81, (gunichar)0x00fc});
    m_table.insert({(unsigned char)0x84, (gunichar)0x00e4});
    m_table.insert({(unsigned char)0xc4, (gunichar)0x2500});
    m_table.insert({(unsigned char)0xe1, (gunichar)0x00df});
}

bool CodepageTranslator::translate(unsigned char in, gunichar &out)
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

