//  $Id: Controllable.hh,v 1.1 2001/04/14 11:45:01 grumbel Exp $
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

#ifndef CONTROLLABLE_HH
#define CONTROLLABLE_HH

#include <ClanLib/core.h>
#include "Controller.hh"

class Controllable
{
protected:
  CL_Slot left_press_slot;
  CL_Slot middle_press_slot;
  CL_Slot right_press_slot;

  CL_Slot abort_press_slot;
  CL_Slot pause_press_slot;

  CL_Slot scroll_left_press_slot;
  CL_Slot scroll_right_press_slot;

  CL_Slot next_action_press_slot;
  CL_Slot previous_action_press_slot;

  
  CL_Slot left_release_slot;
  CL_Slot middle_release_slot;
  CL_Slot right_release_slot;

  CL_Slot abort_release_slot;
  CL_Slot pause_release_slot;

  CL_Slot scroll_left_release_slot;
  CL_Slot scroll_right_release_slot;

  CL_Slot next_action_release_slot;
  CL_Slot previous_action_release_slot;

  boost::shared_ptr<Controller> controller;
public:
  Controllable (boost::shared_ptr<Controller> arg_controller);
  virtual ~Controllable ();

  virtual void on_left_press (const CL_Vector& pos) {}
  virtual void on_middle_press (const CL_Vector& pos) {}
  virtual void on_right_press (const CL_Vector& pos) {}

  virtual void on_abort_press (const CL_Vector& pos) {}
  virtual void on_pause_press (const CL_Vector& pos) {}

  virtual void on_scroll_left_press (const CL_Vector& pos) {}
  virtual void on_scroll_right_press (const CL_Vector& pos) {}

  virtual void on_next_action_press (const CL_Vector& pos) {}
  virtual void on_previous_action_press (const CL_Vector& pos) {}


  virtual void on_left_release (const CL_Vector& pos) {}
  virtual void on_middle_release (const CL_Vector& pos) {}
  virtual void on_right_release (const CL_Vector& pos) {}

  virtual void on_abort_release (const CL_Vector& pos) {}
  virtual void on_pause_release (const CL_Vector& pos) {}

  virtual void on_scroll_left_release (const CL_Vector& pos) {}
  virtual void on_scroll_right_release (const CL_Vector& pos) {}

  virtual void on_next_action_release (const CL_Vector& pos) {}
  virtual void on_previous_action_release (const CL_Vector& pos) {}
};

#endif

/* EOF */
