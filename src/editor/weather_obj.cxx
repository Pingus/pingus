//  $Id: weather_obj.cxx,v 1.5 2002/07/02 13:36:07 torangan Exp $
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
#include "../xml_helper.hxx"
#include "editor_view.hxx"
#include "weather_obj.hxx"

WeatherObj::WeatherObj(const WeatherData& data)
  : SpriteEditorObj (pos)
{
  type = data.type;
  pos = CL_Vector(0,0,200);
  dragging = false;

  if (type == "rain")
    {
      sprite = Sprite("editor/weather_rain", "core");
    }
  else if (type == "snow")
    {
      sprite = Sprite("editor/weather_snow", "core");
    }  
  else 
    {
      std::cout << "WeatherObj: Unknown weather type: " << type << std::endl;
      sprite = Sprite("editor/weather_rain", "core");
    }
}

WeatherObj::~WeatherObj()
{
}

void 
WeatherObj::draw (EditorView * view)
{
  //std::cout << "Dragging: " << dragging << std::endl;
      
  if (dragging)
    {
      for (int x = 0; x < 320; x += sprite.get_width () + 4)
	{
	  view->draw_fillrect (x, 0,
			       x + sprite.get_width (), sprite.get_height (),
			       1.0f, 1.0f, 1.0f, 0.5f);
	}
    }

  CL_Vector tmp_pos (pos);
  pos.x = int((pos.x + sprite.get_width ()/2)
	      /sprite.get_width ()) * sprite.get_width ();
  pos.y = int((pos.y - sprite.get_height ()/2)
	      /sprite.get_height ()) * sprite.get_height ();
  SpriteEditorObj::draw (view);
  pos = tmp_pos;
}
  
void   
WeatherObj::write_xml(std::ostream& xml)
{
  xml << "  <weather>\n"
      << "    <type>" << type << "</type>\n";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "  </weather>\n"
      << std::endl;
}
 
EditorObj*
WeatherObj::duplicate()
{
  return new WeatherObj(*this);
}

void 
WeatherObj::drag ()
{
  dragging = true;
}

void 
WeatherObj::drop ()
{
  dragging = false;
  pos.x = int((pos.x + sprite.get_width ()/2)
	      /sprite.get_width ()) * sprite.get_width ();
  pos.y = int((pos.y - sprite.get_height ()/2)
	      /sprite.get_height ()) * sprite.get_height ();
}

/* EOF */
