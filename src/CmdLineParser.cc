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
#include <string.h>

#include <unistd.h>
#include <getopt.h>

#include "CmdLineParser.h"
#include "PageSizeFactory.h"
#include "MarginsFactory.h"
#include "PreprocessorFactory.h"
#include "CodepageTranslator.h"

const struct option CmdLineParser::LONG_OPTIONS[] =
{
    {"page",        required_argument,  0,  'p'},
    {"landscape",   no_argument,        0,  'l'},
    {"output",      required_argument,  0,  'o'},
    {"preprocessor",required_argument,  0,  'P'},
    {"translator",  required_argument,  0,  't'},
    {"font-face",   required_argument,  0,  'f'},
    {"font-size",   required_argument,  0,  's'},
    {"margins",     required_argument,  0,  'm'},
    {"help",        no_argument,        0,  'h'},
    { 0, 0, 0, 0 }
};

const char *CmdLineParser::SHORT_OPTIONS="p:lo:P:t:m:h";

const char *CmdLineParser::DEFAULT_FONT_FACE = "Courier New";
const double CmdLineParser::DEFAULT_FONT_SIZE = 11.0;

CmdLineParser::CmdLineParser(int argc, char* const argv[]):
    m_ProgName((argc>0 && argv[0] != nullptr)? argv[0] : "dotprint"),
    m_PageSize(PageSizeFactory::GetDefault()),
    m_PageMargins(MarginsFactory::GetDefault()),
    m_Landscape(false),
    m_Preprocessor(PreprocessorFactory::GetDefault()),
    m_Translator(nullptr),
    m_OutputFileSet(false),
    m_FontFace(DEFAULT_FONT_FACE),
    m_FontSize(DEFAULT_FONT_SIZE)
{
    while (true)
    {
        int option_index = 0;

        int c = getopt_long(argc, argv, SHORT_OPTIONS, LONG_OPTIONS, &option_index);
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

        case 't':
            // Set codepage translator
            SetTranslator(optarg);
            break;

        case 'f':
            // Set font face
            SetFontFace(optarg);
            break;

        case 's':
            // Set font size
            SetFontSize(optarg);
            break;

        case 'm':
            // Set page margins
            SetPageMargins(optarg);
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

const Margins &CmdLineParser::GetPageMargins() const
{
    return m_PageMargins;
}

bool CmdLineParser::GetLandscape() const
{
    return m_Landscape;
}

ICharPreprocessor *CmdLineParser::GetPreprocessor() const
{
    return m_Preprocessor;
}

ICodepageTranslator *CmdLineParser::GetCodepageTranslator() const
{
    return m_Translator;
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

void CmdLineParser::SetPageMargins(const char *arg)
{
    double mtop, mright, mbottom, mleft;

    if (!strcmp(arg, "formats"))
    {
        std::cout << m_ProgName << ": supported margin formats:" << std::endl << std::endl;
        std::cout << "number:               one value for all margins." << std::endl;
        std::cout << "num1,num2:            top & bottom, then left & right." << std::endl;
        std::cout << "num1,num2,num3:       top, then left & right, then bottom." << std::endl;
        std::cout << "num1,num2,num3,num4:  top, then right, then bottom, then left." << std::endl;
        exit(0);
    }

    switch (sscanf(arg, "%lf,%lf,%lf,%lf", &mtop, &mright, &mbottom, &mleft))
    {
        case 1:
            m_PageMargins.m_Top = mtop * milimeter;
            m_PageMargins.m_Right = mtop * milimeter;
            m_PageMargins.m_Bottom = mtop * milimeter;
            m_PageMargins.m_Left = mtop * milimeter;
            break;
        case 2:
            m_PageMargins.m_Top = mtop * milimeter;
            m_PageMargins.m_Right = mright * milimeter;
            m_PageMargins.m_Bottom = mtop * milimeter;
            m_PageMargins.m_Left = mright * milimeter;
            break;
        case 3:
            m_PageMargins.m_Top = mtop * milimeter;
            m_PageMargins.m_Right = mright * milimeter;
            m_PageMargins.m_Bottom = mbottom * milimeter;
            m_PageMargins.m_Left = mright * milimeter;
            break;
        case 4:
            m_PageMargins.m_Top = mtop * milimeter;
            m_PageMargins.m_Right = mright * milimeter;
            m_PageMargins.m_Bottom = mbottom * milimeter;
            m_PageMargins.m_Left = mleft * milimeter;
            break;
        default:
            std::cerr << m_ProgName << ": wrong margin format: " << arg << std::endl;
            std::cerr << m_ProgName << ": Use \"--margins formats\" to get a list of valid formats." << std::endl;
            exit(1);
    }
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

void CmdLineParser::SetTranslator(const char *arg)
{
    CodepageTranslator *t = new CodepageTranslator();

    t->loadTable(arg);

    m_Translator = t;
}

void CmdLineParser::SetFontFace(const char *arg)
{
    m_FontFace = arg;
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
    std::cout << "Usage: " << m_ProgName << " [OPTION]... INPUT_FILE -o OUTPUT_FILE" << std::endl;
    std::cout << "Convert input text file into a PDF." << std::endl << std::endl;

    std::cout << "  -o, --output        Specify output file (PDF). Required." << std::endl;
    std::cout << "  -p, --page          Specify page size." << std::endl;
    std::cout << "                      Use \"-p list\" to see available values." << std::endl;
    std::cout << "  -l, --landscape     Set landscape mode." << std::endl;
    std::cout << "  -P, --preprocessor  Select preprocessor to use." << std::endl;
    std::cout << "                      Use \"-P list\" to see available values." << std::endl;
    std::cout << "  -t, --translator    Select codepage translator to use." << std::endl;
    std::cout << "  -f, --font-face     Font to use." << std::endl;
    std::cout << "                      Default value: \"" << DEFAULT_FONT_FACE << "\"" << std::endl;
    std::cout << "  -s, --font-size     Font size to use." << std::endl;
    std::cout << "                      Default value: " << DEFAULT_FONT_SIZE << std::endl;
    std::cout << "  -m, --margins       Set page margins (in millimeters)." << std::endl;
    std::cout << "                      Use \"-m formats\" to see available formats." << std::endl;
    std::cout << "                      Default value: " << MarginsFactory::DEFAULT_MARGIN_VALUE << " mm for all margins." << std::endl;
    std::cout << "  -h, --help          Display this help." << std::endl;
}
