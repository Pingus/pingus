//  $Id: StarfieldBackground.hh,v 1.1 2000/09/29 15:43:52 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef STARFIELDBACKGROUND_HH
#define STARFIELDBACKGROUND_HH

#include <ClanLib/core.h>
#include "../WorldObj.hh"
#include "../PingusResource.hh"
#include "BackgroundData.hh"

class StarfieldBackgroundData : public BackgroundData
{
public:
  int small_stars_count;
  int middle_stars_count;
  int large_stars_count;

  StarfieldBackgroundData () {}
  virtual ~StarfieldBackgroundData () {}

  void write_xml(ofstream* xml);
  static StarfieldBackgroundData* create(xmlDocPtr doc, xmlNodePtr cur);
};

class StarfieldBackgroundStars : public WorldObj
{
private:
  static CL_Surface* small_star;
  static CL_Surface* middle_star;
  static CL_Surface* large_star;
  static bool is_init;

  CL_Surface* sur;

  float x_pos;
  float y_pos;

  float x_add;
  float y_add;

public:
  enum Type { SMALL_STAR, MIDDLE_STAR, LARGE_STAR };

  ///
  StarfieldBackgroundStars () {}
  ///
  StarfieldBackgroundStars (Type type);
  ///
  void init ();
  ///
  void let_move(void);
  ///
  void draw_offset(int x_of, int y_of, float s = 1.0);  
};

class StarfieldBackground : public Background,
			    private StarfieldBackgroundData
{
private:
  vector<StarfieldBackgroundStars> stars;

public:
  ///
  StarfieldBackground ();
  ///
  StarfieldBackground (StarfieldBackgroundData*);
  ///
  static StarfieldBackground* create (BackgroundData* arg_data);
  ///
  virtual ~StarfieldBackground ();
  ///
  void let_move(void);
  ///
  void draw_offset(int x_of, int y_of, float s = 1.0);
};

#endif

/* EOF */
