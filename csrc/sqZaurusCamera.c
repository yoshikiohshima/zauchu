/*
  Digital camera support code for Squeak/Zaurus
  Written by OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)
  Revised: 2000/ 3/30
*/

#include "sq.h"
#include <cameralb.h>
#include <offscrn.h>

WINDOWHD NewOffscreenInfo(RECTANGLE *rect,
			  unsigned short depth,
			  unsigned short qp);
void SetPixMapAdrOffscreen(WINDOWHD offScreen, PixMap* pixmap);
void DisposeOffscreenInfo(WINDOWHD offScreen);

void setupPrintf(APLFORMPTR AplForm);
char *printf(char *fmt, ...);

extern short headMargin;
#define BASEHEADER 4

#define CAMERA_ABSENT -1
#define CAMERA_AG05 0
#define CAMERA_AG06 1
#define CAMERA_OTHER 2

#define bytesPerLine(width, depth)	((((width)*(depth) + 31) >> 5) << 2)
#define bytesPerLineRD(width, depth)	((((width)*(depth)) >> 5) << 2)

char *printf(char *fmt, ...);

CAMERA *currentCamera = NULL;
int currentSlot = 0;
int currentModel;
int cameraWidth;
int cameraHeight;
int cameraZoom;

WINDOWHD offScreen = 0;

int lastLightValue;

int directToLCDFlag;
RECTANGLE directToLCDSize;
int directZoom;

static int
getCameraModel(int slot)
{
  static CAMERANAME cname;
  
  switch (slot) {
  case 1:
    if (GetCameraInfo(CAMERA_SOCKET16, &cname) != SUCCESS) {
      return CAMERA_ABSENT;
    }
    break;
  case 2:
    if (GetCameraInfo(CAMERA_SOCKET0, &cname) != SUCCESS) {
      return CAMERA_ABSENT;
    }
    break;
  default:
    return CAMERA_ABSENT;
  }

  if (strncmp((const char*)cname, "SHARP CEAG05", 12) == 0) {
    return CAMERA_AG05;
  } else if (strncmp((const char*)cname, "SHARP CEAG06", 12) == 0) {
    return CAMERA_AG06;
  } else {
    return CAMERA_OTHER;
  }
}

static CAMERA*
getCameraStruct(int slot)
{
  if (currentSlot != slot) {
    return NULL;
  }

  return currentCamera; /* may be NULL */
}

static int
allocateOffScreenInfoIfNeeded(RECTANGLE *size)
{
  OFFSCRNINFO info;

  if (offScreen != 0) {
    if (GetOffscreenInfo(offScreen, &info) == false) {
      printf("get offscreen info\n");
      return false;
    }

    if (info.rect.width != size->width ||
	info.rect.height != size->height) {
      DisposeOffscreenInfo(offScreen);
      offScreen = 0;
    }
  }

  if (offScreen == 0) {
    if ((offScreen = NewOffscreenInfo(size, 16, 2)) == 0) {
      printf("allocation of offScreenInfo\n");
      return false;
    }
  }
  return true;
}


static int
bitsToBits(int *bits, int bitsWidth, int bitsHeight)
{
  int i;
  unsigned int pixel2;
  unsigned int *lastWord;
  unsigned int *from;
  unsigned int pixel;

  if (bitsWidth & 1) {
    return false;
  }

  lastWord = bits + bitsWidth / 2 * bitsHeight;
  for (from = bits; from < lastWord; from++) {
    pixel2 = *from;
    pixel = (0xFFFF&pixel2) << 16;
    pixel2 >>= 16;
    pixel2 = (((pixel2>>1)&0x7FE0) |
	      ((pixel2&0x1f)));
    pixel = (((pixel>>1)&0x7FE00000) |
	     ((pixel&0x001F0000)));
    pixel2 |= pixel;
    *from = pixel2;
  }
  return true;
}

int
sqZaurusCameraModel(int slot)
{
  return getCameraModel(slot);
}

