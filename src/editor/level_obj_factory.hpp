// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_LEVEL_OBJ_FACTORY_HPP
#define HEADER_PINGUS_EDITOR_LEVEL_OBJ_FACTORY_HPP

#include "editor/level_obj_ptr.hpp"

#include <prio/fwd.hpp>

namespace pingus::editor {

class LevelObjFactory
{
public:
  static LevelObjPtr create(prio::ReaderObject const& reader_object);

private:
  LevelObjFactory(LevelObjFactory const&);
  LevelObjFactory& operator=(LevelObjFactory const&);
};

} // namespace pingus::editor

#endif

/* EOF */
