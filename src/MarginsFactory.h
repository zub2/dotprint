/*
 * Copyright (C) 2009, 2012, 2014, 2020 David Kozub <zub at linux.fjfi.cvut.cz>
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

#ifndef MARGINSFACTORY_H_
#define MARGINSFACTORY_H_

#include "CairoTTY.h"

class MarginsFactory
{
public:
    static const double DEFAULT_MARGIN_VALUE;
    static const Margins &GetDefault();

    MarginsFactory() = delete;
};

#endif /*MARGINSFACTORY_H_*/
