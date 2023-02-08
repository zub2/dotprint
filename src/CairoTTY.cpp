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

#include "CairoTTY.h"

#include <iostream>
#include <stdexcept>

CairoTTY::CairoTTY(Cairo::RefPtr<Cairo::PdfSurface> cs, const PageSize &p, const Margins &m, ICharPreprocessor *preprocessor,
    std::unique_ptr<ICodepageTranslator> translator):
    m_cairoSurface(std::move(cs)),
    m_fontName("Courier New"),
    m_fontSize(10.0),
    m_fontWeight(FontWeight::Normal),
    m_fontSlant(FontSlant::Normal),
    m_needFontChange(true),
    m_margins(m),
    m_preprocessor(preprocessor),
    m_cpTranslator(std::move(translator))
{
    m_context = Cairo::Context::create(m_cairoSurface);

    setPageSize(p);
    stretchFont(1.0, 1.0);
    setFont();
    home();
}

CairoTTY::~CairoTTY()
{
    m_context.clear();
    m_cairoSurface->finish();
}

CairoTTY &CairoTTY::operator<<(unsigned char c)
{
    if (m_preprocessor)
        m_preprocessor->process(*this, c);
    else
        append((char) c);

    return *this;
}

void CairoTTY::setPreprocessor(ICharPreprocessor *preprocessor)
{
    m_preprocessor = preprocessor;
}

void CairoTTY::setFont(const std::string &family, double size, Cairo::FontSlant slant, Cairo::FontWeight weight)
{
    if (size < 0.0)
    {
        throw std::runtime_error("CairoTTY: Can't specify negative font size!");
    }

    m_context->select_font_face(family, slant, weight);
    m_context->set_font_size(size);

    m_fontExtents.emplace();
    m_context->get_font_extents(*m_fontExtents);
}

void CairoTTY::setFont()
{
    if (m_needFontChange)
    {
        Cairo::FontWeight weight;
        Cairo::FontSlant slant;

        switch (m_fontWeight)
        {
        case FontWeight::Bold:
            weight = Cairo::FONT_WEIGHT_BOLD;
            break;
        default:
            weight = Cairo::FONT_WEIGHT_NORMAL;
            break;
        }

        switch (m_fontSlant)
        {
        case FontSlant::Italic:
            slant = Cairo::FONT_SLANT_ITALIC;
            break;
        default:
            slant = Cairo::FONT_SLANT_NORMAL;
            break;
        }

        setFont(m_fontName, m_fontSize, slant, weight);
    }
}

void CairoTTY::setPageSize(const PageSize &p)
{
    if (p.width <= 0.0)
    {
        throw std::runtime_error("CairoTTY: page width must be positive");
    }
    if (p.height <= 0.0)
    {
        throw std::runtime_error("CairoTTY: page height must be positive");
    }

    m_pageSize = p;
    m_cairoSurface->set_size(m_pageSize.width, m_pageSize.height);
}

void CairoTTY::home()
{
    m_x = 0.0;
    m_y = m_fontExtents->height * m_stretchY; // so that the top of the first line touches 0.0
}

void CairoTTY::newLine()
{
    carriageReturn();
    lineFeed();
}

void CairoTTY::carriageReturn()
{
    m_x = 0.0;
}

void CairoTTY::lineFeed()
{
    m_y += m_fontExtents->height * m_stretchY;

    // check if we still fit on the page
    if (m_margins.top + m_y > m_pageSize.height - m_margins.bottom)
    {
        newPage(); // forced pagebreak
    }
}

void CairoTTY::newPage()
{
    m_context->show_page();
    home();
}

void CairoTTY::setFontName(const std::string &family)
{
    m_fontName = family;
    m_needFontChange = true;
}

void CairoTTY::setFontSize(double size)
{
    m_fontSize = size;
    m_needFontChange = true;
}

void CairoTTY::setFontWeight(FontWeight weight)
{
    m_fontWeight = weight;
    m_needFontChange = true;
}

void CairoTTY::setFontSlant(FontSlant slant)
{
    m_fontSlant = slant;
    m_needFontChange = true;
}

void CairoTTY::stretchFont(double stretch_x, double stretch_y)
{
    m_stretchX = stretch_x;
    m_stretchY = stretch_y;
}

void CairoTTY::append(char c)
{
    gunichar uc;
    if (m_cpTranslator->translate(c, uc))
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
    else if (Glib::Unicode::iscntrl(c))
    {
        std::cout << "Cannot print character 0x" << std::hex << c << std::endl;
        return;
    }

    setFont();

    Glib::ustring s(1, c);

    Cairo::TextExtents t;
    m_context->get_text_extents(s, t);
    double x_advance = m_stretchX * t.x_advance;

    if (m_margins.left + m_x + x_advance > m_pageSize.width - m_margins.right)
    {
        newLine(); // forced linebreak - text wraps to the next line
    }

    m_context->save();
    m_context->move_to(m_margins.left + m_x, m_margins.top + m_y);
    m_context->scale(m_stretchX, m_stretchY);
    m_context->show_text(s);
    m_context->restore();

    // We ignore y_advance, as we in no way can support
    // vertical text layout.
    m_x += x_advance;
}
