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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SURFACE_BACKGROUND_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SURFACE_BACKGROUND_HPP

#include "math/vector2f.hpp"
#include "math/color.hpp"
#include "pingus/worldobj.hpp"

namespace pingus::worldobjs {

class SurfaceBackground final : public WorldObj
{
private:
  float para_x;
  float para_y;

  /** Position of the background, only the z_pos is really used. */
  Vector2f pos;
  float m_z_index;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_x;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_y;

  /** fill_rect() components
      An fill_rect() can be drawn over the background, the following
      for components are passed to the fill_rect() call. */
  Color color;

  /** Stretch the background to the full screen size in x direction */
  bool stretch_x;

  /** Stretch the background to the full screen size in x direction */
  bool stretch_y;

  /** If streched in x or y direction keep the aspect ratio */
  bool keep_aspect;

  /** Background image */
  Sprite bg_sprite;

  /** The horizontal scrolling speed in pixels per tick */
  float scroll_ox;

  /** The vertical scrolling speed in pixels per tick */
  float scroll_oy;

public:
  SurfaceBackground(ReaderMapping const& reader);

  float z_index() const override;
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(Vector2f const& p) override { }
  Vector2f get_pos() const override { return Vector2f(); }

  void update() override;
  void draw(SceneContext& gc) override;

  bool is_solid_background() const override { return true; }

private:
  SurfaceBackground (SurfaceBackground const&);
  SurfaceBackground& operator= (SurfaceBackground const&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
