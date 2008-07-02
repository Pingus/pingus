//  $Id: double_button.hpp 3059 2007-09-02 03:48:52Z grumbel $
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

#ifndef HEADER_PINGUS_INPUT_DOUBLE_BUTTON_HXX
#define HEADER_PINGUS_INPUT_DOUBLE_BUTTON_HXX

#include "../button.hpp"

namespace Input {
namespace Buttons {

/**
   @brief allows two buttons to be used in combination

   XML definition: <double-button><button1><button2></double-button>

   This class allows combinations like CTRL-X whereby it's important that the first
   key is pressed before the second else the DoubleButton itself won't change it's
   own state to pressed.
*/
class DoubleButton : public Button {

private:
  Button* const button1;
  Button* const button2;
  bool          first_pressed;
  bool          ignore_second;

public:

  DoubleButton (Button* button1_, Button* button2_);
  ~DoubleButton ();

  virtual bool is_pressed () const;
  virtual void update (float delta);

private:
  DoubleButton (const DoubleButton&);
  DoubleButton& operator= (const DoubleButton&);
};

} // namespace Buttons
} // namespace Input

#endif

/* EOF */
