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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_ACTION_BUTTON_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_ACTION_BUTTON_HPP

#include "engine/gui/rect_component.hpp"
#include "pingus/state_sprite.hpp"

namespace pingus {

class Server;
class GameSession;
class ActionHolder;
class Vector;

/** ArmageddonButton, press it to start the armageddon and to turn all
    pingus into bomber

    \sa Client */
class ArmageddonButton : public pingus::gui::RectComponent
{
private:
  Server* server;
  bool   pressed;
  float  press_time;
  Sprite sprite;
  Sprite background;
  Sprite backgroundhl;

public:
  ArmageddonButton(Server*, int x, int y);
  ~ArmageddonButton() override;

  void draw(DrawingContext& gc) override;
  void update(float) override;
  void on_primary_button_click (int x, int y) override;

private:
  ArmageddonButton (ArmageddonButton const&);
  ArmageddonButton& operator= (ArmageddonButton const&);
};

/** Fast Forward button, press it to let the game run faster, press it
    again to return to normal speed

    \sa Client */
class ForwardButton : public pingus::gui::RectComponent
{
private:
  GameSession* session;
  Sprite surface;
  Sprite background;
  Sprite backgroundhl;

public:
  ForwardButton(GameSession*, int x, int y);
  ~ForwardButton() override;

  void draw(DrawingContext& gc) override;
  void on_primary_button_press (int x, int y) override;
  void on_primary_button_release (int x, int y) override;

private:
  ForwardButton (ForwardButton const&);
  ForwardButton& operator= (ForwardButton const&);
};

/** Pause button, press it to pause the game, press it again to
    continue

    \sa Client */
class PauseButton : public pingus::gui::RectComponent
{
private:
  GameSession* session;

  Sprite surface;
  Sprite background;
  Sprite backgroundhl;

public:
  PauseButton(GameSession*, int x, int y);
  ~PauseButton() override;

  void draw(DrawingContext& gc) override;

  void on_primary_button_click (int x, int y) override;

private:
  PauseButton (PauseButton const&);
  PauseButton& operator= (PauseButton const&);
};

#endif /* ACTIONBUTTON */

} // namespace pingus

/* EOF */
