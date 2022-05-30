// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/gettext.h"

#include "tinygettext/unix_file_system.hpp"
#include "tinygettext/dictionary_manager.hpp"

tinygettext::DictionaryManager dictionary_manager(std::make_unique<tinygettext::UnixFileSystem>());

std::string _(std::string const& msg) {
  return dictionary_manager.get_dictionary().translate(msg);
}

std::string _(std::string const& msg, std::string const& msg_pl, int num) {
  return dictionary_manager.get_dictionary().translate_plural(msg, msg_pl, num);
}

/* EOF */
