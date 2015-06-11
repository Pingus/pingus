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

#include "util/pathname.hpp"

#include <assert.h>
#include <algorithm>
#include <ostream>
#include <set>

#include "pingus/path_manager.hpp"
#include "util/log.hpp"
#include "util/string_util.hpp"
#include "util/system.hpp"

Pathname Pathname::tmpfile(const std::string& prefix)
{
  assert(false && "unimplemented");
  return Pathname();
}

std::string
Pathname::join(const std::string& lhs, const std::string& rhs)
{
  if (lhs.empty())
  {
    return rhs;
  }
  else
  {
    if (*lhs.rbegin() == '/')
    {
      return lhs + rhs;
    }
    else
    {
      return lhs + "/" + rhs;
    }
  }
}

Pathname::Pathname() :
  pathname(),
  type(INVALID)
{
}

Pathname::Pathname(const std::string& pathname_, Type type_)
  : pathname(pathname_), type(type_)
{
}

std::string
Pathname::get_sys_path() const
{
  switch(type)
  {
    case SYSTEM_PATH:
      return pathname;

    case DATA_PATH:
      return g_path_manager.complete(pathname);

    case INVALID:
      return "";

    default:
      assert(false && "Never reached");
  }
  return "";
}

std::string
Pathname::get_raw_path() const
{
  return pathname;
}

Pathname::Type
Pathname::get_type() const
{
  return type;
}

bool
Pathname::empty() const
{
  return (type == INVALID);
}

bool
Pathname::exist() const
{
  return System::exist(get_sys_path());
}

uint64_t
Pathname::mtime() const
{
  return System::get_mtime(get_sys_path());
}

std::string
Pathname::str() const
{
  switch(type)
  {
    case Pathname::INVALID:
      return "invalid://" + pathname;

    case Pathname::DATA_PATH:
      return "datadir://" + pathname;

    case Pathname::SYSTEM_PATH:
      return "system://" + pathname;

    default:
      assert(false && "never reached");
  }
  return std::string();
}

bool
Pathname::absolute() const
{
  return !pathname.empty() && pathname[0] == '/';
}

std::vector<Pathname>
Pathname::opendir(const std::string& pattern) const
{
  switch(get_type())
  {
    case Pathname::INVALID:
      return std::vector<Pathname>();

    case Pathname::DATA_PATH: {
      // collect all paths
      std::vector<std::string> paths;
      {
        paths.push_back(g_path_manager.get_path());
        auto lst = g_path_manager.get_paths();
        paths.insert(paths.end(), lst.begin(), lst.end());
      }

      std::set<Pathname> result;
      for(auto p = paths.begin(); p != paths.end(); ++p)
      {
        std::string path = Pathname::join(*p, pathname);
        try
        {
          System::Directory lst = System::opendir(path, pattern);
          for(auto it = lst.begin(); it != lst.end(); ++it)
          {
            result.insert(Pathname(Pathname::join(pathname, it->name), Pathname::DATA_PATH));
          }
        }
        catch(const std::exception& err)
        {
          log_info("%1%", err.what());
        }
      }
      return std::vector<Pathname>(result.begin(), result.end());
    }

    case Pathname::SYSTEM_PATH: {
      std::vector<Pathname> result;
      try
      {
        auto lst = System::opendir(pathname, pattern);
        for(auto it = lst.begin(); it != lst.end(); ++it)
        {
          result.push_back(Pathname(it->name, Pathname::SYSTEM_PATH));
        }
      }
      catch(const std::exception& err)
      {
        log_info("%1%", err.what());
      }
      return result;
    }
  }

  return std::vector<Pathname>();
}

void
Pathname::opendir_recursive(std::vector<Pathname>& result) const
{
  switch(get_type())
  {
    case Pathname::INVALID:
      break;

    case Pathname::DATA_PATH: {
      // collect all paths
      std::vector<std::string> paths;
      {
        paths.push_back(g_path_manager.get_path());
        auto lst = g_path_manager.get_paths();
        paths.insert(paths.end(), lst.begin(), lst.end());

        // normalize all paths
        std::transform(paths.begin(), paths.end(),
                       paths.begin(), System::normalize_path);
      }

      for(auto p = paths.begin(); p != paths.end(); ++p)
      {
        std::string path = Pathname::join(*p, pathname);

        auto lst = System::opendir_recursive(path);
        for(auto it = lst.begin(); it != lst.end(); ++it)
        {
          Pathname sub_path(it->substr(p->size()+1), Pathname::DATA_PATH);
          result.push_back(sub_path);
        }
      }
      break;
    }

    case Pathname::SYSTEM_PATH: {
      auto lst = System::opendir_recursive(pathname);
      for(auto it = lst.begin(); it != lst.end(); ++it)
      {
        result.push_back(Pathname(*it, Pathname::SYSTEM_PATH));
      }
      break;
    }
  }
}

std::vector<Pathname>
Pathname::opendir_recursive() const
{
  std::vector<Pathname> result;
  opendir_recursive(result);
  std::sort(result.begin(), result.end());
  result.erase(std::unique(result.begin(), result.end()), result.end());
  return result;
}

std::ostream& operator<< (std::ostream& os, const Pathname& p)
{
  switch(p.get_type())
  {
    case Pathname::INVALID:
      return os << "invalid://" << p.get_raw_path();

    case Pathname::DATA_PATH:
      return os << "datadir://" << p.get_raw_path();

    case Pathname::SYSTEM_PATH:
      return os << "system://" << p.get_raw_path();

    default:
      assert(false && "never reached");
  }
  return os;
}

bool
Pathname::has_extension(const std::string& ext) const
{
  return StringUtil::has_suffix(pathname, ext);
}

bool
Pathname::operator<(const Pathname& rhs) const
{
  if (type < rhs.type)
  {
    return true;
  }
  else if (type > rhs.type)
  {
    return false;
  }
  else
  {
    return pathname < rhs.pathname;
  }
}

bool
Pathname::operator==(const Pathname& rhs) const
{
  return type == rhs.type && pathname == rhs.pathname;
}

/* EOF */
