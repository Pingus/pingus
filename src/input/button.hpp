//  $Id: button.hxx,v 1.11 2003/10/20 13:33:44 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_BUTTON_HXX
#define HEADER_PINGUS_INPUT_BUTTON_HXX

#include "../pingus.hxx"

namespace Input {

/// abstract base class which defines the button interface
class Button
{
public:
  Button () { }
  virtual ~Button () { }

  /// returns true if the button is pressed, false otherwise
  virtual bool is_pressed ()      const =0;
  virtual void update     (float)       =0;

  CL_Signal_v0& sig_button_down() { return button_down; }
  CL_Signal_v0& sig_button_up() { return button_up; }

protected:
  CL_Signal_v0 button_down; 
  CL_Signal_v0 button_up; 

private:
  Button (const Button&);
  Button& operator= (const Button&);
};

} // namespace Input

#endif

/* EOF */
