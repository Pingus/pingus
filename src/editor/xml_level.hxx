//  $Id: xml_level.hxx,v 1.00 2005/11/11 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_XMLLEVEL_HXX
#define HEADER_PINGUS_EDITOR_XMLLEVEL_HXX

namespace Pingus {

namespace Editor {

	class LevelImpl;

class XMLLevel
{
private:
	LevelImpl* impl;

public:

	/** Construct new blank level */
	XMLLevel();

	/** Destructor */
	~XMLLevel();

	/** Verify that level is valid */
	bool is_valid();

	/** Save the level to a file.  Returns true if successful */
	bool save_level(const std::string& filename);

	/** Load an existing level from a file */
	void load_level(const std::string& filename);

private:
	XMLLevel (const XMLLevel&);
  XMLLevel& operator= (const XMLLevel&);
};	// XMLLevel class

}		// Editor namespace
}		// Pingus namespace

#endif

/* EOF */