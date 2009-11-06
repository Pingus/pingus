//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_ENGINE_INPUT_DRIVER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_DRIVER_HPP

#include "engine/input/control.hpp"
#include "util/file_reader.hpp"

namespace Input {

class Driver
{
public:
  Driver() {}
  virtual ~Driver() {}

  virtual std::string get_name() const =0;
  virtual void update(float delta) =0;

  virtual Button*   create_button  (const FileReader& reader, Control* parent) =0;
  virtual Axis*     create_axis    (const FileReader& reader, Control* parent) =0;
  virtual Scroller* create_scroller(const FileReader& reader, Control* parent) =0;
  virtual Pointer*  create_pointer (const FileReader& reader, Control* parent) =0;
  virtual Keyboard* create_keyboard(const FileReader& reader, Control* parent) =0;
};

} // namespace Input

#endif

/* EOF */
