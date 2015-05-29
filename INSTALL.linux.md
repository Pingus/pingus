Pingus Installation Guide for GNU/Linux
=======================================

Requirements
------------

To compile Pingus you need:

* [g++](http://gcc.gnu.org/)
* [SDL2](http://libsdl.org/)
* [SDL2_mixer](https://www.libsdl.org/projects/SDL_image/)
* [SDL2_image](https://www.libsdl.org/projects/SDL_mixer/)
* [boost](http://www.boost.org)
* [libpng](http://libpng.org/)
* [cmake](http://cmake.org/)

In most cases you will find all of these in your distribution and
there shouln't be a need to compile anything manually.

In Ubuntu 15.04 you can install everything by typing:

    sudo apt-get install \
     g++ \
     libsdl2-dev \
     libsdl2-mixer-dev \
     libsdl2-image-dev \
     libboost-dev \
     libboost-signals-dev \
     libpng12-dev \
     cmake


Compilation
-----------

Pingus uses git submodules for some parts of the code. To initialize
those you can either run (when cloning the repository):

    git clone --recursive https://github.com/Pingus/pingus.git

Or run the following on an existing clone:

    git submodule update --init --recursive

To compile the code:

    mkdir -p build
    cd build
    cmake ..
    make

If you need to change the compiler or other build variables you can do
so with in the `cmake` step, e.g.:

    cmake .. -DCMAKE_BUILD_TYPE=Debug

The following variables are available:

* `-DCMAKE_BUILD_TYPE=Release`
* `-DCMAKE_BUILD_TYPE=Debug`
* `-DBUILD_TESTS=ON`
* `-DCMAKE_INSTALL_PREFIX:PATH=/usr`


Running
-------

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


Installation
------------

As mentioned above, you don't need to install Pingus to run it, if you
still want to do it, you can do so with:

    make install

The `DESTDIR` and `PREFIX` can be configured as follows:

    make install DESTDIR=/tmp/pingus

    cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/usr

