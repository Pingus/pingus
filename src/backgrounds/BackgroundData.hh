// $Id: BackgroundData.hh,v 1.5 2000/12/16 23:11:22 grumbel Exp $
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

#ifndef BACKGROUNDDATA_HH
#define BACKGROUNDDATA_HH

#include <fstream>

#include "../boost/smart_ptr.hpp"
#include "../XMLhelper.hh"
#include "../ResDescriptor.hh"
#include "../Color.hh"

/** This class represents the data that is needed to create a
 background.  To create a new background type you have to hinherit
 from BackgroundData and supply members to write the xml data, you
 also have to provide a create() member which parses the xml data and
 returns a newly allocated BackgroundData object, this member needs to
 be hooked up into BackgroundData::create(). */
class BackgroundData
{
public:
  BackgroundData();
  virtual ~BackgroundData();

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(ofstream* xml) =0;
  
  static boost::shared_ptr<BackgroundData> create(xmlDocPtr doc, xmlNodePtr cur);
};

#endif

/* EOF */
