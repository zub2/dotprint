/*
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
#include <sstream>
#include "CairoTTY.h"

class CodepageTranslator : public ICodepageTranslator
{
public:
    CodepageTranslator();
    virtual ~CodepageTranslator();

    virtual bool translate(unsigned char in, gunichar &out);

private:
    typedef std::map<char, std::string> TTransTable;

    TTransTable m_table;
};

