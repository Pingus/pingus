#include <config.h>

#include "lisp.hpp"
#include "properties.hpp"

namespace lisp
{

Properties::Properties(const Lisp* lisp)
{
  if (lisp)
    {
      if(lisp->get_type() != Lisp::TYPE_LIST)
        throw std::runtime_error("Lisp is not a list");

      for(size_t i = 0; i < lisp->get_list_size(); ++i) {
        const boost::shared_ptr<Lisp> child = lisp->get_list_elem(i);
        if(i == 0 && child->get_type() == Lisp::TYPE_SYMBOL)
          continue;
        if(child->get_type() != Lisp::TYPE_LIST)
          throw std::runtime_error("child of properties lisp is not a list");
        if(child->get_list_size() > 1)
          {    
            const boost::shared_ptr<Lisp> name = child->get_list_elem(0);
            if(name->get_type() != Lisp::TYPE_SYMBOL)
              throw std::runtime_error("property has no symbol as name");
            properties.insert(std::make_pair(
                                             std::string(name->get_symbol()), ListEntry(child)));
          }
      }
    }
}

Properties::~Properties()
{
}

void
Properties::print_unused_warnings(const std::string& context) const
{
  for(PropertyMap::const_iterator i = properties.begin();
      i != properties.end(); ++i) {
    if(i->second.used)
      continue;

    std::cout << "Warning: property '" << i->first << "' not used (in "
              << context << ")\n";
  }
}

}
