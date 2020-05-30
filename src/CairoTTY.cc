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
#include "CairoTTY.h"
#include "AsciiCodepageTranslator.h"
#include "CodepageTranslator.h"

CairoTTY::CairoTTY(Cairo::RefPtr<Cairo::PdfSurface> cs, const PageSize &p, const Margins &m, ICharPreprocessor *preprocessor, ICodepageTranslator *translator):
    m_CairoSurface(cs),
    m_Margins(m),
    m_Preprocessor(preprocessor),
    m_CpTranslator(translator)
{
    m_Context = Cairo::Context::create(m_CairoSurface);

    SetPageSize(p);
    StretchFont(1.0, 1.0);
    SetFont("Courier New", 10.0);

    Home();
}

CairoTTY::~CairoTTY()
{
    m_Context.clear();
    m_CairoSurface->finish();
}

CairoTTY &CairoTTY::operator<<(unsigned char c)
{
    if (m_Preprocessor)
        m_Preprocessor->process(*this, c);
    else
        append((char) c);

    return *this;
}

void CairoTTY::SetPreprocessor(ICharPreprocessor *preprocessor)
{
    m_Preprocessor = preprocessor;
}

void CairoTTY::SetFont(const std::string &family, double size, Cairo::FontSlant slant, Cairo::FontWeight weight)
{
    assert(size > 0.0);

    m_Context->select_font_face(family, slant, weight);
    m_Context->set_font_size(size);

    m_Context->get_font_extents(m_FontExtents);
}

void CairoTTY::SetPageSize(const PageSize &p)
{
    assert(p.m_Width > 0.0);
    assert(p.m_Height > 0.0);

    m_PageSize = p;
    m_CairoSurface->set_size(m_PageSize.m_Width, m_PageSize.m_Height);
}

void CairoTTY::Home()
{
    m_x = 0.0;
    m_y = m_FontExtents.height * m_StretchY; // so that the top of the first line touches 0.0
}

void CairoTTY::NewLine()
{
    CarriageReturn();
    LineFeed();
}

void CairoTTY::CarriageReturn()
{
    m_x = 0.0;
}

void CairoTTY::LineFeed()
{
    m_y += m_FontExtents.height * m_StretchY;

    // check if we still fit on the page
    if (m_Margins.m_Top + m_y > m_PageSize.m_Height - m_Margins.m_Bottom)
        NewPage(); // forced pagebreak
}

void CairoTTY::NewPage()
{
    m_Context->show_page();
    Home();
}

void CairoTTY::StretchFont(double stretch_x, double stretch_y)
{
    m_StretchX = stretch_x;
    m_StretchY = stretch_y;
}

void CairoTTY::append(char c)
{
    gunichar uc;

    if (m_CpTranslator == nullptr)
    {
        m_CpTranslator = new AsciiCodepageTranslator();
    }

    if (m_CpTranslator->translate(c, uc))
    {
        append(uc);
    }
}

void CairoTTY::append(gunichar c)
{
    if (c == 0x09)
    {
        // TODO: tab handling
        return;
    }
    Glib::ustring s(1, c);

    Cairo::TextExtents t;
    m_Context->get_text_extents(s, t);
    double x_advance = m_StretchX * t.x_advance;

    if (m_Margins.m_Left + m_x + x_advance > m_PageSize.m_Width - m_Margins.m_Right)
        NewLine(); // forced linebreak - text wraps to the next line

    m_Context->save();
    m_Context->move_to(m_Margins.m_Left + m_x, m_Margins.m_Top + m_y);
    m_Context->scale(m_StretchX, m_StretchY);
    m_Context->show_text(s);
    m_Context->restore();

    // We ignore y_advance, as we in no way can support
    // vertical text layout.
    m_x += x_advance;
}

