//  $Id: savegame.hxx,v 1.3 2003/04/19 10:23:17 torangan Exp $
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

#ifndef HEADER_PINGUS_SAVEGAME_HXX
#define HEADER_PINGUS_SAVEGAME_HXX

#include <string>
#include "libxmlfwd.hxx"

/** The Savegame class holds savegame informations for a single
    level */
class Savegame
{
private:
public:
  enum Status { FINISHED,   // level is successfully finished
                ACCESSIBLE, // level is accessible and can be played
                NONE};      // level is not finished and cannot be accessed
  static std::string status_to_string (Status s);
  static Status string_to_status (std::string s);


  std::string levelname;
  Status status;

  /** Time needed to finish the level, only valid if status == FINISHED */
  int time;

  /** Number of Pingus that where saved while playing the level */
  int saved_pingus;

  Savegame();
  Savegame(xmlDocPtr doc, xmlNodePtr node);
  Savegame(std::string arg_levelname, Status arg_status, int arg_time, int arg_saved_pingus);

  void read_xml (xmlDocPtr doc, xmlNodePtr node);
  void write_xml(std::ostream& xml);
};

#endif

/* EOF */
