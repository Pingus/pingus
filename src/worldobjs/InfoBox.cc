//  $Id: InfoBox.cc,v 1.1 2001/11/30 20:22:21 grumbel Exp $
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

#include "../PingusResource.hh"
#include "InfoBox.hh"

InfoBoxData::InfoBoxData ()
{
}

InfoBoxData::~InfoBoxData ()
{
}

boost::shared_ptr<WorldObj> 
InfoBoxData::create_WorldObj()
{
  std::cout << "InfoBoxData::create_WorldObj(): " << this->pos << std::endl;
  return boost::shared_ptr<WorldObj> (new InfoBox (*this));
}

EditorObjLst
InfoBoxData::create_EditorObj()
{
  std::cout << "InfoBoxData::create_EditorObj()" << this->pos << std::endl;
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj>(new EditorInfoBox (*this)));
  return lst;
}

boost::shared_ptr<WorldObjData>
InfoBoxData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  InfoBoxData* data = new InfoBoxData ();

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
	  data->pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "info-text") == 0)
	{
	  data->info_text = XMLhelper::parse_string (doc, cur);
	}
      else
	std::cout << "InfoBox::creata (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }

  return boost::shared_ptr<WorldObjData>(data);
}

void 
InfoBoxData::write_xml(ofstream* xml)
{
  (*xml) << "  <worldobj type=\"infobox\">";
  XMLhelper::write_position_xml (xml, pos);
  
  (*xml) << "   <info-text>" << info_text << "</info-text>\n" 
	 << "  </worldobj>\n" << std::endl;
}

InfoBox::InfoBox (const InfoBoxData& data)
  : InfoBoxData (data),
    sprite ("infobox", "worldobjs")
{
  sprite.set_align_center_bottom ();
  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
}

void
InfoBox::draw_offset (int x, int y, float s)
{
  sprite.put_screen (pos + CL_Vector (x, y));

  int x_pos = int(pos.x) + x;
  int y_pos = int(pos.y) + y - 60;

  font->print_center (x_pos, y_pos, info_text.c_str ()); 
}

void
InfoBox::update (float delta)
{
  sprite.update (delta);
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
