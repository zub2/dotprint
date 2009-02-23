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
	enum PrinterState
	{
		Normal,
  		SingleLineExpanded,
  		Condensed
	};
	
	PrinterState m_State;
	
public:
	EpsonPreprocessor():
		m_State(Normal)
	{}
	
	virtual void process(ICairoTTYProtected &ctty, gunichar c)
	{
		if (Unicode::iscntrl(c))
		{
			// Control codes handled here
			switch (c)
			{
				case 0x0e: // Expanded printing for one line
					ctty.StretchFont(2.0);
					m_State = SingleLineExpanded;
					break;
					
				case 0x14: // Cancel one-line expanded printing
					ctty.StretchFont(1.0);
					m_State = Normal;
					break;
					
				case 0x0f: // Condensed printing
					ctty.StretchFont(10.0/17.0); // Change from 10 CPI to 17 CPI
					m_State = Condensed;
					break;
					
				case 0x12: // Cancel condensed printing
					ctty.StretchFont(1.0);
					m_State = Normal;
					break;
					
				case '\r':
					ctty.CarriageReturn();
					break;
					
				case '\n':
					if (m_State == SingleLineExpanded)
					{
						ctty.StretchFont(1.0);
						m_State = Normal;
					}
					ctty.LineFeed();
					break;
					
				case 0x0c:
					ctty.NewPage();
					break;
					
				default:
					std::cerr << "EpsonPreprocessor::process(): ignoring unknown character 0x" << std::hex << c << std::endl; 
					//assert(0);
			}
		}
		else
			ctty.append(c);
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
