##  -*- python -*-
##  $Id: pingus_main.hxx,v 1.14 2003/10/18 12:11:30 grumbel Exp $
##
##  Pingus - A free Lemmings clone
##  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>,
##                     Francois Beerten
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
import SCons.Util

pingus_sources = \
               Glob('external/tinygettext/*.cpp') + \
               Glob('external/binreloc-2.0/*.c') + \
               Glob('src/pingus/actions/*.cpp') + \
               Glob('src/colliders/*.cpp') + \
               Glob('src/components/*.cpp') + \
               Glob('src/display/*.cpp') + \
               Glob('src/editor/*.cpp') + \
               Glob('src/gui/*.cpp') + \
               Glob('src/input/*.cpp') + \
               Glob('src/lisp/*.cpp') + \
               Glob('src/math/*.cpp') + \
               Glob('src/movers/*.cpp') + \
               Glob('src/particles/*.cpp') + \
               Glob('src/pingus/*.cpp') + \
               Glob('src/screen/*.cpp') + \
               Glob('src/sound/*.cpp') + \
               Glob('src/util/*.cpp') + \
               Glob('src/worldmap/*.cpp') + \
               Glob('src/worldobjs/*.cpp')

class _SpaceListOptionClass:
   """An option type for space-separated lists with arbitrary elements."""
   def CheckDir(self, val):
      if not os.path.isdir(val):
         raise SCons.Errors.UserError("No directory at %s" % val)

   def _convert(self, key, val, env):
      if SCons.Util.is_List(val): # prefer val if it's already a list
         return val
      elif len(val) > 0 and val[0] == '[' and val[-1] == ']':
         # or a repr of a list
         return eval(val)
      elif env: # otherwise, use whatever's in env
         val = env[key]
         if not SCons.Util.is_List(val):
            val = val.split(None)
         return val
      else: # val was substituted into a string, losing its structure
         # We'll be called again with env, hopefully that's more useful
         raise TypeError("try again with the environment")

   def _validate(self, val, env, check, converter):
      for i in converter(val, env):
         if check(i):
            return True
      return False

   def __call__(self, key, help, check=None, default=[]):
      def converter(val, env = None):
         return self._convert(key, val, env)
 
      validator = None
      if check is not None:
         validator = lambda k, v, e: self._validate(v, e, check, converter)
      return (key, help, default, validator, converter)
 
SpaceListOption = _SpaceListOptionClass()

def DefineOptions(filename, args):
   opts = Variables(filename, args)
   opts.Add('CC', 'C Compiler', 'gcc')
   opts.Add('CXX', 'C++ Compiler', 'g++')
#   opts.Add('debug', 'Build with debugging options', 0)
#   opts.Add('profile', 'Build with profiling support', 0)

   opts.Add('CPPPATH',    'Additional preprocessor paths', [])
   opts.Add('LIBPATH',    'Additional library paths',      [])
   opts.Add('CPPFLAGS',   'Additional preprocessor flags', [])
   opts.Add('CPPDEFINES', 'defined constants', [])
   opts.Add('LIBS',       'Additional libraries', [])
   opts.Add('CCFLAGS',    'C Compiler flags', [])
   opts.Add('CXXFLAGS',   'C++ Compiler flags', [])
   opts.Add('LINKFLAGS',  'Linker Compiler flags', [])

   opts.Add(BoolVariable('with_opengl',        'Build with OpenGL support', True))
   opts.Add(BoolVariable('with_xinput',        'Build with Xinput support', False))
   opts.Add(BoolVariable('with_linuxusbmouse', 'Build with Linux USB mouse support', True))
   opts.Add(BoolVariable('with_linuxevdev',    'Build with Linux evdev support', True))
   opts.Add(BoolVariable('with_wiimote',       'Build with Wiimote support', False))
   opts.Add(BoolVariable('ignore_errors',      'Ignore any fatal configuration errors', False))
   opts.Add('optional_sources', 'Additional source files', [])
   return opts

def CheckSDLLib(context, sdllib):
   """
   On some platforms, SDL does this ugly redefine-main thing, that can
   interact badly with CheckLibWithHeader.
   """
   lib = "SDL_%s" % sdllib
   context.Message('Checking for %s...' % lib)
   text = """
#include "SDL.h"
#include "%s.h"
int main(int argc, char* argv[]) { return 0; }
""" % lib
   context.AppendLIBS(lib)
   if context.BuildProg(text, ".cpp"):
      context.Result("failed")
      return False
   else:
      context.Result("ok")
      return True

def CheckIconv(context):
   text = """
#include <iconv.h>
int main() {
   %s char *foo;
   (void)iconv((iconv_t)0, &foo, (size_t*)0, (char**)0, (size_t*)0);
   return 0;
}
"""
   config.CheckLibWithHeader('iconv', 'iconv.h', 'c++') # Ok to fail
   context.Message('Check how to call iconv...')

   for i in ['', 'const']:
      if config.TryCompile(text % i, ".cpp"):
         context.Result("use '%s'" % i)
         return i
   context.Result("failed")
   return False

def CheckMyProgram(context, prgn):
   context.Message('Checking for %s...' % prgn)
   for i in context.env['ENV']['PATH'].split(":"):
      if os.path.exists(i + "/sdl-config"):
         context.Result(i + "/sdl-config")
         return True
   context.Result("failed")
   return False

Alias('configure')
    
