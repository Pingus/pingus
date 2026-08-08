UITest
======

UITest is simple helper library for writing interactive tests,
somewhat inspired by gtest. Interactive tests are simple programs that
perform a function and take arguments from command line, not much
different then writing a small program using `main(int, char**)`.

UITest simplifies this by providing simple argument parsing and doc
string handling that frees the test from having to manually pick
appart `argv[]`. Furthermore UITest allows to combine several tests
into a single source file and in turn into a single executable. This
can drastically reduce the file count as well as link times.


Usage
-----

UITest comes in the form of a simple macro:

    UITEST(class, method, args="", docstring="")
    {
      ...
    }

`class` and `method` are strings that form the name of the test case,
while `docstring` provides a description. `args` is a special string
that gets interpreted by UITest and is used to parse `argv[]`, it's
similar to the regular `Usage:` strings found in many programs
`--help` output.

A `args` string of the form `"FILE"` means the program expects a single
argument. `"FILE..."` would indicate the program multiple arguments,
but at least one. `"[FILE]..."` indicates multiple arguments, but
allows zero arguments. A string of the form `"FILE IMAGETYPE"` would
indicate two arguments. The arguments themselves are passed to the
body of the test case via the `args` and `rest` variables, `args` will
take all the regular arguments while `rest` will contain the last
variable market with `...`.

The `UITEST_S` macro simplifies things even further, it only takes a
single argument which is passed to the body as `arg`. If multiple
arguments are specified the body will be called multiple times. A call
of the form:

    UITEST_S(class, method, "FILE", "")
    {
      ...
    }

Is equivalent to:

    UITEST(class, method, "FILE...", "")
    {
      for(const auto& arg : rest)
      {
        ...
      }
    }


Example
-------

A trivial example of UITest would look like this:

    #include <uitest/uitest.hpp>
    #include "imageloader.hpp"

    UITEST(ImageLoader, get_size, "FILE",
           "Load and image and display it's size")
    {
      auto img = load_img(args[0]);
      std::cout << img.get_width() << "x" << img.get_height() << std::endl;
    }

Once compiled the test case can be run like:

    $ ./test_example
    Usage: ./test_examples TESTCASE [ARG]...

    Test Cases:
       ImageLoader.get_size FILE

    $ ./test_example ImageLoader.get_size test.png
    640x480

