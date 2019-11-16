/*
  Display support and miscellaneous code for Squeak/Zaurus
  Written by: OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)

  * Direct VRAM access support for VGA and QVGA screen written by
    Ishitani-san (T.I.) (ishi@gray.psd.nara.sharp.co.jp) 
  
*/

#define USE_VRAM_ACCESS	/* T.I. */
#define USE_FASTTICK
#define USE_LOOKUP_EVENTS
#undef USE_16BIT_DEPTH_COLOR_TABLE /* this makes bitblt slower and kicks the compiler's bug */

#include "sq.h"

#include <cameralb.h>
#include <offscrn.h>
#include <sndtool.h>
#include <devmodel.h>

#include <lcdzdm.h>		/* T.I. */
#include <sysiolib.h>	/* T.I. for AC */
#include <battlib.h>

short headMargin;
extern int fullScreenFlag;
char fastTickIsUsed;

#define HEADMARGIN 16


static int windowWidth;
static int windowHeight;
static int dispImageSize;

#ifdef USE_VRAM_ACCESS	/* T.I. */
int drawdisp_max_line; 	/* •`‰æŽž‚ÌÅ‰ºLine */
int vram_width;
#endif	/* T.I. */

extern unsigned char *memory;

extern int interruptPending;
extern int interruptCheckCounter;

char squeakStarted = 0;
unsigned char squeakTerminated = 0;
unsigned long formSetUpTime;
unsigned long startUpTime;

APLFORMPTR sqAplForm;
WINDOWHD sqWnHandle;

#ifndef USE_VRAM_ACCESS
RECTANGLE rect0;
IMAGEDATA *stImage;
#endif

COLOR sqBlack;
COLOR sqWhite;


unsigned int memorySize; /*  = ((4096+512)*1024); */
unsigned int initialHeapSize;
#define STROOM 100000
/* #define MAXHEAP (5*1024*1024-(256*1024)) */
/* #define MAXHEAP (3*1024*1024-(768*1024)) */

void displayImage1To16(int *fromImageData,
		       int width, int height,
		       int affectedL, int affectedT,
		       int affectedR, int affectedB);

void displayImage8To16(int *fromImageData,
		       int width, int height,
		       int affectedL, int affectedT,
		       int affectedR, int affectedB);

void displayImage16To16(int *fromImageData,
			int width, int height,
			int affectedL, int affectedT,
			int affectedR, int affectedB);

void setupPrintf(APLFORMPTR AplForm);
char *printf(char *fmt, ...);
int convertToSqueakTime(TIME *t);
void aioPoll();
void closeAllSerialPorts();


#define IMAGE_NAME_SIZE DFNAME_LEN
char imageName[IMAGE_NAME_SIZE];		/* full path to image */
char shortImageName[IMAGE_NAME_SIZE];	/* just the base name */
char vmPath[IMAGE_NAME_SIZE];		/* full path to interpreter's directory */

#define bytesPerLine(width, depth)	((((width)*(depth) + 31) >> 5) << 2)
#define bytesPerLineRD(width, depth)	((((width)*(depth)) >> 5) << 2)

unsigned short stColors16[256];

#ifdef USE_16BIT_DEPTH_COLOR_TABLE
unsigned short stColors1516[32768];
#endif

#define MAXPOLLSPERSEC 33

/*** Variables -- Event Recording ***/
#define KEYBUF_SIZE 64

int keyBuf[KEYBUF_SIZE];	/* circular buffer */
int keyBufGet= 0;		/* index of next item of keyBuf to read */
int keyBufPut= 0;		/* index of next item of keyBuf to write */
int keyBufOverflows= 0;		/* number of characters dropped */

int buttonState= 0;		/* mouse button and modifier state when mouse
				   button went down or 0 if not pressed */
int mousePositionX;
int mousePositionY;

static char blueButtonPressed = 0;

#ifndef WITHOUT_KEYPAD
static char *keypadPtr = NULL;
static char keypadinit = false;
#endif

char shouldNotPowerOff;

extern CAMERA *cameras[];
extern WINDOWHD offScreen;


int
clipboardSize(void)
{
  USHORT binSize;
  USHORT textSize;
  CLIPID clipid = CP_APL_CUT_BUF;
  
  success(!CPCheckLength(clipid, &binSize,&textSize));
  return binSize;
}

int
clipboardReadIntoAt(int count, int byteArrayIndex, int startIndex)
{
  int len;
  
  len = CPReadBin(CP_APL_CUT_BUF, (char*)byteArrayIndex + startIndex, count);
  
  success(len == count);
  return 0;
}
  
int
clipboardWriteFromAt(int count, int byteArrayIndex, int startIndex)
{
  unsigned char* pos;
  
  CPClear(CP_APL_CUT_BUF);

  CPWriteText(CP_APL_CUT_BUF, (char*)byteArrayIndex + startIndex, count);
  pos = CPWriteBin(CP_APL_CUT_BUF, (char*)byteArrayIndex + startIndex, count);

  success((int)pos != 0);
  return 0;
}

int
ioBeep()
{
  PlaySysAlert(SYS_KEYDOWN_SND);
  /* PlayBeep(); */
}

void
ioExitBody()
{
  sqNetworkShutdown();
  closeAllSerialPorts();

#if 0 /* USE_VRAM_ACCESS */
  if (stImage != NULL) {
    DisposeHeapPtr(stImage);
    stImage = NULL;
  }
#endif

  if (memory != NULL) {
    DisposeHeapPtr(memory);
    memory = NULL;
  }

  sqZaurusCameraRelease(1);
  sqZaurusCameraRelease(2);

  sqZaurusSoundShutdown();

  DisableFastTick();

}

int
ioExit()
{
  MESSAGE m;

  ioExitBody();

  m.CommonMsg.MessageID = AppliProcEndReqID;
  m.AppliProcEndReq.MessageID = AppliProcEndReqID;
  m.AppliProcEndReq.AplFilter = AF_THIS_APPLI;
  GetMessage(&m);
}

int
ioForceDisplayUpdate()
{
  int more = 1;
  do {
    more = handleEvents();
  } while (more);
}

int
ioFormPrint(int bitsAddr, int width, int height, int depth,
	    double hScale, double vScale, int landscapeFlag)
{
  return false;
}

