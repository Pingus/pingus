//  $Id: xml_plf.hxx,v 1.6 2002/09/08 18:13:04 grumbel Exp $
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

#ifndef HEADER_PINGUS_XML_PLF_HXX
#define HEADER_PINGUS_XML_PLF_HXX

#include "plf.hxx"
#include "libxmlfwd.hxx"

class XMLPLF : public PLF
{
private:
  xmlDocPtr doc;  

  void parse_file ();

  void parse_background  (xmlNodePtr cur);
  void parse_actions     (xmlNodePtr cur);
  void parse_global      (xmlNodePtr cur);
  void parse_groundpiece (xmlNodePtr cur);
  void parse_group       (xmlNodePtr cur);
  void parse_start_pos   (xmlNodePtr cur);
  void parse_weather     (xmlNodePtr cur);

  /** trap parsing is only here for backward compatibility, traps are
      worldobjs now */
  void parse_traps       (xmlNodePtr node);

public:
  XMLPLF (const std::string& filename);
  virtual ~XMLPLF();
  
private:
  XMLPLF (const XMLPLF&);
  XMLPLF operator= (const XMLPLF&);
};

#endif

/* EOF */
