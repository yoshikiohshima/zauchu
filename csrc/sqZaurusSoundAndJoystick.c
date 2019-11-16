#include <sound.h>
#include "sq.h"
#include "wtii.h"
#include "wtii_C_usr.h"

char *printf(char *fmt, ...);

DRFFILE *sqDRF;
SndParam *sqSndParam;

/* values returned by PlayFile and RecordFile */
#define ShouldNotHappen 0
#define SoundSuccess 1
#define SoundBusy 2
#define FileNotFound 3
#define CorruptFile 4

/* action names */
#define SoundPlay 2
#define SoundRecord 3
#define SoundPause 4
#define SoundContinue 5

static char*
getRealFileName(char *nameIndex, int nameSize)
{
  static char realFileName[256];
  int i;
  /* copy the file names into null-terminated C strings */
  for (i = 0; i < nameSize; i++) {
    realFileName[i] = *((char *) (nameIndex + 4 + i));
  }
  realFileName[nameSize] = 0;
  return realFileName;
}

static int
checkFinishedInner(int status)
{

  /* 128 seems to be named STATUS_COMPLETE */
  
  return sqSndParam == NULL
    ? (status == 128 || status == STATUS_NULL ||
       (status & STATUS_END))
    : ((sqSndParam->endFlag != SNDPROC_RUNNING) &&
       ((status == 128) ||
	((status == STATUS_NULL || status & STATUS_END))));
}
  

static int
checkSoundFinished()
{
  int finished;  

  finished = checkFinishedInner(ZsdGetStatus());

  if (finished) {
    if (sqDRF != NULL) {
      ZsdDRFfclose(sqDRF);
      sqDRF = NULL;
    }
    if (sqSndParam != NULL) {
      ZsdDisposeSndParam(sqSndParam);
      sqSndParam = NULL;
    }
  }
  return finished;
}

/*  public functions */
  
void
sqZaurusSoundInitialize()
{
  /* this is nop now */
}

void
sqZaurusSoundShutdown()
{
  if (sqSndParam != NULL) {
    sqZaurusSoundStop(sqSndParam);

    while(sqSndParam->endFlag == SNDPROC_RUNNING) {
      Wait100mSec( 2 );
    }
    
    if (sqDRF != NULL) {
      ZsdDRFfclose(sqDRF);
      sqDRF = NULL;
    }
    if (sqSndParam != NULL) {
      ZsdDisposeSndParam(sqSndParam);
      sqSndParam = NULL;
    }
  }
  AD_END();
}

void
sqZaurusSoundPrepareForRecording()
{
  AD_INIT();
}

void
sqZaurusSoundPlayVolume(int volume)
{
  char gain;
  
  /* volume: 0 - 100 */
  volume = volume * 7 / 100;
  if (volume >= 7)
    gain = 0;	/* Max */
  else if (volume <= 0)
    gain = 7;	/* Min */
  else
    gain = 7 - volume;
  
  DA_GAIN(gain);
}

void
sqZaurusSoundRecordVolume(int volume)
{
  char gain;
  
  /* volume : 0 - 100 */
  volume = volume * 55 / 100;
  if (volume >= 55)
    gain = 0;	/* Max */
  else if (volume <= 0)
    gain = 55;	/* Min */
  else
    gain = 55 - volume;
  
  AD_GAIN(gain);
}

int
sqZaurusSoundGetFileInfo(char *nameIndex, int nameSize,
			 int *format, int *samplingRate, int *channel, 
			 int *compressionScheme, int *dataSize, int *dataTime)
{
  DRFFILE *drf;
  HDR tmpSndHeader;
  char * fileName;
  int result;

  fileName = getRealFileName(nameIndex, nameSize);

  drf = ZsdDRFfopen(fileName, "r");
  if (drf == NULL) {
    return FileNotFound;
  }

  if ((result = ZsdGetCond(drf, &tmpSndHeader)) < 0) {
    return CorruptFile;
  }

  *samplingRate = tmpSndHeader.smplFreq;
  *compressionScheme = tmpSndHeader.compID;
  *format = tmpSndHeader.fileID;
  *channel = tmpSndHeader.channelNum;
  *dataSize = tmpSndHeader.dataSize;
  *dataTime = ZsdSize2Time(&tmpSndHeader, *dataSize);

  ZsdDRFfclose(drf);

  return SoundSuccess;
}