int
ioSetFullScreen(int fullScreen)
{
  if (fullScreenFlag != fullScreen) {
    if (fullScreen) {
      headMargin = 0;
      fullScreenFlag = true;
    } else {
      headMargin = HEADMARGIN;
      fullScreenFlag = false;
      FormDefRedisp(sqAplForm, sqWnHandle);
    }
    drawdisp_max_line = windowHeight - headMargin;
    fullDisplayUpdate();
    
  }
  return fullScreen;
}

int
ioSetDisplayMode(int width, int height, int depth, int fullscreenFlag)
{
  return true;
}

int
ioHasDisplayDepth(int depth)
{
  if (depth == 1 || depth == 8 || depth == 16) {
    return true;
  }
  return false;
}


int
ioGetButtonState()
{
  ioProcessEvents();
  return buttonState;
}

int
ioGetKeystroke()
{
  int keystate;
  ioProcessEvents(); /* needed? */

  if (keyBufGet == keyBufPut)
    return -1;

  keystate = keyBuf[keyBufGet];
  keyBufGet = (keyBufGet + 1) % KEYBUF_SIZE;

  buttonState = ((keystate >> 5) & 0xF8) | (buttonState & 0x7);
  return keystate;
}

int
ioMSecs()
{
  static unsigned long disabledTime = 0;
  unsigned long now;
  if (fastTickIsUsed) {
    return ((GetFastTickCount() + disabledTime) * 10) & 0x1fffffff;
  } else {
    now = GetSystemTickCount();
    disabledTime += now * 10;
    return (now * 100) & 0x1fffffff;
  }
}

int
ioMousePoint()
{
  ioProcessEvents();
  return (mousePositionX << 16) | (0xffff & mousePositionY);
}

int
ioPeekKeystroke()
{
  int keystate;
  
  ioProcessEvents(); /* needed? */
  if (keyBufGet == keyBufPut)
    return -1;  /* keystroke buffer is empty */

  keystate = keyBuf[keyBufGet];
  /* set modifer bits in buttonState to reflect the last keystroke peeked at */
  buttonState = ((keystate >> 5) & 0xF8) | (buttonState & 0x7);
  return keystate;
}

int
recordKeystroke(char *str)
{
  int len = strlen(str);
  int i = 0;
  unsigned char c;

  for (; c = *str; str++) {

    if (c >= 0x80) {
      str++;
      continue;
    }

    if (c == KC_CURSOR_LEFT) {
      c = KC_CURSOR_RIGHT;
    } else if (c == KC_CURSOR_RIGHT) {
      c = KC_CURSOR_LEFT;
    }
    
    keyBuf[keyBufPut] = c;

    keyBufPut = (keyBufPut + 1) % KEYBUF_SIZE;
    if (keyBufGet == keyBufPut) {
      /* buffer overflow; drop the last character */
      keyBufGet = (keyBufGet + 1) % KEYBUF_SIZE;
      keyBufOverflows++;
    }
  }
}

/* This table maps the modifier key bits of
   Zaurus external keyboard to 4 Squeak modifier bits.
   (The Zaurus KINOU is mapped to command, and ctrl+shift is
   mapped to option.
        Zaurus bits:    <shift><KINOU><?><control>
	X bits:		<meta><control><shift-lock><shift>
	Squeak bits:	<command><option><control><shift>
*/
char modifierMap[16]= {
  0, 2, 2, 2,
  8, 8, 8, 8,
  1, 1, 1, 1,
  4, 4, 4, 4
};

void
recordKeyDrvDown(int key, int attr)
{

  if (key == 255) {
    return;
  }
  
  if ((112 <= key && key <= 120) ||
      (122 <= key) && (key <= 123)) {
    return;
  }

  if (44 <= key && key <= 47) {
    if (attr & 8) {
      key += 16;
    }
  } else if (49 <= key && key <= 57) {
    if (attr & 8) {
      key -= 16;
    }
  } else if (58 <= key && key <= 59) {
    if (attr & 8) {
      key -= 16;
    }
  } else if (key == 64) {
    if (attr & 8) {
      key = 96;
    }
  } else if (65 <= key && key <= 90) {
    if (!(attr & 8)) {
      key += 32;
    }
  } else if (91 <= key && key <= 94) {
    if (attr & 8) {
      key += 32;
    }
  } else if (key == 96) {
    if (attr & 8) {
      key = 95;
    }
  } else if (key == KC_CURSOR_LEFT) {
    key = KC_CURSOR_RIGHT;
  } else if (key == KC_CURSOR_RIGHT) {
    key = KC_CURSOR_LEFT;
  } else if (key == 121) {
    key = 32;
  } else if (key == 124) {
    key = 13;
  }

  keyBuf[keyBufPut] = key | (modifierMap[attr & 0xF] << 8);
  keyBufPut = (keyBufPut + 1) % KEYBUF_SIZE;
  if (keyBufGet == keyBufPut) {
    /* buffer overflow; drop the last character */
    keyBufGet = (keyBufGet + 1) % KEYBUF_SIZE;
    keyBufOverflows++;
  }
}

void
toggleBlueButton()
{
  MESSAGE k;
  short check;

  if (blueButtonPressed) {
    check = CHECK_OFF;
    blueButtonPressed = 0;
  } else {
    check = CHECK_ON;
    blueButtonPressed = 1;
  }

  if (!fullScreenFlag) {
    SetCtrlProperty(BlueButton, VALUE_PID, sValue, check,
		    sqAplForm->Control);
    k.CommonMsg.MessageID = RedispID;
    ControlCall(BlueButton, sqAplForm->Control, &k);
  }

}

void
recordPenOperation(int x, int y, unsigned char attr, char down)
{
  int stButtons = 0;

  if (down) {
    stButtons = 4;
    if (blueButtonPressed) {
      stButtons = 1;
    } else if (attr & MODIF_2ndF) {
      stButtons = 2;
    }
  } else {
    if (blueButtonPressed) {
      toggleBlueButton();
    }
  }
  
  if ((x < 0) || (y < headMargin * 4) ||
      (x >= (windowWidth * 4)) ||
      (y >= (windowHeight * 4))) {
    return;
  }

  mousePositionX = x / 4;
  mousePositionY = (y / 4) - headMargin;
  /* button state: low three bits are mouse buttons; next 4 bits are modifier bits */
  buttonState = stButtons & 0x7;
}

