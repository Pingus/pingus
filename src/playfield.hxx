//  $Id: playfield.hxx,v 1.17 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_PLAYFIELD_HXX
#define HEADER_PINGUS_PLAYFIELD_HXX

#include <ClanLib/Core/Math/rect.h>
#include "client.hxx"
#include "graphic_context_state.hxx"
#include "gui/component.hxx"
#include "capture_rectangle.hxx"

namespace Pingus {

class Pingu;
class World;
class Server;
class ButtonPanel;
class Controller;
class View;

/** This class encapsulates all the different Views */
class Playfield : public GUI::Component
{
private:
  friend class Client;

  CL_Rect rect;

  CL_Surface buffer;
  Server* server;
  Client* client;

  ButtonPanel* buttons;
  World*       world;
  View*        view;

  Pingu* current_pingu;
  bool mouse_scrolling;
  bool needs_clear_screen;
  int scroll_speed;

  CL_Point scroll_center;

  SceneContext* scene_context;
  GraphicContextState state;
  CaptureRectangle cap;

  std::vector<CL_Rect> clipping_rectangles;
  
  CL_Point  mouse_pos;
  CL_Pointf old_state_pos;
public:
  Playfield (Client*, const CL_Rect& rect);
  virtual ~Playfield();

  /** Returns the point onto which the Playfield is currently focused
      (ie. center of the Playfield) in WorldCO */
  CL_Point get_pos() const;

  void scroll (int x, int y);

  void set_viewpoint(int, int);

  void draw(DrawingContext& gc);
  void update(float delta);
  Pingu* current_pingu_find(const CL_Pointf& pos);

  void on_primary_button_press (int x, int y);
  void on_secondary_button_press (int x, int y);
  void on_secondary_button_release (int x, int y);
  void on_pointer_move (int x, int y);

  void enable_scroll_mode();
  void do_scrolling();
  void disable_scroll_mode();

  void generate_clipping_rects(int, int, int, int);

  /// Members used to communicate between different screen objs
  void set_server(Server*);

  bool is_at (int x, int y) { UNUSED_ARG(x); UNUSED_ARG(y); return true; }

private:
  Playfield (const Playfield&);
  Playfield& operator= (const Playfield&);
};

} // namespace Pingus

#endif

/* EOF */
