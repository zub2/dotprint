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
#include <iostream>
#include <iomanip>

CodepageTranslator::CodepageTranslator()
{
}

CodepageTranslator::~CodepageTranslator()
{
}

bool CodepageTranslator::translate(unsigned char in, gunichar &out)
{
    bool ret = false;

    if (in >= 21 && in < 127)
    {
        ret = true;
        out = in;
    }
    else if (in == 9) // Tab
    {
        ret = true;
        out = in;
    }
    else if (in == 0x81)
    {
        ret = true;
        out = 0x00fc;
    }
    else if (in == 0x84)
    {
        ret = true;
        out = 0x00e4;
    }
    else if (in == 0xc4)
    {
        ret = true;
        out = 0x2500;
    }
    else if (in == 0xe1)
    {
        ret = true;
        out = 0x00df;
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

