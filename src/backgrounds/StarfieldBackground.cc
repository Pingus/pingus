//  $Id: StarfieldBackground.cc,v 1.24 2002/06/11 18:28:37 torangan Exp $
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

#include <fstream>
#include "../globals.hh"
#include "../editor/SpriteEditorObj.hh"
#include "../World.hh"
#include "../PingusResource.hh"
#include "../XMLhelper.hh"
#include "StarfieldBackground.hh"

StarfieldBackgroundData::StarfieldBackgroundData ()
  : small_stars_count (400),
    middle_stars_count (200),
    large_stars_count (100)
{
}

void 
StarfieldBackgroundData::write_xml(std::ofstream* xml)
{
  (*xml) << "<background type=\"starfield\">\n"
	 << "  <small-stars count=\"" << small_stars_count << "\"/>\n"
	 << "  <middle-stars count=\"" << middle_stars_count << "\"/>\n"
	 << "  <large-stars count=\"" << large_stars_count << "\"/>\n"
	 << "</background>\n"
	 << std::endl;
}
 
StarfieldBackgroundData::StarfieldBackgroundData (xmlDocPtr /*doc*/, xmlNodePtr cur)
{
  small_stars_count = 100;
  middle_stars_count = 50;
  large_stars_count = 25;

  cur = cur->children;

    while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      if (strcmp((char*)cur->name, "small-stars") == 0)
	{
	  char* count = (char*)xmlGetProp(cur, (xmlChar*)"count");
	  if (count)
	    {
	      small_stars_count = StringConverter::to_int(count);
	      free (count);
	    }
	}
      else if (strcmp((char*)cur->name, "middle-stars") == 0)
	{
	  char* count = (char*)xmlGetProp(cur, (xmlChar*)"count");
	  if (count)
	    {
	      middle_stars_count = StringConverter::to_int(count);
	      free (count);
	    }	  
	}
      else if (strcmp((char*)cur->name, "large-stars") == 0)
	{
	  char* count = (char*)xmlGetProp(cur, (xmlChar*)"count");
	  if (count)
	    {
	      large_stars_count = StringConverter::to_int(count);
	      free (count);
	    }	  
	}
      else
	{
	  std::cout << "StarfildBackgroundData:create: Unhandled tag: " << cur->name << std::endl;
	} 
      cur = cur->next;
    }
}

WorldObj* 
StarfieldBackgroundData::create_WorldObj()
{
  return new StarfieldBackground (*this);
}

EditorObjLst
StarfieldBackgroundData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj> (new EditorStarfieldBackground (*this)));
  return lst;
}

/****************************/
/* StarfieldBackgroundStars */
/****************************/

CL_Surface StarfieldBackgroundStars::small_star;
CL_Surface StarfieldBackgroundStars::middle_star;
CL_Surface StarfieldBackgroundStars::large_star;
bool StarfieldBackgroundStars::is_init = false;

StarfieldBackgroundStars::StarfieldBackgroundStars (Type type)
{
  init ();
 
  switch (type)
    {
    case SMALL_STAR:
      sur = small_star;
      break;
    case MIDDLE_STAR:
      sur = middle_star;
      break;
    case LARGE_STAR:
      sur = large_star;
      break;
    }
 
  x_pos = rand () % world->get_width ();
  y_pos = rand () % world->get_height ();
  
  x_add = rand () % 5 + 1.0;
  y_add = 0.0;
}

void
StarfieldBackgroundStars::init ()
{
  if (!is_init)
    {
      small_star  = PingusResource::load_surface ("Stars/small_star", "game");
      middle_star = PingusResource::load_surface ("Stars/middle_star", "game");
      large_star  = PingusResource::load_surface ("Stars/large_star", "game");
      is_init = true;
    }
}

void
StarfieldBackgroundStars::update(float /*delta*/)
{
  x_pos += x_add;
  y_pos += y_add;
  
  if (x_pos > world->get_width ())
    {
      x_pos = -32;
      y_pos = rand () % world->get_height ();
    }
}

void
StarfieldBackgroundStars::draw_offset(int x_of, int y_of, float /*s*/)
{
  sur.put_screen (int(x_pos + x_of), int(y_pos + y_of));
}

StarfieldBackground::StarfieldBackground (const StarfieldBackgroundData& data)
{
  small_stars_count  = data.small_stars_count;
  middle_stars_count = data.middle_stars_count;
  large_stars_count  = data.large_stars_count;

  for (int i=0; i < small_stars_count; i++)
    stars.push_back (StarfieldBackgroundStars (StarfieldBackgroundStars::SMALL_STAR));

  for (int i=0; i < middle_stars_count; i++)
    stars.push_back (StarfieldBackgroundStars (StarfieldBackgroundStars::MIDDLE_STAR));

  for (int i=0; i < large_stars_count; i++)
    stars.push_back (StarfieldBackgroundStars (StarfieldBackgroundStars::LARGE_STAR));
}

StarfieldBackground::~StarfieldBackground ()
{
  
}

/*
boost::shared_ptr<StarfieldBackground>
StarfieldBackground::create (boost::shared_ptr<BackgroundData> arg_data)
{
  StarfieldBackgroundData* data = dynamic_cast<StarfieldBackgroundData*>(arg_data.get());
  return boost::shared_ptr<StarfieldBackground>(new StarfieldBackground (data));
}*/

///
void 
StarfieldBackground::update(float delta)
{
  for (std::vector<StarfieldBackgroundStars>::iterator i = stars.begin ();
       i != stars.end (); i++)
    {
      i->update (delta);
    }
}
///
void 
StarfieldBackground::draw_offset(int x_of, int y_of, float /*s*/)
{
  for (std::vector<StarfieldBackgroundStars>::iterator i = stars.begin ();
       i != stars.end (); i++)
    {
      i->draw_offset (x_of, y_of);
    }
}

/* EOF */

