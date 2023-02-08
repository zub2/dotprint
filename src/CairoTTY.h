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

#ifndef CAIRO_TTY_H_
#define CAIRO_TTY_H_

#include <cstdint>
#include <memory>
#include <string>
#include <algorithm>

#include <glibmm.h>
#include <cairomm/cairomm.h>

/** \brief Structure describing page margins. */
struct Margins
{
    constexpr Margins(double l, double r, double t, double b):
        left(l),
        right(r),
        top(t),
        bottom(b)
    {}

    constexpr Margins(double x, double top, double bottom):
        left(x),
        right(x),
        top(top),
        bottom(bottom)
    {}

    /** \brief Position of the left margin to the right of the page edge. */
    double left;

    /** \brief Position of the right margin to the left of the page edge. */
    double right;

    /** \brief Position of the top margin to the bottom of the page edge. */
    double top;

    /** \brief Position of the bottom margin to the top of the page edge. */
    double bottom;
};

struct PageSize
{
    constexpr PageSize(double w = 0.0, double h = 0.0):
        width(w),
        height(h)
    {}

    /** \brief Flips the page lanscape. */
    void rotate()
    {
        std::swap(width, height);
    }

    double width;
    double height;
};

enum class FontWeight
{
    Normal,
    Bold
};

enum class FontSlant
{
    Normal,
    Italic
};

class ICairoTTYProtected
{
public:
    virtual void setPageSize(const PageSize &p) = 0;

    virtual void home() = 0;
    virtual void newLine() = 0;
    virtual void carriageReturn() = 0;
    virtual void lineFeed() = 0;
    virtual void newPage() = 0;

    virtual void setFontName(const std::string &family) = 0;
    virtual void setFontSize(double size) = 0;
    virtual void setFontWeight(FontWeight weight) = 0;
    virtual void setFontSlant(FontSlant slant) = 0;
    virtual void stretchFont(double stretch_x, double stretch_y = 1.0) = 0;
    virtual void useCurrentFont() = 0;

    virtual void append(char c) = 0;
    virtual void append(gunichar c) = 0;

    virtual ~ICairoTTYProtected() = default;
};

class ICharPreprocessor
{
public:
    virtual void process(ICairoTTYProtected &ctty, uint8_t c) = 0;

    virtual ~ICharPreprocessor() = default;
};

class ICodepageTranslator
{
public:
    virtual bool translate(uint8_t in, gunichar &out) = 0;

    virtual ~ICodepageTranslator() = default;
};

class CairoTTY: protected ICairoTTYProtected
{
public:
    CairoTTY(Cairo::RefPtr<Cairo::PdfSurface> cs, const PageSize &p, const Margins &m, ICharPreprocessor *preprocessor,
        std::unique_ptr<ICodepageTranslator> translator);

    virtual ~CairoTTY();

    CairoTTY &operator<<(uint8_t c);

    void setPreprocessor(ICharPreprocessor *preprocessor);

    virtual void useCurrentFont() override;

    virtual void setPageSize(const PageSize &p) override;

    virtual void home() override;
    virtual void newLine() override;
    virtual void carriageReturn() override;
    virtual void lineFeed() override;
    virtual void newPage() override;

    virtual void setFontName(const std::string &family) override;
    virtual void setFontSize(double size) override;
    virtual void setFontWeight(FontWeight weight) override;
    virtual void setFontSlant(FontSlant slant) override;
    virtual void stretchFont(double stretch_x, double stretch_y = 1.0) override;

    virtual void append(char c) override;
    virtual void append(gunichar c) override;

private:
    Cairo::RefPtr<Cairo::PdfSurface> m_cairoSurface;
    Cairo::RefPtr<Cairo::Context> m_context;

    std::string m_fontName;
    double m_fontSize;
    FontWeight m_fontWeight;
    FontSlant m_fontSlant;
    Cairo::FontExtents m_fontExtents;

    Margins m_margins;
    PageSize m_pageSize;

    double m_x;
    double m_y;

    double m_stretchX;
    double m_stretchY;

    ICharPreprocessor *m_preprocessor;
    std::unique_ptr<ICodepageTranslator> m_cpTranslator;

    void setFont(const std::string &family, double size,
        Cairo::FontSlant slant = Cairo::FONT_SLANT_NORMAL,
        Cairo::FontWeight weight = Cairo::FONT_WEIGHT_NORMAL);
};

#endif // CAIRO_TTY_H_
