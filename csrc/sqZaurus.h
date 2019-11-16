/*
  Zaurus Specific Configuration
  Written by:  OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)
*/

#ifndef SQ_ZAURUS_H
#define SQ_ZAURUS_H

#include <zheaders.h>
#include <wtii_res.h>
#include <wtii_C_usr.h>
#include <wtii.h>

#undef sqAllocateMemory
#define sqAllocateMemory(bytes, desired) NewHeapPtrS(desired)

#define exit(n) ioExit()
#define rename zrename
#define remove zunlink
#define getchar()

#undef ioMSecs
#undef ioLowResMSecs
#undef ioMicroMSecs

#define ioLowResMSecs ioMSecs
#define ioMicroMSecs ioMSecs

#undef sqImageFileRead
#undef sqImageFileWrite
#define sqImageFileRead(ptr, sz, count, f)   fread((char*)ptr, sz, count, f)
#define sqImageFileWrite(ptr, sz, count, f)  fwrite((char*)ptr, sz, count, f)

#define clock ioSeconds
#define time(x) 0

#endif
