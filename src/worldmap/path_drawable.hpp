//  $Id: path_drawable.hpp 3582 2008-06-29 16:56:56Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PATH_DRAWABLE_HPP
#define HEADER_PATH_DRAWABLE_HPP

#include "drawable.hpp"
#include "path_graph.hpp"

namespace WorldMapNS {

/** Drawable to show the path between two nodes, only used for
    debugging */
class PathDrawable : public Drawable
{
private:
  Path path;
public:
  PathDrawable(const Path& arg_path);

  void draw(DrawingContext& gc);
  void update(float delta);

private:
  PathDrawable (const PathDrawable&);
  PathDrawable& operator= (const PathDrawable&);
};

} // namespace WorldMapNS

#endif

/* EOF */
