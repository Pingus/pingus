#include <iostream>
#include <locale.h>
#include <ctype.h>

int main()
{
  int i;
  
  setlocale(LC_ALL, "");
  
  for(i = 0; i < 256; ++i)
    if (isgraph(i))
      std::cout << (unsigned char)i << " ";
  std::cout << std::endl;
}

/* EOF */
