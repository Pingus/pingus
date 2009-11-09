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
   (void)iconv((iconv_t)0, &foo, (size_t*)0, (char**)0, (size_t*)0);
   return 0;
}
"""
    # config.CheckLibWithHeader(context, 'iconv', 'iconv.h', 'c++') # Ok to fail
    for i in ['', 'const']:
        if context.TryCompile(text % i, ".cpp"):
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

class Project:
    def __init__(self):
        Alias('configure')
        Alias('install')

        if ('configure' in COMMAND_LINE_TARGETS) or \
           not (File('config.py').exists() and File('config.h').exists()) and \
           not GetOption('clean'):
            self.configure()
        elif ('install' in COMMAND_LINE_TARGETS):
            self.install()
        else:
            self.build()

    def configure(self):
        self.configure_begin()
        self.configure_opengl()
        self.configure_linuxusbmouse()
        self.configure_linuxevdev()
        self.configure_wiimote()
        self.configure_xinput()
        self.configure_boost()
        self.configure_png()
        self.configure_sdl()
        self.configure_iconv()
        self.configure_end()

    def configure_begin(self):
        self.define_options(None, ARGUMENTS)
        self.env = Environment(options = self.opts)
        Help(self.opts.GenerateHelpText(self.env))

        self.opts.Update(self.env)

        if os.environ.has_key('PATH'):
            self.env['ENV']['PATH'] = os.environ['PATH']
        if os.environ.has_key('HOME'):
            self.env['ENV']['HOME'] = os.environ['HOME']

        if os.environ.has_key('PKG_CONFIG_PATH'):
            self.env['ENV']['PKG_CONFIG_PATH'] = os.environ['PKG_CONFIG_PATH']

        self.env['CPPPATH'] += ['.', 'src/', 'external/']

        self.config_h_defines = []      

        self.config = self.env.Configure(custom_tests = {
            'CheckMyProgram' : CheckMyProgram,
            'CheckSDLLib': CheckSDLLib,
            'CheckIconv': CheckIconv,
            })
        self.fatal_error = ""
        self.reports = ""

    def configure_end(self):
        self.env = self.config.Finish()
        self.opts.Save("config.py", self.env)

        print "Reports:"
        print self.reports

        if not self.fatal_error == "":
            print "Fatal Errors:"
            print self.fatal_error
            if not self.env['ignore_errors']:
                Exit(1)
            else:
                print "\nError are being ignored, the build continues"

        config_h = open('config.h', 'w')
        config_h.write('#define VERSION "0.7.3"\n')
        config_h.write('#define ENABLE_BINRELOC 1\n')
        config_h.write('#define ICONV_CONST %s\n' % self.iconv_const)
        for (v,k) in self.config_h_defines:
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

    def configure_gxx(self): 
        # FIXME: Seems to require a rather new version of SCons
        ret = config.CheckBuilder(context, None, "C++")
        if ret != "":
            reports += "  * C++ Compiler missing: " + ret

    def configure_opengl(self):
        if not self.env['with_opengl']:
            self.reports += "  * OpenGL support: disabled\n"
        else:
            self.reports += "  * OpenGL support: enabled\n"
            self.config_h_defines  += [('HAVE_OPENGL', 1)]
            self.env['LIBS']       += ['GL']
            self.env['optional_sources'] += ['src/engine/display/opengl/opengl_framebuffer_surface_impl.cpp', 
                                             'src/engine/display/opengl/opengl_framebuffer.cpp' ]

    def configure_linuxusbmouse(self):
        if not self.env['with_linuxusbmouse']:
            self.reports += "  * Linux USB mouse support: disabled\n"
        else:
            self.reports += "  * Linux USB mouse support: enabled\n"
            self.config_h_defines  += [('HAVE_LINUXUSBMOUSE', 1)]
            self.env['optional_sources'] += ['src/engine/input/usbmouse/usbmouse_driver.cpp']

    def configure_linuxevdev(self):
        if not self.env['with_linuxevdev']:
            reports += "  * Linux evdev support: disabled\n"
        else:
            self.reports += "  * Linux evdev support: ok\n"
            self.config_h_defines  += [('HAVE_LINUXEVDEV', 1)]
            self.env['optional_sources'] += ['src/engine/input/evdev/evdev_driver.cpp',
                                        'src/engine/input/evdev/evdev_device.cpp']

    def configure_wiimote(self):
        if not self.env['with_wiimote']:
            self.reports += "  * Wiimote support: disabled\n"        
        elif config.CheckLibWithHeader('cwiid', 'cwiid.h', 'c++'):
            self.reports += "  * Wiimote support: yes\n"
            self.config_h_defines  += [('HAVE_CWIID', 1)]
            self.env['LIBS']       += ['cwiid']
            self.env['optional_sources'] += ['src/engine/input/wiimote/wiimote_driver.cpp',
                                             'src/engine/input/wiimote/wiimote.cpp']
        else:
            self.reports += "  * Wiimote support: no (libcwiid or cwiid.h not found)\n"

    def configure_xinput(self):
        if not self.env['with_xinput']:
            self.reports += "  * XInput support: disabled\n"
        elif not config.CheckLibWithHeader('Xi', 'X11/extensions/XInput.h', 'c++'):
            self.reports += "  * XInput support: no (library Xi not found)\n" ## FIXME: Need to set a define
        else:
            self.reports += "  * XInput support: yes\n"
            self.config_h_defines  += [('HAVE_XINPUT', 1)]
            self.env['LIBS'] += ['Xi']
            self.env['optional_sources'] += ['src/engine/input/xinput/xinput_driver.cpp',
                                             'src/engine/input/xinput/xinput_device.cpp']

    def configure_boost(self):
        if not self.config.CheckLibWithHeader('boost_signals', 'boost/signals.hpp', 'c++'):
            if not self.config.CheckLibWithHeader('boost_signals-mt', 'boost/signals.hpp', 'c++'):
                self.fatal_error += "  * library 'boost_signals' not found\n"

    def configure_png(self):
        if not self.config.CheckLibWithHeader('png', 'png.h', 'c++'):
            self.fatal_error += "  * library 'png' not found\n"

    def configure_sdl(self):
        if self.config.CheckMyProgram('sdl-config'):
            self.env.ParseConfig('sdl-config  --cflags --libs')
            for sdllib in ['image', 'mixer']:
                if not self.config.CheckSDLLib(sdllib):
                    self.fatal_error += "  * SDL library '%s' not found\n" % sdllib
        else:
            fatal_error += "  * couldn't find sdl-config, SDL missing\n"

    def configure_iconv(self):
        self.iconv_const = self.config.CheckIconv()
        if self.iconv_const == False:
            self.fatal_error += "  * can't call iconv\n"

    def define_options(self, filename, args):
        self.opts = Variables(filename, args)
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
        self.opts.Add('CXXFLAGS',   'C++ Compiler flags', [])
        self.opts.Add('LINKFLAGS',  'Linker Compiler flags', [])

        self.opts.Add(BoolVariable('with_opengl',        'Build with OpenGL support', True))
        self.opts.Add(BoolVariable('with_xinput',        'Build with Xinput support', False))
        self.opts.Add(BoolVariable('with_linuxusbmouse', 'Build with Linux USB mouse support', True))
        self.opts.Add(BoolVariable('with_linuxevdev',    'Build with Linux evdev support', True))
        self.opts.Add(BoolVariable('with_wiimote',       'Build with Wiimote support', False))
        self.opts.Add(BoolVariable('ignore_errors',      'Ignore any fatal configuration errors', False))
        self.opts.Add('optional_sources', 'Additional source files', [])
        
    def build(self):
        if not ('configure' in COMMAND_LINE_TARGETS):
            if ARGUMENTS != {}:
                print "Error: You must not supply arguments to the compile step."
                print "Use:"
                print ""
                print "  scons configure [ARGUMENTS]..."
                print ""
                print "If you want to change the build configuration."
                os.sys.exit(1)

            self.define_options("config.py", {})
            self.env = Environment(options = self.opts)
            Help(self.opts.GenerateHelpText(self.env))

            self.opts.Update(self.env)
            self.env['CPPPATH'] += ['.', 'src/']

            Clean('pingus', ['config.py', 'config.h'])

            libpingus = self.env.StaticLibrary('pingus',
                                               Glob('external/binreloc-2.0/*.c') + \
                                               Glob('external/tinygettext/*.cpp') + \
                                               Glob('src/editor/*.cpp') + \
                                               Glob('src/engine/display/*.cpp') + \
                                               Glob('src/engine/gui/*.cpp') + \
                                               Glob('src/engine/input/*.cpp') + \
                                               Glob('src/engine/screen/*.cpp') + \
                                               Glob('src/engine/sound/*.cpp') + \
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
            self.env.Program('pingus', ['src/main.cpp', libpingus])

            for filename in Glob("test/*_test.cpp", strings=True):
                self.env.Program(filename[:-4], [filename, libpingus])

    def install(self):
        AddOption('--prefix',
                  dest='prefix',
                  type='string',
                  nargs=1,
                  action='store',
                  metavar='DIR',
                  help='installation prefix')
        prefix = GetOption('prefix')
        if not prefix:
            raise Exception("no prefix given")
        
        self.env = Environment(PREFIX = prefix)

        patterns = [
            'data/music/*.it',
            'data/music/*.ogg',
            'data/music/*.s3m',
            'data/levels/*/*.pingus',
            'data/worldmap/*.worldmap',
            ]

        install_files = []
        for pattern in patterns:
            install_files += Glob(pattern, strings=True)

        for filename in install_files:
            target_filename = '$PREFIX/share/pingus/%s' % filename[5:]
            Alias('install', self.env.InstallAs(target_filename, filename))

project = Project()

## EOF ##
