//  $Id: story.hxx,v 1.3 2002/07/29 22:17:53 grumbel Exp $
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

#ifndef HEADER_PINGUS_STORY_HXX
#define HEADER_PINGUS_STORY_HXX

#include <ClanLib/Display/Display/surface.h>
#include "pingus_sub_menu.hxx"

class CL_Font;
class PingusMenuManager;

/** This class displays the story of the Pingus. */
class Story : public PingusSubMenu
{
private:
  bool   is_init;
  CL_Surface background;
  CL_Surface story;
  CL_Font*  large_font;
  CL_Font*  small_font;

public:
  Story(PingusMenuManager* manager);
  ~Story();

  /// Load all surfaces into memory.
  void init();

  void preload () { init (); }

  void on_button_press (CL_InputDevice*,const CL_Key &); 

  void draw ();
  void update (float delta);

  /// Display a string at the right position
  void display_string(std::string);
};

#endif

/* EOF */
