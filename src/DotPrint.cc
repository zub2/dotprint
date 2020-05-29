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
#include <fstream>
#include <stdexcept>

#include <assert.h>

#include <getopt.h>

#include "CairoTTY.h"
#include "PageSizeFactory.h"
#include "CmdLineParser.h"

int main(int argc, char *argv[])
{
    CmdLineParser cmdline(argc, argv);

    PageSize p = cmdline.GetPageSize();
    if (cmdline.GetLandscape())
        p.Landscape();

    ICharPreprocessor *preproc = cmdline.GetPreprocessor();

    Cairo::RefPtr<Cairo::PdfSurface> cs = Cairo::PdfSurface::create(cmdline.GetOutputFile(), p.m_Width, p.m_Height);
    assert(cs);

    Margins m(10.0 * milimeter,10.0 * milimeter,10.0 * milimeter);

    CairoTTY ctty(cs, p, m, preproc);

    // copy the file into ctty
    ctty.SetFont(cmdline.GetFontFace(), cmdline.GetFontSize(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);

    std::fstream f(cmdline.GetInputFile(), std::fstream::in | std::fstream::binary);
    assert(f.is_open());
    char c;
    while (!f.get(c).eof())
    {
        ctty << c;
    }

    return 0;
}
