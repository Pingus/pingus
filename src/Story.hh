//  $Id: Story.hh,v 1.1 2000/06/18 22:19:48 grumbel Exp $
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

#ifndef STORY_HH
#define STORY_HH

/** This class displays the story of the Pingus. */
class Story
{
private:
  ///
  bool   is_init;
  ///
  CL_Surface* background;
  ///
  CL_Font*  large_font;
  ///
  CL_Font*  small_font;

public:
  ///
  Story();
  ///
  ~Story();

  /// Load all surfaces into memory.
  void init();

  /// Display the story.
  void display();

  /// Display a string at the right position, thread line breaks.
  void display_string(std::string);
};

///
extern Story pingus_story;

#endif

/* EOF */
