//  $Id: info_box.cxx,v 1.10 2002/09/10 19:24:19 grumbel Exp $
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
#include "../graphic_context.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../xml_helper.hxx"
#include "info_box.hxx"
#include "../pingu.hxx"

namespace WorldObjsData {

InfoBoxData::InfoBoxData ()
{
}

InfoBoxData::~InfoBoxData ()
{
}

WorldObj* 
InfoBoxData::create_WorldObj()
{
  return new WorldObjs::InfoBox (*this);
}

EditorObjLst
InfoBoxData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (new EditorObjs::EditorInfoBox (*this));
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

InfoBoxData::InfoBoxData (const InfoBoxData& old) : WorldObjData(old),
                                                    info_text(old.info_text),
						    pos(old.pos),
						    text_pos(old.text_pos)
{
}

InfoBoxData
InfoBoxData::operator= (const InfoBoxData& old)
{
  if (this == &old)
    return *this;
  
  WorldObjData::operator=(old);
  
  info_text = old.info_text;
  pos       = old.pos;
  text_pos  = old.text_pos;
  
  return *this;
}

void 
InfoBoxData::write_xml(std::ostream& xml)
{
  xml << "  <worldobj type=\"infobox\">\n";
  XMLhelper::write_vector_xml (xml, pos);
  xml << "   <info-text>" << info_text << "</info-text>\n" 
      << "  </worldobj>\n" << std::endl;
}

} // namespace WorldObjsData

namespace WorldObjs {

InfoBox::InfoBox (const WorldObjsData::InfoBoxData& data)
  : InfoBoxData (data),
    sprite ("infobox", "worldobjs"), 
    is_open (false)
{
  sprite.set_align_center_bottom ();
  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
}

void
InfoBox::draw (GraphicContext& gc)
{
  int x = int(gc.get_x_offset () + (gc.get_width ()/2));
  int y = int(gc.get_y_offset () + (gc.get_height ()/2));

  int x_pos = int(pos.x) + x;
  int y_pos = int(pos.y) + y - 100;

  if (is_open)
    {
      int width = font->get_text_width (info_text.c_str ());
      int border = 6;
      gc.draw_line (pos, pos + CL_Vector(0, 0 - 100), 0.0f, 1.0f, 0.0f, 1.0f);
      gc.draw(sprite, pos);
      CL_Display::fill_rect (x_pos - width/2 - border, y_pos - border,
			     x_pos + width/2 + border, y_pos + font->get_height () + border,
			     0.0, 0.0, 0.0, 1.0);
      font->print_center (x_pos, y_pos, info_text.c_str ()); 
    }
  else
    {
      gc.draw (sprite, pos);
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

} // namespace WorldObjs

namespace EditorObjs {

EditorInfoBox::EditorInfoBox(const InfoBoxData& data)
  : InfoBoxData (data),
    SpriteEditorObj ("infobox", "worldobjs", &pos)
{
  sprite.set_align_center_bottom ();
}

EditorInfoBox::EditorInfoBox (const EditorInfoBox& old) : InfoBoxData(old), 
                                                          SpriteEditorObj(old)
{
}

EditorInfoBox
EditorInfoBox::operator= (const EditorInfoBox& old)
{
  if (this == &old)
    return *this;
    
  InfoBoxData::operator=(old);
  SpriteEditorObj::operator=(old);
  
  return *this;
}

EditorObjLst
EditorInfoBox::create (const CL_Vector& pos)
{
  EditorObjLst lst;
  InfoBoxData data;
  data.pos = pos;
  lst.push_back (new EditorInfoBox (data));
  return lst;
}

EditorObj*
EditorInfoBox::duplicate()
{
  return new EditorInfoBox (*this);
}

std::string 
EditorInfoBox::status_line ()
{
  return "InfoBox";
}

} // namespace EditorObjs

/* EOF */
