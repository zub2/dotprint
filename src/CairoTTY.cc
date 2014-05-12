#include <iostream>
#include <assert.h>
#include "CairoTTY.h"

using namespace Glib;
using namespace Cairo;

CairoTTY::CairoTTY(Cairo::RefPtr<PdfSurface> cs, const PageSize &p, const Margins &m, ICharPreprocessor *preprocessor):
    m_CairoSurface(cs),
    m_Margins(m),
    m_Preprocessor(preprocessor)
{
    m_Context = Context::create(m_CairoSurface);

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

CairoTTY &CairoTTY::operator<<(const ustring &s)
{
    for (unsigned i = 0; i < s.length(); i++)
        operator<<(s[i]);

    return *this;
}

CairoTTY &CairoTTY::operator<<(gunichar c)
{
    if (m_Preprocessor)
        m_Preprocessor->process(*this, c);
    else
        append(c);

    return *this;
}

void CairoTTY::SetPreprocessor(ICharPreprocessor *preprocessor)
{
    m_Preprocessor = preprocessor;
}

void CairoTTY::SetFont(const std::string &family, double size, FontSlant slant, FontWeight weight)
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

void CairoTTY::append(gunichar c)
{
    assert(!Unicode::iscntrl(c));

    ustring s(1, c);

    TextExtents t;
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
