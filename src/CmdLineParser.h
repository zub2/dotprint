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

#ifndef CMDLINEPARSER_H_
#define CMDLINEPARSER_H_

#include "CairoTTY.h"

class CmdLineParser
{
public:
    CmdLineParser(int argc, char* const argv[]);
    const PageSize &GetPageSize() const;
    const Margins &GetPageMargins() const;
    bool GetLandscape() const;
    const std::string &GetOutputFile() const;
    const std::string &GetInputFile() const;
    ICharPreprocessor *GetPreprocessor() const;
    ICodepageTranslator *GetCodepageTranslator() const;
    const std::string &GetFontFace() const;
    double GetFontSize() const;

protected:
    void SetPageSize(const char *arg);
    void SetPageMargins(const char *arg);
    void SetPreprocessor(const char *arg);
    void SetTranslator(const char *arg);
    void SetFontFace(const char *arg);
    void SetFontSize(const char *arg);

    void PrintHelp();

private:
    static const struct option LONG_OPTIONS[];
    static const char *SHORT_OPTIONS;

    static const char *DEFAULT_FONT_FACE;
    static const double DEFAULT_FONT_SIZE;

    const std::string m_ProgName;

    PageSize    m_PageSize;
    Margins     m_PageMargins;
    bool        m_Landscape;
    ICharPreprocessor *m_Preprocessor;
    ICodepageTranslator *m_Translator;
    std::string m_OutputFile;
    bool m_OutputFileSet;
    std::string m_InputFile;
    std::string m_FontFace;
    double m_FontSize;
};

#endif /*CMDLINEPARSER_H_*/
