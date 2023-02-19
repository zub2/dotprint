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

#ifndef CMD_LINE_PARSER_H_
#define CMD_LINE_PARSER_H_

#include <string>
#include <memory>

#include "CairoTTY.h"

class CmdLineParser
{
public:
    CmdLineParser(int argc, char* const argv[]);
    const PageSize &getPageSize() const;
    const Margins &getPageMargins() const;
    bool isLandscape() const;
    const std::string & getOutputFile() const;
    const std::string & getInputFile() const;
    ICharPreprocessor * getPreprocessor() const;
    std::unique_ptr<ICodepageTranslator> getCodepageTranslator() const;
    const std::string & getFontFace() const;
    double getFontSize() const;

protected:
    void setPageSize(const char *arg);
    void setPageMargins(const char *arg);
    void setPreprocessor(const char *arg);
    void setTranslator(const char *arg);
    void setIconvTranslator(const char *arg);
    void setFontFace(const char *arg);
    void setFontSize(const char *arg);

    void printHelp();

private:
    static const struct option LONG_OPTIONS[];
    static const char *SHORT_OPTIONS;

    static const char *DEFAULT_FONT_FACE;
    static const double DEFAULT_FONT_SIZE;

    const std::string m_progName;

    PageSize m_pageSize;
    Margins m_pageMargins;
    bool m_isLandscape;
    ICharPreprocessor *m_preprocessor;
    std::string m_translatorArg;
    std::string m_iconvTranslatorArg;
    std::string m_outputFile;
    bool m_outputFileSet;
    std::string m_inputFile;
    std::string m_fontFace;
    double m_fontSize;
};

#endif // CMD_LINE_PARSER_H_
