//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "fonts.hpp"
#include "gui/display.hpp" 
#include "display/drawing_context.hpp"
#include "resource.hpp"
#include "sprite.hpp"
#include "gettext.h"
#include "math/vector2i.hpp"
#include "editor_screen.hpp"
#include "gui/gui_manager.hpp"
#include "panel.hpp"

namespace Editor {

class PanelSeparator : public GUI::Component
{
private:
  Sprite sprite;
  Vector2i pos;

public:
  PanelSeparator(const Vector2i& pos_)
    : sprite(Resource::load_sprite("core/editor/separator")),
      pos(pos_)
  {
  }
  
  void draw (DrawingContext& gc)
  {
    gc.draw(sprite, pos);
  } 
  
  int get_width() const { return sprite.get_width(); }
};

class PanelButton : public GUI::Component
{
private:
  EditorScreen* editor;

  Sprite button_raised;
  Sprite button_pressed;
  Sprite sprite;
  bool   mouse_over;
  bool   mouse_down;
  Vector2i    pos;
  std::string tooltip;

  typedef void (EditorScreen::*Callback)();
  Callback callback;

public:
  PanelButton(EditorScreen* editor_, 
              const Vector2i& pos_, const std::string& name, const std::string& tooltip_, 
              Callback callback_ = 0)
    : editor(editor_),
      button_raised(Resource::load_sprite("core/editor/button-raised")),
      button_pressed(Resource::load_sprite("core/editor/button-pressed")),
      sprite(Resource::load_sprite(name)),
      mouse_over(false),
      mouse_down(false),
      pos(pos_),
      tooltip(tooltip_),
      callback(callback_)
  {
  }

  void draw (DrawingContext& gc)
  {
    if (mouse_down)
      gc.draw(button_pressed, pos);
    else if (mouse_over)
      gc.draw(button_raised, pos);

    gc.draw(sprite, pos + Vector2i(5,5));

    if (mouse_over)
      {
        int t_w = Fonts::verdana11.get_width(tooltip);
        Rect t_r(pos.x + 17 - t_w/2 - 4, pos.y + 38 - 2, 
                 pos.x + 17 + t_w/2 + 4, pos.y + 38 + Fonts::verdana11.get_height() + 2);
        gc.draw_fillrect(t_r, Color(255, 255, 200), 1000.0f);
        gc.draw_rect(t_r, Color(0,0,0), 1000.0f);
        gc.print_center(Fonts::verdana11, pos.x + 17, pos.y + 38, tooltip, 1000.0f);
      }
  }

  /** Emmitted when pointer enters the region of the component */
  void on_pointer_enter () 
  {
    mouse_over = true;
  }

  /** Emmitted when pointer leaves the region of the component */
  void on_pointer_leave () 
  {
    mouse_over = false;
  }
  
  void on_primary_button_press (int x, int y) 
  {
    mouse_down = true;
  }

  void on_primary_button_release (int x, int y) 
  { 
    mouse_down = false;
    if (mouse_over && callback)
      ((*editor).*callback)();
  }
  
  bool is_at(int x, int y)
  {
    return (pos.x <= x && pos.x + 34 > x &&
            pos.y <= y && pos.y + 34 > y);
  }

  void update (float delta)
  {
    sprite.update();
  }
  
  int get_width() const {
    return 34;
  }
};

Panel::Panel(EditorScreen* editor_)
  : editor(editor_),
    logo(Resource::load_sprite("core/editor/logo")),
    pos(2,2)
{  
  editor->get_gui_manager()->add(this, true);

  add_button("core/editor/document-new",  _("    New level"),
             &EditorScreen::level_new);
  add_button("core/editor/document-open", _("Open level..."),
             &EditorScreen::level_load);
  add_button("core/editor/document-save", _("Save level..."), 
             &EditorScreen::level_save);
  add_button("core/editor/document-save-as", _("Save level as..."), 
             &EditorScreen::level_save_as);
  add_separator();
  add_button("core/editor/play", _("Play level..."), 
             &EditorScreen::level_play);
  add_separator();
  add_button("core/editor/actions", _("Configure actions"), 
             &EditorScreen::toggle_action_properties);
  add_button("core/editor/document-properties", _("Configure level"), 
             &EditorScreen::toggle_level_properties);
  add_button("core/editor/object-properties", _("Display object properties"), 
             &EditorScreen::toggle_object_properties);
  add_button("core/editor/objects", _("Show object insertion window"), 
             &EditorScreen::toggle_object_selector);
  add_button("core/editor/minimap", _("Show minimap"), 
             &EditorScreen::toggle_minimap);
  add_separator();
  add_button("core/editor/copy", _("Duplicate the selected objects") + " (D)",  
             &EditorScreen::objects_duplicate);
  add_button("core/editor/delete", _("Delete the selected objects") + " (Delete)",  
             &EditorScreen::objects_delete);
  add_separator();
  add_button("core/editor/object-top",    _("Raise object to top") + " (Shift+])", 
             &EditorScreen::objects_raise_to_top);
  add_button("core/editor/object-up",     _("Raise object") + " (])", 
             &EditorScreen::objects_raise);
  add_button("core/editor/object-down",   _("Lower object") + " ([)", 
             &EditorScreen::objects_lower);
  add_button("core/editor/object-bottom", _("Lower object to bottom") + " (Shift+[)", 
             &EditorScreen::objects_lower_to_bottom);

  if (0)
    { // rotate and flip are now in the object properties
      add_separator();
      add_button("core/editor/object-flip-horizontal", _("Flip object horizontally") + " (F)", 
                 &EditorScreen::objects_flip_horizontal);
      add_button("core/editor/object-flip-vertical", _("Flip object vertically") + " (Shift+F)", 
                 &EditorScreen::objects_flip_vertical);
      add_separator();
      add_button("core/editor/object-rotate-left",  _("Rotate object -90 degree") + " (Shift+R)", 
                 &EditorScreen::objects_rotate_left);
      add_button("core/editor/object-rotate-right", _("Rotate object 90 degree") + " (R)", 
                 &EditorScreen::objects_rotate_right);
    }

  // add_separator();
  //add_button("core/editor/snap-grid", "Snap objects to grid", 
  //             &EditorScreen::toggle_grid_snap);
  // add_separator();
  add_separator();
  add_button("core/editor/help", _("Display help"), 
             &EditorScreen::toggle_help);
  add_button("core/editor/exit", _("Exit"), 
             &EditorScreen::exit);
}

Panel::~Panel()
{
}

void
Panel::draw (DrawingContext& gc)
{
  // FIXME: Could use draw_line
  gc.draw_fillrect(0, 0, Display::get_width(), 38, Color(255, 255, 255));
  gc.draw_fillrect(1, 1, Display::get_width(), 38, Color(169, 157, 140));
  gc.draw_fillrect(1, 1, Display::get_width()-1, 37, Color(237, 233, 227));
}

void
Panel::update (float delta)
{
  
}

void
Panel::add_button(const std::string& image, const std::string& tooltip, Callback callback)
{
  PanelButton* comp = new PanelButton(editor, pos, image, tooltip, callback);
  pos.x += comp->get_width();
  editor->get_gui_manager()->add(comp, true);
}

void
Panel::add_toggle_button(const std::string& image)
{
  PanelButton* comp = new PanelButton(editor, pos, image, "");
  pos.x += comp->get_width();
  editor->get_gui_manager()->add(comp, true);
}

void
Panel::add_separator()
{
  PanelSeparator* comp = new PanelSeparator(pos);
  pos.x += comp->get_width();
  editor->get_gui_manager()->add(comp, true);
}

} // namespace Editor

/* EOF */
