//  $Id: InfoBox.cc,v 1.10 2002/06/11 18:28:37 torangan Exp $
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
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Display/display.h>
#include "../PinguHolder.hh"
#include "../World.hh"
#include "../PingusResource.hh"
#include "../XMLhelper.hh"
#include "InfoBox.hh"
#include "../Pingu.hh"

InfoBoxData::InfoBoxData ()
{
}

InfoBoxData::~InfoBoxData ()
{
}

WorldObj* 
InfoBoxData::create_WorldObj()
{
  return new InfoBox (*this);
}

EditorObjLst
InfoBoxData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj>(new EditorInfoBox (*this)));
  return lst;
}

InfoBoxData::InfoBoxData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "info-text") == 0)
	{
	  info_text = XMLhelper::parse_string (doc, cur);
	}
      else
	std::cout << "InfoBox::creata (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }
}

void 
InfoBoxData::write_xml(std::ofstream* xml)
{
  (*xml) << "  <worldobj type=\"infobox\">\n";
  XMLhelper::write_vector_xml (xml, pos);
  
  (*xml) << "   <info-text>" << info_text << "</info-text>\n" 
	 << "  </worldobj>\n" << std::endl;
}

InfoBox::InfoBox (const InfoBoxData& data)
  : InfoBoxData (data),
    sprite ("infobox", "worldobjs"), 
    is_open (false)
{
  sprite.set_align_center_bottom ();
  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
}

void
InfoBox::draw_offset (int x, int y, float /*s*/)
{
  int x_pos = int(pos.x) + x;
  int y_pos = int(pos.y) + y - 100;

  if (is_open)
    {
      int width = font->get_text_width (info_text.c_str ());
      int border = 6;
      CL_Display::draw_line (int(pos.x + x), int(pos.y + y),
			     x_pos, y_pos, 0.0f, 1.0f, 0.0f, 1.0f);
      sprite.put_screen (pos + CL_Vector (x, y));    
      CL_Display::fill_rect (x_pos - width/2 - border, y_pos - border,
			     x_pos + width/2 + border, y_pos + font->get_height () + border,
			     0.0, 0.0, 0.0, 1.0);
      font->print_center (x_pos, y_pos, info_text.c_str ()); 
    }
  else
    {
      sprite.put_screen (pos + CL_Vector (x, y));
    }
}

void
InfoBox::update (float delta)
{
  sprite.update (delta);

  PinguHolder* holder = world->get_pingu_p();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if ((*pingu)->is_inside (int(pos.x - 16), int(pos.y - 32),
			       int(pos.x + 16), int(pos.y)))
	{
	  is_open = true;
	}
    }
}

EditorInfoBox::EditorInfoBox(const InfoBoxData& data)
  : InfoBoxData (data),
    SpriteEditorObj ("infobox", "worldobjs", pos)
{
  sprite.set_align_center_bottom ();
}

std::list<boost::shared_ptr<EditorObj> >
EditorInfoBox::create (const CL_Vector& pos)
{
  std::list<boost::shared_ptr<EditorObj> > lst;
  InfoBoxData data;
  data.pos = pos;
  lst.push_back (boost::shared_ptr<EditorObj>(new EditorInfoBox (data)));
  return lst;
}

boost::shared_ptr<EditorObj>
EditorInfoBox::duplicate()
{
  return boost::shared_ptr<EditorObj>(new EditorInfoBox (*this));
}

std::string 
EditorInfoBox::status_line ()
{
  return "InfoBox";
}

/* EOF */
