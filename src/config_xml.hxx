//  $Id: config_xml.hxx,v 1.4 2003/04/19 10:23:17 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_CONFIG_XML_HXX
#define HEADER_PINGUS_CONFIG_XML_HXX

#include <string>
#include "libxmlfwd.hxx"

/** Reader for the pingus config file */
class ConfigXML
{
private:
  //std::map<std::string, Prop>
public:
  ConfigXML(const std::string& filename);
  ConfigXML(xmlDocPtr doc, xmlNodePtr cur);

private:
  void parse_directory(xmlDocPtr doc, xmlNodePtr cur, const std::string& prefix);
  bool is_directory(xmlDocPtr doc, xmlNodePtr cur);
  bool is_value(xmlDocPtr doc, xmlNodePtr cur);

  ConfigXML (const ConfigXML&);
  ConfigXML& operator= (const ConfigXML&);
};

#endif

/* EOF */
