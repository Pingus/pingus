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

#include "tinygettext/dictionary_manager.hpp"

tinygettext::DictionaryManager dictionary_manager;

std::string _(const std::string& msg) {
  return dictionary_manager.get_dictionary().translate(msg);
}

std::string _(const std::string& msg, const std::string& msg_pl, int num) {
  return dictionary_manager.get_dictionary().translate_plural(msg, msg_pl, num);
}

/* EOF */
