//  $Id: stat_manager.hxx,v 1.2 2003/04/19 10:23:17 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_PINGUS_STAT_MANAGER_HXX
#define HEADER_PINGUS_STAT_MANAGER_HXX

#include <map>
#include <string>

/** */
class StatManager
{
private:
  static StatManager* instance_;

  /** File from which it loads/saves the stats */
  std::string statfilename;
  typedef std::map<std::string, std::string> Table;
  Table stats;

public:
  static StatManager* instance();
  static void init();
  static void deinit();

  StatManager(const std::string& filename);
  ~StatManager();

  bool get_int   (const std::string&, int& value);
  bool get_bool  (const std::string&, bool& value);
  bool get_string(const std::string&, std::string& vlaue);

  void set_string(const std::string& name, const std::string& value);
  void set_int   (const std::string& name, int value);
  void set_bool  (const std::string& name, bool value);

  /** Write the current status down */
  void flush();

  void load(const std::string& filename);
  void save(const std::string& filename);
private:
  StatManager (const StatManager&);
  StatManager& operator= (const StatManager&);
};

#endif

/* EOF */
