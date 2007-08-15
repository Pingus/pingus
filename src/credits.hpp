//  $Id: credits.hxx,v 1.16 2003/10/20 19:28:54 grumbel Exp $
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

#ifndef HEADER_PINGUS_CREDITS_HXX
#define HEADER_PINGUS_CREDITS_HXX

#include <string>
#include <vector>
#include "gui/gui_screen.hpp"
#include "font.hpp"
#include "display/scene_context.hpp"
#include "sprite.hpp"

class Credits : public GUIScreen
{
private:
  static Credits* instance_;

  SceneContext* scene_context;

  bool fast_scrolling;
  Sprite background;
  Sprite pingu;

  Font    font;
  Font    font_small;

  bool is_init;

  float end_offset;
  float offset;

  /** The string's which are shown in the scrolling text, the first
      character of the string can be used to apply a special format.

      'n' - Inserts a newline
      '_' - Use small font
      '-' - Use large font
  */
  std::vector<std::string> credits;

public:
  Credits ();
  virtual ~Credits ();

  static void init();
  static void deinit();

  void update (float);
  void draw_background (DrawingContext& gc);

  void on_startup ();

  static Credits* instance ();

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_escape_press ();


private:
  Credits (const Credits&);
  Credits& operator= (const Credits&);

};


#endif

/* EOF */
