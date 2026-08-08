[![Build Status](https://travis-ci.org/lispparser/sexp-cpp.svg?branch=master)](https://travis-ci.org/lispparser/sexp-cpp)

SExp - A S-Expression Parser for C++
====================================

SExp is an [S-Expression](https://en.wikipedia.org/wiki/S-expression) parser for C++.

Example Code:

    sexp::Value value = sexp::Parser::from_string("(1 2 3 4 5)")
    sexpr::Value const& head = value.get_car();
    sexpr::Value const& tail = value.get_cdr();
    if (head.is_integer())
    {
       std::cout << head.as_int() << std::endl;
    }

Arrays vs Cons
--------------

When dealing with large amounts of data the classic Scheme list comes
with quite abit of a performance penentaly, for this reason SExp
allows you to interpret all list as arrays:

    sexp::Parser::from_stream(fin, sexp::Parser::USE_ARRAYS);


C++ locales
-----------

If C++ locales are used in your code, compile with:

    cmake -DSEXP_USE_LOCALE=ON

Otherwise the input and output functions will produce incorrect
results (i.e. "1,5" instead of "1.5").
