//  $Id: FileSelector.cc,v 1.4 2000/06/27 16:05:16 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

// FIXME: The FileSelector is ugly and unusable, it should be rewritten.

#include <sys/types.h>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <ClanLib/clanlib.h>

#ifndef WIN32
#include <dirent.h>
#include <fnmatch.h>
#else /* !WIN32 */
#include <windows.h>
#endif /* !WIN32 */

#include "globals.hh"
#include "QuestionDialog.hh"
#include "PingusError.hh"
#include "PingusResource.hh"
#include "PingusMessageBox.hh"
#include "View.hh"

#include "FileSelector.hh"

// Define some macros to make the code more readable
#define FS_START  (20)
#define FS_END    (CL_Display::get_height() - 20)
#define MAX_ITEMS ((FS_END - FS_START) / 20)
#define FS_BAR_START (int(FS_START + 2 + double(FS_END - FS_START - 4) / entry.size() * file_offset))
#define FS_BAR_MAX_LENGTH (FS_END - FS_START - 4)
#define FS_BAR_LENGTH (int)((double)FS_BAR_MAX_LENGTH / entry.size() * (file_offset + MAX_ITEMS))
//#define FS_BAR_END (FS_BAR_START + ((FS_BAR_LENGTH > FS_BAR_MAX_LENGTH) ? (FS_BAR_MAX_LENGTH): FS_BAR_LENGTH))
//#define FS_BAR_END (FS_BAR_START + (FS_BAR_LENGTH > FS_BAR_MAX_LENGTH ? FS_BAR_MAX_LENGTH: FS_BAR_LENGTH))

//#define FS_BAR_END (5 ? 2 : 2)

// Little helper class for sort()
struct Entry_less : public binary_function<FileSelectorEntry, FileSelectorEntry, bool>
{
  bool operator() (FileSelectorEntry a, FileSelectorEntry b) const
    {
      return a.directory < b.directory;
    }
};

void
display_string(CL_Font* font, int x, int y, std::string str)
{
  //vector<std::string> strs;
  string::size_type pos = pos = str.find("\n", 0);
  
  for(int i = 0;; pos = str.find("\n", 0), ++i) {
    font->print_left(x, y + (20*i), str.substr(0, pos).c_str());
    //cout << "Pos: " << pos << "Str: " << str.substr(0, pos) << std::endl;
    if (pos == string::npos) break;
    if (pos >= str.size()) break;
    str = str.substr(pos + 1);
  }
}
			    
