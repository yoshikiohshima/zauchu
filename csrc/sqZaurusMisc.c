#include "sq.h"
#include <pimmgr.h>

int zaurusScreenSize;

void*
calloc(size_t nelem, size_t elsize)
{
  char *p = sqAllocateMemory(nelem * elsize, nelem * elsize);
  if (p == NULL) {
    ioExit();
  } 

  memset(p, 0, nelem*elsize);
  return p;
}