int
sqZaurusCameraInitialize(int slot)
{
  int zaurusSlot;

  if (currentCamera != NULL) {
    if (CloseCamera(currentCamera) != SUCCESS) {
      printf("CloseCamera\n");
    }
    currentCamera = NULL;
    currentSlot = 0;
    currentModel = CAMERA_ABSENT;
  }
  
  switch (slot) {
  case 1:
    zaurusSlot = CAMERA_SOCKET16;
    break;
  case 2:
    zaurusSlot = CAMERA_SOCKET0;
    break;
  default:
    success(false);
    return false;
  }

  if ((currentCamera = OpenCamera(zaurusSlot)) == NULL) {
    printf("opening camera\n");
    currentSlot = 0;
    success(false);
    return false;
  }

  currentSlot = slot;
  currentModel = getCameraModel(slot);

  if (ActivateCamera(currentCamera) != SUCCESS) {
    printf("activation of the camera\n");
    currentSlot = 0;
    success(false);
    return false;
  }

  lastLightValue = 0;
  SetConfigCamera(currentCamera, CAMCON_LIGHT, CAMCON_LIGHT_RESET);

  directToLCDFlag = false;
  ioFastTickUsage(true);

  return true;
}

int
sqZaurusCameraRelease(int slot)
{
  CAMERA *camera;

  if (offScreen != 0) {
    DisposeOffscreenInfo(offScreen);
    offScreen = 0;
  }
  
  if ((camera = getCameraStruct(slot)) == NULL) {
    return false;
  }
  
  if (DeactivateCamera(camera) != SUCCESS) {
    ;
  }

  if (CloseCamera(camera) != SUCCESS) {
    ;
  }

  currentCamera = NULL;
  currentSlot = 0;
  currentModel = CAMERA_ABSENT;

  return true;
}

int
sqZaurusCameraSetSizeAndZoom(int slot, int width,
			     int height, int zoom)
{
  CAMERA *camera;
  RECTANGLE size;

  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  size.left = 0;
  size.top = 0;
  size.width = width;
  size.height = height;
  cameraZoom = zoom;
  if (SetConfigCamera(camera, CAMCON_SIZE|CAMCON_IS_AVAILABLE,
		      &size, (ULONG)zoom) != SUCCESS) {
    printf("wrong camera size");
    return false;
  }

  cameraWidth = width;
  cameraHeight = height;
  return true;
}

int
sqZaurusCameraSetLight(int slot, int light)
{
  CAMERA *camera;
  CAMCONFTYPE type;
  
  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  if (light == 0) {
    type = CAMCON_LIGHT_RESET;
  } else if (light > lastLightValue) {
    type = CAMCON_LIGHT_INC;
  } else if (light < lastLightValue) {
    type = CAMCON_LIGHT_DEC;
  }
    
  if (SetConfigCamera(camera, CAMCON_LIGHT, type) != SUCCESS) {
    return false;
  }
  lastLightValue = light;
  return true;
}

int
sqZaurusCameraSetDirectToLCD(int slot, int direct, 
			     int left, int top, int width, int height, int zoom)
{
  CAMERA *camera;
  
  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  directToLCDFlag = direct;
  if (direct) {
    directToLCDSize.left = left;
    directToLCDSize.top = top + headMargin;
    directToLCDSize.width = width;
    directToLCDSize.height = height;
    directZoom = zoom;
  }

  return true;
}

int
sqZaurusCameraSetMode(int slot, int mode)
{
  CAMERA *camera;
  
  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }
  
  if (SetConfigCamera(camera, CAMCON_CONDITION,
			mode) != SUCCESS) {
    return false;
  }
  return true;
}
 
int
sqZaurusCameraShutterButtonPressed(int slot)
{
  CAMERA *camera;
  int val;
  
  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  return IsPressedTrigCamera(camera);
}

int
sqZaurusCameraInFocus(int slot)
{
  CAMERA *camera;

  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  return IsFocusedCamera(camera);
}