void
ioPowerManagement(int allowPowerOff)
{
  shouldNotPowerOff = !allowPowerOff;
}

int
ioPowerBatteryRemaining()
{
  return (int)GetBatteryCondition();
}
  

static void
powerManagement()
{
  static int preventPowerOff = 600;
  unsigned char battery;
  if (--preventPowerOff <= 0) {
    preventPowerOff = 600;
    
    if ((GetACAdaptorCondition() == AC_ADAPTOR_IN) || shouldNotPowerOff) {
      ClearApOffCount();
    } else {
      battery = GetBatteryCondition();
      if ((BATTERY_REST_FATAL <= battery) && (battery <= BATTERY_REST_20_PER)) {
	shouldNotPowerOff = false;
      }
    }
  }
}
  

int
handleEvents()
{
  MESSAGE m;
  unsigned short msg;

#ifndef WITHOUT_KEYPAD
  short keypad;
#endif

  aioPoll();

  while (1) {
#ifdef USE_LOOKUP_EVENTS
    if (LookEventMsg(&m) == IOCS_ERROR) {
      /*
      printf(" no pending events ");
      */
      return 0;
    }
#endif

    m.CommonMsg.MessageID = NullMessageID;
    GetMessage(&m);
    msg = m.CommonMsg.MessageID;
    /*
    printf("%d,", msg);
    */
#ifndef WITHOUT_KEYPAD  
    if (strlen(keypadPtr) > 0) {
      recordKeystroke(keypadPtr);
      *keypadPtr = '\0';
    }
#endif

    switch (msg) {
    case SystemTickID:
#ifndef WITHOUT_KEYPAD    
      ControlCall(KEYPAD00, sqAplForm->Control, &m);
#endif
      powerManagement();
      return 0;
    case FastTickID:
      return 0;
      /* break; */
    case PenDownID:
#ifndef WITHOUT_KEYPAD
      {
	int x = m.PenDown.PositionX;
	int y = m.PenDown.PositionY;
	unsigned short h;	/* T.I. */
	if (m.PenDown.WindowNo != sqWnHandle) {
	  ControlCall(KEYPAD00, sqAplForm->Control, &m);
	} else if (!fullScreenFlag &&(y < headMargin * 4 - 1)) {
	  /* for VGA/QVGA T.I. */
	  unsigned short keybtn_x, keybtn_w;
	  GetCtrlProperty(KeyState, LEFT_X_PID, usValue,
			  keybtn_x, sqAplForm->Control, USHORT);
	  GetCtrlProperty(KeyState, WIDTH_PID, usValue,
			  keybtn_w, sqAplForm->Control, USHORT);

	  if (keybtn_x < x && x <= keybtn_x + keybtn_w) {
	    MESSAGE k;
	    GetCtrlProperty(KeyState, VALUE_PID, sValue,
			    keypad, sqAplForm->Control, short);
	    if (keypad == CHECK_ON) {
	      k.CommonMsg.MessageID = ToolCloseID;
	      ControlCall(KEYPAD00, sqAplForm->Control, &k);
	      SetCtrlProperty(KeyState, VALUE_PID, sValue, CHECK_OFF,
			      sqAplForm->Control);
#ifdef USE_VRAM_ACCESS
	      drawdisp_max_line = windowHeight - headMargin;	/* T.I. */
#endif
	      fullDisplayUpdate();
	    } else {
	      k.CommonMsg.MessageID = ToolOpenID;
	      ControlCall(KEYPAD00, sqAplForm->Control, &k);
	      SetCtrlProperty(KeyState, VALUE_PID, sValue, CHECK_ON,
			      sqAplForm->Control);
	      if (!keypadinit) {
		k.CommonMsg.MessageID = BoardChangeID;
		k.BoardChange.BoardNo = QWERTY_PAD_ID;
		ControlCall(KEYPAD00, sqAplForm->Control, &k);
		k.CommonMsg.MessageID = PadModeChangeID;
		k.PadModeChange.PadMode = HANKAKU_STATE | EI_STATE;
		ControlCall(KEYPAD00, sqAplForm->Control, &k);
		keypadinit = true;
	      }
#ifdef USE_VRAM_ACCESS
	      GetCtrlProperty(KEYPAD00, PAD_HEIGHT_PID, usValue, h,
	           sqAplForm->Control, USHORT);
	      drawdisp_max_line = windowHeight - ((h+1)/4) - headMargin;	/* T.I. */
#endif

	    }
	    k.CommonMsg.MessageID = RedispID;
	    ControlCall(KeyState, sqAplForm->Control, &k);
	  }
	} else {
	  recordPenOperation(m.PenDown.PositionX, m.PenDown.PositionY, m.PenDown.PenAttr, 1);
	}
      }
#else
      {
	int x = m.PenDown.PositionX;
	int y = m.PenDown.PositionY;
	
	if (!(y < headMargin * 4 - 1)) {
	  recordPenOperation(x, y, m.PenDown.PenAttr, 1);
	}
      }
#endif  
      break;
    case PenStillDownID:
#ifndef WITHOUT_KEYPAD
      if (m.PenStillDown.WindowNo != sqWnHandle) {
	ControlCall(KEYPAD00, sqAplForm->Control, &m);
      } else {    
	recordPenOperation(m.PenStillDown.PositionX, m.PenStillDown.PositionY, m.PenStillDown.PenAttr, 1);
      }
#else
      recordPenOperation(m.PenStillDown.PositionX, m.PenStillDown.PositionY, m.PenStillDown.PenAttr, 1);
#endif
      break;
    case PenUpID:
#ifndef WITHOUT_KEYPAD
      if (m.PenUp.WindowNo != sqWnHandle) {
	ControlCall(KEYPAD00, sqAplForm->Control, &m);
      } else {
	recordPenOperation(m.PenUp.PositionX, m.PenUp.PositionY, 0, 0);
      }
#else
      recordPenOperation(m.PenUp.PositionX, m.PenUp.PositionY, 0, 0);
#endif
      break;
    case KeyDownID:
      {
	static char c[2] = {0, 0};
	short v;
      
	switch (v = m.KeyDown.KeyCode) {
	case KC_CURSOR_RIGHT:
	case KC_CURSOR_LEFT:
	case KC_CURSOR_UP:
	case KC_CURSOR_DOWN:
	  c[0] = (char)v;
	  recordKeystroke(c);
	  break;
	case KC_DECIDE:
	  c[0] = '\r';
	  recordKeystroke(c);
	  break;
	case KC_BACK:
	  c[0] = 0x08;
	  recordKeystroke(c);
	  break;
	case KC_MENU:
	  toggleBlueButton();
	  break;
	default:
	  break;
	}
      }
      break;
    case KeyDrvDownID:
      recordKeyDrvDown(m.KeyDrvDown.KeyCode, m.KeyDrvDown.KeyAttr);
      break;
    case OnBreakID:
      interruptPending = true;
      interruptCheckCounter = 0;
      break;
#ifndef WITHOUT_KEYPAD
    case AppliActivateID:
    case KeyDrvStatusID:
      {
	unsigned short h;
	ControlCall(KEYPAD00, sqAplForm->Control, &m);
#ifdef USE_VRAM_ACCESS
	GetCtrlProperty(KEYPAD00, PAD_HEIGHT_PID, usValue, h, sqAplForm->Control, USHORT);
	drawdisp_max_line = windowHeight - (h+1) / 4 - headMargin;
#endif
	fullDisplayUpdate();
	break;
      }
#endif
    }
  }
  return 0;
}

