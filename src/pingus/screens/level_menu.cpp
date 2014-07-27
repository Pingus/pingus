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

#include "pingus/screens/level_menu.hpp"

#include <boost/format.hpp>

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/screens/start_screen.hpp"
#include "util/log.hpp"
#include "util/system.hpp"

class LevelMenuAbortButton : public GUI::SurfaceButton
{
private:
  LevelMenu* parent;

public:
  LevelMenuAbortButton(LevelMenu* p, int x, int y)
    : GUI::SurfaceButton(x, y,
                         "core/start/back",
                         "core/start/back_clicked",
                         "core/start/back_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, Vector2i(x_pos + 55, y_pos), _("Back"));
  }

  void on_click() {
    parent->on_escape_press();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }

private:
  LevelMenuAbortButton(const LevelMenuAbortButton&);
  LevelMenuAbortButton & operator=(const LevelMenuAbortButton&);
};

class LevelScrollButton : public GUI::SurfaceButton
{
private:
  std::function<void(void)> callback;

public:
  LevelScrollButton(int x, int y, const std::string& str, std::function<void (void)> callback_)
    : GUI::SurfaceButton(x, y,
                         str,
                         str + "_pressed",
                         str + "_hover"),
      callback(callback_)
  {
  }

  void on_click() {
    callback();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound("tick");
  }

private:
  LevelScrollButton(const LevelScrollButton&);
  LevelScrollButton & operator=(const LevelScrollButton&);
};

struct LevelsetPrioritySorter
{
  bool operator()(Levelset* lhs, Levelset* rhs) const
  {
    return lhs->get_priority() > rhs->get_priority();
  }
};

class LevelsetSelector : public GUI::RectComponent
{
private:
  LevelMenu* level_menu;
  typedef std::vector<Levelset*> Levelsets;
  Levelsets levelsets;
  Levelset* current_levelset;
  Sprite marker;
  int page;

  int  item_height;
  int  items_per_page;
  Rect list_rect;

public:
  LevelsetSelector(LevelMenu* level_menu_, const Rect& rect_) :
    RectComponent(rect_),
    level_menu(level_menu_), 
    levelsets(),
    current_levelset(),
    marker(),
    page(0),
    item_height(95),
    items_per_page(4),
    list_rect(30, 70,
              680 - 90,
              70 + items_per_page * item_height)
  {
    marker      = Sprite("core/menu/marker");
      
    auto directory = Pathname("levelsets", Pathname::DATA_PATH).opendir("*.levelset");
    for(auto i = directory.begin(); i != directory.end(); ++i)
    {
      try 
      {
        std::unique_ptr<Levelset> levelset = Levelset::from_file(*i);
        if (!levelset->get_developer_only() || globals::developer_mode)
        {
          levelsets.push_back(levelset.release());
        }
      }
      catch(const std::exception& err)
      {
        log_error("%1%", err.what());
      }
    }

    if (globals::developer_mode)
    {
      levelsets.push_back(Levelset::from_directory(_("Under Construction"),
                                                   _("Untested, unpolished and broken levels"),
                                                   "levelsets/underconstruction",
                                                   Pathname("levels", Pathname::DATA_PATH)).release());
    }

    std::sort(levelsets.begin(), levelsets.end(), LevelsetPrioritySorter());
  }

  ~LevelsetSelector() 
  {
    for(Levelsets::iterator i = levelsets.begin(); i != levelsets.end(); ++i)
    {
      delete *i;
    }
  }
  
  void draw(DrawingContext& gc)
  {
    gc.push_modelview();
    gc.translate(rect.left, rect.top);

    gc.print_center(Fonts::chalk_large, Vector2i(rect.get_width()/2, 10), _("Levelsets"));

    int y = list_rect.top;
    for(int i = page; (i < (page+items_per_page)) && (i < int(levelsets.size())); ++i)
    {
      Levelset* levelset = levelsets[i];

      if (levelset == current_levelset)
        gc.draw(marker, Vector2i(15, y - 5));

      gc.draw(levelset->get_image(), Vector2i(list_rect.left + 10, y));

      gc.print_left(Fonts::chalk_normal, Vector2i(list_rect.left + 105, 15 + y), _(levelset->get_title()));
      gc.print_left(Fonts::chalk_small,  Vector2i(list_rect.left + 105, 40 + y), _(levelset->get_description()));

      gc.print_right(Fonts::chalk_normal, Vector2i(list_rect.right, 15 + y), (boost::format("%1% %2%%%") % _("Solved:") % levelset->get_completion()).str());
      gc.print_right(Fonts::chalk_small,  Vector2i(list_rect.right, 40 + y), (boost::format("%1% %2%") % levelset->get_level_count() % _("levels")).str());

      y += item_height;
    }
    
    //int total_pages = static_cast<int>(levelsets.size());; 
    //gc.print_center(Fonts::chalk_normal, Vector2i(rect.get_width()/2, 360),
    //                (boost::format("%1% %2%/%3%") % _("Page") % (page+1) % total_pages).str());

    gc.pop_modelview();
  }

