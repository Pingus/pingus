//  $Id: story.hxx,v 1.13 2003/04/06 12:40:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_STORY_HXX
#define HEADER_PINGUS_STORY_HXX

#include "story_screen.hxx"

/** */
class Story
{
private:
public:
  std::string title;
  std::vector<StoryPage> pages;

  static Story credits;
  static Story intro;

  static void init();
private:
  static void init_intro();
  static void init_credits();
};

#endif

/* EOF */
