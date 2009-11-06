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

#include "key_button.hpp"

#include <iostream>
#include <map>

namespace Input {
namespace Buttons {

KeyButton::KeyButton(int button_) 
  : button(button_),
    pressed(false)
{
  Controller::add_key_callback(&KeyButton::key_handler, this);
}

void
KeyButton::update(float)
{
}

bool
KeyButton::is_pressed() const
{
  return pressed;
}

void
KeyButton::key_handler(const SDL_Event& event, void* userdata)
{
  KeyButton* kb = (KeyButton*)userdata;
  if (event.key.keysym.sym == kb->button)
  {
    if (event.key.state == SDL_PRESSED)
    {
      kb->pressed = true;
      kb->button_down();
    }
    else
    {
      kb->pressed = false;
      kb->button_up();
    }
  }
}

int
KeyButton::string_to_keyid(std::string key)
{
  static bool init = true;
  static std::map<std::string, int> string_to_id;

  if (init)
  {
    init = false;

    string_to_id["backspace"] = SDLK_BACKSPACE;
    string_to_id["tab"] = SDLK_TAB;
    string_to_id["clear"] = SDLK_CLEAR;
    string_to_id["return"] = SDLK_RETURN;
    //      string_to_id["shift"] = CL_KEY_SHIFT;
    //      string_to_id["control"] = CL_KEY_CONTROL;
    string_to_id["menu"] = SDLK_MENU;
    string_to_id["pause"] = SDLK_PAUSE;
    string_to_id["escape"] = SDLK_ESCAPE;
    string_to_id["space"] = SDLK_SPACE;
    string_to_id["prior"] = SDLK_PAGEUP;
    string_to_id["next"] = SDLK_PAGEDOWN;
    string_to_id["end"] = SDLK_END;
    string_to_id["home"] = SDLK_HOME;
    string_to_id["left"] = SDLK_LEFT;
    string_to_id["up"] = SDLK_UP;
    string_to_id["right"] = SDLK_RIGHT;
    string_to_id["down"] = SDLK_DOWN;
    //      string_to_id["select"] = CL_KEY_SELECT;
    //      string_to_id["print"] = CL_KEY_PRINT;
    //      string_to_id["execute"] = CL_KEY_EXECUTE;
    string_to_id["insert"] = SDLK_INSERT;
    string_to_id["delete"] = SDLK_DELETE;
    string_to_id["help"] = SDLK_HELP;
    string_to_id["0"] = SDLK_0;
    string_to_id["1"] = SDLK_1;
    string_to_id["2"] = SDLK_2;
    string_to_id["3"] = SDLK_3;
    string_to_id["4"] = SDLK_4;
    string_to_id["5"] = SDLK_5;
    string_to_id["6"] = SDLK_6;
    string_to_id["7"] = SDLK_7;
    string_to_id["8"] = SDLK_8;
    string_to_id["9"] = SDLK_9;
    string_to_id["a"] = SDLK_a;
    string_to_id["b"] = SDLK_b;
    string_to_id["c"] = SDLK_c;
    string_to_id["d"] = SDLK_d;
    string_to_id["e"] = SDLK_e;
    string_to_id["f"] = SDLK_f;
    string_to_id["g"] = SDLK_g;
    string_to_id["h"] = SDLK_h;
    string_to_id["i"] = SDLK_i;
    string_to_id["j"] = SDLK_j;
    string_to_id["k"] = SDLK_k;
    string_to_id["l"] = SDLK_l;
    string_to_id["m"] = SDLK_m;
    string_to_id["n"] = SDLK_n;
    string_to_id["o"] = SDLK_o;
    string_to_id["p"] = SDLK_p;
    string_to_id["q"] = SDLK_q;
    string_to_id["r"] = SDLK_r;
    string_to_id["s"] = SDLK_s;
    string_to_id["t"] = SDLK_t;
    string_to_id["u"] = SDLK_u;
    string_to_id["v"] = SDLK_v;
    string_to_id["w"] = SDLK_w;
    string_to_id["x"] = SDLK_x;
    string_to_id["y"] = SDLK_y;
    string_to_id["z"] = SDLK_z;
    string_to_id["lwin"] = SDLK_LSUPER;
    string_to_id["rwin"] = SDLK_RSUPER;
    string_to_id["numpad0"] = SDLK_KP0;
    string_to_id["numpad1"] = SDLK_KP1;
    string_to_id["numpad2"] = SDLK_KP2;
    string_to_id["numpad3"] = SDLK_KP3;
    string_to_id["numpad4"] = SDLK_KP4;
    string_to_id["numpad5"] = SDLK_KP5;
    string_to_id["numpad6"] = SDLK_KP6;
    string_to_id["numpad7"] = SDLK_KP7;
    string_to_id["numpad8"] = SDLK_KP8;
    string_to_id["numpad9"] = SDLK_KP9;
    string_to_id["multiply"] = SDLK_KP_MULTIPLY;
    string_to_id["add"] = SDLK_KP_PLUS;
    //      string_to_id["separator"] = CL_KEY_SEPARATOR;
    string_to_id["subtract"] = SDLK_KP_MINUS;
    string_to_id["decimal"] = SDLK_KP_PERIOD;
    string_to_id["divide"] = SDLK_KP_DIVIDE;
    string_to_id["f1"] = SDLK_F1;
    string_to_id["f2"] = SDLK_F2;
    string_to_id["f3"] = SDLK_F3;
    string_to_id["f4"] = SDLK_F4;
    string_to_id["f5"] = SDLK_F5;
    string_to_id["f6"] = SDLK_F6;
    string_to_id["f7"] = SDLK_F7;
    string_to_id["f8"] = SDLK_F8;
    string_to_id["f9"] = SDLK_F9;
    string_to_id["f10"] = SDLK_F10;
    string_to_id["f11"] = SDLK_F11;
    string_to_id["f12"] = SDLK_F12;
    string_to_id["f13"] = SDLK_F13;
    string_to_id["f14"] = SDLK_F14;
    string_to_id["f15"] = SDLK_F15;
    string_to_id["numlock"] = SDLK_NUMLOCK;
    string_to_id["scroll"] = SDLK_SCROLLOCK;
    string_to_id["lshift"] = SDLK_LSHIFT;
    string_to_id["rshift"] = SDLK_RSHIFT;
    string_to_id["lcontrol"] = SDLK_LCTRL;
    string_to_id["rcontrol"] = SDLK_RCTRL;
    string_to_id["lmenu"] = SDLK_LALT;
    string_to_id["rmenu"] = SDLK_RALT;
  }

  return string_to_id[key];
}

} // namespace Buttons
} // namespace Input

/* EOF */
