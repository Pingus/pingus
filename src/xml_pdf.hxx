//  $Id: xml_pdf.hxx,v 1.2 2003/01/15 21:16:41 torangan Exp $
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

#ifndef HEADER_PINGUS_XML_PDF_HXX
#define HEADER_PINGUS_XML_PDF_HXX

#include <string>
#include <vector>
#include <iosfwd>

#include "server_event.hxx"

class PLF;

/** XMLPDF stands for Pingus Demo File, it contains all the infos
    necesarry to play a pingus demo recording */
class XMLPDF
{
private:
  PLF* plf;
  std::string levelname;
  std::vector<ServerEvent> events;
public:
  XMLPDF(const std::string& filename);
  ~XMLPDF();

  /** @return a pointer to the level structure */
  PLF* get_plf();

  /** @return the levelname */
  std::string get_levelname();

  /** Returns a reference to the events of this demo */
  std::vector<ServerEvent> get_events();

  void write_xml(std::ostream& xml);

private:
  XMLPDF (const XMLPDF&);
  XMLPDF& operator= (const XMLPDF&);
};

#endif

/* EOF */
