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

#ifndef HEADER_PINGUS_PINGUS_OPTION_MENU_HPP
#define HEADER_PINGUS_PINGUS_OPTION_MENU_HPP

#include <vector>
#include <map>
#include <boost/signals.hpp>
#include "pingus/sprite.hpp"
#include "screen/gui_screen.hpp"
#include "components/label.hpp"
#include "gui/rect_component.hpp"
#include "gui/surface_button.hpp"
#include "tinygettext/language.hpp"

class CheckBox;
class SliderBox;

/** */
class OptionMenu : public GUIScreen
{
private:
  Sprite background;
  GUI::SurfaceButton* ok_button;
  int x_pos;
  int y_pos;

  struct Option {
    Label*         label;
    GUI::RectComponent* control;

    Option(Label* label_, GUI::RectComponent* control_)
      : label(label_), control(control_)
    {}
  };

  typedef std::vector<Option> Options;
  Options options;

  CheckBox* fullscreen_box;
  CheckBox* swcursor_box;
  CheckBox* autoscroll_box;
  CheckBox* mousegrab_box;
  CheckBox* printfps_box;

  SliderBox* master_volume_box;
  SliderBox* sound_volume_box;
  SliderBox* music_volume_box;

  Label* defaults_label;
  CheckBox* defaults_box;

  typedef std::vector<boost::signals::connection> Connections;
  Connections connections;

  std::string language;
  std::map<std::string, tinygettext::Language> language_map;

public:
  OptionMenu();
  ~OptionMenu();
  
  void draw_background (DrawingContext& gc);
  void on_escape_press ();

  void add_item(const std::string& label, GUI::RectComponent* control);

  void resize(const Size&);
  void close_screen();

  void on_swcursor_change(bool v);
  void on_fullscreen_change(bool v);
  void on_autoscroll_change(bool v);
  void on_fastmode_change(bool v);
  void on_mousegrab_change(bool v);
  void on_printfps_change(bool v);

  void on_master_volume_change(int v);
  void on_sound_volume_change(int v);
  void on_music_volume_change(int v);

  void on_language_change(const std::string& str);
  void on_resolution_change(const std::string& str);

  void save_language();

private:
  OptionMenu (const OptionMenu&);
  OptionMenu& operator= (const OptionMenu&);
};

#endif

/* EOF */
