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

#include <iostream>
#include <stdexcept>
#include <string>

#include <unistd.h>
#include <getopt.h>

#include "CmdLineParser.h"
#include "PageSizeFactory.h"
#include "MarginsFactory.h"
#include "PreprocessorFactory.h"
#include "translators/AsciiCodepageTranslator.h"
#include "translators/CodepageTranslator.h"
#include "translators/IconvCodepageTranslator.h"

const struct option CmdLineParser::LONG_OPTIONS[] =
{
    {"page",        required_argument,  0,  'p'},
    {"landscape",   no_argument,        0,  'l'},
    {"output",      required_argument,  0,  'o'},
    {"preprocessor",required_argument,  0,  'P'},
    {"translator",  required_argument,  0,  't'},
    {"iconv-translator", required_argument, 0, 'T'},
    {"font-face",   required_argument,  0,  'f'},
    {"font-size",   required_argument,  0,  's'},
    {"margins",     required_argument,  0,  'm'},
    {"help",        no_argument,        0,  'h'},
    { 0, 0, 0, 0 }
};

const char *CmdLineParser::SHORT_OPTIONS="p:lo:P:t:T:f:s:m:h";

const char *CmdLineParser::DEFAULT_FONT_FACE = "Courier New";
const double CmdLineParser::DEFAULT_FONT_SIZE = 11.0;

CmdLineParser::CmdLineParser(int argc, char* const argv[]):
    m_progName((argc>0 && argv[0] != nullptr)? argv[0] : "dotprint"),
    m_pageSize(PageSizeFactory::getDefault()),
    m_pageMargins(MarginsFactory::getDefault()),
    m_isLandscape(false),
    m_preprocessor(PreprocessorFactory::getDefault()),
    m_outputFileSet(false),
    m_fontFace(DEFAULT_FONT_FACE),
    m_fontSize(DEFAULT_FONT_SIZE)
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
            setPageSize(optarg);
            break;

        case 'l':
            // Set landscape mode
            m_isLandscape = true;
            break;

        case 'o':
            // Set output file
            m_outputFile = optarg;
            m_outputFileSet = true;
            break;

        case 'P':
            setPreprocessor(optarg);
            break;

        case 't':
            setTranslator(optarg);
            break;

        case 'T':
            setIconvTranslator(optarg);
            break;

        case 'f':
            setFontFace(optarg);
            break;

        case 's':
            setFontSize(optarg);
            break;

        case 'm':
            setPageMargins(optarg);
            break;

        case 'h':
            printHelp();
            exit(1);

        case '?':
            // unknown option
            exit(-1);

        default:
            throw std::runtime_error("Unexpectd argument");
        }
    }

    if (!m_outputFileSet)
    {
        std::cerr << m_progName << ": you must specify an output file with --output output.pdf\n";
        exit(-1);
    }

    if (!m_translatorArg.empty() && !m_iconvTranslatorArg.empty())
    {
        std::cerr << m_progName << ": at most one of -t and -T may be specified\n";
        exit(-1);
    }

    // optind is the index of the first file arg
    if (optind >= argc)
    {
        std::cerr << m_progName << ": you must specify an input file!\n";
        exit(-1);
    }

    if (optind + 1 < argc)
    {
        std::cerr << m_progName << ": too many input files. Only single input file is allowed.\n";
        exit(-1);
    }

    m_inputFile = argv[optind];
}

const PageSize &CmdLineParser::getPageSize() const
{
    return m_pageSize;
}

const Margins &CmdLineParser::getPageMargins() const
{
    return m_pageMargins;
}

bool CmdLineParser::isLandscape() const
{
    return m_isLandscape;
}

ICharPreprocessor *CmdLineParser::getPreprocessor() const
{
    return m_preprocessor;
}

std::unique_ptr<ICodepageTranslator> CmdLineParser::getCodepageTranslator() const
{
    if (!m_iconvTranslatorArg.empty())
    {
        return std::make_unique<IconvCodepageTranslator>(m_iconvTranslatorArg);
    }

    if (!m_translatorArg.empty())
    {
        return std::make_unique<CodepageTranslator>(m_translatorArg);
    }

    // nothing specified, fall back to ASCII
    return std::make_unique<AsciiCodepageTranslator>();
}

const std::string &CmdLineParser::getOutputFile() const
{
    return m_outputFile;
}

const std::string &CmdLineParser::getInputFile() const
{
    return m_inputFile;
}

const std::string &CmdLineParser::getFontFace() const
{
    return m_fontFace;
}

double CmdLineParser::getFontSize() const
{
    return m_fontSize;
}

