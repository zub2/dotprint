/*
 * Copyright (C) 2009, 2012, 2014, 2023 David Kozub <zub at linux.fjfi.cvut.cz>
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

    PageSize p = cmdline.getPageSize();
    if (cmdline.isLandscape())
        p.rotate();

    ICharPreprocessor *preprocessor = cmdline.getPreprocessor();
    auto translator = cmdline.getCodepageTranslator();

    Cairo::RefPtr<Cairo::PdfSurface> cs = Cairo::PdfSurface::create(cmdline.getOutputFile(), p.width, p.height);
    if (!cs)
    {
        throw std::runtime_error("Can't create cairo PdfSurface");
    }

    Margins m = cmdline.getPageMargins();

    CairoTTY ctty(cs, p, m, preprocessor, std::move(translator));

    // Set the font
    ctty.setFontName(cmdline.getFontFace());
    ctty.setFontSize(cmdline.getFontSize());
    ctty.home();

    std::fstream f(cmdline.getInputFile(), std::fstream::in | std::fstream::binary);
    if (!f.is_open())
    {
        throw std::ios_base::failure("Unable to open file \"" + cmdline.getInputFile() + "\"");
    }

    char c;
    while (!f.get(c).eof())
    {
        ctty << static_cast<uint8_t>(c);
    }

    return 0;
}
