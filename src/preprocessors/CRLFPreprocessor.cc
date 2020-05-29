/*
 * Copyright (C) 2009, 2012, 2014 David Kozub <zub at linux.fjfi.cvut.cz>
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

#include <iostream>
#include <iomanip>
#include <glibmm.h>
#include "CRLFPreprocessor.h"

void CRLFPreprocessor::process(ICairoTTYProtected &ctty, unsigned char c)
{
    if (iscntrl(c))
    {
        // Control codes handled here
        switch (c)
        {
        case '\r':
            ctty.CarriageReturn();
            break;

        case '\n':
            ctty.LineFeed();
            break;

        case 0x0c:
            ctty.NewPage();
            break;

        default:
            {
                int i = c;
                std::cerr << "CRLFPreprocessor::process(): ignoring unknown character 0x"
                    << std::setfill('0') << std::setw(2) << std::hex << i << std::endl;
            }
        }
    }
    else
        ctty.append((char) c);
}