void CmdLineParser::setPageSize(const char *arg)
{
    if (!strcmp(arg, "list"))
    {
        std::cout << m_progName << ": supported page sizes:\n";
        PageSizeFactory::print(std::cout);
        exit(0);
    }

    const PageSize *p = PageSizeFactory::lookup(arg);

    if (!p)
    {
        std::cerr << m_progName << ": unknown page size. Use --page list to get a list.\n";
        exit(1);
    }

    m_pageSize = *p;
}

void CmdLineParser::setPageMargins(const char *arg)
{
    double mtop, mright, mbottom, mleft;

    if (!strcmp(arg, "formats"))
    {
        std::cout << m_progName << ": supported margin formats:\n\n"
            "number:               one value for all margins.\n"
            "num1,num2:            top & bottom, then left & right.\n"
            "num1,num2,num3:       top, then left & right, then bottom.\n"
            "num1,num2,num3,num4:  top, then right, then bottom, then left.\n";
        exit(0);
    }

    switch (sscanf(arg, "%lf,%lf,%lf,%lf", &mtop, &mright, &mbottom, &mleft))
    {
    case 1:
        m_pageMargins.top = mtop * milimeter;
        m_pageMargins.right = mtop * milimeter;
        m_pageMargins.bottom = mtop * milimeter;
        m_pageMargins.left = mtop * milimeter;
        break;
    case 2:
        m_pageMargins.top = mtop * milimeter;
        m_pageMargins.right = mright * milimeter;
        m_pageMargins.bottom = mtop * milimeter;
        m_pageMargins.left = mright * milimeter;
        break;
    case 3:
        m_pageMargins.top = mtop * milimeter;
        m_pageMargins.right = mright * milimeter;
        m_pageMargins.bottom = mbottom * milimeter;
        m_pageMargins.left = mright * milimeter;
        break;
    case 4:
        m_pageMargins.top = mtop * milimeter;
        m_pageMargins.right = mright * milimeter;
        m_pageMargins.bottom = mbottom * milimeter;
        m_pageMargins.left = mleft * milimeter;
        break;
    default:
        std::cerr << m_progName << ": wrong margin format: " << arg << '\n'
            << m_progName << ": Use \"--margins formats\" to get a list of valid formats.\n";
        exit(1);
    }
}

void CmdLineParser::setPreprocessor(const char *arg)
{
    if (!strcmp(arg, "list"))
    {
        std::cout << m_progName << ": supported preprocessors:\n";
        PreprocessorFactory::print(std::cout);
        exit(0);
    }

    ICharPreprocessor *p = PreprocessorFactory::lookup(arg);

    if (!p)
    {
        std::cerr << m_progName << ": unknown preprocessor. Use --preprocessor list to get a list.\n";
        exit(1);
    }

    m_preprocessor = p;
}

void CmdLineParser::setTranslator(const char *arg)
{
    m_translatorArg = arg;
}

void CmdLineParser::setIconvTranslator(const char *arg)
{
    m_iconvTranslatorArg = arg;
}

void CmdLineParser::setFontFace(const char *arg)
{
    m_fontFace = arg;
}

void CmdLineParser::setFontSize(const char *arg)
{
    if (sscanf(arg, "%lf", &m_fontSize) != 1)
    {
        std::cerr << m_progName << ": wrong font size: " << arg << '\n';
    }
}

void CmdLineParser::printHelp()
{
    std::cout <<
        "Usage: " << m_progName << " [OPTION]... INPUT_FILE -o OUTPUT_FILE\n"
        "Convert input text file into a PDF.\n"
        "  -o, --output        Specify output file (PDF). Required.\n"
        "  -p, --page          Specify page size.\n"
        "                      Use \"-p list\" to see available values.\n"
        "  -l, --landscape     Set landscape mode.\n"
        "  -P, --preprocessor  Select preprocessor to use.\n"
        "                      Use \"-P list\" to see available values.\n"
        "  -t, --translator    Select codepage translator to use.\n"
        "                      Use a translation file as argument.\n"
        "  -T, --iconv-translator Use iconv for translating the input.\n"
        "                      Use a character set name that iconv can recognize\n"
        "                      like CP850. Only single-byte encodings are supported.\n"
        "  -f, --font-face     Font to use.\n"
        "                      Default value: \"" << DEFAULT_FONT_FACE << "\"\n"
        "  -s, --font-size     Font size to use.\n"
        "                      Default value: " << DEFAULT_FONT_SIZE << "\n"
        "  -m, --margins       Set page margins (in millimeters).\n"
        "                      Use \"-m formats\" to see available formats.\n"
        "                      Default value: " << MarginsFactory::DEFAULT_MARGIN_VALUE << " mm for all margins.\n"
        "  -h, --help          Display this help.\n";
}