FileSelector::FileSelector()
{
#ifdef WIN32
  file_npos = -1;
#endif
  std::cout << "File npos: " << file_npos << std::endl;
  if (verbose > 1) std::cout << "Construction FileSelector" << std::endl;
  font_h = CL_Res_Font::load("Fonts/smallfont", *fonts");
  font   = CL_Res_Font::load("Fonts/smallfont_h",*fonts");  
  up     = CL_Res_Surface::load("Buttons/up", *global");
  down   = CL_Res_Surface::load("Buttons/down", *global");
  cursor_sur = CL_Res_Surface::load("Cursors/cursor", *game");
  bg = CL_Res_Surface::load("Textures/stone", *global");

  file_index = 0;
  file_offset = 0;

  dir_read = false;
  down_x = 320 - (up->get_width()/2);
  down_y = 320;
  up_x = 320 - (up->get_width()/2);
  up_y = 100;
  last_update = CL_System::get_time();
}

string
FileSelector::select(std::string pathname, std::string pattern)
{
  std::string filename;

  CL_Display::sync_buffers();

  FileSelector::readdir(pathname, pattern);

  filename = FileSelector::select_file();

  return filename;
}

string
FileSelector::select_file(void)
{
  index_type temp_index = file_npos;

  while(true) {
    try {
      CL_System::keep_alive();
      
      if (cursor_enabled && (temp_mouse_x != CL_Mouse::get_x() 
			     || temp_mouse_y != CL_Mouse::get_y()))
	{
	  temp_mouse_x = CL_Mouse::get_x();
	  temp_mouse_y = CL_Mouse::get_y();
	  draw(file_index);
	}

      if (CL_Keyboard::get_keycode(CL_KEY_UP)) {
	--file_index;
      } else if (CL_Keyboard::get_keycode(CL_KEY_DOWN)) {
	++file_index;
      }

      if (CL_Keyboard::get_keycode(CL_KEY_ESCAPE)
	  || CL_Mouse::middle_pressed()) 
	{
	  while(CL_Keyboard::get_keycode(CL_KEY_ESCAPE)
		|| CL_Mouse::middle_pressed())
	    CL_System::keep_alive();
	  return "";
	}
      
      if ((CL_Mouse::left_pressed() && file_index != file_npos
	   && CL_Mouse::get_x() > 590 && CL_Mouse::get_x() < 630
	   && CL_Mouse::get_y() > 200 && CL_Mouse::get_y() < 220) 
	  || CL_Keyboard::get_keycode(CL_KEY_ENTER))
	{
	  return entry[file_index].directory;
	}

      // Get the selected item
      if (CL_Mouse::get_x() < 255 && CL_Mouse::left_pressed()) {
	index_type temp_index;
	temp_index = (CL_Mouse::get_y() - FS_START - 10) / 20 + file_offset;

	if (temp_index >= file_offset && file_index < file_offset + MAX_ITEMS
	    && file_index < entry.size()) {
	  file_index = temp_index;
	}
      }
    
      // If something as changed and some milisec have passed update the
      // screen 
      if (temp_index != file_index) {
	draw(file_index);
	temp_index = file_index;
      }
    }
    catch (CL_Error err) {
      std::string str = "CL_Error: ";
      str += err.message.get_string();
      
      PingusMessageBox box(str);
      file_index = file_npos;
    }

    catch (PingusError err) {
      std::string str = "PingusError: ";
      str += err.message;
      PingusMessageBox box(str);
      file_index = file_npos;
    }
    
    catch (...) {
      PingusMessageBox("Some unknown error, this is a bug!");
      file_index = file_npos;
    }
  }

  return entry[file_index].directory;
}

void
FileSelector::readdir(std::string path, std::string pattern)
{
  if (dir_read) {
    std::cout << "FileSelector: Dir allready read, skipping" << std::endl;
    return;
  }
  std::cout << "FileSelector: Reading directory" << std::endl;

#ifndef WIN32
  DIR* dp;
  dirent* de;
      
  std::string pathname;
  string::size_type pos = 0; 
  string::size_type last_pos = 0; 
  bool exit_for = false;
  
  for(pos = path.find(":", last_pos);
      !exit_for;
      last_pos = pos + 1, pos = path.find(":", last_pos))
    {
      if (pos == string::npos) {
	pathname = path.substr(last_pos, string::npos);
	exit_for = true;
      } else {
	pathname = path.substr(last_pos, pos - last_pos);
      }

      pathname += "/levels";

      // After we have the directory name, open it and read its
      // content
      dp = opendir(pathname.c_str());
    
      if (!dp) {
	cout << "Warrning: Couldn't open directory: " << pathname << std::endl;
      } else {
	while ((de = ::readdir(dp)) != NULL) {
	  if (fnmatch(pattern.c_str(), de->d_name, FNM_PATHNAME) == 0) {
	    try { 
	      entry.push_back(FileSelectorEntry(pathname + "/" + de->d_name));
	    }
	    catch (PingusError err) {
	      std::cout << "Warning: " << err.message << std::endl;
	    }
	  }
	} // BUG, errors aren't checked
	closedir(dp);
      }
    }
#else /* !WIN32 */
  WIN32_FIND_DATA coFindData;
  std::string FindFileDir = pingus_datadir + "levels\\*.plf";
  std::string LevelLocation;
  HANDLE hFind = FindFirstFile(TEXT(FindFileDir.c_str()),&coFindData);

  if (hFind == INVALID_HANDLE_VALUE)
  {
     //BUG, errors aren't supported :)
    std::cout << "Error: No Files found";
  }

  do
  {     
	 LevelLocation = "data\\levels\\";
	 LevelLocation += coFindData.cFileName;	 
	 entry.push_back(FileSelectorEntry(LevelLocation));
  }
  while (FindNextFile(hFind,&coFindData));

  FindClose(hFind);  
#endif
  if (fs_preload) {
    std::cout << "Preloading level preview images" << std::endl;
    std::cout << "That could take a while, sorry." << std::endl;
    for (index_type i=0; i < entry.size(); ++i) {
      entry[i].preview->load();
      std::cout << i * 100 / (entry.size()-1) << "% done\r" << std::flush;
    }
  }

  // Sort the directory list
  sort(entry.begin(), entry.end(), Entry_less());
  dir_read = true;
}

void
FileSelector::draw(int highlight)
{
  CL_Display::clear_display();

  for(int y = 0; y < CL_Display::get_height(); y += bg->get_height()) {
    for(int x = 0; x < CL_Display::get_width(); x += bg->get_width()) {
      bg->put_screen(x, y);
    }
  }

  CL_Display::fill_rect(10, FS_START, 250, FS_END, 0.0, 0.0, 0.0, 0.5);

  for(index_type i=file_offset; (i + 1) * 20 < FS_END && i < entry.size(); ++i) {
    if (file_index == i) {
      CL_Display::fill_rect(15,
			    20*(i-file_offset) + FS_START + 5,
			    240, 
			    20*(i-file_offset)  + FS_START + font_h->get_height() + 15,
			    1.0, 1.0, 1.0, 0.5);
      font_h->print_left(20,20*(i-file_offset) + FS_START + 10, entry[i].plf->get_levelname().c_str());
      std::cout << "Levelfile: " << entry[i].directory.c_str() << std::endl;
    } else {
      font->print_left(20,20*(i-file_offset) + FS_START + 10, entry[i].plf->get_levelname().c_str());
    }
  }

  int fs_bar_end;

  if (FS_BAR_LENGTH > FS_BAR_MAX_LENGTH) {
    fs_bar_end = FS_START + FS_BAR_MAX_LENGTH;
  } else {
    fs_bar_end = FS_START + FS_BAR_LENGTH;
  }
  
  CL_Display::fill_rect(252, FS_START, 262, FS_END, 0.0, 0.0, 0.0, 0.5);
  CL_Display::fill_rect(254, FS_BAR_START,
			260, fs_bar_end, 1.0, 1.0, 1.0, 0.5);

  CL_Display::fill_rect(280, 20, 630, 220, 0.0, 0.0, 0.0, 0.5);

  // The play button
  CL_Display::fill_rect(590, 200, 630, 220, 1.0, 1.0, 1.0, 0.9);
  font_h->print_left(600, 205,"Play");

  if ((file_index != file_npos) && (file_index < entry.size())) {
    font->print_left(300, 30, entry[file_index].plf->get_levelname().c_str());
    display_string(font, 310, 60, entry[file_index].plf->get_description());

    if (previews_enabled) {
      // Draw the level preview and load it if not allready done
      if (entry[file_index].preview->get_surface() == 0) {
	CL_Display::fill_rect(280, 230, 630, 460, 0.0, 0.0, 0.0, 0.5);
	font->print_center(450, 320, "Loading...");
	Display::flip_display();
	CL_Display::sync_buffers();
	entry[file_index].preview->load();
	// draw(highlight);
      } 
      CL_Display::fill_rect(280, 230, 630, 460, 0.0, 0.0, 1.0, 1.0);
      entry[file_index].preview->get_surface()->put_screen(280, 230, 630-280, 460-230);
    }

    if (cursor_enabled)
      cursor_sur->put_screen(CL_Mouse::get_x(), CL_Mouse::get_y());      
    Display::flip_display();
  }
}

/* EOF */
