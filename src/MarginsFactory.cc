/*
 * Copyright (C) 2009, 2012, 2014, 2020 Felipe BF <smprg.6502 at gmail.com>
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

#include "MarginsFactory.h"
#include "PageSizeFactory.h"

const double MarginsFactory::DEFAULT_MARGIN_VALUE = 10.0;

namespace {
    const Margins DefaultPageMargins(MarginsFactory::DEFAULT_MARGIN_VALUE * milimeter,
            MarginsFactory::DEFAULT_MARGIN_VALUE * milimeter,
            MarginsFactory::DEFAULT_MARGIN_VALUE * milimeter,
            MarginsFactory::DEFAULT_MARGIN_VALUE * milimeter);
}

const Margins &MarginsFactory::GetDefault()
{
    return DefaultPageMargins;
}