  void next_page()
  {
    page += 1;
    if (page >= (static_cast<int>(levelsets.size()) - items_per_page))
    {
      page = std::max(0, (static_cast<int>(levelsets.size())) - items_per_page);
    }
  }

  void prev_page()
  {
    page -= 1;
    if (page < 0)
      page = 0;
  }

  void on_pointer_leave()
  {
    current_levelset = 0;
  }

  void on_pointer_move(int x, int y)
  {
    x -= rect.left;
    y -= rect.top;
    
    if (!list_rect.contains(Vector2i(x, y)))
    {
      current_levelset = NULL;
    }
    else
    {
      x -= list_rect.left;
      y -= list_rect.top;

      if (!levelsets.empty())
      {
        int i = y / item_height + page;

        if (i >= 0 && i < static_cast<int>(levelsets.size()))
          current_levelset = levelsets[i];
        else
          current_levelset = NULL;
      }
    }
  }

  void on_primary_button_press (int x, int y)
  {
    on_pointer_move(x, y);

    if (current_levelset)
    {
      level_menu->set_levelset(current_levelset);
    }
  }

  void update_layout() {}

private:
  LevelsetSelector(const LevelsetSelector&);
  LevelsetSelector & operator=(const LevelsetSelector&);
};

class LevelSelector : public GUI::RectComponent
{
private:
  LevelMenu* level_menu;
  Sprite marker;

  Sprite m_checkbox_marked;
  Sprite m_checkbox_locked;
  Sprite m_checkbox;

  Levelset* levelset;
  int current_level;
  int page;

  int  item_height;
  int  items_per_page;
  Rect list_rect;

public:
  LevelSelector(LevelMenu* level_menu_, const Rect& rect_) :
    RectComponent(rect_),
    level_menu(level_menu_),
    marker(),
    m_checkbox_marked(),
    m_checkbox_locked(),
    m_checkbox(),
    levelset(0),
    current_level(-1),
    page(0),
    item_height(32),
    items_per_page(10),
    list_rect(50, 112, 680 - 90, 112 + items_per_page * item_height)
  {
    marker        = Sprite("core/menu/marker2");

    m_checkbox_marked = Sprite("core/menu/checkbox_marked_small");
    m_checkbox_locked = Sprite("core/menu/locked_small");
    m_checkbox = Sprite("core/menu/checkbox_small");
  }

  void draw(DrawingContext& gc) 
  {
    gc.push_modelview();
    gc.translate(rect.left, rect.top);

    Sprite sprite = levelset->get_image();
    gc.draw(sprite, Vector2i(rect.get_width()/2 - sprite.get_width()/2 - 275, 15));
    gc.draw(sprite, Vector2i(rect.get_width()/2 - sprite.get_width()/2 + 275, 15));

    gc.print_center(Fonts::chalk_large, Vector2i(rect.get_width()/2, 10), _(levelset->get_title()));
    gc.print_center(Fonts::chalk_normal,  Vector2i(rect.get_width()/2, 62), _(levelset->get_description()));

    if (levelset)
    {
      levelset->refresh(); // should be better placed in on_startup() or so
        
      //gc.draw_fillrect(Rect(Vector2i(0,0), Size(rect.get_width(), rect.get_height())),
      //                 Color(255, 255, 0, 100));

      //gc.print_left(Fonts::chalk_normal,  Vector2i(30, -32), _("Title"));
      //gc.print_right(Fonts::chalk_normal, Vector2i(rect.get_width() - 30, -32), _("Status"));

      int y = list_rect.top;
      for(int i = page; i < (page + items_per_page) && i < levelset->get_level_count(); ++i)
      {
        // draw background highlight mark
        if (levelset->get_level(i)->accessible && i == current_level)
        {
          gc.draw(marker, Vector2i(20, y));
        }

        // draw levelname
        if (globals::developer_mode)
        {
          gc.print_left(Fonts::chalk_normal, Vector2i(list_rect.left + 40, y+4), levelset->get_level(i)->plf.get_resname());
        }
        else
        {
          gc.print_left(Fonts::chalk_normal, Vector2i(list_rect.left + 40, y+4), _(levelset->get_level(i)->plf.get_levelname()));
        }

        // draw icon
        if (!levelset->get_level(i)->accessible)
        {
          gc.draw(m_checkbox_locked, Vector2i(list_rect.left + 0, y));
        }
        else if (levelset->get_level(i)->finished)
        {
          gc.draw(m_checkbox_marked, Vector2i(list_rect.left + 0, y));
        }
        else
        {
          gc.draw(m_checkbox, Vector2i(list_rect.left + 0, y));
        }

        y += item_height;
      }
    }

    gc.pop_modelview();
  }

  void prev_page()
  {
    page -= 1;
    if (page < 0)
      page = 0;
  }

  void next_page()
  {
    page += 1;
    if (page >= (levelset->get_level_count() - items_per_page))
    {
      page = std::max(0, (levelset->get_level_count()) - items_per_page);
    }
  }

