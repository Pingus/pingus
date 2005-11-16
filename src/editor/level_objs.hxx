//  $Id: level_objs.hxx,v 1.00 2005/11/15 23:41:12 Jave27 Exp $
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

#ifndef HEADER_PINGUS_EDITOR_LEVEL_OBJS_HXX
#define HEADER_PINGUS_EDITOR_LEVEL_OBJS_HXX

#include <ClanLib/Display/sprite.h>
#include "../vector.hxx"
#include <string>

namespace Pingus {

	class XMLFileWriter;
	class DrawingContext;

namespace Editor {

/** Generic Level Object (groundpiece, sign, etc.)  Only special objects will have
		to inherit from this class - most objects will be able to use this class alone */
class LevelObj
{
protected:
	/** Sprite used to draw this object */
	CL_Sprite sprite;

	/** Resource name of this object's sprite */
	std::string res_name;

	/** Modifier used on the sprite (ROT90, ROT180, etc.) */
	std::string modifier;

	/** Location of this object in the World */
	Vector pos;

	/** Name of the section header for this object (hotspot, groundpiece, etc.) */
	std::string section_name;

	/** Optional type field to further describe the resource */
	std::string object_type;

/////////////////////////////////////////////////////////
/// Retrieve info
public:
	/** Retrieve the object's position */
	Vector get_pos() const { return pos; }

	/** Retrieve the object's resource name */
	std::string get_res_name() const { return res_name; }

	/** Retrieve the object's resource modifier */
	std::string get_modifier() const { return modifier; }

	/** Retrieve the name of the section header for this object */
	std::string get_section_name() const { return section_name; }

	/** Retrieve the object's type */
	std::string get_type() const { return object_type; }


/////////////////////////////////////////////////////////
/// Operations
public:
	/** Set the object's position */
	void set_pos(Vector p) { pos = p; }

	/** Set the object's resource name */
	void set_res_name(const std::string res) { res_name = res; }

	/** Set the object's modifier */
	void set_modifier(const std::string m);

	/** Set the object's section header name */
	void set_section_name(const std::string sn) { section_name = sn; }

	/** Set the object's type */
	void set_type(const std::string t) { object_type = t; }

	/** Write any additional properties to the XML file for this type */
	virtual void write_extra_properties(XMLFileWriter& xml) { }

	/** Draws the sprite with the modifier applied */
	virtual void draw(DrawingContext &gc);

	/** Default Constructor
			@param res_name The name of the resource for the main sprite */
	LevelObj(const std::string res_name);

	/** Destructor */
	virtual ~LevelObj() { }

private:
	LevelObj();
	LevelObj (const LevelObj&);
  LevelObj& operator= (const LevelObj&);

};	// LevelObj class

}		// Editor namespace
}		// Pingus namespace

#endif

/* EOF */