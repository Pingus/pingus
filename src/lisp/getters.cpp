
#include "getters.hpp"

namespace lisp
{

bool get(const Lisp* lisp, bool& val)
{
  if(lisp->get_type() != Lisp::TYPE_BOOL)
    return false;
  val = lisp->get_bool();
  return true;
}

bool get(const Lisp* lisp, float& val)
{
  if(lisp->get_type() == Lisp::TYPE_INT)
    val = static_cast<float>(lisp->get_int());
  else if(lisp->get_type() == Lisp::TYPE_FLOAT)
    val = lisp->get_float();
  else
    return false;
  return true;
}

bool get(const Lisp* lisp, int& val)
{
  if(lisp->get_type() != Lisp::TYPE_INT)
    return false;
  val = lisp->get_int();
  return true;
}

bool get(const Lisp* lisp, std::string& val)
{
  if(lisp->get_type() != Lisp::TYPE_STRING)
    return false;
  val = lisp->get_string();
  return true;
}

}
