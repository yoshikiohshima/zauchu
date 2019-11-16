/*
  This is a modified version of 'sqUnixSerialAndMIDIPort.c'.
  Modification by OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)
*/

#include "sq.h"

/*** Imported Variables ***/
extern int successFlag;

#define MAXPORT 2

/*** Local Variables ***/

static unsigned char serialPorts[MAXPORT]; /*  = {0, 0}; */

int
portIsOpen(int portNum)
{
  return serialPorts[portNum] != 0;
}

void
closeAllSerialPorts()
{
  int i;

  for (i = 0; i < MAXPORT; i++) {
    serialPortClose(i);
  }
}
 

/*** Public Functions ***/

int
serialPortOpen(
  int portNum, int baudRate, int stopBitsType, int parityType, int dataBits,
  int inFlowCtrl, int outFlowCtrl, int xOnChar, int xOffChar) {
/* Open the given serial port using the given settings. The baud rate can be
   any number between about 224 and 57600; the driver will pick a clock
   divisor that will generate the closest available baud rate. */

  /* For Zaurus, port 0 stands for COM0: and port 1 stands for IRCOM:. */

  char options[50] = {0};
  char tmpOptions[10] = {0};
  FILEHDERROR err;
  
  if ((portNum < 0) || (portNum > 1) || portIsOpen(portNum)) {
    return success(false); /* bad port number or port already open */
  }

  switch (portNum) {
  case 0:
    strcat(options, "COM0:");
    break;
  case 1:
    strcat(options, "IRCOM:");
    break;
  default:
    success(false);
    return false;
  }

  switch (baudRate) {
  case 300:
  case 600:
  case 900:
  case 1200:
  case 1800:
  case 2400:
  case 3600:
  case 4800:
  case 7200:
  case 9600:
  case 1440:
  case 19200:
  case 28800:
  case 38400:
  case 57600:
  case 115200:
    sprintf(tmpOptions, "/B%d", baudRate);
    strcat(options, tmpOptions);
  break;
  default:
    printf("unsupported bps\n");
    success(false);
    return false;
  }

  switch (parityType) {
  case 0:
  case 1:
  case 2:
    sprintf(tmpOptions, "/P%d", parityType);
    strcat(options, tmpOptions);
    break;
  default:
    success(false);
    return false;
  }

  switch (dataBits) {
  case 7:
  case 8:
    sprintf(tmpOptions, "/L%d", dataBits);
    strcat(options, tmpOptions);
    break;
  default:
    success(false);
    return false;
  }

  switch (stopBitsType) {
  case 1:
  case 2:
    sprintf(tmpOptions, "/S%d", stopBitsType);
    strcat(options, tmpOptions);
    break;
  default:
    success(false);
    return false;
  }

  if (inFlowCtrl != outFlowCtrl) {
    printf("in & out flow control should match\n");
    success(false);
    return false;
  }

  switch (inFlowCtrl) {
  case 0:
    strcat(options, "/C0/X0");
    break;
  case 1:
    strcat(options, "/C0/X1");
    break;
  case 2:
    strcat(options, "/C1/X0");
    break;
  default:
    success(false);
    return false;
  }

  strcat(options, "/J0");

  strcat(options, "/W0");
  strcat(options, "/E0");
  strcat(options, "/M0");

  err = FileOpen((unsigned char*)options, RW_OPEN);

  if (err < 0) {
    printf("serial port open error: code = %d\n", err);
    success(false);
    return false;
  }

  serialPorts[portNum] = (FILEHD)err;
  return true;
}

int
serialPortClose(int portNum)
{
  FILEHDERROR err;

  if ((portNum < 0) || (portNum > 1)) {
    success(false);
    return false;
  }

  if (!portIsOpen(portNum)) {
    return true;
  }

  err = FileClose(serialPorts[portNum]);
  if (err != 0) {
    success(false);
    return false;
  }
  serialPorts[portNum] = 0;
  return true;
}

int
serialPortReadInto(int portNum, int count, int startPtr)
{
/* Read up to count bytes from the given serial port into the given byte array.
   Read only up to the number of bytes in the port's input buffer; if fewer bytes
   than count have been received, do not wait for additional data to arrive.
   Return zero if no data is available. */

  unsigned long actual;
  ERRORCODE err;
  
  if (portNum < 0 | 1 < portNum) {
    success(false);
    return false;
  }

  if (!portIsOpen(portNum)) {
    success(false);
    return false;
  }

  actual = count;
  err = FileBlockRead(serialPorts[portNum], FC_FILE_END_PH, (unsigned char*)startPtr, &actual);
  if (err != 0) {
    success(false);
    return false;
  }

  return actual;
}



int
serialPortWriteFrom(int portNum, int count, int startPtr)
{
/* Write count bytes from the given byte array to the given serial port's
   output buffer. Return the number of bytes written. This implementation is
   synchronous: it doesn't return until the data has been sent. However, other
   implementations may return before transmission is complete. */

  unsigned long actual;
  ERRORCODE err;
  
  if (portNum < 0 | 1 < portNum) {
    success(false);
    return false;
  }

  if (!portIsOpen(portNum)) {
    success(false);
    return false;
  }

  actual = count;
  err = FileBlockWrite(serialPorts[portNum], (unsigned char*)startPtr, &actual);
  if (err != 0) {
    success(false);
    return false;
  }

  return actual;
}

int
serialPortMidiClockRate(int portNum, int interfaceClockRate) {
  success(false);
  return 0;
}


/* #ifdef WITH_MIDI_SUPPORT */

int
sqMIDIClosePort(int portNum) {
  success(false);
  return 0;
}

int
sqMIDIGetClock(void) {
  success(false);
  return 0;
}

int
sqMIDIGetPortCount(void) {
  success(false);
  return 0;
}

int
sqMIDIGetPortDirectionality(int portNum) {
  success(false);
  return 0;
}

int
sqMIDIGetPortName(int portNum, int namePtr, int length) {
  success(false);
  return 0;
}

int
sqMIDIOpenPort(int portNum, int readSemaIndex, int interfaceClockRate) {
  success(false);
  return 0;
}

int
sqMIDIParameter(int whichParameter, int modify, int newValue) {
  success(false);
  return 0;
}

int
sqMIDIPortReadInto(int portNum, int count, int bufferPtr) {
  success(false);
  return 0;
}

int
sqMIDIPortWriteFromAt(int portNum, int count, int bufferPtr, int time) {
  success(false);
  return 0;
}

/* #endif */