int
ioProcessEvents()
{
  static unsigned long nextPollTick = 0;

  if ((unsigned long)ioMSecs() > nextPollTick) {
    while (handleEvents()) {
      ;
    }

    nextPollTick = ioMSecs() + (1000 / MAXPOLLSPERSEC);
  }
  return 0;
}

int
ioRelinquishProcessorForMicroseconds(int microSeconds)
{
  /*self_sleep((microSeconds / 100) * 64);*/  /* need to be checked */
  /* "(microSeconds / 100) * 64" result in 10 seconds sleep */
  /* printf("relinquish: %d\n", microSeconds); */
  self_sleep(microSeconds / 1000);
}

int
ioScreenSize()
{
  return (windowWidth << 16) | ((windowHeight - headMargin) & 0xffff);
}

int
ioSeconds()
{
  TIME t;

  LocalTimeReadLtd(&t, CURRENT_CITY_ID);

  return convertToSqueakTime(&t);
}

int
ioSetCursor(int cursorBitsIndex, int offsetX, int offsetY)
{
  /* may need to be written */
  return 0;
}

int
ioSetCursorWithMask(int cursorBitsIndex, int cursorMaskIndex, int offsetX, int offsetY)
{
  /* may need to be written */
  return 0;
}


int
ioShowDisplay(int dispBitsIndex, int width, int height, int depth,
		  int affectedL, int affectedR, int affectedT, int affectedB)
{
  if(affectedR <= affectedL || affectedT >= affectedB)
    return 1;

  if (depth != 1 && depth != 8 && depth != 16) {
    printf("unsupported depth");
    ioExit();
    return 0;
  }

  /* this can happen after resizing the window */
  if (affectedR > width) affectedR= width;
  if (affectedB > height) affectedB= height;

  if (depth == 1) {
    displayImage1To16((int *)dispBitsIndex, 
		      width, height,
		      affectedL, affectedT, affectedR, affectedB);
  }
  else if (depth == 8) {
    displayImage8To16((int *)dispBitsIndex, 
	 	      width, height,
		      affectedL, affectedT, affectedR, affectedB);
  } else if (depth == 16) {
    displayImage16To16((int *)dispBitsIndex, 
		       width, height,
		       affectedL, affectedT, affectedR, affectedB);
  }
  
  return 0;
}

void
displayImage1To16(int *fromImageData,
		  int width, int height,
		  int affectedL, int affectedT,
		  int affectedR, int affectedB)
{
  int scanLine1 = bytesPerLine(width, 1);  
  int scanLine16 = bytesPerLine(width, 16); 

  int firstWord1 = scanLine1 * affectedT + bytesPerLineRD(affectedL, 1);
  int lastWord1 = scanLine1 * affectedT + bytesPerLine(affectedR, 1);
  int line;
  int aL = (affectedL>>5)<<5;
  int aR = ((affectedR+31)>>5)<<5;

#ifndef USE_VRAM_ACCESS		/* T.I. */
  int aW = aR - aL;
  int aH = affectedB - affectedT;
  
  int i;

  int rline = dispImageSize / aW;
  int repeat = aH / rline;
  int rT = affectedT;
  int rB = affectedT;
  unsigned short *to;
  for (i = 0; i <= repeat; i++) {
    if (i < repeat) {
      rT = affectedT + rline * i;
      rB = rT + rline;
    } else {
      rT = rB;
      rB = affectedB;
      rline = rB - rT;
      if (rline == 0) {
	return;
      }
    }
    
    stImage->DataSize = sizeof(IMAGEDATA) +
      sizeof(short) * (rline * aW) - sizeof(short);
    stImage->Width = aW * 4 - 1;
    stImage->Height = rline * 4 -1;
    stImage->DataType = 0;
    stImage->Dot = 4;
    stImage->ColorDepth = 16;
    
    to = &(stImage->PixMap[0]);
    for (line = rT; line < rB; line++) {
      int *from = (int*)(((char*)fromImageData) + firstWord1);
      int *limit = (int*)(((char*)fromImageData) + lastWord1);
      while (from < limit) {
	int m;
	unsigned int mask = 0x80000000;
	unsigned int swapedFrom = *from;
	for (m = 0; m < 32; m++) {
	  *to = ~-!!(mask & swapedFrom);
	  mask >>= 1;
	  to++;
	}
	from++;
      }
      firstWord1 += scanLine1;
      lastWord1 += scanLine1;
    }
    rect0.left = aL * 4;
    rect0.top = (rT + headMargin) * 4;
    rect0.width = aW * 4 - 1;
    rect0.height = rline * 4 - 1;
    DispImage(sqWnHandle, &rect0, FSET, stImage, NORM_SCALE, NULL);
  }
#else	/* USE_VRAM_ACCESS T.I. */
  ACSVRAM acsVram;
  unsigned char *vramTopPtr = GetVramTop();
  int scanVramLine = vram_width * sizeof(short);
  int firstVramWord = scanVramLine * affectedT + aL * sizeof(short);
  int maxLine;

  vramTopPtr += scanVramLine * headMargin;

  maxLine = (affectedB < drawdisp_max_line) ? affectedB : drawdisp_max_line;

  acsVram = EnableAccessVRAM();

  for (line = affectedT; line < maxLine; line++) {
    int *from = (int*)(((char*)fromImageData) + firstWord1);
    int *limit = (int*)(((char*)fromImageData) + lastWord1);
    unsigned short *to = (unsigned short *)(vramTopPtr + firstVramWord);
    while (from < limit) {
      int m;
      unsigned int mask = 0x80000000;
      unsigned int swapedFrom = *from;
      for (m = 0; m < 32; m++) {
	*to = ~-!!(mask & swapedFrom);
	mask >>= 1;
	to++;
      }
      from++;
    }
    firstWord1 += scanLine1;
    lastWord1 += scanLine1;
    firstVramWord += scanVramLine;
  }

  DisableAccessVRAM(acsVram);
#endif	/* USE_VRAM_ACCESS T.I. */
}

