//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_ACTION_BUTTON_HPP
#define HEADER_PINGUS_ACTION_BUTTON_HPP

#include <string>
#include "../font.hpp"
#include "../pingu_enums.hpp"
#include "../state_sprite.hpp"
#include "../gui/component.hpp"

class Server;
class GameSession;
class ActionHolder;
class Vector;

/** ArmageddonButton, press it to start the armageddon and to turn all
    pingus into bomber

    \sa Client */
class ArmageddonButton : public GUI::Component
{
private:
  Server* server;
  int    x_pos;
  int    y_pos;
  bool   pressed;
  float  press_time;
  Sprite sprite;
  Sprite background;
  Sprite backgroundhl;

public:
  ArmageddonButton(Server*, int x, int y);
  virtual ~ArmageddonButton();

  void draw(DrawingContext& gc);
  void update(float);
  bool is_at(int x, int y);
  void on_primary_button_click (int x, int y);

private:
  ArmageddonButton (const ArmageddonButton&);
  ArmageddonButton& operator= (const ArmageddonButton&);
};

/** Fast Forward button, press it to let the game run faster, press it
    again to return to normal speed

    \sa Client */
class ForwardButton : public GUI::Component
{
private:
  GameSession* session;
  int x_pos;
  int y_pos;
  Sprite surface;
  Sprite background;
  Sprite backgroundhl;

public:
  ForwardButton(GameSession*, int x, int y);
  virtual ~ForwardButton();

  void draw(DrawingContext& gc);
  bool is_at (int x, int y);
  void on_primary_button_click (int x, int y);

private:
  ForwardButton (const ForwardButton&);
  ForwardButton& operator= (const ForwardButton&);
};

/** Pause button, press it to pause the game, press it again to
    continue

    \sa Client */
class PauseButton : public GUI::Component
{
private:
  GameSession* session;
  int x_pos;
  int y_pos;
  Sprite surface;
  Sprite background;
  Sprite backgroundhl;

public:
  PauseButton(GameSession*, int x, int y);
  virtual ~PauseButton();

  void draw(DrawingContext& gc);
  bool is_at (int x, int y);
  void on_primary_button_click (int x, int y);

private:
  PauseButton (const PauseButton&);
  PauseButton& operator= (const PauseButton&);
};

/** The button class manage a simple button for the button_panel. It
    keeps his position, his surfaces and his font. */
class ActionButton : public GUI::Component
{
protected:
  ActionHolder* action_holder;
  Sprite sprite;
  Sprite background;
  Sprite backgroundhl;

  int x_pos;
  int y_pos;
  Font    font;
  // Added for printing action names next to the button.
  Font    font_b;

  Actions::ActionName name;
  bool is_multi_direct;

public:
  bool pressed;

  ActionButton(ActionHolder* h, int x, int y, Actions::ActionName name_, int owner_id);
  virtual ~ActionButton();

  void init(int x, int y, Actions::ActionName name_, int owner_id);

  void draw(DrawingContext& gc);
  void update(float delta);

  /// Returns the name of the action the button represents.
  Actions::ActionName get_action_name();

  bool is_at(int x, int y);

private:
  ActionButton (const ActionButton&);
  ActionButton& operator= (const ActionButton&);
};

#endif /* ACTIONBUTTON */

/* EOF */
