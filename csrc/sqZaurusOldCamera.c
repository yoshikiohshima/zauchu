/*
  Digital camera support code for Squeak/Zaurus
  Written by OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)

*/

#include "sq.h"
#include <cameralb.h>
#include <offscrn.h>

#define HEADMARGIN 16
#define WINDOWWIDTH 640
#define WINDOWHEIGHT 480

#define bytesPerLine(width, depth)	((((width)*(depth) + 31) >> 5) << 2)
#define bytesPerLineRD(width, depth)	((((width)*(depth)) >> 5) << 2)

char *printf(char *fmt, ...);

CAMERA *cameras[] = {NULL, NULL};
WINDOWHD oldOffScreen = 0;

static int
offScreenToBits(int *bits, WINDOWHD offScreen,
		int width, int height)
{
  OFFSCRNINFO info;
  int scanLine16;
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

  if (info.rect.width != width ||
      info.rect.height != height) {
    printf("offscreen size\n");
    success(false);
    return false;
  }

  srcPixMap = (unsigned short *)GetPixMapAdrOffscreen(offScreen);
  to = (unsigned short *)bits;
  scanLine16 = bytesPerLine(width, 16); /* width * 2 */
  /* assume height and width is multiple of 2 */
  for (i = 0; i < height * width; i += 2) {
    pixel = srcPixMap[i+1];
    to[i] = (((pixel>>1)&0x7FE0) |
	     ((pixel&0x1f)));
    pixel = srcPixMap[i];
    to[i+1] = (((pixel>>1)&0x7FE0) |
	       ((pixel&0x1f)));
  }
  return true;
}

int
ioCameraAvailable()
{
  CAMERANAME cname;

  if (GetCameraInfo(CAMERA_SOCKET16, &cname) == SUCCESS) {
    return 1;
  }
  
  if (GetCameraInfo(CAMERA_SOCKET0, &cname) == SUCCESS) {
    return 2;
  }
  
  success(false);
  return 0;
}

int
ioCameraStart(int slot, int width, int height, double zoom, int depth)
{
  OFFSCRNINFO info;
  RECTANGLE size;
  CAMERA *camera;
  int APIzoom;

  if (depth != 16) {
    printf("camera depth should be 16\n");
    success(false);
    return false;
  }

  switch (slot) {
  case 1:
    camera = OpenCamera(CAMERA_SOCKET16);
    if (camera == NULL) {
      printf("opening camera\n");
      success(false);
      return false;
    }
    cameras[0] = camera;
    break;
  case 2:
    camera = OpenCamera(CAMERA_SOCKET0);
    if (camera == NULL) {
      printf("opening camera\n");
      success(false);
      return false;
    }
    cameras[1] = camera;
    break;
  default:
    success(false);
    return false;
  }

  if (0.9 < zoom && zoom < 1.1) {
    APIzoom = 1;
  } else if (1.9 < zoom && zoom < 2.1) {
    APIzoom = 2;
  } else if (1.5 < zoom && zoom < 1.7) {
    APIzoom = 1600;
  } else {
    APIzoom = 1;
  }

  size.left = 0;
  size.top = 0;
  size.width = width;
  size.height = height;
  if (SetConfigCamera(camera, CAMCON_SIZE|CAMCON_IS_AVAILABLE,
		      &size, (ULONG)APIzoom) != SUCCESS) {
    printf("SetConfigCamera\n");
    success(false);
    return false;
  }
  SetConfigCamera(camera, CAMCON_ZOOM, (ULONG)APIzoom);
  
  if (ActivateCamera(camera) != SUCCESS) {
    printf("activation of the camera\n");
    success(false);
    return false;
  }

  if (oldOffScreen != 0) {
    if (GetOffscreenInfo(oldOffScreen, &info) == false) {
      printf("get offscreen info\n");
      success(false);
      return false;
    }

    if (info.rect.width != width || info.rect.height != height) {
      DisposeOffscreen(oldOffScreen);
      oldOffScreen = 0;
    }
  }

  if (oldOffScreen == 0) {
    if ((oldOffScreen = NewOffscreen(&size, 16, 2)) == 0) {
      printf("allocation of off screen\n");
      success(false);
      return false;
    }
  }

  return true;
}

int
ioCameraTakePicture(int slot, int *bits,
		    int width, int height,
		    int buffering,
		    int bufferL, int bufferR,
		    int bufferT, int bufferB)
{
  CAMERA *camera;
  RECTANGLE size;

  if (slot < 1 || 2 < slot) {
    printf("range of slot\n");
    success(false);
    return false;
  }

  camera = cameras[slot-1];
  if (camera == NULL) {
    success(false);
    return false;
  }

  if (buffering) {
    size.left = bufferL;
    size.top = bufferT + HEADMARGIN;
    size.width = bufferR - bufferL;
    size.height = bufferB - bufferT;

    if (SetConfigCamera(camera, CAMCON_DMAMODE, CAMCON_DMA_START, &size, 1) != SUCCESS) {
      printf("SetConfigCamera for direct mode\n");
      success(false);
      return false;
    }
    
  }

  while (!IsPressedTrigCamera(camera)) {
  }

  StartCaptureCamera(camera);
  
  size.left = 0;
  size.top = 0;
  size.width = width;
  size.height = height;
  if (GetCameraPhoto(camera, oldOffScreen, &size, 1) != SUCCESS) {
    printf("GetCameraPhoto 1\n");
    success(false);
    return false;
  }

  if (offScreenToBits(bits, oldOffScreen, width, height) == false) {
    printf("offScreenToBits\n");
    success(false);
    return false;
  }

  if (buffering) {
    if (SetConfigCamera(camera, CAMCON_DMAMODE, CAMCON_DMA_STOP) != SUCCESS) {
      printf("stop direct mode\n");
      success(false);
      return false;
    }
  }

  return true;
}
    
int
ioCameraPeekPicture(int slot, int *bits, int width, int height)
{
  CAMERA *camera;
  RECTANGLE size;

  if (slot < 1 || 2 < slot) {
    printf("range of slot\n");
    success(false);
    return false;
  }

  camera = cameras[slot-1];
  if (camera == NULL) {
    success(false);
    return false;
  }

  StartCaptureCamera(camera);
  
  size.left = 0;
  size.top = 0;
  size.width = width;
  size.height = height;
  if (GetCameraPhoto(camera, oldOffScreen, &size, 1) != SUCCESS) {
    printf("GetCameraPhoto 1\n");
    success(false);
    return false;
  }

  if (offScreenToBits(bits, oldOffScreen, width, height) == false) {
    printf("offScreenToBits\n");
    success(false);
    return false;
  }

  return true;
}

int
ioCameraRelease(int slot)
{
  CAMERA *camera;

  if (slot < 1 || 2 < slot) {
    printf("range of slot\n");
    success(false);
    return false;
  }

  camera = cameras[slot-1];
  if (camera == NULL) {
    return false;
  }

  if (DeactivateCamera(camera) != SUCCESS) {
    printf("DeactivateCamera\n");
    return false;
  }
  
  if (CloseCamera(camera) != SUCCESS) {
    printf("CloseCamera\n");
    return false;
  }
  cameras[slot-1] = NULL;
  return true;
}

int
ioCameraInFocus(int slot)
{
  CAMERA *camera;

  if (slot < 1 || 2 < slot) {
    printf("slot out of range\n");
    success(false);
    return false;
  }

  camera = cameras[slot-1];
  if (camera == NULL) {
    success(false);
    return false;
  }

  return IsFocusedCamera(camera);
}

