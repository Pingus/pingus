//  $Id: story.cxx,v 1.21 2003/04/08 19:56:40 torangan Exp $
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
      _("Now after you and the Pingus have learned the basics and\n"
        "practiced a bit it is time to move on and begin the journey into\n"
        "the world. Since the floe with which the Pingus traveled to the\n"
        "Tutorial Island isn't going to hold the whole way into the warmer\n"
        "climated regions the Pingus have to find something else to guide\n"
        "there on their journey.\n")));

  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits2", "story"), 
      _("But as the eldest have said, the Tutorial Island provides not\n"
        "only a way to practive, but it is also the starting point into\n"
        "the world, cause the wood of the large tree at the end of the\n"
        "island brings good wood to construct a float.\n")));
     
  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits3", "story"), 
      _("So the Pingus set out and constructed a some large floats, enough\n"
        "to carry them all. After also packaging up a bunch of provisions\n"
        "they where prepared to finally start of their journey and leave\n"
        "their familiar ground and entering the unknown parts of the\n"
        "world.\n")));
     
  credits.pages.push_back
    (StoryPage
     (ResDescriptor("Story/credits4", "story"), 
      _("So the Pingus sat on their float, worrying whats to come and\n"
        "where to go, while floating into the sunset.\n"
        "\n"
        "To be continued...")));

  std::reverse(credits.pages.begin(), credits.pages.end());
}

void
Story::init_intro() 
{   
  intro.title = _("The Journey Begins");
  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story0", "story"), 
      _("For a long time now the Pingus have lived happily in peace on the\n"
        "south pole along with all the other animals, everything was in\n"
        "balance and it seemed like nothing could disrupt their peace.  The\n"
        "Pingus where happy and it seemed like this could never end.\n")));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story1", "story"), 
      _("But then one day, things began to change slowly, the sky got darker\n"
        "and the earth got warmer, first of they thought this was just a\n"
        "normal fluctuation in the worlds climate, but things seemed to get\n"
        "worse and worse with every year.")));


  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story2", "story"), 
      _("The snow began to melt away in a few regions and food became an\n"
        "issue. Other animals already tend to leave the region to search\n"
        "for colder areas, but the Pingus knew that this couldn't help,\n"
        "they knew that they had to do something about this problem.")));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story3", "story"), 
      _("So the circle of the eldest came together to decide what to do\n"
        "about it. They decided to send out an expedition around the world\n"
        "to find the cause for this warming. The expedition consisted of\n"
        "hundreds of the bravest Pingus around on the south pole.")));
    
  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story4", "story"), 
      _("And they picked you to lead them on their journey around the\n"
        "world. Since the journey will be dangerous and difficult, your\n"
        "first goal is the Island of Mogorok, also know has Tutorial\n"
        "Island. According to the eldest, this island has always been the\n"
        "first stop of Pingus which where send out into the world.")));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story5", "story"), 
      _("The island consists of many regions which are ideally to train the\n"
        "abilities of the Pingus. The Pingus can train there their ability\n"
        "to build bridges, climb, bash and all the rest of their talents\n"
        "which they will surly need on their long and dangerous journey.")));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story6", "story"), 
      _("While you on the other side can practice there your ability to\n"
        "command and guide the Pingus. You can also get familiar there with\n"
        "all the abilities the Pingus provide and learn to master them,\n"
        "since you will need them all for sure an your journey around the\n"
        "world.")));

  intro.pages.push_back
    (StoryPage
     (ResDescriptor("Story/story6", "story"), 
      _("Now that you and the Pingus have arrived at tutorial island it\n"
        "is time to take over the command and begin your mission.\n"
        "\n"
        "                                                  Good Luck!")));

  std::reverse(intro.pages.begin(), intro.pages.end());
}

/* EOF */
