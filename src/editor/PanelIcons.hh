//  $Id: PanelIcons.hh,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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

#ifndef PANELICONS_HH
#define PANELICONS_HH

#include "Panel.hh"

class PanelIconNew : public PanelIcon
{
public:
  PanelIconNew();
  virtual void on_click();
};

class PanelIconLoad : public PanelIcon
{
public:
  PanelIconLoad();
  virtual void on_click();
};

class PanelIconSave : public PanelIcon
{
public:
  PanelIconSave();
  virtual void on_click();
};

class PanelIconCopy : public PanelIcon
{
public:
  PanelIconCopy();
  virtual void on_click();
};

class PanelIconPaste : public PanelIcon
{
public:
  PanelIconPaste();
  virtual void on_click();
};

class PanelIconEdit : public PanelIcon
{
public:
  PanelIconEdit();
  virtual void on_click();
};

class PanelIconPreferences : public PanelIcon
{
public:
  PanelIconPreferences();
  virtual void on_click();
};

class PanelIconDelete : public PanelIcon
{
public:
  PanelIconDelete();
  virtual void on_click();
};

class PanelIconExit : public PanelIcon
{
public:
  PanelIconExit();
  virtual void on_click();
};

class PanelIconRun : public PanelIcon
{
public:
  PanelIconRun();
  virtual void on_click();
};

class PanelIconInsertObject : public PanelIcon
{
public:
  PanelIconInsertObject();
  virtual void on_click();
};

#endif

/* EOF */
