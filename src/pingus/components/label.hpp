// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_LABEL_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_LABEL_HPP

#include "engine/gui/rect_component.hpp"

class Label : public GUI::RectComponent
{
private:
  std::string label;

public:
  Label(const std::string& label, const Rect& rect);

  void draw(DrawingContext& gc);
  void update_layout() {}

private:
  Label (const Label&);
  Label& operator= (const Label&);
};

#endif

/* EOF */
