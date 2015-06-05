Pingus Installation Guide for Mac OS X
======================================

Disclaimer
----------

The following guide assumes that you use macports to install the requirements.
You can also do this the hard way by finding/compiling the stuff yourself,
but if you're capable of doing so you won't need this guide anyway.

See http://code.google.com/p/pingus/wiki/MacOS_X_Compiling if you are adventurous or do not want to us macports.

This guide was written on OS X 10.5 "Leopard", where XCode brings gcc-4.0.
Since that's too old gcc-4.4 is installed using macports and also referenced
in the Makefile.macosx. If you have a recent compiler, just skip these parts
and adapt the makefile.


Requirements
------------

First get and install macports from http://www.macports.org/
Next use macports to install pingus' requirements by typing:

    sudo port install libsdl libsdl_image libsdl_mixer boost scons
    sudo port install gcc44


Compilation
-----------

Once all libraries are in place, you can compile Pingus with just:

    make -f Makefile.macosx


Running
-------

Once the compilation is successful you can run Pingus directly from
the top level directory of the source tree via:

    ./pingus

For details about the command line options etc. see INSTALL.unix


Creating an application bundle
------------------------------

To make pingus feel at home on OS X you may want to create an application bundle.
The follwogin steps will create a Pingus.app that can be used and distributed like
any other Mac OS X application.

First we need some little helper utilities

    sudo port install dylibbundler makeicns

Now the application bundle can be build

    make -f Makefile.macosx bundle


Distribute
----------

To create a compressed package for distribution frun the following command:

    make -f Makefile.macosx package
