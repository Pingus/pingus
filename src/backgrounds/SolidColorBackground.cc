//  $Id: SolidColorBackground.cc,v 1.7 2001/08/12 23:05:22 grumbel Exp $
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

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "../editor/SpriteEditorObj.hh"
#include "../editor/EditorObj.hh"
#include "../XMLhelper.hh"
#include "SolidColorBackground.hh"

void
SolidColorBackgroundData:: write_xml(std::ofstream* xml)
{
  (*xml) << "<worldobj type=\"solidcolor-background\"></worldobj>" << std::endl;
}

boost::shared_ptr<WorldObjData> 
SolidColorBackgroundData::create (xmlDocPtr doc, xmlNodePtr cur)
{
  SolidColorBackgroundData* data (new SolidColorBackgroundData ());

  cur = cur->children;
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp ((char*) cur->name, "color") == 0)
	{
	  data->color = XMLhelper::parse_color (doc, cur);
	}
      else
	{
	  std::cout << "SolidColorBackground: Unhandled tag: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
  return boost::shared_ptr<WorldObjData>(data);
}

boost::shared_ptr<WorldObj> 
SolidColorBackgroundData::create_WorldObj()
{
  return boost::shared_ptr<WorldObj> (new SolidColorBackground (*this));
}

class EditorSolidColorBackground : public SolidColorBackgroundData,
				   public SpriteEditorObj
{
private:
  CL_Vector pos;
public:
  EditorSolidColorBackground (const SolidColorBackgroundData& data)
    : SolidColorBackgroundData (data),
      SpriteEditorObj ("Stars/starfield_icon", "game", pos),
      pos (-64.0f, 0.0f)
  {}
  void write_xml(ofstream* xml) { this->SolidColorBackgroundData::write_xml (xml); }

  boost::shared_ptr<EditorObj> duplicate() {
    return boost::shared_ptr<EditorObj>(new EditorSolidColorBackground (*this));
  }

  std::string status_line () { return "SolidColorBackground"; }
};

EditorObjLst 
SolidColorBackgroundData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj>(new EditorSolidColorBackground (*this)));
  return lst;
}

SolidColorBackground::SolidColorBackground (const SolidColorBackgroundData& data)
  : SolidColorBackgroundData (data)
{
}

void
SolidColorBackground::draw_offset (int x_of, int y_of, float s = 1.0)
{
  // FIXME: Probally fill_rect is better here, but slower?
  CL_Display::clear_display (color.red, color.green, color.blue, color.alpha);
}

/* EOF */
