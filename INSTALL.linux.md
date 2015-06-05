Pingus Installation Guide for GNU/Linux
=======================================

Requirements
------------

To compile Pingus you need:

 g++           - http://gcc.gnu.org/
 SDL           - http://libsdl.org/
 SDL_mixer
 SDL_image
 boost         - http://www.boost.org
 boost-signals - http://www.boost.org
 libpng        - http://libpng.org/
 scons         - http://scons.org/

In most cases you will find all of these in your distribution and
there shouln't be a need to compile anything manually.

In Ubuntu you can install everything by typing:

    sudo apt-get install \
     g++ \
     libsdl2-dev \
     libsdl2-mixer-dev \
     libsdl2-image-dev \
     libboost-dev \
     libboost-signals-dev \
     libpng12-dev \
     cmake


In Fedora 7, 8 and 9 (and most likely later versions) you can install
everything by typing:

    su -c 'yum install \
      gcc-c++ \
      SDL-devel \
      SDL_image-devel \
      SDL_mixer-devel \
      boost-devel \
      libpng-devel \
      glibc-common \
      scons'


Compilation
-----------

Once all libraries are in place, you can compile Pingus with just:

    make

or

    mkdir -p build
    scons src
    scons

If you need to change the compiler or other build variables you can do
so with:

    scons configure CXX="ccache g++" CPPPATH=/your/custom/path with_wiimote=True
    scons

A full list of variables is available via:

    scons -h


Running:
--------
Once the compilation is successful you can run Pingus directly from
the top level directory of the source tree via:

    build/pingus

There is no need to install Pingus.

If you have a slow machine, starting Pingus with:

    build/pingus -g 640x480 --frame-skip 3

Might lead to a better playable game. If you want to run Pingus in a
larger resolution, you can do so with:

    build//pingus -g 1024x768

Fullscreen support is available via:

    build/pingus --fullscreen

Available languages can be listed with:

    build/pingus --list-languages

And used with:

    build/pingus --language de


Installation:
-------------
As mentioned above, you don't need to install Pingus to run it, if you
still want to do it, you can do so with:

    make install PREFIX=/usr

