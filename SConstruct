##  -*- python -*-
##  $Id: pingus_main.hxx,v 1.14 2003/10/18 12:11:30 grumbel Exp $
##
##  Pingus - A free Lemmings clone
##  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
##
##  This program is free software; you can redistribute it and/or
##  modify it under the terms of the GNU General Public License
##  as published by the Free Software Foundation; either version 2
##  of the License, or (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, write to the Free Software
##  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

import sys, os

opts = Options(['options.cache', 'custom.py'])
opts.Add('CXX', 'The C++ compiler')
opts.Add('PKG_CONFIG_PATH', 'Path for pkg-config files')

env = Environment(options=opts)

def CheckPKG_CONFIG_DIR(context):
    context.Message( 'Checking for PKG_CONFIG_PATH environment variable... ')
    if os.environ.has_key('PKG_CONFIG_PATH'):
        context.Result(os.environ['PKG_CONFIG_PATH'])
        return 1
    else:
        context.Result('not set')
        return 0

if 'configure' in COMMAND_LINE_TARGETS:
    conf = Configure( env, custom_tests = { 'CheckPKG_CONFIG_DIR' : CheckPKG_CONFIG_DIR })
    if conf.CheckPKG_CONFIG_DIR():
        os.environ['PKG_CONFIG_PATH']
    
    env = conf.Finish() 
else:
    Export('env')
    SConscript('src/SConscript')

## EOF ##
