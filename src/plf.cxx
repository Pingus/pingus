//  $Id: plf.cxx,v 1.17 2003/08/19 19:56:55 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "xml_plf.hxx"
#include "system.hxx"
#include "pingus_error.hxx"
#include "cheat.hxx"

PLF::PLF()
{
  start_x_pos = start_y_pos = 0;
  width = 1600; height = 800;
  number_of_pingus = 100;
  max_time = -1;
  number_to_save = 0;
  number_of_pingus = 100;
  difficulty = 5;
  playable = false;
  music = "none";
}

// Destroy all data
PLF::~PLF()
{
  std::cout << "PLF::~PLF: Deleting std::vector<WorldObjData*>" << std::endl;
  for (std::vector<WorldObjData*>::iterator i = worldobjs_data.begin ();
       i != worldobjs_data.end (); ++i)
    {
      delete *i;
    }
}

int
PLF::get_startx()
{
  return start_x_pos;
}

int
PLF::get_starty()
{
  return start_y_pos;
}

int
PLF::get_width(void)
{
  return width;
}

int
PLF::get_height(void)
{
  return height;
}

int
PLF::get_pingus()
{
  return number_of_pingus;
}

std::vector<ActionData>
PLF::get_actions()
{
  // FIXME: we should merge duplicated actions
  return actions;
}

std::string
PLF::get_music()
{
  return music;
}

std::map<std::string, std::string>
PLF::get_description()
{
  return description;
}

std::string
PLF::get_filename()
{
  return filename;
}

std::string
PLF::get_resname()
{
  return resname;
}

std::map<std::string, std::string>
PLF::get_levelname()
{
  return levelname;
}

int
PLF::get_time(void)
{
  // FIXME: Cheats shouldn't be handled in the PLF, but instead in the
  // client code, but since we havn't any holder for the time in the
  // client, we quick&dirty place it here.
  if (Cheat::no_time_limit)
    return -1;
  else
    return max_time;
}

int
PLF::get_number_to_save()
{
  return number_to_save;
}

int
PLF::get_difficulty()
{
  return difficulty;
}

bool
PLF::get_playable()
{
  return playable;
}

std::string
PLF::get_comment ()
{
  return comment;
}

std::string
PLF::get_author()
{
  return author;
}

std::vector<WorldObjsData::GroundpieceData>
PLF::get_groundpieces(void)
{
  return groundpieces;
}

std::vector<WorldObjData*>
PLF::get_worldobjs_data ()
{
  //std::cout << "World: " << worldobjs_data.size () << std::endl;
  return worldobjs_data;
}

PLF*
PLF::create (const std::string& pathname)
{
  return new XMLPLF (pathname);
}

/* EOF */
