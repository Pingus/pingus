//  $Id: ScrollMap.hh,v 1.3 2001/05/18 19:17:08 grumbel Exp $
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

#ifndef SCROLLMAP_HH
#define SCROLLMAP_HH

#include "../boost/dummy_ptr.hpp"
#include "EditorView.hh"
#include "EditorEvent.hh"

class EditorEvent;

class ScrollMap
{
private:
  int x_pos, y_pos;
  int width, height;
public:
  static EditorEvent* editor_event;

  ScrollMap();
  virtual ~ScrollMap();

  void draw(boost::dummy_ptr<EditorView> view);

  void  on_button_press(CL_InputDevice *device, const CL_Key& key);
  void  on_button_release(CL_InputDevice *device, const CL_Key& key);
  bool  mouse_over(int x, int y);
};

#endif

/* EOF */
