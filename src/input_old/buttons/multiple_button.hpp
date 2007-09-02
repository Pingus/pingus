//  $Id$
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

#ifndef HEADER_PINGUS_INPUT_MULTIPLE_BUTTON_HXX
#define HEADER_PINGUS_INPUT_MULTIPLE_BUTTON_HXX

#include <vector>
#include "../button.hpp"

namespace Input {
namespace Buttons {

/**
   @brief wrapper class mapping multiple buttons into one

   XML definition: <multiple-button> <button 1>...<button n> </multiple-button>

   A multiple button is pressed whenever at least one of the buttons contained is pressed.
*/
class MultipleButton : public Button {

private:
  std::vector<Button*> buttons;
  int down_count;
public:
  MultipleButton(const std::vector<Button*>& buttons_);
  ~MultipleButton();

  virtual bool is_pressed() const;
  virtual void update(float delta);

private:
  static void on_button_up(void*);
  static void on_button_down(void*);

  MultipleButton(const MultipleButton&);
  MultipleButton& operator= (const MultipleButton&);
};

} // namespace Buttons
} // namespace Input

#endif

/* EOF */
