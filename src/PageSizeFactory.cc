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

#include "PageSizeFactory.h"

#include <map>
#include <string>

namespace
{
    constexpr PageSize PAGE_SIZE_A4(210.0 * milimeter, 297.0 * milimeter);
    constexpr PageSize PAGE_SIZE_A5(148.5 * milimeter, 210.0 * milimeter);
    constexpr PageSize PAGE_SIZE_Letter(215.9 * milimeter, 279.4 * milimeter);
    constexpr PageSize PAGE_SIZE_HalfLetter(139.7 * milimeter, 215.9 * milimeter);
    constexpr PageSize PAGE_SIZE_Legal(215.9 * milimeter, 355.6 * milimeter);
    constexpr PageSize PAGE_SIZE_Oficio(215.9 * milimeter, 340.36 * milimeter);

    const std::map<std::string, const PageSize*> PAGE_SIZES =
    {
        { "A4", &PAGE_SIZE_A4 },
        { "A5", &PAGE_SIZE_A5 },
        { "Letter", &PAGE_SIZE_Letter },
        { "Half Letter", &PAGE_SIZE_HalfLetter },
        { "Legal", &PAGE_SIZE_Legal },
        { "Oficio", &PAGE_SIZE_Oficio }
    };

    const PageSize* DEFAULT_PAGE_SIZE = &PAGE_SIZE_A4;
}

void PageSizeFactory::print(std::ostream &s)
{
    for (const auto& entry : PAGE_SIZES)
    {
        s << entry.first;
        if (entry.second == DEFAULT_PAGE_SIZE)
            s << " [default]";
        s << '\n';
    }
}

const PageSize *PageSizeFactory::lookup(const std::string& name)
{
    const auto it = PAGE_SIZES.find(name);
    return it != PAGE_SIZES.end() ? it->second : nullptr;
}

const PageSize &PageSizeFactory::getDefault()
{
    return *DEFAULT_PAGE_SIZE;
}
