#include <iostream>
#include <string.h>
#include "PreprocessorFactory.h"

using namespace Glib;

/*
This certainly is ugly.

TODO: tidy, avoid copy-pasting of code
*/

class SimplePreprocessor: public ICharPreprocessor
{
public:
    virtual void process(ICairoTTYProtected &ctty, gunichar c)
    {
        if (Unicode::iscntrl(c))
        {
            // Control codes handled here
            switch (c)
            {
            case '\n':
                ctty.NewLine();
                break;

            case 0x0c:
                ctty.NewPage();
                break;

            default:
                std::cerr << "SimplePreprocessor::process(): ignoring unknown character 0x" << std::hex << c << std::endl;
                //assert(0);
            }
        }
        else
            ctty.append(c);
    }
};

static SimplePreprocessor Simple;

class CRLFPreprocessor: public ICharPreprocessor
{
public:
    virtual void process(ICairoTTYProtected &ctty, gunichar c)
    {
        if (Unicode::iscntrl(c))
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
                std::cerr << "CRLFPreprocessor::process(): ignoring unknown character 0x" << std::hex << c << std::endl;
                //assert(0);
            }
        }
        else
            ctty.append(c);
    }
};

static CRLFPreprocessor CRLF;

class EpsonPreprocessor: public ICharPreprocessor
{
protected:
    enum InputState
    {
        InputNormal,
        Escape
    };

    InputState m_InputState;

    enum EscapeState
    {
        Entered,
        Underline
    };

    EscapeState m_EscapeState;

    enum FontSizeState
    {
        FontSizeNormal,
        SingleLineExpanded,
        Condensed
    };

    FontSizeState m_FontSizeState;

public:
    EpsonPreprocessor():
        m_InputState(InputNormal),
        m_FontSizeState(FontSizeNormal)
    {}

    virtual void process(ICairoTTYProtected &ctty, gunichar c)
    {
        if (m_InputState == Escape)
            handleEscape(ctty, c);
        else if (Unicode::iscntrl(c))
        {
            // Control codes handled here
            switch (c)
            {
            case 0x0e: // Expanded printing for one line
                ctty.StretchFont(2.0);
                m_FontSizeState = SingleLineExpanded;
                break;

            case 0x14: // Cancel one-line expanded printing
                ctty.StretchFont(1.0);
                m_FontSizeState = FontSizeNormal;
                break;

            case 0x0f: // Condensed printing
                ctty.StretchFont(10.0/17.0); // Change from 10 CPI to 17 CPI
                m_FontSizeState = Condensed;
                break;

            case 0x12: // Cancel condensed printing
                ctty.StretchFont(1.0);
                m_FontSizeState = FontSizeNormal;
                break;

            case '\r': // Carriage Return
                ctty.CarriageReturn();
                break;

            case '\n': // Line Feed
                if (m_FontSizeState == SingleLineExpanded)
                {
                    ctty.StretchFont(1.0);
                    m_FontSizeState = FontSizeNormal;
                }
                ctty.LineFeed();
                break;

            case 0x0c: // Form Feed
                ctty.NewPage();
                break;

            case 0x1b: // Escape
                m_InputState = Escape;
                m_EscapeState = Entered;
                break;

            //default:
                // Do what an epson printer does: Silently ignore the char (?)
                // if (verbose)
                //std::cerr << "EpsonPreprocessor::process(): ignoring unknown character 0x" << std::hex << c << std::endl;
                //assert(0);
            }
        }
        else
            ctty.append(c);
    }

    void handleEscape(ICairoTTYProtected &ctty, gunichar c)
    {
        (void)ctty; // currently unused

        // Determine what escape code follows
        if (m_EscapeState == Entered)
        {
            switch (c)
            {
            case 0x2d: // Underline
                m_EscapeState = Underline;
                break;

            default:
                std::cerr << "EpsonPreprocessor::handleEscape(): ignoring unknown escape ESC 0x" << std::hex << c << std::endl;
                m_InputState = InputNormal; // Leave escape state
            }
            return;
        }

        // For multi-byte escapes:
        switch (m_EscapeState)
        {
        case Underline: // ESC 0x2d u, turn underline off (0) or on (!0)
            //std::cerr << "escape: ESC 0x2d " << std::hex << c << std::endl;
            if (c != 0)
            {
                // TODO: Set underline
                //std::cerr << "EpsonPreprocessor::handleEscape(): Ignoring unsupported escape: SET UNDERLINE" << std::endl;
            }
            else
            {
                // TODO: Cancel underline
                //std::cerr << "EpsonPreprocessor::handleEscape(): Ignoring unsupported escape: DROP UNDERLINE" << std::endl;
            }
            m_InputState = InputNormal; // Leave escape state
            break;

        default:
            std::cerr << "EpsonPreprocessor::handleEscape(): Internal error: entered unknown escape state " << m_EscapeState << std::endl;
            abort();
        }
    }

private:
    bool m_Escape;
};

static EpsonPreprocessor Epson;

const PreprocessorFactory::PreprocessorItem PreprocessorFactory::m_Preprocessors[] =
{
    { "simple", &Simple },
    { "crlf", &CRLF },
    { "epson", &Epson },
    { 0, 0 }
};

void PreprocessorFactory::Print(std::ostream &s)
{
    int i = 0;
    while (m_Preprocessors[i].name != NULL)
    {
        s << m_Preprocessors[i].name;
        if (i == 0)
            s << " [default]";
        s << std::endl;
        ++i;
    }
}

ICharPreprocessor* PreprocessorFactory::Lookup(const char *name)
{
    int i = 0;

    while (m_Preprocessors[i].name != NULL && strcmp(m_Preprocessors[i].name, name))
        ++i;

    if (m_Preprocessors[i].name == NULL)
        return NULL;

    return m_Preprocessors[i].preproc;
}

ICharPreprocessor* PreprocessorFactory::GetDefault()
{
    return m_Preprocessors[0].preproc;
}
