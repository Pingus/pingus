//  $Id: XMLPLF.hh,v 1.12 2001/08/10 10:56:14 grumbel Exp $
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

#ifndef XMLPLF_HH
#define XMLPLF_HH

#include "XMLhelper.hh"
#include "Position.hh"
#include "Color.hh"
#include "PLF.hh"

class XMLPLF : public PLF
{
private:
  xmlDocPtr doc;  

  void parse_file();

  void parse_background (xmlNodePtr cur);
  void parse_actions (xmlNodePtr cur);
  void parse_global (xmlNodePtr cur);
  void parse_groundpiece (xmlNodePtr cur);
  void parse_group (xmlNodePtr cur);
  void parse_start_pos (xmlNodePtr cur);
  void parse_weather (xmlNodePtr cur);

public:
  XMLPLF(const std::string& filename);
  virtual ~XMLPLF();
};

#endif

/* EOF */