void
displayImage8To16(int *fromImageData,
		  int width, int height,
		  int affectedL, int affectedT,
		  int affectedR, int affectedB)
{
  int scanLine8 = bytesPerLine(width, 8);
  int scanLine16 = bytesPerLine(width, 16);

  int firstWord8 = scanLine8 * affectedT + bytesPerLineRD(affectedL, 8);
  int lastWord8 = scanLine8 * affectedT + bytesPerLine(affectedR, 8);
  int line;
  int aL = (affectedL>>2)<<2;
  int aR = ((affectedR+3)>>2)<<2;

#ifndef USE_VRAM_ACCESS		/* T.I. */
  int aW = aR - aL;
  int aH = affectedB - affectedT;
  unsigned short *to;

  int rline = dispImageSize / aW;
  int repeat = aH / rline;
  int i;
  int rT = affectedT;
  int rB = affectedT;
  for (i = 0; i <= repeat; i++) {
    if (i < repeat) {
      rT = affectedT + rline * i;
      rB = rT + rline;
    } else {
      rT = rB;
      rB = affectedB;
      rline = rB - rT;
      if (rline == 0) {
	return;
      }
    }

    stImage->DataSize = sizeof(IMAGEDATA) +
      sizeof(short) * (rline * aW) - sizeof(short);
    stImage->Width = aW * 4 - 1;
    stImage->Height = rline * 4 - 1;
    stImage->DataType = 0;
    stImage->Dot = 4;
    stImage->ColorDepth = 16;
    
    to = &(stImage->PixMap[0]);
    for (line = rT; line < rB; line++) {
      unsigned char *from = ((unsigned char*)fromImageData) + firstWord8;
      unsigned char *limit = ((unsigned char*)fromImageData) + lastWord8;
      while (from < limit) {
	*to++ = stColors16[from[3]];
	*to++ = stColors16[from[2]];
	*to++ = stColors16[from[1]];
	*to++ = stColors16[from[0]];
	from += 4;
      }
      firstWord8 += scanLine8;
      lastWord8 += scanLine8;
    }
    
    rect0.left = aL * 4;
    rect0.top = (rT + headMargin) * 4;
    rect0.width = aW * 4 - 1;
    DispImage(sqWnHandle, &rect0, FSET, stImage, NORM_SCALE, NULL);
  }
#else	/* USE_VRAM_ACCESS T.I. */
  ACSVRAM acsVram;
  unsigned char *vramTopPtr = GetVramTop();
  int scanVramLine = vram_width * sizeof(short);
  int firstVramWord = scanVramLine * affectedT + aL * sizeof(short);
  int maxLine;

  vramTopPtr += scanVramLine * headMargin;
  
  maxLine = (affectedB < drawdisp_max_line) ? affectedB : drawdisp_max_line;
  
  acsVram = EnableAccessVRAM();

  for (line = affectedT; line < maxLine; line++) {
    unsigned char *from = ((unsigned char*)fromImageData) + firstWord8;
    unsigned char *limit = ((unsigned char*)fromImageData) + lastWord8;
    unsigned short *to = (unsigned short *)(vramTopPtr + firstVramWord);
    while (from < limit) {
      *to++ = stColors16[from[3]];
      *to++ = stColors16[from[2]];
      *to++ = stColors16[from[1]];
      *to++ = stColors16[from[0]];
      from += 4;
    }
    firstWord8 += scanLine8;
    lastWord8 += scanLine8;
    firstVramWord += scanVramLine;
  }

  DisableAccessVRAM(acsVram);
#endif	/* USE_VRAM_ACCESS T.I. */
}

