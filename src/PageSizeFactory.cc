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

#include <string.h>
#include "PageSizeFactory.h"

void PageSizeFactory::Print(std::ostream &s)
{
    int i = 0;
    while (m_Pages[i].name != NULL)
    {
        s << m_Pages[i].name;
        if (i == 0)
            s << " [default]";
        s << std::endl;
        ++i;
    }
}

const PageSize *PageSizeFactory::Lookup(const char *name)
{
    int i = 0;

    while (m_Pages[i].name != NULL && strcmp(m_Pages[i].name, name))
        ++i;

    if (m_Pages[i].name == NULL)
        return NULL;

    return &m_Pages[i].page;
}

const PageSize &PageSizeFactory::GetDefault()
{
    return m_Pages[0].page;
}

const PageSizeFactory::Page PageSizeFactory::m_Pages[] =
{
    { "A4", PageSize(210.0 * milimeter, 297.0 * milimeter) },
    { "A5", PageSize(148.5 * milimeter, 210.0 * milimeter) },
    { 0, PageSize(0.0, 0.0) }
};
