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
#include <assert.h>

//#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#include "CairoTTY.h"
#include "PageSizeFactory.h"
#include "CmdLineParser.h"

using namespace Cairo;
using namespace Glib;

class UniFileStream
{
public:
    UniFileStream(const std::string &fname):
        m_io(NULL),
        m_Eof(false)
    {
        GError *err = NULL;

        m_io = g_io_channel_new_file(fname.c_str(), "r", &err);
        if (!m_io)
        {
            assert(err != NULL);

            // TODO: throw exception
            std::cerr << "Cannot open input file." << err->message << std::endl;
            exit(1);
        }
    }

    bool ReadUniChar(gunichar &c)
    {
        assert(m_io != NULL);
        GIOStatus status = g_io_channel_read_unichar(m_io, &c, NULL);

        if (status == G_IO_STATUS_NORMAL)
            return true;

        if (status == G_IO_STATUS_EOF)
            m_Eof = true;
        //else
        //  TODO: throw exception

        c = 0;
        return false;
    }

    bool Eof() const
    {
        return m_Eof;
    }

    ~UniFileStream()
    {
        assert(m_io != NULL);
        g_io_channel_shutdown (m_io, TRUE, NULL);
        m_io = NULL;
    }

protected:
    GIOChannel *m_io;
    bool    m_Eof;
};

int main(int argc, char *argv[])
{
    CmdLineParser cmdline(argc, argv);

    //std::cout<<"DotPrint"<<std::endl;

    PageSize p = cmdline.GetPageSize();
    if (cmdline.GetLandscape())
        p.Landscape();

    ICharPreprocessor *preproc = cmdline.GetPreprocessor();

    Cairo::RefPtr<PdfSurface> cs = PdfSurface::create(cmdline.GetOutputFile(), p.m_Width, p.m_Height);
    assert(cs);

    Margins m(10.0 * milimeter,10.0 * milimeter,10.0 * milimeter);

    CairoTTY ctty(cs, p, m, preproc);

    // copy the file into ctty
    ctty.SetFont(cmdline.GetFontFace(), cmdline.GetFontSize(), FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);

    UniFileStream f(cmdline.GetInputFile());

    gunichar c;
    while (f.ReadUniChar(c))
    {
        ctty << c;
    }

/*  ctty.SetFont("Courier New", 10.0, FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
    ctty << "ABCDEFGHIJKLM\n";
    ctty.SetFont("Courier New", 10.0, FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);
    ctty << "ABCDEFGHIJKLM\n";
    ctty.StretchFont(2.0, 1.0);
    ctty << "ABCDEFGHIJKLM\n";
    ctty.StretchFont(1.0, 1.0);
    ctty << "ABCDEFGHIJKLM";
    ctty.StretchFont(1.0, 2.0);
    ctty << "\nABCDEFGHIJKLM";
    ctty.StretchFont(0.5, 1.0);
    ctty << "\nABCDEFGHIJKLM\n";*/

    return 0;
}
