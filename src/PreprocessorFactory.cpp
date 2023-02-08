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

#include <map>

#include "PreprocessorFactory.h"
#include "preprocessors/SimplePreprocessor.h"
#include "preprocessors/CRLFPreprocessor.h"
#include "preprocessors/EpsonPreprocessor.h"

namespace
{
    SimplePreprocessor Simple;
    CRLFPreprocessor CRLF;
    EpsonPreprocessor Epson;

    const std::map<std::string, ICharPreprocessor*> PREPROCESSORS =
    {
        { "simple", &Simple },
        { "crlf", &CRLF },
        { "epson", &Epson }
    };

    ICharPreprocessor* DEFAULT_PREPROCESSOR = &Epson;
}

void PreprocessorFactory::print(std::ostream &s)
{
    for (const auto& preprocessor: PREPROCESSORS)
    {
        s << preprocessor.first;
        if (preprocessor.second == DEFAULT_PREPROCESSOR)
            s << " [default]";
        s << '\n';
    }
}

ICharPreprocessor* PreprocessorFactory::lookup(const std::string& name)
{
    const auto it = PREPROCESSORS.find(name);
    return it != PREPROCESSORS.end() ? it->second : nullptr;
}

ICharPreprocessor* PreprocessorFactory::getDefault()
{
    return DEFAULT_PREPROCESSOR;
}
