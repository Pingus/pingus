/*
	Electric fence, C++ enable code.

	If you need to test for buffer overruns, add -lefence to your linking
	and include this header in one of your sourcefiles.
*/

#ifndef header_efence
#define header_efence

#ifndef WIN32
void *operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void *ptr)
{
	free(ptr);
}

void operator delete[](void *ptr)
{
	free(ptr);
}
#endif

#endif
