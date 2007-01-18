//  $Id: resource_modifier.hxx,v 1.6 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_RESOURCE_MODIFIER_HXX
#define HEADER_PINGUS_RESOURCE_MODIFIER_HXX

#include "pingus.hxx"
#include <string>

namespace ResourceModifierNS {

/** This array contains possible modifications of a surface */
typedef enum {
  ROT0,
  ROT90,
  ROT180,
  ROT270,
  ROT0FLIP,
  ROT90FLIP,
  ROT180FLIP,
  ROT270FLIP
} ResourceModifier;

ResourceModifier horizontal_flip (ResourceModifier);
ResourceModifier vertical_flip (ResourceModifier);
ResourceModifier rotate_90 (ResourceModifier);
ResourceModifier rotate_270 (ResourceModifier);

// we don't specialize the CL_String::to templates here, since the enum
// might get confused with 'int'
std::string rs_to_string (ResourceModifier);
ResourceModifier rs_from_string (const std::string&);

} // namespace ResourceModifierNS

#endif

/* EOF */
