// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_PINGUS_DEMO_HPP
#define HEADER_PINGUS_PINGUS_PINGUS_DEMO_HPP

#include <vector>

#include "pingus/server_event.hpp"

class Pathname;

class PingusDemo
{
private:
  std::string m_levelname;
  std::string m_checksum;
  std::vector<ServerEvent> m_events;

public:
  PingusDemo(const Pathname& pathname);

  std::string get_levelname() const { return m_levelname; }
  std::string get_checksum() const { return m_checksum; }

  std::vector<ServerEvent> get_events() const { return m_events; }

private:
  PingusDemo (const PingusDemo&);
  PingusDemo& operator= (const PingusDemo&);
};

#endif

/* EOF */
