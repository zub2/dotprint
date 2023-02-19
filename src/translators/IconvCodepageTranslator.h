/*
 * Copyright (C) 2023 Peter Kessen <p.kessen at kessen-peter.de>
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

#ifndef ICONV_CODEPAGE_TRANSLATOR_H
#define ICONV_CODEPAGE_TRANSLATOR_H

#include <string>

#include <glibmm.h>
#include <iconv.h>

#include "../CairoTTY.h"

class IconvCodepageTranslator : public ICodepageTranslator
{
public:
    explicit IconvCodepageTranslator(const std::string &sourceEncodingName);
    ~IconvCodepageTranslator();

    virtual bool translate(uint8_t in, gunichar &out) override;

private:
    iconv_t m_iconvDescriptor;
};

#endif // ICONV_CODEPAGE_TRANSLATOR_H
