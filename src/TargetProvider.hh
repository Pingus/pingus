//  $Id: TargetProvider.hh,v 1.1 2000/09/25 16:23:09 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef TARGETPROVIDER_HH
#define TARGETPROVIDER_HH

#include <ClanLib/core.h>

class TargetProvider : public CL_SurfaceProvider
{
private:
  CL_Target* target;
  TargetProvider () {}

public:
  TargetProvider (CL_Target* t) { target = t; }
  virtual ~TargetProvider () {}

  void lock () { target->lock(); }
  void unlock () { target->unlock(); }
  void* get_data () const { target->get_data (); }
  bool  is_indexed () const { target->is_indexed (); }
  unsigned int get_num_frames () const { target->get_num_frames (); }
  unsigned int get_width () const { target->get_width (); }
  unsigned int get_height () const { target->get_height (); }
  unsigned int get_pitch () const { target->get_pitch (); }
  unsigned int get_red_mask () const { target->get_red_mask (); }
  unsigned int get_green_mask () const { target->get_green_mask (); }
  unsigned int get_blue_mask () const { target->get_blue_mask (); }
  unsigned int get_alpha_mask () const { target->get_alpha_mask (); }
  CL_Palette*  get_palette () const { target->get_palette (); }
  bool uses_src_colorkey() const { return false; }
  unsigned int get_src_colorkey () const { return 0; }
  
  static CL_Canvas* create_canvas (CL_Target*);
};

#endif

/* EOF */
