#include "BlueHead.h"

LPTSTR CopyString(LPCTSTR s)
{
	const int i = strlen(s);
	LPTSTR res = new char[i+1];
	memcpy(res, s, i+1);
	return res;
}
