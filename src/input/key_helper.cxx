//  $Id: key_helper.cxx,v 1.4 2003/04/19 10:23:18 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include <ClanLib/Display/Input/key.h>
#include "key_helper.hxx"

std::string
Input::KeyHelper::key_to_string (int key)
{
  switch (key)
    {
    case CL_KEY_0: return "0";
    case CL_KEY_1: return "1";
    case CL_KEY_2: return "2";
    case CL_KEY_3: return "3";
    case CL_KEY_4: return "4";
    case CL_KEY_5: return "5";
    case CL_KEY_6: return "6";
    case CL_KEY_7: return "7";
    case CL_KEY_8: return "8";
    case CL_KEY_9: return "9";
    case CL_KEY_A: return "A";
    case CL_KEY_ALT: return "ALT";
    case CL_KEY_ALTGR: return "ALTGR";
    case CL_KEY_B: return "B";
    case CL_KEY_BACKSPACE: return "BACKSPACE";
    case CL_KEY_C: return "C";
    case CL_KEY_CAPSLOCK: return "CAPSLOCK";
    case CL_KEY_D: return "D";
    case CL_KEY_DELETE: return "DELETE";
    case CL_KEY_DOWN: return "DOWN";
    case CL_KEY_E: return "E";
    case CL_KEY_END: return "END";
    case CL_KEY_ENTER: return "ENTER";
    case CL_KEY_ESCAPE: return "ESCAPE";
    case CL_KEY_F: return "F";
    case CL_KEY_F1: return "F1";
    case CL_KEY_F10: return "F10";
    case CL_KEY_F11: return "F11";
    case CL_KEY_F12: return "F12";
    case CL_KEY_F2: return "F2";
    case CL_KEY_F3: return "F3";
    case CL_KEY_F4: return "F4";
    case CL_KEY_F5: return "F5";
    case CL_KEY_F6: return "F6";
    case CL_KEY_F7: return "F7";
    case CL_KEY_F8: return "F8";
    case CL_KEY_F9: return "F9";
    case CL_KEY_G: return "G";
    case CL_KEY_H: return "H";
    case CL_KEY_HOME: return "HOME";
    case CL_KEY_I: return "I";
    case CL_KEY_INSERT: return "INSERT";
    case CL_KEY_J: return "J";
    case CL_KEY_K: return "K";
    case CL_KEY_KP_0: return "KP_0";
    case CL_KEY_KP_1: return "KP_1";
    case CL_KEY_KP_2: return "KP_2";
    case CL_KEY_KP_3: return "KP_3";
    case CL_KEY_KP_4: return "KP_4";
    case CL_KEY_KP_5: return "KP_5";
    case CL_KEY_KP_6: return "KP_6";
    case CL_KEY_KP_7: return "KP_7";
    case CL_KEY_KP_8: return "KP_8";
    case CL_KEY_KP_9: return "KP_9";
    case CL_KEY_KP_DECIMAL: return "KP_DECIMAL";
    case CL_KEY_KP_DIV: return "KP_DIV";
    case CL_KEY_KP_ENTER: return "KP_ENTER";
    case CL_KEY_KP_MINUS: return "KP_MINUS";
    case CL_KEY_KP_MULT: return "KP_MULT";
    case CL_KEY_KP_PLUS: return "KP_PLUS";
    case CL_KEY_L: return "L";
    case CL_KEY_LCTRL: return "LCTRL";
    case CL_KEY_LEFT: return "LEFT";
    case CL_KEY_LSHIFT: return "LSHIFT";
    case CL_KEY_M: return "M";
    case CL_KEY_N: return "N";
    case CL_KEY_NONE_OF_THE_ABOVE: return "NONE_OF_THE_ABOVE";
    case CL_KEY_NUMLOCK: return "NUMLOCK";
    case CL_KEY_O: return "O";
    case CL_KEY_P: return "P";
    case CL_KEY_PAGEDOWN: return "PAGEDOWN";
    case CL_KEY_PAGEUP: return "PAGEUP";
    case CL_KEY_PAUSE: return "PAUSE";
    case CL_KEY_PRINT: return "PRINT";
    case CL_KEY_Q: return "Q";
    case CL_KEY_R: return "R";
    case CL_KEY_RCTRL: return "RCTRL";
    case CL_KEY_RIGHT: return "RIGHT";
    case CL_KEY_RSHIFT: return "RSHIFT";
    case CL_KEY_S: return "S";
    case CL_KEY_SCRLOCK: return "SCRLOCK";
    case CL_KEY_SPACE: return "SPACE";
    case CL_KEY_T: return "T";
    case CL_KEY_TAB: return "TAB";
    case CL_KEY_U: return "U";
    case CL_KEY_UP: return "UP";
    case CL_KEY_V: return "V";
    case CL_KEY_W: return "W";
    case CL_KEY_X: return "X";
    case CL_KEY_Y: return "Y";
    case CL_KEY_Z: return "Z";
    case CL_MOUSE_LEFTBUTTON: return "CL_MOUSE_LEFTBUTTON";
    case CL_MOUSE_MIDDLEBUTTON: return "CL_MOUSE_MIDDLEBUTTON";
    case CL_MOUSE_RIGHTBUTTON: return "CL_MOUSE_RIGHTBUTTON";
    case CL_MOUSE_THUMBBUTTON: return "CL_MOUSE_THUMBBUTTON";
    case CL_MOUSE_WHEELBUTTON: return "CL_MOUSE_WHEELBUTTON";
    case CL_MOUSE_WHEELDOWN: return "CL_MOUSE_WHEELDOWN";
    case CL_MOUSE_WHEELUP: return "CL_MOUSE_WHEELUP";
    case CL_NO_KEY: return "CL_NO_KEY";
    case CL_NUM_KEYS: return "CL_NUM_KEYS";
    default:
      return "Unhandled key";
    }
}

