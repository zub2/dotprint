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

#include "EpsonPreprocessor.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <type_traits>

#include <glibmm.h>

EpsonPreprocessor::EpsonPreprocessor():
    m_inputState(InputState::InputNormal),
    m_escapeState(EscapeState::Entered), // not used unless m_inputState is Escape
    m_fontSizeState(FontSizeState::FontSizeNormal)
{}

void EpsonPreprocessor::process(ICairoTTYProtected &ctty, uint8_t c)
{
    if (m_inputState == InputState::Escape)
        handleEscape(ctty, c);
    else
    {
        // Control codes handled here
        switch (c)
        {
        case 0x0e: // Expanded printing for one line
            ctty.stretchFont(2.0);
            m_fontSizeState = FontSizeState::SingleLineExpanded;
            break;

        case 0x14: // Cancel one-line expanded printing
            ctty.stretchFont(1.0);
            m_fontSizeState = FontSizeState::FontSizeNormal;
            break;

        case 0x0f: // Condensed printing
            ctty.stretchFont(static_cast<double>(CONDENSED_CPI)/STANDARD_CPI); // Change from 10 CPI to 17 CPI
            m_fontSizeState = FontSizeState::Condensed;
            break;

        case 0x12: // Cancel condensed printing
            ctty.stretchFont(1.0);
            m_fontSizeState = FontSizeState::FontSizeNormal;
            break;

        case '\r': // Carriage Return
            ctty.carriageReturn();
            break;

        case '\n': // Line Feed
            if (m_fontSizeState == FontSizeState::SingleLineExpanded)
            {
                ctty.stretchFont(1.0);
                m_fontSizeState = FontSizeState::FontSizeNormal;
            }
            ctty.lineFeed();
            break;

        case 0x0c: // Form Feed
            ctty.newPage();
            break;

        case 0x1b: // Escape
            m_inputState = InputState::Escape;
            m_escapeState = EscapeState::Entered;
            break;

        default:
            ctty.append(static_cast<char>(c));
            break;
        }
    }
}

void EpsonPreprocessor::handleEscape(ICairoTTYProtected &ctty, uint8_t c)
{
    // Determine what escape code follows
    if (m_escapeState == EscapeState::Entered)
    {
        switch (c)
        {
        case 0x45: // Set bold
            ctty.setFontWeight(FontWeight::Bold);
            break;
        case 0x46: // Unset bold
            ctty.setFontWeight(FontWeight::Normal);
            break;
        case 0x34: // Set italic
            ctty.setFontSlant(FontSlant::Italic);
            break;
        case 0x35: // Unset italic
            ctty.setFontSlant(FontSlant::Normal);
            break;
        case 0x2a: // '*': Draw Graphics
            m_graphicAssembledBytes = 0;
            m_escapeState = EscapeState::DrawGraphics;
            break;
        case 0x2d: // Underline
            m_escapeState = EscapeState::Underline;
            break;
        case 0x33: // '3': Set n/180-inch line spacing
            m_escapeState = EscapeState::SetLineSpacing;
            break;
        case 0x40: // '@': Initialize
            // TODO: add init for Cairo here
            m_inputState = InputState::InputNormal; // Leave escape state
            break;
        case 0x44: // 'D': Set horizontal tabs
            m_escapeState = EscapeState::SetTabWidth;
            break;
        case 0x78: // 'x': Select LQ or draft
            m_escapeState = EscapeState::SelectQuality;
            break;

        default:
            std::cerr << "EpsonPreprocessor::handleEscape(): ignoring unknown escape ESC 0x"
                << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(c) << std::endl;
            m_inputState = InputState::InputNormal; // Leave escape state
        }

        /*
         * If still in the escape state (no change to special), assume
         * the operation has completed, and exit the escape state.
         *
         * This includes the error condition, as no change is done either.
         */
        if (m_escapeState == EscapeState::Entered)
        {
            m_inputState = InputState::InputNormal;
        }

        return;
    }

    // For multi-byte escapes:
    switch (m_escapeState)
    {
    case EscapeState::Underline: // ESC 0x2d u, turn underline off (0) or on (!0)
        //std::cerr << "escape: ESC 0x2d " << std::hex << c << std::endl;
        if (c != 0)
        {
            // TODO: Set underline
            //std::cerr << "EpsonPreprocessor::handleEscape(): Ignoring unsupported escape: SET UNDERLINE" << std::endl;
        }
        else
        {
            // TODO: Cancel underline
            //std::cerr << "EpsonPreprocessor::handleEscape(): Ignoring unsupported escape: DROP UNDERLINE" << std::endl;
        }
        m_inputState = InputState::InputNormal; // Leave escape state
        break;

    case EscapeState::SetLineSpacing:
        // TODO: implement line spacing in CairoTTY
        m_inputState = InputState::InputNormal;
        break;

    case EscapeState::SetTabWidth: // Set width of tabs
        if (0 == c)
        {
            m_inputState = InputState::InputNormal; // Leave escape state
        }
        else
        {
            // TODO: set tab width for CairoTTY or handle tabs here
        }
        break;

    case EscapeState::SelectQuality:
        // TODO: select quality?
        m_inputState = InputState::InputNormal;
        break;

    case EscapeState::DrawGraphics: // Insert tabs in text
        handleGraphics(ctty, c);
        break;

    default:
        throw std::runtime_error("EpsonPreprocessor::handleEscape(): Internal error: entered unknown escape state "
            + std::to_string(static_cast<std::underlying_type_t<decltype(m_escapeState)>>(m_escapeState)));
    }
}

void EpsonPreprocessor::handleGraphics(ICairoTTYProtected & /*ctty*/, uint8_t c)
{
    if (m_graphicAssembledBytes == 0)
    {
        m_graphicsMode = c;
        m_graphicAssembledBytes = 1;
    }
    else if (m_graphicAssembledBytes == 1)
    {
        m_graphicsNrColumns = c;
        m_graphicAssembledBytes = 2;
    }
    else if (m_graphicAssembledBytes == 2)
    {
        m_graphicsNrColumns += c * 256;
        m_graphicAssembledBytes = 3;
    }
    else
    {
        if (m_graphicAssembledBytes == 3)
        {
            m_graphicsMaxBytes = m_graphicsNrColumns * 3;
        }

        int rowGroup = m_graphicAssembledBytes % 3;
        m_graphicsCol |= ((unsigned int) c) << (8 * (2 - rowGroup));
        if (rowGroup == 2)
        {
            m_graphicsCol = 0;
        }

        ++m_graphicAssembledBytes;
        if (m_graphicAssembledBytes >= (m_graphicsMaxBytes + 3))
        {
            m_inputState = InputState::InputNormal; // Leave escape state
        }
    }
}
