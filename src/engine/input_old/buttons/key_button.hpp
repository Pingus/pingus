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

#ifndef HEADER_PINGUS_INPUT_KEY_BUTTON_HXX
#define HEADER_PINGUS_INPUT_KEY_BUTTON_HXX

#include <string>
#include "../button.hpp"

namespace Input {
namespace Buttons {

/**
   @brief maps a keyboard key to a button

   XML definition: <key-button button="key name"/>
*/
class KeyButton : public Button {

private:
  int     button;
  bool    pressed;

public:

  KeyButton(int button_);

  inline int get_button() const { return button; }

  virtual bool is_pressed() const;
  virtual void update(float);

  static int string_to_keyid(std::string key);

private:
  static void key_handler(const SDL_Event& event, void *userdata);
      
  KeyButton(const KeyButton&);
  KeyButton& operator= (const KeyButton&);
};

} // namespace Buttons
} // namespace Input

#endif

/* EOF */