if ('configure' in COMMAND_LINE_TARGETS) or \
   not (File('config.py').exists() and File('config.h').exists()) and \
   not GetOption('clean'):
    opts = DefineOptions(None, ARGUMENTS)
    env = Environment(options = opts)
    Help(opts.GenerateHelpText(env))

    opts.Update(env)

    if os.environ.has_key('PATH'):
        env['ENV']['PATH'] = os.environ['PATH']
    if os.environ.has_key('HOME'):
       env['ENV']['HOME'] = os.environ['HOME']

    if os.environ.has_key('PKG_CONFIG_PATH'):
        env['ENV']['PKG_CONFIG_PATH'] = os.environ['PKG_CONFIG_PATH']
    
    env['CPPPATH'] += ['.', 'src/', 'external/']

    config_h_defines = []      

    config = env.Configure(custom_tests = {
       'CheckMyProgram' : CheckMyProgram,
       'CheckSDLLib': CheckSDLLib,
       'CheckIconv': CheckIconv,
    })
    fatal_error = ""
    reports = ""

    # FIXME: Seems to require a rather new version of SCons
    # ret = config.CheckBuilder(context, None, "C++")
    # if ret != "":
    #   reports += "  * C++ Compiler missing: " + ret

    if not env['with_opengl']:
       reports += "  * OpenGL support: disabled\n"
    else:
       reports += "  * OpenGL support: enabled\n"
       config_h_defines  += [('HAVE_OPENGL', 1)]
       env['LIBS']       += ['GL']
       env['optional_sources'] += ['src/display/opengl/opengl_framebuffer_surface_impl.cpp', 
                                   'src/display/opengl/opengl_framebuffer.cpp' ]

    if not env['with_linuxusbmouse']:
        reports += "  * Linux USB mouse support: disabled\n"
    else:
        reports += "  * Linux USB mouse support: enabled\n"
        config_h_defines  += [('HAVE_LINUXUSBMOUSE', 1)]
        env['optional_sources'] += ['src/input/usbmouse/usbmouse_driver.cpp']
    
    if not env['with_linuxevdev']:
        reports += "  * Linux evdev support: disabled\n"
    else:
        reports += "  * Linux evdev support: ok\n"
        config_h_defines  += [('HAVE_LINUXEVDEV', 1)]
        env['optional_sources'] += ['src/input/evdev/evdev_driver.cpp',
                                    'src/input/evdev/evdev_device.cpp']
    
    if not env['with_wiimote']:
        reports += "  * Wiimote support: disabled\n"        
    elif config.CheckLibWithHeader('cwiid', 'cwiid.h', 'c++'):
        reports += "  * Wiimote support: yes\n"
        config_h_defines  += [('HAVE_CWIID', 1)]
        env['LIBS']       += ['cwiid']
        env['optional_sources'] += ['src/input/wiimote/wiimote_driver.cpp',
                                    'src/input/wiimote/wiimote.cpp']
    else:
        reports += "  * Wiimote support: no (libcwiid or cwiid.h not found)\n"

    if not env['with_xinput']:
        reports += "  * XInput support: disabled\n"
    elif not config.CheckLibWithHeader('Xi', 'X11/extensions/XInput.h', 'c++'):
        reports += "  * XInput support: no (library Xi not found)\n" ## FIXME: Need to set a define
    else:
        reports += "  * XInput support: yes\n"
        config_h_defines  += [('HAVE_XINPUT', 1)]
        env['LIBS'] += ['Xi']
        env['optional_sources'] += ['src/input/xinput/xinput_driver.cpp',
                                    'src/input/xinput/xinput_device.cpp']
        
    if not config.CheckLibWithHeader('boost_signals', 'boost/signals.hpp', 'c++'):
       if not config.CheckLibWithHeader('boost_signals-mt', 'boost/signals.hpp', 'c++'):
          fatal_error += "  * library 'boost_signals' not found\n"

    if not config.CheckLibWithHeader('png', 'png.h', 'c++'):
       fatal_error += "  * library 'png' not found\n"

    if config.CheckMyProgram('sdl-config'):
       env.ParseConfig('sdl-config  --cflags --libs')
       for sdllib in ['image', 'mixer']:
          if not config.CheckSDLLib(sdllib):
             fatal_error += "  * SDL library '%s' not found\n" % sdllib
    else:
       fatal_error += "  * couldn't find sdl-config, SDL missing\n"

    iconv_const = config.CheckIconv()
    if iconv_const == False:
       fatal_error += "  * can't call iconv\n"

    env = config.Finish()
    opts.Save("config.py", env)

    print "Reports:"
    print reports

    if not fatal_error == "":
        print "Fatal Errors:"
        print fatal_error
        if not env['ignore_errors']:
           Exit(1)
        else:
           print "\nError are being ignored, the build continues"

    config_h = open('config.h', 'w')
    config_h.write('#define VERSION "0.7.3"\n')
    config_h.write('#define ENABLE_BINRELOC 1\n')
    config_h.write('#define ICONV_CONST %s\n' % iconv_const)
    for (v,k) in config_h_defines:
        config_h.write('#define %s %s\n' % (v, k))
    config_h.close()
    if ('configure' in COMMAND_LINE_TARGETS):
        print "Configuration written to config.h and config.py, run:"
        print ""
        print "  scons"
        print ""
        print "To start the compile"
    else:
        print "Configuration written to config.h and config.py"
    ARGUMENTS = {}

if not ('configure' in COMMAND_LINE_TARGETS):
    if ARGUMENTS != {}:
        print "Error: You must not supply arguments to the compile step."
        print "Use:"
        print ""
        print "  scons configure [ARGUMENTS]..."
        print ""
        print "If you want to change the build configuration."
        os.sys.exit(1)
        
    opts = DefineOptions("config.py", {})
    env = Environment(options = opts)
    Help(opts.GenerateHelpText(env))

    opts.Update(env)
    env['CPPPATH'] += ['.', 'src/']
    Default(env.Program('pingus', pingus_sources + env['optional_sources']))
    Clean('pingus', ['config.py', 'config.h'])

    
## EOF ##