  void set_levelset(Levelset* levelset_)
  {
    page = 0;
    levelset = levelset_;
  }

  void on_pointer_leave()
  {
    current_level = -1;
  }

  int get_current_level(int x, int y)
  {
    int cl = current_level = y / 32 + page;
    if (cl < 0 || cl >= levelset->get_level_count())
      cl = -1;
    return cl;
  }

  void on_pointer_move(int x, int y)
  {
    x -= rect.left;
    y -= rect.top;

    if (!list_rect.contains(Vector2i(x, y)))
    {
      current_level = -1;
    }
    else
    {
      x -= list_rect.left;
      y -= list_rect.top;

      current_level = y / item_height + page;

      if (current_level < 0 ||
          current_level >= levelset->get_level_count())
      {
        current_level = -1;
      }
    }
  }

  void on_primary_button_press (int x, int y)
  {
    on_pointer_move(x, y);
    if (current_level != -1)
    {
      if (levelset->get_level(current_level)->accessible)
      {
        ScreenManager::instance()->push_screen(std::make_shared<StartScreen>(levelset->get_level(current_level)->plf));
      }
    }
  }

  void update_layout() {}

private:
  LevelSelector(const LevelSelector&);
  LevelSelector & operator=(const LevelSelector&);
};

LevelMenu::LevelMenu() :
  x_pos((Display::get_width()  - globals::default_screen_width)/2),
  y_pos((Display::get_height() - globals::default_screen_height)/2),
  background("core/menu/wood"),
  blackboard("core/menu/blackboard"),
  ok_button(),
  level_selector(),
  levelset_selector(),
  abort_button(),
  next_button(),
  prev_button()
{
  ok_button  = Sprite("core/start/ok");

  levelset_selector = new LevelsetSelector(this, Rect());
  level_selector    = new LevelSelector(this, Rect());

  gui_manager->add(levelset_selector);
  gui_manager->add(level_selector);

  gui_manager->add(prev_button = new LevelScrollButton(Display::get_width()/2  + 280,
                                                       Display::get_height()/2 - 150,
                                                       "core/menu/arrow_up",
                                                       std::bind(&LevelMenu::prev_page, this)));

  gui_manager->add(next_button = new LevelScrollButton(Display::get_width()/2  + 280,
                                                       Display::get_height()/2 + 70,
                                                       "core/menu/arrow_down",
                                                       std::bind(&LevelMenu::next_page, this)));

  gui_manager->add(abort_button = new LevelMenuAbortButton(this, 
                                                           Display::get_width()/2 - 300,
                                                           Display::get_height()/2 + 144));

  level_selector->hide();
  resize(Display::get_size());
}

LevelMenu::~LevelMenu()
{
}

void
LevelMenu::draw_background(DrawingContext& gc)
{
  // Paint the background wood panel
  for(int y = 0; y < gc.get_height(); y += background.get_height())
    for(int x = 0; x < gc.get_width(); x += background.get_width())
      gc.draw(background, Vector2i(x, y));

  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));
}

void
LevelMenu::on_escape_press()
{
  if (level_selector->is_visible())
  {
    levelset_selector->show();
    level_selector->hide();           
  }
  else
  {
    //log_debug("OptionMenu: poping screen");
    ScreenManager::instance()->pop_screen();
  }
}

void
LevelMenu::on_action_up_press()
{
  if (level_selector->is_visible())
    level_selector->prev_page();
  else
    levelset_selector->prev_page();
}

void
LevelMenu::on_action_down_press()
{
  if (level_selector->is_visible())
    level_selector->next_page();
  else
    levelset_selector->next_page();
}

void
LevelMenu::next_page()
{
  if (level_selector->is_visible())
    level_selector->next_page();
  else
    levelset_selector->next_page();  
}

void
LevelMenu::prev_page()
{
  if (level_selector->is_visible())
    level_selector->prev_page();
  else
    levelset_selector->prev_page();
}

void
LevelMenu::set_levelset(Levelset* levelset)
{
  if (levelset)
  {
    level_selector->set_levelset(levelset);
    levelset_selector->hide();
    level_selector->show();
  }
  else
  {
    levelset_selector->show();
    level_selector->hide();      
  }
}

void
LevelMenu::resize(const Size& size_)
{
  GUIScreen::resize(size_);

  x_pos = (size.width  - globals::default_screen_width)/2;
  y_pos = (size.height - globals::default_screen_height)/2;

  levelset_selector->set_rect(Rect(Vector2i(x_pos + 60, y_pos + 50), Size(680, 500)));
  level_selector   ->set_rect(Rect(Vector2i(x_pos + 60, y_pos + 50), Size(680, 500)));

  prev_button->set_pos(size.width/2  + 280, size.height/2 - 48 - 12);
  next_button->set_pos(size.width/2  + 280, size.height/2 + 12);

  abort_button->set_pos(size.width /2 - 300,
                        size.height/2 + 200);
}

/* EOF */
