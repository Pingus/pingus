//  $Id: QuestionDialog.cc,v 1.3 2000/02/11 16:58:26 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "PingusResource.hh"

#include "QuestionDialog.hh"

QuestionDialog::QuestionDialog(std::string q, 
			       std::string b1, 
			       std::string b2,
			       std::string b3,
			       std::string b4)
{
  buttons = 3;

  question = q;
  button1 = b1;
  button2 = b2;
  button3 = b3;
  button4 = b4;
  
  font = CL_Font::load("Fonts/font", PingusResource::get("fonts.dat"));
}

int
QuestionDialog::start()
{
  int mx, my;
  draw();
  CL_Display::sync_buffers();

  while(true) {
    CL_System::keep_alive();
   
    if (CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) {
      return return_value(1);
    } else if (CL_Keyboard::get_keycode(CL_KEY_Y)) {
      return return_value(0);
    } else if (CL_Keyboard::get_keycode(CL_KEY_N)) {
      return return_value(1);      
    } else if (CL_Keyboard::get_keycode(CL_KEY_R)) {
      return return_value(2);
    } else if (CL_Keyboard::get_keycode(CL_KEY_L)) {
      return return_value(3);
    }
    
    if (CL_Mouse::left_pressed()) {
      mx = CL_Mouse::get_x();
      my = CL_Mouse::get_y();
  
      if (mx > 165 && mx < 235 && my > 225 && my < 255) {
	return return_value(0);
      }
    
      if (mx > 245 && mx < 315 && my > 225 && my < 255) {
	return return_value(1); 
      }
  
      if (mx > 325 && mx < 395 && my > 225 && my < 255) {
	return return_value(2);
      }
    
      if (mx > 405 && mx < 475 && my > 225 && my < 255) {
	return return_value(3);
      }
    }
  }
}

void
QuestionDialog::draw()
{
  CL_Display::fill_rect(150, 180, 490, 280, 0.0, 0.0, 0.0, 0.5);
  
  font->print_center(320, 200, question.c_str());

  CL_Display::fill_rect(165, 225, 235, 255, 0.0, 0.0, 0.0, 0.5);
  font->print_center(200, 230, button1.c_str());

  CL_Display::fill_rect(245, 225, 315, 255, 0.0, 0.0, 0.0, 0.5);
  font->print_center(280, 230, button2.c_str());

  CL_Display::fill_rect(325, 225, 395, 255, 0.0, 0.0, 0.0, 0.5);
  font->print_center(360, 230, button3.c_str());
  
  CL_Display::fill_rect(405, 225, 475, 255, 0.0, 0.0, 0.0, 0.5);
  font->print_center(440, 230, button4.c_str());

  CL_Display::flip_display();
}

void
QuestionDialog::wait_for_mouse()
{
    while(CL_Mouse::left_pressed()) { 
      CL_System::keep_alive();
    }
}

/* Wait until all keys are released and than exit with the given value */
int
QuestionDialog::return_value(int i)
{
  wait_for_mouse();

  while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE)
	 || CL_Keyboard::get_keycode(CL_KEY_Y)
	 || CL_Keyboard::get_keycode(CL_KEY_N)
	 || CL_Keyboard::get_keycode(CL_KEY_R)
	 || CL_Keyboard::get_keycode(CL_KEY_L))
    {
      CL_System::keep_alive();
    }

  return i;
}

/* EOF */
