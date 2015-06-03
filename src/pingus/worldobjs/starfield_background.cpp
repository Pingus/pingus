// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/starfield_background.hpp"

#include "pingus/worldobjs/starfield_background_stars.hpp"

namespace WorldObjs {

StarfieldBackground::StarfieldBackground(const ReaderMapping& reader) :
  small_stars_count(100),
  middle_stars_count(50),
  large_stars_count(25),
  stars()
{
  reader.read_int("small-stars",  small_stars_count);
  reader.read_int("middle-stars", middle_stars_count);
  reader.read_int("large-stars",  large_stars_count);

  for (int i=0; i < small_stars_count; ++i)
    stars.push_back(new StarfieldBackgroundStars(StarfieldBackgroundStars::SMALL_STAR));

  for (int i=0; i < middle_stars_count; ++i)
    stars.push_back(new StarfieldBackgroundStars(StarfieldBackgroundStars::MIDDLE_STAR));

  for (int i=0; i < large_stars_count; ++i)
    stars.push_back(new StarfieldBackgroundStars(StarfieldBackgroundStars::LARGE_STAR));
}

StarfieldBackground::~StarfieldBackground ()
{
  for (unsigned int i = 0; i < stars.size(); ++i)
    delete stars[i];
}

void
StarfieldBackground::update ()
{
  for (std::vector<StarfieldBackgroundStars*>::iterator i = stars.begin();
       i != stars.end(); ++i)
  {
    (*i)->update ();
  }
}

void
StarfieldBackground::draw (SceneContext& gc)
{
  for (std::vector<StarfieldBackgroundStars*>::iterator i = stars.begin();
       i != stars.end(); ++i)
  {
    (*i)->draw (gc);
  }
}

} // namespace WorldObjs

/* EOF */
