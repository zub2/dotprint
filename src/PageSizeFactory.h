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

#ifndef PAGESIZEFACTORY_H_
#define PAGESIZEFACTORY_H_

#include "CairoTTY.h"

/*
 * 1 inch = 2.54 centimeters
 * 1 point = 1/72.0 inch
 *
 * 72.0 points =2.54 cm
 *
 * 1 cm = 72.0/2.54 points
 */

/** \brief 1 cm in points. */
const double milimeter = 72.0/25.4;

class PageSizeFactory
{
public:
    static void Print(std::ostream &s);
    static const PageSize *Lookup(const char *name);
    static const PageSize &GetDefault();

protected:
    PageSizeFactory()
    {}

private:
    struct Page
    {
        const char *name;
        PageSize page;
    };

    static const Page m_Pages[];
};

#endif /*PAGESIZEFACTORY_H_*/
