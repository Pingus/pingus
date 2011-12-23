##  -*- python -*-
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

Import('package_version')

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
    context.Message('Check how to call iconv...')
    text = """
#include <iconv.h>
int main() {
   %s char *foo;
   (void)iconv(static_cast<iconv_t>(0), &foo, static_cast<size_t*>(0), static_cast<char**>(0), static_cast<size_t*>(0));
   return 0;
}
"""
    # self.conf.CheckLibWithHeader(context, 'iconv', 'iconv.h', 'c++') # Ok to fail
    for i in ['', 'const']:
        if context.TryCompile(text % i, ".cpp"):
            context.Result("use '%s'" % i)
            return i
    context.Result("failed")
    return False

def CheckMyProgram(context, prgn):
    context.Message('Checking for %s...' % prgn)
    for i in context.env['ENV']['PATH'].split(":"):
        filename = os.path.join(i, prgn)
        if os.path.exists(filename):
            context.Result(filename)
            return True
    context.Result("failed")
    return False

class Project:
    def configure(self):
        self.configure_begin()
        self.configure_opengl()
        self.configure_linuxevdev()
        self.configure_wiimote()
        self.configure_xinput()
        self.configure_boost()
        self.configure_png()
        self.configure_sdl()
        self.configure_iconv()
        self.configure_end()

    def configure_begin(self):
        self.opts = Variables("custom.py", ARGUMENTS)
        self.opts.Add('BUILD', 'Set the build type', "default")
        self.opts.Add('CC', 'C Compiler', 'gcc')
        self.opts.Add('CXX', 'C++ Compiler', 'g++')
    #   self.opts.Add('debug', 'Build with debugging options', 0)
    #   self.opts.Add('profile', 'Build with profiling support', 0)
    
        self.opts.Add('CPPPATH',    'Additional preprocessor paths', [])
        self.opts.Add('LIBPATH',    'Additional library paths',      [])
        self.opts.Add('CPPFLAGS',   'Additional preprocessor flags', [])
        self.opts.Add('CPPDEFINES', 'defined constants', [])
        self.opts.Add('LIBS',       'Additional libraries', [])
        self.opts.Add('CCFLAGS',    'C Compiler flags', [])
        self.opts.Add('CXXFLAGS',   'C++ Compiler flags', ["-O2", "-s"])
        self.opts.Add('LINKFLAGS',  'Linker Compiler flags', [])

        self.opts.Add(BoolVariable('with_opengl',        'Build with OpenGL support', True))
        self.opts.Add(BoolVariable('with_xinput',        'Build with Xinput support', False))
        self.opts.Add(BoolVariable('with_linuxevdev',    'Build with Linux evdev support',  sys.platform == "linux2"))
        self.opts.Add(BoolVariable('with_wiimote',       'Build with Wiimote support', False))
        self.opts.Add(BoolVariable('ignore_errors',      'Ignore any fatal configuration errors', False))
        self.opts.Add('optional_sources', 'Additional source files', [])

        self.env = Environment(options = self.opts, ENV=os.environ)
        self.env.Append(CXXFLAGS = ["-std=c++0x"])
        self.env.Append(CPPDEFINES = [('VERSION', '"\\"%s\\""' % package_version)])
        self.env.Append(CPPPATH = ['src/'])
        
        Help(self.opts.GenerateHelpText(self.env))
                

        self.conf = self.env.Configure(custom_tests = {
            'CheckMyProgram' : CheckMyProgram,
            'CheckSDLLib': CheckSDLLib,
            'CheckIconv': CheckIconv,
            })
        self.fatal_error = ""
        self.reports = ""

    def configure_end(self):
        self.env = self.conf.Finish()

        print "Reports:"
        print self.reports

        if not self.fatal_error == "":
            print "Fatal Errors:"
            print self.fatal_error
            Exit(1)

    def configure_gxx(self): 
        # FIXME: Seems to require a rather new version of SCons
        ret = self.conf.CheckBuilder(context, None, "C++")
        if ret != "":
            self.reports += "  * C++ Compiler missing: " + ret

    def configure_opengl(self):
        if not self.env['with_opengl']:
            self.reports += "  * OpenGL support: disabled\n"
        else:
            self.reports += "  * OpenGL support: enabled\n"           
            self.conf.env.Append(optional_sources = ['src/engine/display/opengl/opengl_framebuffer_surface_impl.cpp', 
                                                     'src/engine/display/opengl/opengl_framebuffer.cpp' ],
                                 CPPDEFINES = [('HAVE_OPENGL', 1)])
            if sys.platform == "darwin":
                self.conf.env.Append(LINKFLAGS = [ '-framework', 'OpenGL' ])
            else:
                self.conf.env.Append(LIBS = ['GL'])

    def configure_linuxevdev(self):
        if not self.env['with_linuxevdev']:
            self.reports += "  * Linux evdev support: disabled\n"
        else:
            self.reports += "  * Linux evdev support: ok\n"
            self.conf.env.Append(CPPDEFINES = [('HAVE_LINUXEVDEV', 1)])
            self.conf.env.Append(optional_sources = ['src/engine/input/evdev/evdev_driver.cpp',
                                                     'src/engine/input/evdev/evdev_device.cpp'])

    def configure_wiimote(self):
        if not self.env['with_wiimote']:
            self.reports += "  * Wiimote support: disabled\n"        
        elif self.conf.CheckLibWithHeader('cwiid', 'cwiid.h', 'c++'):
            self.reports += "  * Wiimote support: yes\n"
            self.conf.env.Append(CPPDEFINES = [('HAVE_CWIID', 1)])
            self.conf.env.Append(LIBS = ['cwiid'])
            self.conf.env.Append(optional_sources = ['src/engine/input/wiimote/wiimote_driver.cpp',
                                                     'src/engine/input/wiimote/wiimote.cpp'])
        else:
            self.reports += "  * Wiimote support: no (libcwiid or cwiid.h not found)\n"

    def configure_xinput(self):
        if not self.env['with_xinput']:
            self.reports += "  * XInput support: disabled\n"
        elif not self.conf.CheckLibWithHeader('Xi', 'X11/extensions/XInput.h', 'c++'):
            self.reports += "  * XInput support: no (library Xi not found)\n" ## FIXME: Need to set a define
        else:
            self.reports += "  * XInput support: yes\n"
            self.conf.env.Append(CPPDEFINES = [('HAVE_XINPUT', 1)])
            self.conf.env.Append(LIBS = ['Xi'])
            self.conf.env.Append(optional_sources = ['src/engine/input/xinput/xinput_driver.cpp',
                                                     'src/engine/input/xinput/xinput_device.cpp'])
            
    def configure_boost(self):
        if not self.conf.CheckLibWithHeader('boost_signals', 'boost/signals.hpp', 'c++'):
            if not self.conf.CheckLibWithHeader('boost_signals-mt', 'boost/signals.hpp', 'c++'):
                self.fatal_error += "  * library 'boost_signals' not found\n"

    def configure_png(self):
        if self.conf.CheckMyProgram('pkg-config'):
            self.conf.env.ParseConfig("pkg-config  --cflags --libs libpng | sed 's/-I/-isystem/g'")
        else:
            if not self.conf.CheckLibWithHeader('png', 'png.h', 'c++'):
                self.fatal_error += "  * library 'png' not found\n"

    def configure_sdl(self):
        if self.conf.CheckMyProgram('sdl-config'):
            self.conf.env.ParseConfig("sdl-config  --cflags --libs | sed 's/-I/-isystem/g'")
            for sdllib in ['image', 'mixer']:
                if not self.conf.CheckSDLLib(sdllib):
                    self.fatal_error += "  * SDL library '%s' not found\n" % sdllib
        else:
            if sys.platform == 'darwin':
                # self.conf.env.StaticLibrary("sdlmain", ["src/macosx/SDLmain.m"], [ 'SDL' ])
                self.conf.env.Append(optional_sources = [ "src/macosx/SDLmain.m" ])
                self.conf.env.Append(LINKFLAGS = [ '-framework', 'SDL',
                                                   '-framework', 'Cocoa' ],
                                     CPPPATH   = ['/Library/Frameworks/SDL.framework/Headers/'])
                self.conf.env.Append(LINKFLAGS = [ '-framework', 'SDL_image' ],
                                     LIBS = [ 'jpeg' ],
                                     CPPPATH = ['/Library/Frameworks/SDL_image.framework/Headers/'])
                self.conf.env.Append(LINKFLAGS = [ '-framework', 'SDL_mixer',
                                                   # see http://forums.libsdl.org/viewtopic.php?t=5176&sid=72ad0db5b3f2ae62cfb0314adb517d42
                                                   '-dylib_file', '@loader_path/Frameworks/mikmod.framework/Versions/A/mikmod:/Library/Frameworks/SDL_mixer.framework/Versions/A/Frameworks/mikmod.framework/Versions/A/mikmod',
                                                   '-dylib_file', '@loader_path/Frameworks/smpeg.framework/Versions/A/smpeg:/Library/Frameworks/SDL_mixer.framework/Versions/A/Frameworks/smpeg.framework/Versions/A/smpeg' ],
                                     CPPPATH = ['/Library/Frameworks/SDL_mixer.framework/Headers/'])
            else:
                self.fatal_error += "  * couldn't find sdl-config, SDL missing\n"
                
    def configure_iconv(self):
        iconv_const = self.conf.CheckIconv()
        if iconv_const == False:
            self.fatal_error += "  * can't call iconv\n"
        else:
            self.conf.env.Append(CPPDEFINES = [ 'HAVE_ICONV_CONST', ('ICONV_CONST', iconv_const) ])
        self.conf.CheckLib('iconv')

    def build(self):
        self.env.Append(CPPPATH = ['.', 'src/', 'external/tinygettext/'])

        libpingus = self.env.StaticLibrary('pingus',
                                           Glob('external/tinygettext/tinygettext/*.cpp') + \
                                           Glob('src/editor/*.cpp') + \
                                           Glob('src/engine/display/*.cpp') + \
                                           Glob('src/engine/display/delta/*.cpp') + \
                                           Glob('src/engine/gui/*.cpp') + \
                                           Glob('src/engine/input/*.cpp') + \
                                           Glob('src/engine/resource/*.cpp') + \
                                           Glob('src/engine/screen/*.cpp') + \
                                           Glob('src/engine/sound/*.cpp') + \
                                           Glob('src/engine/system/*.cpp') + \
                                           Glob('src/lisp/*.cpp') + \
                                           Glob('src/math/*.cpp') + \
                                           Glob('src/pingus/*.cpp') + \
                                           Glob('src/pingus/actions/*.cpp') + \
                                           Glob('src/pingus/colliders/*.cpp') + \
                                           Glob('src/pingus/components/*.cpp') + \
                                           Glob('src/pingus/movers/*.cpp') + \
                                           Glob('src/pingus/particles/*.cpp') + \
                                           Glob('src/pingus/screens/*.cpp') + \
                                           Glob('src/pingus/worldmap/*.cpp') + \
                                           Glob('src/pingus/worldobjs/*.cpp') + \
                                           Glob('src/util/*.cpp') + \
                                           self.env['optional_sources'])

        #if sys.platform == "darwin":
        #    self.env.StaticLibrary("sdlmain", ["src/macosx/SDLmain.m"], [ 'SDL' ])

        self.env.Default(self.env.Program('pingus', ['src/main.cpp', libpingus]))

        # build test and utils
        for filename in Glob("test/*_test.cpp", strings=True):
            self.env.Alias('test', self.env.Program(filename[:-4], [filename, libpingus]))
        for filename in Glob("test/*_util.cpp", strings=True):
            self.env.Alias('test', self.env.Program(filename[:-4], [filename, libpingus]))

        # build extra stuff
        for filename in Glob("extra/*.cpp", strings=True):
            self.env.Alias('extra', self.env.Program(filename[:-4], [filename, libpingus]))

project = Project()
project.configure()
project.build()

## EOF ##
