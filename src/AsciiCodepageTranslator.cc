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

#include "AsciiCodepageTranslator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

bool AsciiCodepageTranslator::translate(uint8_t in, gunichar &out)
{
    bool ret = false;

    if (in <= 127)
    {
        out = in;
        ret = true;
    }
    else
    {
        int i = in;
        ret = false;
        std::cerr << "AsciiCodepageTranslator::translate(): Droppping unknown char 0x"
            << std::setfill('0') << std::setw(2) << std::hex << i << std::endl;
    }

    return ret;
}

