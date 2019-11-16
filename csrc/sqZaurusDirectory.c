/* This file is a heavily modified version of the file "sqMacDicrectory.c".
 *
 * Modifications by: Ian Piumarta (ian.piumarta@inria.fr)
 *
 * The original version of this file can be regenerated from the Squeak
 * image by executing:
 *
 * 	InterpreterSupportCode writeMacSourceFiles
 *
 * $Log: sqUnixDirectory.c,v $
 * Revision 1.1  1996/10/24  13:21:38  piumarta
 * Initial revision
 *
 */

/*
  This file is a modification version of the file "sqUnixDirectory.c".
  Modifications by OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)
*/

#include <timelib.h>
#include <file.h>

#include "sq.h"

/*** Constants ***/
#define ENTRY_FOUND     0
#define NO_MORE_ENTRIES 1
#define BAD_PATH        2

#define DELIMITER ':'

int
convertToSqueakTime(TIME *t)
{
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;

  unsigned int result = 0;
  unsigned int daysFromOrigin = 0;
  unsigned int days = 0;

  year = ((t->TArray[4] & 0xf) << 4) | ((t->TArray[3] & 0xf0)>> 4);
  month = t->TArray[3] & 0xf;
  day = (t->TArray[2] & 0xf8) >> 3;
  
  hour = ((t->TArray[2] & 0x7) << 2) | ((t->TArray[1] & 0xc0)>> 6);
  minute = (t->TArray[1] & 0x3f);
  second = (t->TArray[0] & 0x3f);

  days = CalcPassedDays(t);

  days += (year - 1) * 365;
  days += (year - 1) / 4;

  result = days * 24 * 60 * 60;
  
  result += hour * 60 * 60;
  result += minute * 60;
  result += second;

  return result;
}

int
dir_Create(char *pathString, int pathStringLength)
{
  success(false);
  return false;
}

int
dir_Delete(char *pathString, int pathStringLength)
{
  success(false);
  return false;
}

int
dir_Delimitor(void)
{
  return DELIMITER;
}

int
dir_Lookup(char *pathString, int pathStringLength, int index,
/* outputs: */ char *name, int *nameLength, int *creationDate, int *modificationDate,
	       int *isDirectory, int *sizeIfFile)
{
  /* Lookup the index-th entry of the directory with the given path, starting
     at the root of the file system. Set the name, name length, creation date,
     creation time, directory flag, and file size (if the entry is a file).
     Return:	0 	if a entry is found at the given index
     		1	if the directory has fewer than index entries
		2	if the given path has bad syntax or does not reach a directory
  */

  int nameLen = 0;
  int dirno;

  int result;

  static unsigned char *f0wildcard = (unsigned char*)"F0:*.*";
  static unsigned char *f1wildcard = (unsigned char*)"F1:*.*";
  
  unsigned char *w;
  char buf[DFNAME_LEN+1];

  FDIRBUF pDirAcc;
  FINFOFILE finfo;
  TIME t;
  
  FSEARCHMODE p;

  if (strcmp(pathString, "F0") == 0) {
    w = f0wildcard;
  } else if (strcmp(pathString, "F1") == 0) {
    w = f1wildcard;
  } else {
    if (pathStringLength == 0) {
      if (index == 1) {
	strcpy(name, "F0");
	*nameLength = 2;
	*creationDate = 0;
	*creationDate = 0;
	*isDirectory = true;
	*sizeIfFile = 0;
	return ENTRY_FOUND;
      } else if (index == 2) {
	p.DirNo = 0;
	p.Behav = FC_SEARCH_INV_NEXT;
	
	if (FileSearch(f1wildcard, &p, (unsigned char*)buf) >= 0) {
	  strcpy(name, "F1");
	  *nameLength = 2;
	  *creationDate = 0;
	  *creationDate = 0;
	  *isDirectory = true;
	  *sizeIfFile = 0;
	  return ENTRY_FOUND;
	} else {
	  return 1;
	}
      } else {
	return 1;
      }
    }
    return 2;
  }

  /* default return values */
  *name             = 0;
  *nameLength       = 0;
  *creationDate     = 0;
  *modificationDate = 0;
  *isDirectory      = false;
  *sizeIfFile       = 0;

  p.DirNo = index - 1; /* DirNo begins with 0 */
  p.Behav = FC_SEARCH_INV_NEXT;

  result = FileSearch(w, &p, (unsigned char*)buf);

  if (result < 0) {
    return 1;
  }

  if (FileDirAcc((unsigned char*)buf, FC_ACC_READ, &pDirAcc)) {
    return BAD_PATH;
  }

  finfo.Time = pDirAcc.Time;
  finfo.Date = pDirAcc.Date;
  finfo.Atr = pDirAcc.Atr;

  if (FormatRtcToMsdos(&finfo, &t, TO_RTC)) {
    printf("time format error?");
    return 2;
  }

  strcpy(name, buf);
  *nameLength = strlen(name);

  *creationDate= convertToSqueakTime(&t);
  *modificationDate= convertToSqueakTime(&t);
  *isDirectory = false;
  *sizeIfFile= pDirAcc.Size;

  return ENTRY_FOUND;
}

int
dir_SetMacFileTypeAndCreator(char *filename, int filenameSize,
				 char *fType, char *fCreator)
{
  /* unix files are untyped, and the creator is correct by default */
  return true;
}

int
asyncFileClose(AsyncFile *f)
{
  success(false);
  return false;
}

int
asyncFileOpen(AsyncFile *f, int fileNamePtr, int fileNameSize, int writeFlag, int semaIndex)
{
  success(false);
  return false;
}

int
asyncFileRecordSize()
{
  success(false);
  return false;
}

int
asyncFileReadResult(AsyncFile *f, int bufferPtr, int bufferSize)
{
  success(false);
  return false;
}
  
int
asyncFileReadStart(AsyncFile *f, int fPosition, int count)
{
  success(false);
  return false;
}

int
asyncFileWriteResult(AsyncFile *f)
{
  success(false);
  return false;
}

int
asyncFileWriteStart(AsyncFile *f, int fPosition, int bufferPtr, int bufferSize)
{
  success(false);
  return false;
}

