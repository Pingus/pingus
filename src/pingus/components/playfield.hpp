// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_PLAYFIELD_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_PLAYFIELD_HPP

#include <memory>

#include "engine/display/graphic_context_state.hpp"
#include "engine/gui/rect_component.hpp"
#include "pingus/capture_rectangle.hpp"

class Pingu;
class World;
class Server;
class GameSession;
class ButtonPanel;
class Controller;
class View;

/** This class encapsulates all the different Views */
class Playfield : public GUI::RectComponent
{
private:
  Server* server;
  GameSession* session;

  Pingu* current_pingu;
  bool mouse_scrolling;
  int  scroll_speed;

  Vector2i scroll_center;

  std::unique_ptr<SceneContext> scene_context;
  GraphicContextState state;
  CaptureRectangle capture_rectangle;

  std::vector<Rect> clipping_rectangles;

  Vector2i mouse_pos;
  Vector2i old_state_pos;

public:
  Playfield(Server*, GameSession*, const Rect& rect);
  virtual ~Playfield();

  /** Returns the point onto which the Playfield is currently focused
      (ie. center of the Playfield) in WorldCO */
  Vector2i get_pos() const;

  void scroll (int x, int y);

  void set_viewpoint(int, int);

  void draw(DrawingContext& gc);
  void update(float delta);
  Pingu* current_pingu_find(const Vector2f& pos);

  void on_primary_button_press (int x, int y);
  void on_secondary_button_press (int x, int y);
  void on_secondary_button_release (int x, int y);
  void on_pointer_move (int x, int y);
  void on_key_pressed(const Input::KeyboardEvent& ev);

  void enable_scroll_mode();
  void do_scrolling();
  void disable_scroll_mode();

  void update_layout();

private:
  Playfield (const Playfield&);
  Playfield& operator= (const Playfield&);
};

#endif

/* EOF */