static int
cameraBody(CAMERA *camera, int bits, int bitsWidth, int bitsHeight, int needDisplay)
{
  RECTANGLE size;

  size.left = 0;
  size.top = 0;
  size.width = cameraWidth;
  size.height = cameraHeight;
  
  if (!allocateOffScreenInfoIfNeeded(&size)) {
    return false;
  }
  
  if (bitsWidth != cameraWidth || bitsHeight != cameraHeight) {
    return false;
  } else {
    SetPixMapAdrOffscreen(offScreen, (PixMap*)(bits+BASEHEADER));
  }
  
  StartCaptureCamera(camera);
  if (needDisplay) {
    DispCameraPhoto(camera, directZoom);
  }

  if (GetCameraPhoto(camera, offScreen, &size, cameraZoom) != SUCCESS) {
    printf("GetCameraPhoto 1\n");
    return false;
  }
  
  if (bitsToBits((int*)(bits + BASEHEADER),
		 bitsWidth, bitsHeight) == false) {
    printf("bitsToBits\n");
    return false;
  }
  return true; /* normal end */
}

int
sqZaurusCameraWaitForShutter(int slot, int bits,
			     int bitsWidth,
			     int bitsHeight, 
			     int timeout)
{
  CAMERA *camera;
  long start;
  long now;
  int shutter = false;
  int needDispCameraPhoto = false;
  int result = true;
  int more;

  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  /*
  if (cameraWidth == 0) {
    success(false);
    return false;
  }
  */

  if (directToLCDFlag) {
    switch (currentModel) {
    case CAMERA_AG05:
      if (SetConfigCamera(camera, CAMCON_DMAMODE, CAMCON_DMA_START
			  , &directToLCDSize, directZoom) != SUCCESS) {
	success(false);
	return false;
      }
      break;
    case CAMERA_AG06:
      needDispCameraPhoto = true;
      break;
    case CAMERA_OTHER:
    default:
      success(false);
      return false;
      break;
    }
  }

  more = true;
  start = ioMSecs();
  while (more) {
    if (IsPressedTrigCamera(camera)) {
      shutter = true;
      more = false;
    }
    if (!shutter) {
      now = ioMSecs();
      if (now < start) { /* counter wrap */
	more = false;
      } else if (now - start > timeout) {
	more = false;
      }
    }
    if (needDispCameraPhoto) {
      StartCaptureCamera(camera);
      DispCameraPhoto(camera, directZoom);
    }
  }

  if (directToLCDFlag && (currentModel == CAMERA_AG05)) {
    if (SetConfigCamera(camera, CAMCON_DMAMODE, CAMCON_DMA_STOP) != SUCCESS) {
      printf("stop direct mode\n");
      success(false);
      return false;
    }
  }

  if (shutter) {
    result = cameraBody(camera, bits, bitsWidth, bitsHeight, false);
  }

  if (result) {
    return shutter;
  }

  success(false);
  return false;
}

    
int
sqZaurusCameraCopy(int slot, int bits, int bitsWidth, int bitsHeight)
{
  CAMERA *camera;
  int needDisplay = false;

  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  /*
  if (cameraWidth == 0) {
    printf("wrong camera size");
    return false;
  }
  */

  if (currentModel == CAMERA_AG06) {
    needDisplay = directToLCDFlag;
  }

  if (!cameraBody(camera, bits, bitsWidth, bitsHeight, needDisplay)) {
    success(false);
    return false;
  }
  return true;
}