void
displayImage16To16(int *fromImageData,
		    int width, int height,
		    int affectedL, int affectedT,
		   int affectedR, int affectedB)
{
  int scanLine16 = bytesPerLine(width, 16);

  int firstWord16 = scanLine16 * affectedT + bytesPerLineRD(affectedL, 16);
  int lastWord16 = scanLine16 * affectedT + bytesPerLine(affectedR, 16);

  int aL = (affectedL>>1)<<1;
  int aR = ((affectedR+1)>>1)<<1;
  int aW = aR - aL;
  int aH = affectedB - affectedT;
  
  int line;

#ifndef USE_VRAM_ACCESS		/* T.I. */
  int rline = dispImageSize / aW;
  int repeat = aH / rline;
  int i;
  int rT = affectedT;
  int rB = affectedT;
  unsigned short *to;
  for (i = 0; i <= repeat; i++) {
    if (i < repeat) {
      rT = affectedT + rline * i;
      rB = rT + rline;
    } else {
      rT = rB;
      rB = affectedB;
      rline = rB - rT;
      if (rline == 0) {
	return;
      }
    }

    stImage->DataSize = sizeof(IMAGEDATA) +
      sizeof(short) * (rline * aW) - sizeof(short);
    stImage->Width = aW * 4 - 1;
    stImage->Height = rline * 4 - 1;
    stImage->DataType = 0;
    stImage->Dot = 4;
    stImage->ColorDepth = 16;
    
    to = &(stImage->PixMap[0]);
    for (line = rT; line < rB; line++) {
      unsigned short *from = (unsigned short *)(((unsigned char*)fromImageData) + firstWord16);
      unsigned short *limit = (unsigned short *)(((unsigned char*)fromImageData) + lastWord16);
      /*    unsigned short *to = (unsigned short *)(vramTopPtr + firstVramWord); */
      /* unsigned int *to = (unsigned int *)(vramTopPtr + firstVramWord); */
      unsigned short pixel;
      while (from < limit) {
#ifdef USE_16BIT_DEPTH_COLOR_TABLE
	pixel = *(from+1);
	*to++ = stColors1516[pixel];
	pixel = *(from);
	*to++ = stColors1516[pixel];
#else
	pixel = *(from+1);
	*to++ = (((pixel&0x7FE0)<<1) |
		 ((pixel&0x1f)));
	pixel = *(from);
	*to++ = (((pixel&0x7FE0)<<1) |
		 ((pixel&0x001F)));
#endif
	from += 2;
      }
      firstWord16 += scanLine16;
      lastWord16 += scanLine16;
    }
    
    rect0.left = aL * 4;
    rect0.top = (rT + headMargin) * 4;
    rect0.width = aW * 4 - 1;
    DispImage(sqWnHandle, &rect0, FSET, stImage, NORM_SCALE, NULL);
  }
#else	/* USE_VRAM_ACCESS */
  ACSVRAM acsVram;
  unsigned char *vramTopPtr = GetVramTop();
  int scanVramLine = vram_width * sizeof(short);
  int firstVramWord = scanVramLine * affectedT + aL * sizeof(short);
  int maxLine;

  vramTopPtr += scanVramLine * headMargin;
  
  maxLine = (affectedB < drawdisp_max_line) ? affectedB : drawdisp_max_line;
  
  acsVram = EnableAccessVRAM();

  for (line = affectedT; line < maxLine; line++) {
    unsigned int *from = (unsigned int *)(((unsigned char*)fromImageData) + firstWord16);
    unsigned int *limit = (unsigned int *)(((unsigned char*)fromImageData) + lastWord16);
    /*    unsigned short *to = (unsigned short *)(vramTopPtr + firstVramWord); */
    unsigned int *to = (unsigned int *)(vramTopPtr + firstVramWord);
    unsigned int pixel2;
    unsigned int pixel;
    unsigned int result;
    while (from < limit) {
#ifdef USE_16BIT_DEPTH_COLOR_TABLE
      pixel2 = *from;
      pixel = (unsigned short)pixel2;
      result = stColors1516[pixel];
      result <<= 16;
      result |= stColors1516[(pixel2>>16)];
      *to++ = result;
#else
      pixel2 = *from;
      pixel = (0xFFFF&pixel2) << 16;
      pixel2 >>= 16;
      pixel2 = (((pixel2&0x7FE0)<<1) |
	       ((pixel2&0x1f)));
      pixel = (((pixel&0x7FE00000)<<1) |
		((pixel&0x001F0000)));
      pixel2 |= pixel;
      *to++ = pixel2;
#endif
      from++;
    }
    firstWord16 += scanLine16;
    lastWord16 += scanLine16;
    firstVramWord += scanVramLine;
  }
  
  DisableAccessVRAM(acsVram);
#endif	/* T.I. */
}


/*** Image File Naming ***/

void
recordFullPathForImageName(char * const localImageName)
{
#if 0
  int i;

  strcpy(vmPath, localImageName);
  
  strcpy(imageName, vmPath);

  /* truncate vmPath to dirname */
  for (i= strlen(vmPath); i >= 0; i--)
    if (':' == vmPath[i])
      {
	vmPath[i+1]= '\0';
	return;
      }
  /* this might just work in an emergency... */
  strcpy(imageName, vmPath);
#endif
}

int
imageNameSize(void)
{
  return strlen(imageName);
}

int
imageNameGetLength(int sqImageNameIndex, int length)
{
  char *sqImageName = (char *)sqImageNameIndex;
  int count, i;

  count= strlen(imageName);
  count= (length < count) ? length : count;

  /* copy the file name into the Squeak string */
  for (i= 0; i < count; i++)
    sqImageName[i]= imageName[i];

  return count;
}

int
imageNamePutLength(int sqImageNameIndex, int length)
{
  char *sqImageName = (char *)sqImageNameIndex;
  int count, i;

  count= (IMAGE_NAME_SIZE < length) ? IMAGE_NAME_SIZE : length;

  /* copy the file name into a null-terminated C string */
  for (i= 0; i < count; i++) imageName[i]= sqImageName[i];
  imageName[count]= 0;

  return count;
}

void
imageNotFound(char *imageName)
{
  /* image file is not found */
  printf("image file '%s'", imageName);
  exit(1);
}

/*** VM Home Directory Path ***/

int
vmPathSize(void)
{
  return strlen(vmPath);
}

int
vmPathGetLength(int sqVMPathIndex, int length)
{
  char *stVMPath = (char *)sqVMPathIndex;
  int count, i;

  count= strlen(vmPath);
  count= (length < count) ? length : count;

  /* copy the file name into the Squeak string */
  for (i= 0; i < count; i++)
    stVMPath[i]= vmPath[i];

  return count;
}


int clearProfile(void) { return 0; }
int dumpProfile(void) { return 0; }
int startProfiling(void) { return 0; }
int stopProfiling(void) { return 0; }

int tabletGetParameters(int cursorIndex, int result[]) {return 0;}
int tabletRead(int cursorIndex, int result[]) {return 0;}
int tabletResultSize(void) {return 0;}


void
setupScreenSize()
{
  if (zaurusScreenSize == ZAURUS_SCREEN_VGA) {
    windowWidth = 640;
    windowHeight = 480;
    dispImageSize = 640 * 50; /* must be smaller than 32768 */
#ifdef USE_VRAM_ACCESS
    vram_width = VRAM_WIDTH;	/* MI-EX1 */
#endif
  } else if (zaurusScreenSize == ZAURUS_SCREEN_QVGA) {
    windowWidth = 320;
    windowHeight = 240;
    dispImageSize = 320 * 100; /* must be smaller than 32768 */
#ifdef USE_VRAM_ACCESS
    vram_width = windowWidth;	/* MI-C1 QVGA */
#endif
  }
#ifdef USE_VRAM_ACCESS
  drawdisp_max_line = windowHeight - headMargin;
#endif
}

