//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "editor/action_properties.hpp"

#include <iostream>
#include <boost/bind.hpp>

#include "editor/checkbox.hpp"
#include "editor/editor_level.hpp"
#include "editor/gui_style.hpp"
#include "editor/inputbox.hpp"
#include "pingus/pingu_enums.hpp"
#include "util/string_util.hpp"

namespace Editor {

/*
 *
 * ActionDialog: 
 * [active] [Actionname] [count]
 *
 */
ActionProperties::ActionProperties(EditorScreen* editor_, const Rect& rect_) :
  GroupComponent(rect_),
  editor(editor_),
  level(),
  y_pos(0),
  action_comps()
{
  add_action(Actions::BASHER);
  add_action(Actions::BLOCKER);
  add_action(Actions::BOMBER);
  add_action(Actions::BRIDGER);
  add_action(Actions::CLIMBER);
  add_action(Actions::DIGGER);
  add_action(Actions::FLOATER);
  add_action(Actions::JUMPER);
  add_action(Actions::MINER);
  add_action(Actions::SLIDER);
}

ActionProperties::~ActionProperties()
{
}

void
ActionProperties::draw_background(DrawingContext& gc)
{
  GUIStyle::draw_raised_box(gc, Rect(0,0,rect.get_width(), rect.get_height()));
}

void
ActionProperties::update (float delta)
{
  
}

void
ActionProperties::add_action(Actions::ActionName id)
{
  ActionComponent comp;
  comp.checkbox = new Checkbox(Rect(Vector2i(10,10 + y_pos), Size(80,20)), Actions::action_to_screenname(id));
  comp.inputbox = new Inputbox(Rect(Vector2i(100,10 + y_pos), Size(40,20)));

  comp.checkbox->set_checked(true);
  comp.inputbox->set_text("20");
 
  add(comp.checkbox);
  add(comp.inputbox);
  
  action_comps[id] = comp;

  comp.checkbox->on_change.connect(boost::bind(&ActionProperties::on_checkbox_change, this, _1, id));
  comp.inputbox->on_change.connect(boost::bind(&ActionProperties::on_inputbox_change, this, _1, id));

  y_pos += 22;
}

void
ActionProperties::set_level(EditorLevel* level_)
{
  level = level_;

  for(ActionComponents::iterator i = action_comps.begin(); i != action_comps.end(); ++i)
    {
      i->second.inputbox->set_text("20");
      i->second.checkbox->set_checked(false);
    }

  std::map<std::string, int> actions = level->get_actions();
  for(std::map<std::string, int>::iterator i = actions.begin(); i != actions.end(); ++i)
    {
      ActionComponents::iterator j = action_comps.find(Actions::action_from_string(i->first));
      if (j != action_comps.end())
        {
          j->second.inputbox->set_text(StringUtil::to_string(i->second));
          j->second.checkbox->set_checked(true);
        }
    }
}

void
ActionProperties::on_checkbox_change(bool t, Actions::ActionName id)
{
  if (t)
    {
      level->set_action(Actions::action_to_string(id), 
                        StringUtil::to<int>(action_comps[id].inputbox->get_text()));
    }
  else
    {
      level->set_action(Actions::action_to_string(id), 0);
    }
}

void
ActionProperties::on_inputbox_change(const std::string& value, Actions::ActionName id)
{
  level->set_action(Actions::action_to_string(id), StringUtil::to<int>(value));
}


} // namespace Editor

/* EOF */
