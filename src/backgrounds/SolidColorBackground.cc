//  $Id: SolidColorBackground.cc,v 1.4 2001/03/18 17:45:04 grumbel Exp $
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
#include "../XMLhelper.hh"
#include "SolidColorBackground.hh"

  /** Writte the content of this object formated as xml to the given
      stream */
void
SolidColorBackgroundData:: write_xml(ofstream* xml)
{
  (*xml) << "<background type=\"solidcolor\"></background>" << std::endl;
}

boost::shared_ptr<SolidColorBackgroundData> 
SolidColorBackgroundData::create (xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<SolidColorBackgroundData> data (new SolidColorBackgroundData ());

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
  return data;
}

boost::shared_ptr<SolidColorBackground>
SolidColorBackground::create (boost::shared_ptr<BackgroundData> arg_data)
{
  SolidColorBackgroundData* data = dynamic_cast<SolidColorBackgroundData*>(arg_data.get());
  assert (data);
  boost::shared_ptr<SolidColorBackground> background = 
    boost::shared_ptr<SolidColorBackground>(new SolidColorBackground());

  background->color = data->color;
  return background;
}

void
SolidColorBackground::draw_offset (int x_of, int y_of, float s = 1.0)
{
  // FIXME: Probally fill_rect is better here, but slower?
  CL_Display::clear_display (color.red, color.green, color.blue, color.alpha);
}

/* EOF */