int
Input::KeyHelper::string_to_key (const std::string& key)
{
  if (key == "0") { return CL_KEY_0; }
  else if (key == "1") { return CL_KEY_1; }
  else if (key == "2") { return CL_KEY_2; }
  else if (key == "3") { return CL_KEY_3; }
  else if (key == "4") { return CL_KEY_4; }
  else if (key == "5") { return CL_KEY_5; }
  else if (key == "6") { return CL_KEY_6; }
  else if (key == "7") { return CL_KEY_7; }
  else if (key == "8") { return CL_KEY_8; }
  else if (key == "9") { return CL_KEY_9; }
  else if (key == "A") { return CL_KEY_A; }
  else if (key == "ALT") { return CL_KEY_ALT; }
  else if (key == "ALTGR") { return CL_KEY_ALTGR; }
  else if (key == "B") { return CL_KEY_B; }
  else if (key == "BACKSPACE") { return CL_KEY_BACKSPACE; }
  else if (key == "C") { return CL_KEY_C; }
  else if (key == "CAPSLOCK") { return CL_KEY_CAPSLOCK; }
  else if (key == "D") { return CL_KEY_D; }
  else if (key == "DELETE") { return CL_KEY_DELETE; }
  else if (key == "DOWN") { return CL_KEY_DOWN; }
  else if (key == "E") { return CL_KEY_E; }
  else if (key == "END") { return CL_KEY_END; }
  else if (key == "ENTER") { return CL_KEY_ENTER; }
  else if (key == "ESCAPE") { return CL_KEY_ESCAPE; }
  else if (key == "F") { return CL_KEY_F; }
  else if (key == "F1") { return CL_KEY_F1; }
  else if (key == "F10") { return CL_KEY_F10; }
  else if (key == "F11") { return CL_KEY_F11; }
  else if (key == "F12") { return CL_KEY_F12; }
  else if (key == "F2") { return CL_KEY_F2; }
  else if (key == "F3") { return CL_KEY_F3; }
  else if (key == "F4") { return CL_KEY_F4; }
  else if (key == "F5") { return CL_KEY_F5; }
  else if (key == "F6") { return CL_KEY_F6; }
  else if (key == "F7") { return CL_KEY_F7; }
  else if (key == "F8") { return CL_KEY_F8; }
  else if (key == "F9") { return CL_KEY_F9; }
  else if (key == "G") { return CL_KEY_G; }
  else if (key == "H") { return CL_KEY_H; }
  else if (key == "HOME") { return CL_KEY_HOME; }
  else if (key == "I") { return CL_KEY_I; }
  else if (key == "INSERT") { return CL_KEY_INSERT; }
  else if (key == "J") { return CL_KEY_J; }
  else if (key == "K") { return CL_KEY_K; }
  else if (key == "KP_0") { return CL_KEY_KP_0; }
  else if (key == "KP_1") { return CL_KEY_KP_1; }
  else if (key == "KP_2") { return CL_KEY_KP_2; }
  else if (key == "KP_3") { return CL_KEY_KP_3; }
  else if (key == "KP_4") { return CL_KEY_KP_4; }
  else if (key == "KP_5") { return CL_KEY_KP_5; }
  else if (key == "KP_6") { return CL_KEY_KP_6; }
  else if (key == "KP_7") { return CL_KEY_KP_7; }
  else if (key == "KP_8") { return CL_KEY_KP_8; }
  else if (key == "KP_9") { return CL_KEY_KP_9; }
  else if (key == "KP_DECIMAL") { return CL_KEY_KP_DECIMAL; }
  else if (key == "KP_DIV") { return CL_KEY_KP_DIV; }
  else if (key == "KP_ENTER") { return CL_KEY_KP_ENTER; }
  else if (key == "KP_MINUS") { return CL_KEY_KP_MINUS; }
  else if (key == "KP_MULT") { return CL_KEY_KP_MULT; }
  else if (key == "KP_PLUS") { return CL_KEY_KP_PLUS; }
  else if (key == "L") { return CL_KEY_L; }
  else if (key == "LCTRL") { return CL_KEY_LCTRL; }
  else if (key == "LEFT") { return CL_KEY_LEFT; }
  else if (key == "LSHIFT") { return CL_KEY_LSHIFT; }
  else if (key == "M") { return CL_KEY_M; }
  else if (key == "N") { return CL_KEY_N; }
  else if (key == "NONE_OF_THE_ABOVE") { return CL_KEY_NONE_OF_THE_ABOVE; }
  else if (key == "NUMLOCK") { return CL_KEY_NUMLOCK; }
  else if (key == "O") { return CL_KEY_O; }
  else if (key == "P") { return CL_KEY_P; }
  else if (key == "PAGEDOWN") { return CL_KEY_PAGEDOWN; }
  else if (key == "PAGEUP") { return CL_KEY_PAGEUP; }
  else if (key == "PAUSE") { return CL_KEY_PAUSE; }
  else if (key == "PRINT") { return CL_KEY_PRINT; }
  else if (key == "Q") { return CL_KEY_Q; }
  else if (key == "R") { return CL_KEY_R; }
  else if (key == "RCTRL") { return CL_KEY_RCTRL; }
  else if (key == "RIGHT") { return CL_KEY_RIGHT; }
  else if (key == "RSHIFT") { return CL_KEY_RSHIFT; }
  else if (key == "S") { return CL_KEY_S; }
  else if (key == "SCRLOCK") { return CL_KEY_SCRLOCK; }
  else if (key == "SPACE") { return CL_KEY_SPACE; }
  else if (key == "T") { return CL_KEY_T; }
  else if (key == "TAB") { return CL_KEY_TAB; }
  else if (key == "U") { return CL_KEY_U; }
  else if (key == "UP") { return CL_KEY_UP; }
  else if (key == "V") { return CL_KEY_V; }
  else if (key == "W") { return CL_KEY_W; }
  else if (key == "X") { return CL_KEY_X; }
  else if (key == "Y") { return CL_KEY_Y; }
  else if (key == "Z") { return CL_KEY_Z; }
  else if (key == "CL_MOUSE_LEFTBUTTON") { return CL_MOUSE_LEFTBUTTON; }
  else if (key == "CL_MOUSE_MIDDLEBUTTON") { return CL_MOUSE_MIDDLEBUTTON; }
  else if (key == "CL_MOUSE_RIGHTBUTTON") { return CL_MOUSE_RIGHTBUTTON; }
  else if (key == "CL_MOUSE_THUMBBUTTON") { return CL_MOUSE_THUMBBUTTON; }
  else if (key == "CL_MOUSE_WHEELBUTTON") { return CL_MOUSE_WHEELBUTTON; }
  else if (key == "CL_MOUSE_WHEELDOWN") { return CL_MOUSE_WHEELDOWN; }
  else if (key == "CL_MOUSE_WHEELUP") { return CL_MOUSE_WHEELUP; }
  else if (key == "CL_NO_KEY") { return CL_NO_KEY; }
  else if (key == "CL_NUM_KEYS") { return CL_NUM_KEYS; }
  else
    {
      std::cout << "Key Name " << key << "not known" << std::endl;
      return CL_NO_KEY;
    }
}

/* EOF */
