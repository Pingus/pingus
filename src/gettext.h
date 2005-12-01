//  $Id: tinygettext.cpp,v 1.4 2004/11/25 13:15:56 matzebraun Exp $
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_GETTEXT_HXX
#define HEADER_GETTEXT_HXX

#include "tinygettext/tinygettext.hxx"

namespace Pingus {
extern TinyGetText::DictionaryManager dictionary_manager;
}

#define _(String)        ::Pingus::dictionary_manager.get_dictionary().translate(String).c_str()
#define N_(id, id2, num) ::Pingus::dictionary_manager.get_dictionary().translate(id, di2, num).c_str()

#endif /* HEADER_GETTEXT_HXX */
