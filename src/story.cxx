//  $Id: story.cxx,v 1.27 2003/04/21 18:07:21 torangan Exp $
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

#include <algorithm>
#include "my_gettext.hxx"
#include "story.hxx"
#include "string_format.hxx"
#include "fonts.hxx"

Story Story::credits;
Story Story::intro;

void
Story::init()
{
  init_intro();
  init_credits();
}

void
Story::init_credits()
{
  credits.title = _("The Journey Continues");
  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits1", "story"),
      StringFormat::break_line(
      _("Now after you and the Pingus have learned the basics and\n"
        "practiced a bit it is time to move on and begin the journey into\n"
        "the world. Since the flow with which the Pingus traveled to the\n"
        "Tutorial Island isn't going to hold on the whole way into the warmer\n"
        "climates the Pingus have to find something else to guide\n"
        "them on their journey.\n"), 570, Fonts::chalk_normal)
        ));

  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits2", "story"),
      StringFormat::break_line(
      _("But as the eldest have said, the Tutorial Island provides not\n"
        "only a good way to practice, but it is also the starting point into\n"
        "the world. After some searching the Pingus discovered the meaning\n"
        "of this, and they found the large tree at the end of the island\n"
        "which gave them wood to construct a float.\n"), 570, Fonts::chalk_normal)
       ));

  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits3", "story"),
      StringFormat::break_line(
      _("So the Pingus set out and constructed some large floats, enough\n"
        "to carry them all. After also packing a bunch of provisions\n"
        "they were prepared to start of their journey and leave\n"
        "their familiar ground and enter the unknown parts of the world.\n"), 570, Fonts::chalk_normal)
        ));

  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits4", "story"),
      StringFormat::break_line(
      _("So the Pingus sat on their float, worrying about what's to come and\n"
        "where to go, while floating into the sunset.\n"
        "\n"
        "To be continued..."), 570, Fonts::chalk_normal)
       ));

  std::reverse(credits.pages.begin(), credits.pages.end());
}

void
Story::init_intro()
{
  intro.title = _("The Journey Begins");
  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story0", "story"),
      StringFormat::break_line(
      _("For a long time, the Pingus have lived happily in peace on the\n"
        "South Pole along with all the other animals. Everything was in\n"
        "balance and it seemed like nothing could disrupt their peace.  The\n"
        "Pingus where happy and it seemed like this could never end.\n"), 570, Fonts::chalk_normal)
       ));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story1", "story"),
      StringFormat::break_line(
      _("But then one day, things began to change slowly, the sky got darker\n"
        "and the earth got warmer. Firstly, they thought this was just a\n"
        "normal fluctuation in the world's climate, but things seemed to get\n"
        "worse and worse with every year."), 570, Fonts::chalk_normal)
       ));


  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story2", "story"),
      StringFormat::break_line(
      _("The snow began to melt away in a few areas and food became an\n"
        "issue. Other animals tried to leave the region to search\n"
        "for colder areas, but the Pingus knew that this wouldn't help,\n"
        "they knew that they had to do something about this."), 570, Fonts::chalk_normal)
       ));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story3", "story"),
      StringFormat::break_line(
      _("So the circle of the eldest came together to decide what to do\n"
        "about it. They decided to send out an expedition around the world\n"
        "to find the cause for this warming. The expedition consisted of\n"
        "hundreds of the bravest Pingus around on the South Pole."), 570, Fonts::chalk_normal)
       ));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story4", "story"),
      StringFormat::break_line(
      _("And they picked you to lead them on their journey around the\n"
        "world. Since the journey will be dangerous and difficult, your\n"
        "first goal is the Island of Mogorok, also known as the Tutorial\n"
        "Island. According to the eldest, this island has always been the\n"
        "first stop of Pingus that were send out into the world."), 570, Fonts::chalk_normal)
       ));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story5", "story"),
      StringFormat::break_line(
      _("The island consists of many areas which are ideal to train the\n"
        "abilities of the Pingus. The Pingus can train their ability\n"
        "to build bridges, climb, bash and all the rest of their talents\n"
        "which they will need on their long and dangerous journey."), 570, Fonts::chalk_normal)
       ));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story6", "story"),
      StringFormat::break_line(
      _("You may, on the other hand, practice there your ability to\n"
        "command and guide the Pingus. You can also get familiar with\n"
        "all the abilities the Pingus provide and learn to master them,\n"
        "since you will need all of them on your journey around the\n"
        "world."), 570, Fonts::chalk_normal)
       ));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story6", "story"),
      StringFormat::break_line(
      _("Now that you and the Pingus have arrived at Tutorial Island it\n"
        "is time to take command and begin your mission.\n"
        "\nGood Luck!"), 570, Fonts::chalk_normal)
       ));

  std::reverse(intro.pages.begin(), intro.pages.end());
}

/* EOF */
