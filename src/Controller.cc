//  $Id: Controller.cc,v 1.6 2001/04/15 12:28:14 grumbel Exp $
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

#include "Controller.hh"

ControllerButton::ControllerButton (boost::dummy_ptr<Controller> arg_controller)
  : controller (arg_controller),
    pressed (false)
{
}

void 
ControllerButton::keep_alive ()
{
  if (is_pressed ())
    {
      if (!pressed)
	signal_pressed (controller->get_pos ());
      pressed = true;
    }
  else
    {
      if (pressed)
	signal_released (controller->get_pos ());
      pressed = false;
    }
}

DummyButton::DummyButton (Controller* c)
  : ControllerButton (c)
{
}

InputDeviceButton::InputDeviceButton (Controller* arg_controller, CL_InputButton* arg_button)
  : ControllerButton (arg_controller),
    button (arg_button)
{
  assert (button);
}
  
bool 
InputDeviceButton::is_pressed ()
{
  return button->is_pressed ();
}

KeyboardButton::KeyboardButton (boost::dummy_ptr<Controller> c, int arg_button_id)
  : ControllerButton (c),
    button_id (arg_button_id)
    
{
}

bool 
KeyboardButton::is_pressed ()
{
  return CL_Keyboard::get_keycode (button_id);
}


MultiplexButton::MultiplexButton (boost::dummy_ptr<Controller> c)
  : ControllerButton (c)
{
}
  
void 
MultiplexButton::add (boost::shared_ptr<ControllerButton> button)
{
  buttons.push_back (button);
}

void 
MultiplexButton::remove (boost::shared_ptr<ControllerButton> button)
{
  buttons.remove (button);
}
  
bool 
MultiplexButton::is_pressed ()
{
  for (ButtonIter i = buttons.begin (); i != buttons.end (); ++i)
    {
      if ((*i)->is_pressed ())
	return true;
    }
  return false;
}

Controller::Controller (int arg_owner_id)
  : owner_id (arg_owner_id),
    rect (0, 0, CL_Display::get_width ()-1, CL_Display::get_height ()-1),
    // We create some dummy buttons, so we don't get SEGFAULTS when
    // somebody tries to access a NULL pointer
    left  (new DummyButton (this)),
    middle(new DummyButton (this)),
    right (new DummyButton (this)),
    abort (new DummyButton (this)),
    pause (new DummyButton (this)),
    scroll_left (new DummyButton (this)),
    scroll_right(new DummyButton (this)),
    scroll_up (new DummyButton (this)),
    scroll_down(new DummyButton (this)),
    next_action  (new DummyButton (this)),
    previous_action (new DummyButton (this))
{
}

void 
Controller::set_range (const CL_Rect& arg_rect)
{
  rect = arg_rect;
}

void 
Controller::keep_alive ()
{
  if (left.get ()) left->keep_alive ();
  if (middle.get ()) middle->keep_alive ();
  if (right.get ()) right->keep_alive ();
  if (abort.get ()) abort->keep_alive ();
  if (pause.get ()) pause->keep_alive ();
  if (scroll_left.get ()) scroll_left->keep_alive ();
  if (scroll_right.get ()) scroll_right->keep_alive ();
  if (scroll_up.get ()) scroll_up->keep_alive ();
  if (scroll_down.get ()) scroll_down->keep_alive ();
  if (next_action.get ()) next_action->keep_alive ();
  if (previous_action.get ()) previous_action->keep_alive ();
}

int 
Controller::get_owner ()
{
  return owner_id;
}

/* EOF */
