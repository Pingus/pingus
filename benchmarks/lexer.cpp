// SExp - A S-Expression Parser for C++
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <benchmark/benchmark.h>

#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "sexp/lexer.hpp"

static void BM_lexer(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    std::ifstream fin("benchmarks/test.sexp");
    if (!fin)
    {
      throw std::runtime_error("failed to open benchmarks/test.sexp");
    }
    else
    {
      sexp::Lexer lexer(fin);
      while(lexer.get_next_token() != sexp::Lexer::TOKEN_EOF) {}
    }
  }
}
BENCHMARK(BM_lexer);

BENCHMARK_MAIN();

/* EOF */
