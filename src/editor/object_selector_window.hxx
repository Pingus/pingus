//  $Id: object_selector_window.hxx,v 1.3 2003/04/19 10:23:18 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_WINDOW_HXX
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_WINDOW_HXX

#include <string>
#include <vector>
#include <ClanLib/GUI/window.h>
#include <ClanLib/GUI/button.h>

/** A Little window that lets you select an object type to insert */
class ObjectSelectorWindow : public CL_Window
{
private:
  typedef void (ObjectSelectorWindow::*Callback)();

  class ButtonPair {
  private:
    ObjectSelectorWindow* parent;
    Callback callback;
    CL_Button button;
    CL_Slot button_click_slot;

  public:
    ButtonPair (ObjectSelectorWindow* p, const std::string& name, Callback c, int y_pos)
      : parent (p),
        callback(c),
        button (CL_Rect(10, y_pos, 190, y_pos + 20), name, parent->get_client_area())
    {
      button_click_slot = button.sig_clicked().connect(this, &ButtonPair::on_click);
    }

    void on_click()
    {
      // Call the callback, yeah, func_ptr synaxt is cool...
      ((*parent).*callback)();
    }
  };

  /** Position for the next inserted button */
  int y_pos;

  /** container for buttons and callbacks */
  std::vector<ButtonPair*> buttons;

public:
  ObjectSelectorWindow(CL_Component*);
  ~ObjectSelectorWindow();

  void on_close_press();

  void on_groundpiece_ground_press();
  void on_groundpiece_solid_press();
  void on_groundpiece_transparent_press();
  void on_groundpiece_remove_press();
  void on_hotspot_press();
  void on_entrance_press();
  void on_exit_press();
  void on_liquid_press();
  void on_weather_press();
  void on_trap_press();
  void on_worldobject_press();
  void on_background_press();
  void on_prefab_press();
  void on_from_file_press();

private:
  void add_button (const std::string& name, Callback callback);

  ObjectSelectorWindow (const ObjectSelectorWindow&);
  ObjectSelectorWindow& operator= (const ObjectSelectorWindow&);
};

#endif

/* EOF */
