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

#include "result.hpp"

#include "statistics.hpp"
#include <stdexcept>

Statistics* Statistics::s_instance = 0;

Statistics::Statistics() :
  m_filename("statistics.txt"),
  m_username("<unset>")
{

}

Statistics::~Statistics()
{
  
}

void
Statistics::set_username(const std::string& username)
{
  m_username = username;
  for(std::string::iterator i = m_username.begin(); i != m_username.end(); ++i)
  {
    if (*i == ';')
      *i = ':';
  }
}

void
Statistics::save_result(const Result& result, int actions_used)
{
  //m_out << "# username, levelname, saved, killed, time, success" << std::endl;
  std::ofstream m_out(m_filename.c_str(), std::ios::app);
  if (!m_out)
  {
    throw std::runtime_error(m_filename + ": couldn't open file for writing");
  }
  else
  {
    m_out << m_username << ";"
          << result.plf.get_resname() << ";"
          << result.saved << ";"
          << result.killed << ";"
          << result.used_time << ";"
          << actions_used << ";"
          << (result.aborted ? "aborted" : (result.success()?"success":"failure")) << std::endl;
  }
}

void
Statistics::mark_session_end()
{
  std::ofstream m_out(m_filename.c_str(), std::ios::app);
  if (!m_out)
  {
    throw std::runtime_error(m_filename + ": couldn't open file for writing");
  }
  else
  {
    m_out << "### session end: " <<  m_username << " ###\n" << std::endl;
  }
}

void
Statistics::mark_session_start()
{
  std::ofstream m_out(m_filename.c_str(), std::ios::app);
  if (!m_out)
  {
    throw std::runtime_error(m_filename + ": couldn't open file for writing");
  }
  else
  {
    m_out << "### session start: " <<  m_username << " ###\n" << std::endl;
  }
}

/* EOF */