typedef struct _attr {
  int attrID;
  char *attrStr;
  int attrSize;
} attr;

static attr attrs[4];

static void
setupAttribute()
{
  char *tmp;
  static unsigned char modelName[MODEL_NAME_MAX];
  unsigned long version;

  GetSystemInfo(modelName,&version);

  attrs[0].attrID = 1001;
  attrs[0].attrStr = "ZaurusOS";
  attrs[0].attrSize = strlen(attrs[0].attrStr);

  attrs[1].attrID =1002;
  attrs[1].attrStr = (char*)modelName;
  attrs[1].attrSize = strlen((char*)modelName);

  attrs[2].attrID = 1003;
  attrs[2].attrStr = "SH-3";
  attrs[2].attrSize = strlen(attrs[2].attrStr);

  attrs[3].attrID = 1004;
  attrs[3].attrStr = "2.7 (Apr. 9, 2000)";
  attrs[3].attrSize = strlen(attrs[3].attrStr);

}
  

int
attributeSize(int id)
{
  if (id < 1000) {
    return 0;
  }

  id -= 1001;

  if (id > 3) {
    return 0;
  }
  return attrs[id].attrSize;
}

int
getAttributeIntoLength(int id, int byteArrayIndex, int length)
{
  int i;
  char *attrStr;
  
  if (id < 1000) {
    return 0;
  }

  id -= 1001;

  if (id > 3) {
    return 0;
  }

  attrStr = attrs[id].attrStr;
  for (i = 0; i < length; i++) {
    ((char*)byteArrayIndex)[i] = attrStr[i];
  }
  
  return attrs[id].attrSize;
}

void
ioFastTickUsage(int value)
{
  if (value) {
    EnableFastTick(1);
    fastTickIsUsed = true;
  } else {
    DisableFastTick();
    fastTickIsUsed = false;
  }
}

void
setupTimers()
{
  fastTickIsUsed = true;
  EnableFastTick(1); /* the argument should be 3? */
  startUpTime = GetFastTickCount();
}

void
setupImages(APLFORMPTR AplForm)
{
  int size;
  
  size = dispImageSize * sizeof(short) +
    sizeof(IMAGEDATA) - sizeof(short);

  sqAplForm = AplForm;
  sqWnHandle = AplForm->FormWnHandle;

#ifndef USE_VRAM_ACCESS
  if (stImage == NULL) {
    stImage = NewHeapPtrS(size + sizeof(short));
  }
#endif
}

void
SetColorEntry(int index, int red, int green, int blue)
{
  unsigned short r, g, b;
  unsigned short w;

  if (index >= 256)
    return;

  r = red;
  g = green;
  b = blue;

  w = (((r>>11)<<11) |
       ((g>>10)<<5) | 
       ((b>>11)));

  stColors16[index] = w;
}

#ifdef USE_16BIT_DEPTH_COLOR_TABLE
void
setupPixmap16()
{
  int i;

  for (i = 1; i < 32768; i++) {
    stColors1516[i] = ((i&0x7FE0)<<1) | (i&0x1f);
  }
}
#endif
    
void
setupPixmap()
{
  
  /* 1-bit colors (monochrome) */
  SetColorEntry(0, 65535, 65535, 65535);	/* white or transparent */
  SetColorEntry(1,     0,     0,     0);	/* black */

	/* additional colors for 2-bit color */
  SetColorEntry(2, 65535, 65535, 65535);	/* opaque white */
  SetColorEntry(3, 32768, 32768, 32768);	/* 1/2 gray */

	/* additional colors for 4-bit color */
  SetColorEntry( 4, 65535,     0,     0);	/* red */
  SetColorEntry( 5,     0, 65535,     0);	/* green */
  SetColorEntry( 6,     0,     0, 65535);	/* blue */
  SetColorEntry( 7,     0, 65535, 65535);	/* cyan */
  SetColorEntry( 8, 65535, 65535,     0);	/* yellow */
  SetColorEntry( 9, 65535,     0, 65535);	/* magenta */
  SetColorEntry(10,  8192,  8192,  8192);	/* 1/8 gray */
  SetColorEntry(11, 16384, 16384, 16384);	/* 2/8 gray */
  SetColorEntry(12, 24576, 24576, 24576);	/* 3/8 gray */
  SetColorEntry(13, 40959, 40959, 40959);	/* 5/8 gray */
  SetColorEntry(14, 49151, 49151, 49151);	/* 6/8 gray */
  SetColorEntry(15, 57343, 57343, 57343);	/* 7/8 gray */

	/* additional colors for 8-bit color */
	/* 24 more shades of gray (does not repeat 1/8th increments) */
  SetColorEntry(16,  2048,  2048,  2048);	/*  1/32 gray */
  SetColorEntry(17,  4096,  4096,  4096);	/*  2/32 gray */
  SetColorEntry(18,  6144,  6144,  6144);	/*  3/32 gray */
  SetColorEntry(19, 10240, 10240, 10240);	/*  5/32 gray */
  SetColorEntry(20, 12288, 12288, 12288);	/*  6/32 gray */
  SetColorEntry(21, 14336, 14336, 14336);	/*  7/32 gray */
  SetColorEntry(22, 18432, 18432, 18432);	/*  9/32 gray */
  SetColorEntry(23, 20480, 20480, 20480);	/* 10/32 gray */
  SetColorEntry(24, 22528, 22528, 22528);	/* 11/32 gray */
  SetColorEntry(25, 26624, 26624, 26624);	/* 13/32 gray */
  SetColorEntry(26, 28672, 28672, 28672);	/* 14/32 gray */
  SetColorEntry(27, 30720, 30720, 30720);	/* 15/32 gray */
  SetColorEntry(28, 34815, 34815, 34815);	/* 17/32 gray */
  SetColorEntry(29, 36863, 36863, 36863);	/* 18/32 gray */
  SetColorEntry(30, 38911, 38911, 38911);	/* 19/32 gray */
  SetColorEntry(31, 43007, 43007, 43007);	/* 21/32 gray */
  SetColorEntry(32, 45055, 45055, 45055);	/* 22/32 gray */
  SetColorEntry(33, 47103, 47103, 47103);	/* 23/32 gray */
  SetColorEntry(34, 51199, 51199, 51199);	/* 25/32 gray */
  SetColorEntry(35, 53247, 53247, 53247);	/* 26/32 gray */
  SetColorEntry(36, 55295, 55295, 55295);	/* 27/32 gray */
  SetColorEntry(37, 59391, 59391, 59391);	/* 29/32 gray */
  SetColorEntry(38, 61439, 61439, 61439);	/* 30/32 gray */
  SetColorEntry(39, 63487, 63487, 63487);	/* 31/32 gray */

  /* The remainder of color table defines a color cube with six steps
     for each primary color. Note that the corners of this cube repeat
     previous colors, but simplifies the mapping between RGB colors and
     color map indices. This color cube spans indices 40 through 255.
     */
  {
    int r, g, b;

    for (r= 0; r < 6; r++)
      for (g= 0; g < 6; g++)
	for (b= 0; b < 6; b++)
	  {
	    int i= 40 + ((36 * r) + (6 * b) + g);
	    if (i > 255) error("index out of range in color table compuation");
	    SetColorEntry(i, (r * 65535) / 5, (g * 65535) / 5, (b * 65535) / 5);
	  }
  }
#ifdef USE_16BIT_DEPTH_COLOR_TABLE
  setupPixmap16();
#endif
}