int
sqZaurusSoundPlayFile(char *nameIndex, int nameSize, int startByte, int endByte)
{
  char *fileName;
  int result;
  int status;
  struct stat statBuf;
  int i;

  /* AD_INIT(); */
  
  status = checkSoundFinished();
  if (!status) {
    return SoundBusy;
  }

  if (sqDRF != NULL) {
    printf("should not happen\n");
    return ShouldNotHappen;
  }

  if (sqSndParam != NULL) {
    printf("should not happen\n");
    return ShouldNotHappen;
  }

  sqSndParam = ZsdCreateSndParam();

  if (sqSndParam == NULL) {
    printf("should not happen\n");
    return ShouldNotHappen;
  }

  fileName = getRealFileName(nameIndex, nameSize);
  sqDRF = ZsdDRFfopen(fileName, "r");
  if (sqDRF == NULL) {
    return FileNotFound;
  }

  DA_GAIN(4);
  
  /* sqSndParam.priority = SNDPROC_PRIORITY; */
  if ((result = ZsdGetCond(sqDRF, sqSndParam->zsd)) < 0) {
    return CorruptFile;
  }

  if (endByte <= 0) {
    endByte = ZSD_SIZE_ALL;
  }

  ZsdCont(sqSndParam);
  for (i = 0; i < 10; i++) {
    if (ZsdPlayDRF(sqSndParam, sqDRF,
		   startByte, endByte) == NORMAL_END) {
      return SoundSuccess;
    }
  }
  return ShouldNotHappen;
}


int
sqZaurusSoundRecordFile(char *nameIndex, int nameSize,
			int format, int samplingRate,
			int compressionScheme, int sizeInByte)
{
  char *fileName;
  int result;
  int status;
  int i;

  /* AD_INIT(); */

  status = checkSoundFinished();
  if (!status) {
    return SoundBusy;
  }

  if (sqDRF != NULL) {
    printf("should not happen\n");
    return 0;
  }

  if (sqSndParam != NULL) {
    printf("should not happen\n");
    return ShouldNotHappen;
  }

  sqSndParam = ZsdCreateSndParam();

  if (sqSndParam == NULL) {
    printf("should not happen\n");
    return ShouldNotHappen;
  }

  fileName = getRealFileName(nameIndex, nameSize);
  sqDRF = ZsdDRFfopen(fileName, "w");
  if (sqDRF == NULL) {
    return FileNotFound;
  }

  result = ZsdSetCond(format, compressionScheme,
		      samplingRate, sqSndParam->zsd);
  if (result != NORMAL_END) {
    CorruptFile;
  }

  AD_GAIN(0);

  ZsdCont(sqSndParam);
  for (i = 0; i < 10; i++) {
    if (ZsdRecDRF(sqSndParam, sqDRF,
		  sizeInByte + sqSndParam->zsd->hdrSize)
	== NORMAL_END) {
      return SoundSuccess;
    }
  }
  return ShouldNotHappen;
}

void
sqZaurusSoundPause()
{
  if (sqSndParam != NULL) {
    ZsdPause(sqSndParam);
  }
}

void
sqZaurusSoundContinue()
{
  if (sqSndParam != NULL) {
    ZsdCont(sqSndParam);
  }
}

void
sqZaurusSoundStop()
{
  DRFFILE *drf;

  if (sqSndParam != NULL) {
    ZsdStop(sqSndParam);
  }
  AD_END();
}

int
sqZaurusSoundGetPosition()
{
  if (sqSndParam != NULL) {
    return sqSndParam->size;
  }
  return -1;
}

int
sqZaurusSoundIsReadyForAction(int action)
{
  int finished;
  int status;

  status = ZsdGetStatus();
  finished = checkFinishedInner(status);
  
  switch (action) {
  case SoundPlay:
  case SoundRecord:
    return finished;
    break;
  case SoundPause:
    return
      ((status & STATUS_PLAY) || (status & STATUS_REC)) &&
      !(status & STATUS_PAUSE);
    break;
  case SoundContinue:
    return
      ((status & STATUS_PLAY) || (status & STATUS_REC)) &&
      (status & STATUS_PAUSE);
    break;
  }

  return false;
}


/*  stubbed out standard primitives */

int
snd_AvailableSpace()
{
  success(false);
  return 0;
}

int
snd_InsertSamplesFromLeadTime(int frameCount, int srcBufPtr, int samplesOfLeadTime)
{
  success(false);
  return 0;
}
  
int
snd_PlaySamplesFromAtLength(int frameCount, int arrayIndex, int startIndex)
{
  success(false);
  return 0;
}

int
snd_PlaySilence(void)
{
  success(false);
  return 0;
}

int
snd_Start(int frameCount, int samplesPerSec, int stereo, int semaIndex)
{
  success(false);
  return 0;
}
  
int
snd_Stop(void)
{
  success(false);
  return 0;
}


int
snd_SetRecordLevel(int level)
{
  success(false);
  return 0;
}

int
snd_StartRecording(int desiredSamplesPerSec, int stereo, int semaIndex)
{
  success(false);
  return 0;
}

int
snd_StopRecording()
{
  success(false);
  return 0;
}

double
snd_GetRecordingSampleRate(void)
{
  success(false);
  return 0;
}

int
snd_RecordSamplesIntoAtLength(int buf, int startSliceIndex, int bufferSizeInBytes)
{
  success(false);
  return 0;
}



int
joystickInit()
{
  success(false);
  return 0;
}

int
joystickRead(int stickIndex)
{
  success(false);
  return 0;
}


