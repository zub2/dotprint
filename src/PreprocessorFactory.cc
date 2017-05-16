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

    const std::map<std::string, ICharPreprocessor*> Preprocessors =
    {
        { "simple", &Simple },
        { "crlf", &CRLF },
        { "epson", &Epson }
    };

    ICharPreprocessor* DefaultPreprocessor = &Simple;
}

void PreprocessorFactory::Print(std::ostream &s)
{
    for (const auto& preprocessor: Preprocessors)
    {
        s << preprocessor.first;
        if (preprocessor.second == DefaultPreprocessor)
            s << " [default]";
        s << std::endl;
    }
}

ICharPreprocessor* PreprocessorFactory::Lookup(const std::string& name)
{
    auto it = Preprocessors.find(name);
    return it != Preprocessors.end() ? it->second : nullptr;
}

ICharPreprocessor* PreprocessorFactory::GetDefault()
{
    return Preprocessors.begin()->second;
}
