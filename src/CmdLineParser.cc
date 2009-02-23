#include <iostream>
#include <assert.h>
#include <string.h>

//#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#include "CmdLineParser.h"
#include "PageSizeFactory.h"
#include "PreprocessorFactory.h"

CmdLineParser::CmdLineParser(int argc, char* const argv[]):
	m_ProgName((argc>0 && argv[0] != NULL)? argv[0] : "dotprint"),
	m_PageSize(PageSizeFactory::GetDefault()),
	m_Landscape(false),
	m_Preprocessor(PreprocessorFactory::GetDefault()),
	m_OutputFileSet(false),
	m_FontFace("Courier New"),
	m_FontSize(11.0)
{
	while (1)
	{
		int option_index = 0;
		
		int c = getopt_long(argc, argv, short_options, long_options, &option_index);
		if (c == -1)
			break; // end of options
			
		switch (c)
		{
		case 'p':
			// page
			SetPageSize(optarg);
			break;
			
		case 'l':
			// Set landscape mode
			m_Landscape = true;
			break;
			
		case 'o':
			// Set output file
			m_OutputFile = optarg;
			m_OutputFileSet = true;
			break;
			
		case 'P':
			// Set preprocessor
			SetPreprocessor(optarg);
			break;
		
		case 'f':
			// Set font face
			SetFontFace(optarg);
			break;
			
		case 's':
			// Set font size
			SetFontSize(optarg);
			break;
			
		case 'h':
			// help
			PrintHelp();
			exit(1);
			
		case '?':
			// unknown option
			exit(-1);
			
		default:
			assert(0);		
		}
	}
	
	if (!m_OutputFileSet)
	{
		std::cerr << m_ProgName << ": you must specify an output file with --output output.pdf" << std::endl;
		exit(-1);
	}
	
	// optind is the index of the first file arg
	if (optind >= argc)
	{
		std::cerr << m_ProgName << ": you must specify an input file!" << std::endl;
		exit(-1);
	}
	
	if (optind + 1 < argc)
	{
		std::cerr << m_ProgName << ": too many input files. Only single input file is allowed." << std::endl;
		exit(-1);
	}
	
	m_InputFile = argv[optind];
}
	
const PageSize &CmdLineParser::GetPageSize() const
{
	return m_PageSize;
}

bool CmdLineParser::GetLandscape() const
{
	return m_Landscape;
}

ICharPreprocessor *CmdLineParser::GetPreprocessor() const
{
	return m_Preprocessor;
}

const std::string &CmdLineParser::GetOutputFile() const
{
	return m_OutputFile;
}

const std::string &CmdLineParser::GetInputFile() const
{
	return m_InputFile;
}

const std::string &CmdLineParser::GetFontFace() const
{
	return m_FontFace;
}

double CmdLineParser::GetFontSize() const
{
	return m_FontSize;
}

void CmdLineParser::SetPageSize(const char *arg)
{
	if (!strcmp(arg, "list"))
	{
		std::cout << m_ProgName << ": supported page sizes:" << std::endl;
		PageSizeFactory::Print(std::cout);
		exit(0);
	}
		
	const PageSize *p = PageSizeFactory::Lookup(arg);
	
	if (!p)
	{
		std::cerr << m_ProgName << ": unknown page size. Use --page list to get a list." << std::endl;
		exit(1);
	}
		
	m_PageSize = *p;
}

void CmdLineParser::SetPreprocessor(const char *arg)
{
	if (!strcmp(arg, "list"))
	{
		std::cout << m_ProgName << ": supported preprocessors:" << std::endl;
		PreprocessorFactory::Print(std::cout);
		exit(0);
	}
		
	ICharPreprocessor *p = PreprocessorFactory::Lookup(arg);
	
	if (!p)
	{
		std::cerr << m_ProgName << ": unknown preprocessor. Use --preprocessor list to get a list." << std::endl;
		exit(1);
	}
		
	m_Preprocessor = p;
}
	
void CmdLineParser::SetFontFace(const char *arg)
{
	m_FontFace = arg;
	//m_FontSize = 7.0;
}
	
void CmdLineParser::SetFontSize(const char *arg)
{
	if (sscanf(arg, "%lf", &m_FontSize) != 1)
	{
		std::cerr << m_ProgName << ": wrong font size: " << arg << std::endl;
	}
}
	
void CmdLineParser::PrintHelp()
{
	std::cout << m_ProgName << ": some help" << std::endl;
}

const struct option CmdLineParser::long_options[] =
{
	{"page",		required_argument,	0,	'p'},
	{"landscape",	no_argument,		0,	'l'},
	{"output",		required_argument,	0,	'o'},
 	{"preprocessor",required_argument,	0,	'P'},
 	{"font-face",	required_argument,	0,	'f'},
 	{"font-size",	required_argument,	0,	's'},
 	{"help",		no_argument,		0,	'h'},
	{ 0, 0, 0, 0 }
};
	
const char *CmdLineParser::short_options="p:lo:P:h";	