void
setupKeypad(APLFORMPTR AplForm)
{
#ifndef WITHOUT_KEYPAD
  MESSAGE k;
  unsigned char v;
  
  GetCtrlProperty(KEYPAD00, PAD_MOJI_PID, pValue, keypadPtr, AplForm->Control, char *);
  if (keypadPtr == NULL) {
    printf("keypadPtr");
  }

  *keypadPtr = '\0';

  GetCtrlProperty(KEYPAD00, PAD_POP_UP_MODE_PID, ucValue, v, AplForm->Control, UCHAR);
  v |= OPT_KEYPAD_SMALL;
  SetCtrlProperty(KEYPAD00, PAD_POP_UP_MODE_PID, ucValue, v, AplForm->Control);
  
#endif
}
  
const int format_size = 56;
static char buf[56];
static int cur = 0;
static int cr = 0;

static POINT s;
static POINT e;
static COLOR bg;
static COLOR fg;

static POINT p;

void
setupPrintf(APLFORMPTR AplForm)
{
  sqAplForm = AplForm;
  sqWnHandle = AplForm->FormWnHandle;

  s.x = 24 * 4;
  s.y = 0 * 4;
  e.x = 274 * 4 - 1;
  e.y = 16 * 4 - 1;

  p.x = 24 * 4;
  p.y = 16 * 4 - 1;

  SetColorV(&fg, BLACK);
  SetColorV(&bg, LIGHT_GRAY);
}

static int
clear_line()
{
  AreaClear(sqWnHandle, &s, &e, &bg, FBSET, NULL);
  p.x = 24 * 4;
  p.y = 16 * 4 - 1;
  cur = 0;
}
  
static int
display_string(char *buf, int len)
{
  if (cur + len > format_size - 1) {
    clear_line();
  }
  if (len == 1) {
    Disp1C(sqWnHandle, &p, &fg, &bg, FBSET|QP40_CHR,
	   (unsigned short)*buf, 16, NULL);
    cur++;
  } else {
    DispNC(sqWnHandle, &p, &fg, &bg, FBSET|QP40_CHR,
	       buf, len, 16, NULL);
    cur += len;
  }
  
  return 0;
}

char *
printf(char *fmt, ...)
{
  va_list ap;
  int i;
  int len;
  char c;

  va_start(ap, fmt);

  while (c = *fmt) {

    if (cr) {
      clear_line();
    }
    
    switch (*fmt++) {
    case '%':
      switch (*fmt++) {
      case 'l':
	fmt++;
      case 'd':
	{
	  int val = va_arg(ap, int);
	
	  itoa(val, buf, 10);
	  len = strlen(buf);
	}
	break;
      case 'x':
	{
	  int val = va_arg(ap, int);
	  itoa(val, buf, 16);
	  len = strlen(buf);
	}
        break;
      case 's':
	{
	  char *src = va_arg(ap, char*);
	  char *dst = buf;

	  len = 0;
	  while (*dst++ = *src++) {
	    len++;
	  }
	  *dst = '\0';
	}
        break;
      }
      cr = 0;
      break;
    case '\r':
    case '\n':
      cr = 1;
      break;
    default:
      cr = 0;
      len = 1;
      buf[0] = c;
      buf[1] = '\0';
    }

    if (cr) {
      continue;
    }

    display_string(buf, len);
  }
}
      
int
putchar(int c)
{
  int len;
  
  if (cr) {
    clear_line();
  }
  cr = 0;

  if (c == '\n' || c == '\r') {
    cr = 1;
    return c;
  }

  len = 1;
  buf[0] = c;
  buf[1] = '\0';
  
  display_string(buf, len);
}

int
main(APLFORMPTR AplForm)
{
  struct stat statBuf;

  strcpy(imageName, shortImageName);

  if (stat(imageName, &statBuf)) {
    printf("can't find the image");
    ioExit();
    return 0;
  }

  initialHeapSize = statBuf.st_size + STROOM;

  if (initialHeapSize < memorySize) {
    initialHeapSize = memorySize;
  }
  
  setupTimers();
  setupScreenSize();
  setupPrintf(AplForm);
  setupPixmap();
  sqFileInit();
  setupAttribute();
#ifdef JOYSTICK
  joystickInit();
#endif
  setupImages(AplForm);
  setupKeypad(AplForm);

  shouldNotPowerOff = false;

  recordFullPathForImageName(imageName);

  {
    FILE *f= fopen(imageName, "r");

    if (f == NULL) {
      imageNotFound(imageName);
      return 0;
    }
    readImageFromFileHeapSize(f, initialHeapSize);
    fclose(f);
  }

  headMargin = fullScreenFlag ? 0 : 16;
  
  /* run Squeak */
  squeakStarted = 1;
  interpret();

  return 0;
  
}

