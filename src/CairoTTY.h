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

#ifndef CAIROTTY_H_
#define CAIROTTY_H_

#include <string>
#include <algorithm>
#include <glibmm.h>
#include <cairomm/cairomm.h>

/** \brief Structure describing page margins. */
struct Margins
{
    Margins(double l, double r, double t, double b):
        m_Left(l),
        m_Right(r),
        m_Top(t),
        m_Bottom(b)
    {}

    Margins(double x, double top, double bottom):
        m_Left(x),
        m_Right(x),
        m_Top(top),
        m_Bottom(bottom)
    {}

    /** \brief Position of the left margin to the right of the page edge. */
    double m_Left;

    /** \brief Position of the right margin to the left of the page edge. */
    double m_Right;

    /** \brief Position of the top margin to the bottom of the page edge. */
    double m_Top;

    /** \brief Position of the bottom margin to the top of the page edge. */
    double m_Bottom;
};

struct PageSize
{
    PageSize(double w = 0.0, double h = 0.0):
        m_Width(w),
        m_Height(h)
    {}

    /** \brief Flips the page lanscape. */
    void Landscape()
    {
        std::swap(m_Width, m_Height);
    }

    double m_Width;
    double m_Height;
};

class ICairoTTYProtected
{
public:
    virtual void SetPageSize(const PageSize &p) = 0;

    virtual void Home() = 0;
    virtual void NewLine() = 0;
    virtual void CarriageReturn() = 0;
    virtual void LineFeed() = 0;
    virtual void NewPage() = 0;
    virtual void StretchFont(double stretch_x, double stretch_y = 1.0) = 0;

    virtual void append(gunichar c) = 0;

    virtual ~ICairoTTYProtected()
    {}
};

class ICharPreprocessor
{
public:
    virtual void process(ICairoTTYProtected &ctty, gunichar c) = 0;

    virtual ~ICharPreprocessor()
    {}
};

class CairoTTY: protected ICairoTTYProtected
{
public:
    CairoTTY(Cairo::RefPtr<Cairo::PdfSurface> cs, const PageSize &p, const Margins &m, ICharPreprocessor *preprocessor);

    virtual ~CairoTTY();

    CairoTTY &operator<<(const Glib::ustring &s);
    CairoTTY &operator<<(gunichar c);

    void SetPreprocessor(ICharPreprocessor *preprocessor);

    void SetFont(const std::string &family, double size,
        Cairo::FontSlant slant = Cairo::FONT_SLANT_NORMAL,
        Cairo::FontWeight weight = Cairo::FONT_WEIGHT_NORMAL);

    virtual void SetPageSize(const PageSize &p);

    virtual void Home();
    virtual void NewLine();
    virtual void CarriageReturn();
    virtual void LineFeed();
    virtual void NewPage();
    virtual void StretchFont(double stretch_x, double stretch_y = 1.0);

protected:
    virtual void append(gunichar c);

private:
    Cairo::RefPtr<Cairo::PdfSurface> m_CairoSurface;
    Cairo::RefPtr<Cairo::Context> m_Context;

    Cairo::FontExtents m_FontExtents;
    Margins m_Margins;
    PageSize m_PageSize;

    double m_x;
    double m_y;

    double m_StretchX;
    double m_StretchY;

    ICharPreprocessor *m_Preprocessor;
};

#endif /*CAIROTTY_H_*/
