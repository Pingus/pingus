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

#ifndef HEADER_PINGUS_PINGUS_CAPTURE_RECTANGLE_HPP
#define HEADER_PINGUS_PINGUS_CAPTURE_RECTANGLE_HPP

#include "engine/display/font.hpp"
#include "engine/display/sprite.hpp"

class Font;
class Pingu;
class PinguAction;
class Pingu;
class GameSession;
class DrawingContext;
class SceneContext;

/** The rectangle that is shown when the mouse cursor is above a
    pingu. The rectangle shows the current pingu direction along with
    the current active action.
*/
class CaptureRectangle
{
private:
  GameSession*  session;
  Pingu*      pingu;
  std::string action_str;

  /// The id of the owner of this capture rectangle
  int owner_id;

  Sprite good;
  Sprite bad;
  Sprite arrow_left;
  Sprite arrow_right;

  Font font;

public:
  CaptureRectangle(GameSession*);
  ~CaptureRectangle();

  void set_pingu(Pingu* pingu);

  void draw(SceneContext& gc);

private:
  CaptureRectangle (const CaptureRectangle&);
  CaptureRectangle& operator= (const CaptureRectangle&);
};

#endif

/* EOF */
