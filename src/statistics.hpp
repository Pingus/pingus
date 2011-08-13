//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_STATISTICS_HPP
#define HEADER_PINGUS_STATISTICS_HPP

#include <fstream>

class Result;

class LevelStat
{
private:
  int m_play_count;
  bool m_finished;

  static int s_max_number_of_tries;
public:
  LevelStat();

  bool is_accessible() const;
  bool is_finished() const;
  int  get_play_count() const;
  void incr_play_count();

  void set_finished(bool v);

  /** Maximum number of times a user is allowed to retry the given level */
  int get_max_play_count() const;
  static void set_max_play_count(int v);
};

class Statistics
{
private:
  static Statistics* s_instance; 
public:
  static Statistics* instance() { 
    if (s_instance) 
      return s_instance; 
    else
      return s_instance = new Statistics; 
  }

private:
  std::string m_filename;
  std::string m_username;

  typedef std::map<std::string, LevelStat> LevelStats;
  LevelStats m_level_stats;

public:
  Statistics();
  ~Statistics();

  void clear();

  void save_result(const Result& result, int actions_used);

  LevelStat& get_level_stat(const std::string& resname);

  void start_session(const std::string& username);
  void end_session();

private:
  void set_username(const std::string& username);

private:
  Statistics(const Statistics&);
  Statistics& operator=(const Statistics&);
};

#endif

/* EOF */
