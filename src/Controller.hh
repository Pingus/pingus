//  $Id: Controller.hh,v 1.2 2001/04/12 20:52:40 grumbel Exp $
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
#include <ClanLib/core.h>

class Controller : public CL_KeepAlive
{
protected:
  int owner_id;
  
public:
  Controller (int arg_owner_id) : owner_id (arg_owner_id) {}
  virtual ~Controller () {}

  virtual int get_x () =0;
  virtual int get_y () =0;
  
  virtual CL_Vector get_pos () =0;

  /** Limit the range to the given area, might be used in multiplayer
      modes to limit the cursor to the players view */
  virtual void set_range (int x1, int y1, int x2, int y2) =0;
  
  /// @return true when the left mouse button is pressed
  virtual bool left_pressed () =0;
  /// @return true when the middle mouse button is pressed
  virtual bool middle_pressed () =0;
  /// @return true when the right mouse button is pressed
  virtual bool right_pressed () =0;
  
  virtual bool next_action () =0;
  virtual bool previous_action () =0;
  
  /** @return true if the abort button was pressed (usually the escape
      key) */
  virtual bool abort_pressed () =0;

  /** @return true if pause is pressed */
  virtual bool pause_pressed () =0;

  virtual bool scroll_left () =0;
  virtual bool scroll_right () =0;

  /// Called once each CL_System::keep_alive () call
  virtual void keep_alive () {}

  CL_Signal_v1<CL_Vector> signal_left_pressed;
  CL_Signal_v1<CL_Vector> signal_middle_pressed;
  CL_Signal_v1<CL_Vector> signal_right_pressed;
  CL_Signal_v1<CL_Vector> signal_abort_pressed;
  CL_Signal_v1<CL_Vector> signal_pause_pressed;
  CL_Signal_v1<CL_Vector> signal_scroll_left;
  CL_Signal_v1<CL_Vector> signal_scroll_right;
};

#endif

/* EOF */
