//  $Id: FileSelector.hh,v 1.2 2000/02/11 16:58:25 grumbel Exp $
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

#ifndef FILESELECTOR_HH
#define FILESELECTOR_HH

#include <vector>
#include <string>

#include "PLFPreview.hh"
#include "PLF.hh"

struct FileSelectorEntry 
{
  std::string directory;
  PLF* plf;
  PLFPreview* preview;

  FileSelectorEntry(std::string d) {
    directory = d;
    plf = new PLF(directory);
    preview = new PLFPreview(plf);
  }
};

class FileSelector
{
private:
  CL_Font* font;
  CL_Font* font_h;
  CL_Surface* up;
  CL_Surface* down;
  CL_Surface* cursor_sur;
  int temp_mouse_x;
  int temp_mouse_y;
  bool dir_read;

  CL_Surface* bg;
  int up_x, up_y;
  int down_x, down_y;
  vector<FileSelectorEntry> entry;
  int last_update;

  typedef vector<FileSelectorEntry>::size_type index_type;

  index_type file_index;
  index_type file_offset;
  
  // FIXME: This does not work with egcs, lets try something else:
  //  static int file_npos = numeric_limits< vector< int >::size_type >::max();
#ifndef WIN32  
  static const index_type file_npos  = 2147483647;
#else
  index_type file_npos;
#endif /* !WIN32 */
  // FIXMEEND
public:
  FileSelector();
  std::string select(std::string pathname, std::string pattern);
  void draw(int highlight = 0);
  void readdir(string, string);
  std::string select_file();
};

#endif

/* EOF */
