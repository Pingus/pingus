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

#include <sstream>
#include <utility>

#include "tinygettext/dictionary_manager.hpp"
#include "tinygettext/file_system.hpp"
#include "tinygettext/unix_file_system.hpp"

#ifdef ANDROID
#  include "util/system.hpp"
#endif

namespace {

#ifdef ANDROID

/** AssetManager cannot be listed with directory_iterator / opendir.
    Provide the known .po basenames and load file bodies via System::read_file. */
class AndroidFileSystem : public tinygettext::FileSystem
{
public:
  std::vector<std::string> open_directory(const std::string& /*pathname*/) override
  {
    // Keep in sync with data/po/*.po (excluding .pot).
    return {
    "ast.po",
    "bg.po",
    "ca.po",
    "cs.po",
    "da.po",
    "de.po",
    "eo.po",
    "es.po",
    "fi.po",
    "fr.po",
    "gd.po",
    "gl.po",
    "hu.po",
    "it.po",
    "ja.po",
    "lt.po",
    "nb.po",
    "nl.po",
    "nn.po",
    "oc.po",
    "pl.po",
    "pt.po",
    "pt_BR.po",
    "ru.po",
    "sq.po",
    "sr.po",
    "sv.po",
    "th.po",
    "tr.po",
    "uk.po",
    "zh_CN.po",
    "zh_TW.po"
    };
  }

  std::unique_ptr<std::istream> open_file(const std::string& filename) override
  {
    try
    {
      std::string body = pingus::System::read_file(filename);
      return std::make_unique<std::istringstream>(std::move(body));
    }
    catch (std::exception const&)
    {
      return std::unique_ptr<std::istream>();
    }
  }
};

std::unique_ptr<tinygettext::FileSystem> make_file_system()
{
  return std::make_unique<AndroidFileSystem>();
}

#else

std::unique_ptr<tinygettext::FileSystem> make_file_system()
{
  return std::make_unique<tinygettext::UnixFileSystem>();
}

#endif

} // namespace

tinygettext::DictionaryManager dictionary_manager(make_file_system());

std::string _(std::string const& msg) {
  return dictionary_manager.get_dictionary().translate(msg);
}

std::string _(std::string const& msg, std::string const& msg_pl, int num) {
  return dictionary_manager.get_dictionary().translate_plural(msg, msg_pl, num);
}

/* EOF */