#if 0
static int
bitsToBits(int *fromBits, int fromBitsWidth, int fromBitsHeight,
	   int *toBits, int toBitsWidth, int toBitsHeight,
	   int clipX, int clipY, int clipWidth, int clipHeight,
	   int offsetX, int offsetY)
{
  int scanLine16;
  int firstWord16;
  int lastWord16;

  unsigned short *src;
  unsigned short *dst;
  unsigned short *to;
  unsigned short pixel;

  int line;
  
  if (clipY < 0 || clipX < 0 ||
      clipWidth > fromBitsWidth ||
      clipHeight > fromBitsHeight) {
    return false;
  }

  if (offsetY < 0 || offsetX < 0 ||
      offsetX + clipWidth > toBitsWidth ||
      offsetY + clipHeight > toBitsHeight) {
    return false;
  }

  if ((fromBitsWidth & 1) || (toBitsWidth & 1) (clipX & 1) ||
      (clipWidth & 1) || (offsetX & 1)) {
    return false;
  }    

  src = (unsigned short *)fromBits;
  dst = (unsigned short *)toBits;

  srcScanLine16 = fromBitsWidth * 2;
  dstScanLine16 = toBitsWidth * 2;

  firstWord16 = srcScanLine16 * clipY + clipX * 2;
  lastWord16 = srcScanLine16 * clipY + (clipX + clipWidth) * 2;
  to = dst + dstScanLine16 * offsetY + offsetX;
  for (line = 0; line < clipHeight; line++) {
    from = firstWord16;
    while (from < lastWord16) {
      pixel = *from++;
      *to++ = (((pixel>>1)&0x7FE0) |
	       ((pixel&0x1f)));
    }
    firstWord16 += srcScanLine16;
    lastWord16 += srcScanLine16;
    to += dstScanLine16;
  }
    
  return true;
}
#endif
#if 0
/* who is responsible for BaseHeader? */
static int
offScreenToBits(WINDOWHD offScreen,
		int *bits, int bitsWidth, int bitsHeight, int clipX,
		int clipY, int clipWidth, int clipHeight,
		int offsetX, int offsetY)
{
  OFFSCRNINFO info;
  int scanLine16;
  int firstWord16;
  int lastWord16;
  unsigned short *srcPixMap;
  unsigned short *to;
  unsigned short pixel;

  int i;
  int j;
  
  if (GetOffscreenInfo(offScreen, &info) == false) {
    printf("get offscreen info\n");
    success(false);
    return false;
  }

  if (clipY < 0 || clipX < 0 ||
      clipWidth > info.rect.width ||
      clipHeight > info.rect.height) {
    success(false);
    return false;
  }

  if (offsetY < 0 || offsetX < 0 ||
      offsetX + clipWidth > bitsWidth ||
      offsetY + clipHeight > bitsHeight) {
    success(false);
    return false;
  }      

  srcPixMap = (unsigned short *)GetPixMapAdrOffscreen(offScreen);
  to = (unsigned short *)bits;

  srcScanLine16 = bytesPerLine(info.rect.width, 16);
  firstWord16 = srcScanLine16 * clipY + bytesPerLineRD(clipX, 16);
  lastWord16 = srcScanLine16 * clipY + bytesPerLine(clipX, 16);

  dstScanLine16 = bytesPerLine(bitsWidth, 16);
  for (line = clipY; line < clipY + clipHeight; line++) {
    to = bits + dstScanLine16 * offsetY + bytesPerLineRD(offsetX, 16);
    from = srcPixMap
      for (; from 
    pixel = 

      srcPixMap[i+1];
    to[i] = (((pixel>>1)&0x7FE0) |
	     ((pixel&0x1f)));
    pixel = srcPixMap[i];
    to[i+1] = (((pixel>>1)&0x7FE0) |
	       ((pixel&0x1f)));
  }
  return true;
}
}
#endif

#if 0
int
sqZaurusCameraActivate(int slot)
{
  CAMERA *camera;

  if ((camera = getCameraStruct(slot)) == NULL) {
    success(false);
    return false;
  }

  if (ActivateCamera(camera) != SUCCESS) {
    printf("activation of the camera\n");
    success(false);
    return false;
  }

  return true;
}


int
sqZaurusCameraDeactivate(int slot)
{
  CAMERA *camera;
  
  if ((camera = getCameraStruct(slot)) == NULL) {
    return false;
  }
  
  if (DeactivateCamera(camera) != SUCCESS) {
    ;
  }
  return true;
}

#endif

