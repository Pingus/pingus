//  $Id: Controller.hh,v 1.7 2001/04/15 11:00:41 grumbel Exp $
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

#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <ClanLib/signals.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include "boost/smart_ptr.hpp"

class Controller;

class ControllerButton 
{
private:
  Controller* controller;
  bool pressed;

public:
  ControllerButton (Controller* );
  virtual ~ControllerButton () {}
  
  virtual bool is_pressed () =0;
  virtual void keep_alive ();

  CL_Signal_v1<const CL_Vector&> signal_pressed;
  CL_Signal_v1<const CL_Vector&> signal_released;
};

class DummyButton : public ControllerButton 
{
private:
public:
  DummyButton (Controller* );
  virtual ~DummyButton () {}
  
  virtual bool is_pressed () { return false; }
};

class InputDeviceButton : public ControllerButton 
{
private:
  CL_InputButton* button;
public:
  InputDeviceButton (Controller* arg_controller, CL_InputButton* arg_button);
  
  virtual bool is_pressed ();
};

/** An abstract controller class representing an controlling device
    for Pingus. Implementations for mouse and the sidewinder gamepad
    are provided by MouseController and GamepadController. */
class Controller : public CL_KeepAlive
{
protected:
  int owner_id;
  CL_Rect rect;
  
public:
  Controller (int arg_owner_id);
  virtual ~Controller () {}

  virtual int get_x () =0;
  virtual int get_y () =0;
  
  virtual CL_Vector get_pos () =0;

  /** Limit the range to the given area, might be used in multiplayer
      modes to limit the cursor to the players view */
  virtual void set_range (const CL_Rect& arg_rect);
  
  /// @return the owner of this controller
  virtual int get_owner (); 

  /// Called once each CL_System::keep_alive () call
  virtual void keep_alive ();

  boost::shared_ptr<ControllerButton> left;
  boost::shared_ptr<ControllerButton> middle;
  boost::shared_ptr<ControllerButton> right;
  boost::shared_ptr<ControllerButton> abort;
  boost::shared_ptr<ControllerButton> pause;
  boost::shared_ptr<ControllerButton> scroll_left;
  boost::shared_ptr<ControllerButton> scroll_right;
  boost::shared_ptr<ControllerButton> scroll_up;
  boost::shared_ptr<ControllerButton> scroll_down;

  boost::shared_ptr<ControllerButton> next_action;
  boost::shared_ptr<ControllerButton> previous_action;
};

#endif

/* EOF */
