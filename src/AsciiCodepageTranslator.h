/*
 * Copyright (C) 2020 Peter Kessen <p.kessen at kessen-peter.de>
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

#include <sstream>
#include <string>
#include "CairoTTY.h"

class AsciiCodepageTranslator : public ICodepageTranslator
{
public:
    virtual bool translate(uint8_t in, gunichar &out);
private:
};

