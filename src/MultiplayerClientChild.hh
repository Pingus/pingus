//  $Id: MultiplayerClientChild.hh,v 1.7 2002/06/07 14:50:34 torangan Exp $
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

#ifndef TWOPLAYERCLIENT_HH
#define TWOPLAYERCLIENT_HH

#include "Controller.hh"
#include "boost/smart_ptr.hpp"
#include "Server.hh"
#include "GuiObj.hh"
#include "Cursor.hh"
#include "ButtonPanel.hh"
#include "Controllable.hh"
#include "PlayfieldView.hh"
#include "Sprite.hh"
#include "PingusCounterBar.hh"
#include "PLF.hh"

class MultiplayerClientChild : public Controllable,
			       public GuiObj
{
private:
  Server * server;
  boost::shared_ptr<Cursor> cursor;
  boost::shared_ptr<ButtonPanel> button_panel;
  Sprite capture_rect;
  
  std::vector<GuiObj*> gui_objs;
  typedef std::vector<GuiObj*>::iterator GuiObjIter;

  boost::shared_ptr<PlayfieldView> playfield;
  boost::shared_ptr<PingusCounterBar> counterbar;

  CL_Rect rect;

  CL_Vector scroll_vec;

public:
  MultiplayerClientChild (boost::shared_ptr<Controller> arg_controller, 
			  Server * s, const CL_Rect& arg_rect);
  ~MultiplayerClientChild ();

  void draw ();
  void update (float delta);

  void on_left_press (const CL_Vector& pos);
  void on_right_press (const CL_Vector& pos);
  void on_right_release (const CL_Vector& pos);

  void on_scroll_left_press (const CL_Vector& pos);
  void on_scroll_right_press (const CL_Vector& pos);
  void on_scroll_up_press (const CL_Vector& pos);
  void on_scroll_down_press (const CL_Vector& pos);

  void on_scroll_left_release (const CL_Vector& pos);
  void on_scroll_right_release (const CL_Vector& pos);
  void on_scroll_up_release (const CL_Vector& pos);
  void on_scroll_down_release (const CL_Vector& pos);

  void on_next_action_press (const CL_Vector& pos);
  void on_previous_action_press (const CL_Vector& pos);
};

#endif

/* EOF */
