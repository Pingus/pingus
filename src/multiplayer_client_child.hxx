//  $Id: multiplayer_client_child.hxx,v 1.8 2002/10/04 16:54:04 grumbel Exp $
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

#ifndef HEADER_PINGUS_MULTIPLAYER_CLIENT_CHILD_HXX
#define HEADER_PINGUS_MULTIPLAYER_CLIENT_CHILD_HXX

#include "vector.hxx"
#include "cursor.hxx"
#include "button_panel.hxx"
#include "playfield_view.hxx"
#include "pingus_counter_bar.hxx"

#include <string>
class TrueServer;

class MultiplayerClientChild : public GuiObj
{
private:
  TrueServer*  server;
  Cursor*      cursor;
  ButtonPanel* button_panel;
  Sprite       capture_rect;
  
  std::vector<GuiObj*> gui_objs;
  typedef std::vector<GuiObj*>::iterator GuiObjIter;

  PlayfieldView*    playfield;
  PingusCounterBar* counterbar;

  CL_Rect rect;

  Vector scroll_vec;

public:
  MultiplayerClientChild (TrueServer * s, const CL_Rect& arg_rect);
  ~MultiplayerClientChild ();

  void draw ();
  void update (float delta);

  void on_left_press (const Vector& pos);
  void on_right_press (const Vector& pos);
  void on_right_release (const Vector& pos);

  void on_scroll_left_press (const Vector& pos);
  void on_scroll_right_press (const Vector& pos);
  void on_scroll_up_press (const Vector& pos);
  void on_scroll_down_press (const Vector& pos);

  void on_scroll_left_release (const Vector& pos);
  void on_scroll_right_release (const Vector& pos);
  void on_scroll_up_release (const Vector& pos);
  void on_scroll_down_release (const Vector& pos);

  void on_next_action_press (const Vector& pos);
  void on_previous_action_press (const Vector& pos);
  
private:
  MultiplayerClientChild (const MultiplayerClientChild&);
  MultiplayerClientChild& operator= (const MultiplayerClientChild&);
};

#endif

/* EOF */
