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

#include "PageSizeFactory.h"

namespace
{
    PageSize PageSizeA4(210.0 * milimeter, 297.0 * milimeter);
    PageSize PageSizeA5(148.5 * milimeter, 210.0 * milimeter);
    PageSize PageSizeLetter(215.9 * milimeter, 279.4 * milimeter);
    PageSize PageSizeHalfLetter(139.7 * milimeter, 215.9 * milimeter);
    PageSize PageSizeLegal(215.9 * milimeter, 355.6 * milimeter);
    PageSize PageSizeOficio(215.9 * milimeter, 340.36 * milimeter);

    const std::map<std::string, PageSize*> PageSizes =
    {
        { "A4", &PageSizeA4 },
        { "A5", &PageSizeA5 },
        { "Letter", &PageSizeLetter },
        { "Half Letter", &PageSizeHalfLetter },
        { "Legal", &PageSizeLegal },
        { "Oficio", &PageSizeOficio }
    };

    PageSize* DefaultPageSize = &PageSizeA4;
}

void PageSizeFactory::Print(std::ostream &s)
{
    for (const auto& entry : PageSizes)
    {
        s << entry.first;
        if (entry.second == DefaultPageSize)
            s << " [default]";
        s << std::endl;
    }
}

const PageSize *PageSizeFactory::Lookup(const std::string& name)
{
    auto it = PageSizes.find(name);
    return it != PageSizes.end() ? it->second : nullptr;
}

const PageSize &PageSizeFactory::GetDefault()
{
    return *DefaultPageSize;
}
