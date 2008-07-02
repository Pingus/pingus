
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_WORLDMAP_WORLD_DOT_HPP
#define HEADER_PINGUS_WORLDMAP_WORLD_DOT_HPP

#include "dot.hpp"

namespace WorldMapNS {

/** Represents a WorldMap dot on a MetaMap */
class WorldDot : public Dot
{
private:
	bool is_accessible;
	bool is_finished;

public:
  WorldDot(FileReader reader);

  /** Draw stuff that should be displayed if the mouse is over the dot */
	virtual void draw_hover(DrawingContext& gc) { UNUSED_ARG(gc); }
	virtual void draw(DrawingContext& gc) { UNUSED_ARG(gc); }
	virtual void update(float delta) { UNUSED_ARG(delta); }

  Vector3f get_pos() { return pos; }

	virtual void on_click() { }

	virtual bool finished() { return is_finished; }
	virtual bool accessible() { return is_accessible; }

  /** makes the node accessible */
	virtual void unlock();

private:
  WorldDot (const WorldDot&);
  WorldDot& operator= (const WorldDot&);
}; // WorldDot class

}	// WorldMapNS namespace

#endif

/* EOF */
