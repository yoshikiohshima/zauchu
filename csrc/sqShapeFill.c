/* Automatically generated from Squeak on (9 October 1999 2:59:52 am ) */

#include "sq.h"

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)

int printCallStack(void);

/*** Variables ***/
int fillDestBits;
int fillFormHeight;
int fillFormWidth;
unsigned int fillMask[33] = {0, 4294967295U, 2147483647U, 1073741823U, 536870911U, 268435455U, 134217727U, 67108863U, 33554431U, 16777215U, 8388607U, 4194303U, 2097151U, 1048575U, 524287U, 262143U, 131071U, 65535U, 32767U, 16383U, 8191U, 4095U, 2047U, 1023U, 511U, 255U, 127U, 63U, 31U, 15U, 7U, 3U, 1U, };
int fillSourceBits;
int fillSourceRaster;
int fillStack;
int fillStackSize;
int fillStackp;

/*** Function Prototypes ***/
int fetchPointerofObject(int fieldIndex, int oop);
int fillLefttoRightatY(int leftBound, int rightBound, int y);
int fillPixelValueAtXy(int x, int y);
int fillPixelValueAtXyput(int x, int y, int value);
int findLeftAtXy(int x, int y);
int findRightAndPaintAtXy(int x, int y);
int findRightAtXy(int x, int y);
int popJob(void);
int primShapeFillXysourceresultstackstackSize(int x, int y, int sourceForm, int resultForm, int stack, int stackSize);
int pushPointXy(int x, int y);
int pushPointspreviousLeftpreviousRightdirection(int y, int pLeft, int pRight, int dir);
int storePointerUncheckedofObjectwithValue(int fieldIndex, int oop, int valuePointer);

#if 0
int fetchPointerofObject(int fieldIndex, int oop) {
	return longAt(((((char *) oop)) + 4) + (fieldIndex << 2));
}
#endif

int fillLefttoRightatY(int leftBound, int rightBound, int y) {
    int start;
    int word1;
    int value;
    int end;
    int word;
    int index;

	start = (y * fillSourceRaster) + (((int) leftBound >> 5));
	end = (y * fillSourceRaster) + (((int) rightBound >> 5));
	if (start == end) {
		word = longAt(((((char *) fillSourceBits)) + 4) + (start << 2));
		word1 = longAt(((((char *) fillDestBits)) + 4) + (start << 2));
		value = (fillMask[(leftBound % 32) + 1]) ^ (((unsigned) (fillMask[(rightBound % 32) + 1])) >> 1);
		longAtput(((((char *) fillSourceBits)) + 4) + (start << 2), word | value);
		return longAtput(((((char *) fillDestBits)) + 4) + (start << 2), word1 | value);
	}
	if (!((leftBound % 32) == 0)) {
		word = longAt(((((char *) fillSourceBits)) + 4) + (start << 2));
		word1 = longAt(((((char *) fillDestBits)) + 4) + (start << 2));
		value = fillMask[(leftBound % 32) + 1];
		longAtput(((((char *) fillSourceBits)) + 4) + (start << 2), word | value);
		longAtput(((((char *) fillDestBits)) + 4) + (start << 2), word1 | value);
		start += 1;
	}
	if (!((rightBound % 32) == 31)) {
		word = longAt(((((char *) fillSourceBits)) + 4) + (end << 2));
		word1 = longAt(((((char *) fillDestBits)) + 4) + (end << 2));
		value = 4294967295U ^ (((unsigned) (fillMask[(rightBound % 32) + 1])) >> 1);
		longAtput(((((char *) fillSourceBits)) + 4) + (end << 2), word | value);
		longAtput(((((char *) fillDestBits)) + 4) + (end << 2), word1 | value);
		end -= 1;
	}
	for (index = start; index <= end; index += 1) {
		longAtput(((((char *) fillSourceBits)) + 4) + (index << 2), 4294967295U);
		longAtput(((((char *) fillDestBits)) + 4) + (index << 2), 4294967295U);
	}
}

int fillPixelValueAtXy(int x, int y) {
    int sourceWord;
    int index;

	index = (y * fillSourceRaster) + (((int) x >> 5));
	sourceWord = longAt(((((char *) fillSourceBits)) + 4) + (index << 2));
	return (((unsigned) sourceWord) >> (31 - (x % 32))) & 1;
}

int fillPixelValueAtXyput(int x, int y, int value) {
    int word;
    int index;

	index = (y * fillSourceRaster) + (((int) x >> 5));
	word = longAt(((((char *) fillSourceBits)) + 4) + (index << 2));
	longAtput(((((char *) fillSourceBits)) + 4) + (index << 2), word | (1 << (31 - (x % 32))));
	word = longAt(((((char *) fillDestBits)) + 4) + (index << 2));
	longAtput(((((char *) fillDestBits)) + 4) + (index << 2), word | (1 << (31 - (x % 32))));
}

int findLeftAtXy(int x, int y) {
    int leftBound;

	if ((fillPixelValueAtXy(x, y)) == 1) {
		return 32367;
	}
	leftBound = x;
	while ((0 <= leftBound) && ((fillPixelValueAtXy(leftBound, y)) == 0)) {
		leftBound -= 1;
	}
	leftBound += 1;
	return leftBound;
}

int findRightAndPaintAtXy(int x, int y) {
    int rightBound;
    int word;
    int index;

	if ((fillPixelValueAtXy(x, y)) == 1) {
		return 0;
	}
	rightBound = x;
	while ((rightBound < fillFormWidth) && ((fillPixelValueAtXy(rightBound, y)) == 0)) {
		/* begin fillPixelValueAtX:y:put: */
		index = (y * fillSourceRaster) + (((int) rightBound >> 5));
		word = longAt(((((char *) fillSourceBits)) + 4) + (index << 2));
		longAtput(((((char *) fillSourceBits)) + 4) + (index << 2), word | (1 << (31 - (rightBound % 32))));
		word = longAt(((((char *) fillDestBits)) + 4) + (index << 2));
		longAtput(((((char *) fillDestBits)) + 4) + (index << 2), word | (1 << (31 - (rightBound % 32))));
		rightBound += 1;
	}
	rightBound -= 1;
	return rightBound;
}

int findRightAtXy(int x, int y) {
    int rightBound;

	if ((fillPixelValueAtXy(x, y)) == 1) {
		return 0;
	}
	rightBound = x;
	while ((rightBound < fillFormWidth) && ((fillPixelValueAtXy(rightBound, y)) == 0)) {
		rightBound += 1;
	}
	rightBound -= 1;
	return rightBound;
}

int popJob(void) {
    int value;

	value = longAt(((((char *) fillStack)) + 4) + (fillStackp << 2));
	fillStackp -= 1;
	return value;
}

int primShapeFillXysourceresultstackstackSize(int x, int y, int sourceForm, int resultForm, int stack, int stackSize) {
    int right;
    int nowX;
    int nowY;
    int nowY1;
    int in;
    int nowX1;
    int jobX;
    int nowY2;
    int in1;
    int nowX2;
    int jobX1;
    int value;
    int rightBound;

	fillStack = stack;
	fillSourceBits = longAt(((((char *) sourceForm)) + 4) + (0 << 2));
	fillDestBits = longAt(((((char *) resultForm)) + 4) + (0 << 2));
	fillFormWidth = fetchIntegerofObject(1, resultForm);
	fillFormHeight = fetchIntegerofObject(2, resultForm);
	fillSourceRaster = ((int) (fillFormWidth + 31) >> 5);
	fillStackp = 0;
	fillStackSize = stackSize;
	;
	;
	pushPointXy(x, y);
	while (fillStackp > 0) {
		/* begin popJob */
		value = longAt(((((char *) fillStack)) + 4) + (fillStackp << 2));
		fillStackp -= 1;
		nowX = value;
		nowY = nowX & 65535;
		nowX = ((unsigned) nowX >> 16);
		/* begin findRightAtX:y: */
		if ((fillPixelValueAtXy(nowX, nowY)) == 1) {
			right = 0;
			goto l3;
		}
		rightBound = nowX;
		while ((rightBound < fillFormWidth) && ((fillPixelValueAtXy(rightBound, nowY)) == 0)) {
			rightBound += 1;
		}
		rightBound -= 1;
		right = rightBound;
	l3:	/* end findRightAtX:y: */;
		if (!(nowX > right)) {
			fillLefttoRightatY(nowX, right, nowY);
			if ((nowY + 1) < fillFormHeight) {
				/* begin pushPoints:previousLeft:previousRight:direction: */
				jobX = null;
				nowY1 = nowY + 1;
				in = 0;
				for (nowX1 = nowX; nowX1 <= right; nowX1 += 1) {
					if (in) {
						if ((fillPixelValueAtXy(nowX1, nowY1)) == 1) {
							in = 0;
							pushPointXy(findLeftAtXy(jobX, nowY1), nowY1);
							if (failed()) {
								fillStackp = null;
								goto l1;
							}
						}
					} else {
						if ((fillPixelValueAtXy(nowX1, nowY1)) == 0) {
							in = 1;
							jobX = nowX1;
						}
					}
				}
				if (in) {
					pushPointXy(findLeftAtXy(jobX, nowY1), nowY1);
					if (failed()) {
						fillStackp = null;
						goto l1;
					}
				}
				fillStackp = fillStackp;
			l1:	/* end pushPoints:previousLeft:previousRight:direction: */;
				if (failed()) {
					return null;
				}
			}
			if ((nowY - 1) >= 0) {
				/* begin pushPoints:previousLeft:previousRight:direction: */
				jobX1 = null;
				nowY2 = nowY + -1;
				in1 = 0;
				for (nowX2 = nowX; nowX2 <= right; nowX2 += 1) {
					if (in1) {
						if ((fillPixelValueAtXy(nowX2, nowY2)) == 1) {
							in1 = 0;
							pushPointXy(findLeftAtXy(jobX1, nowY2), nowY2);
							if (failed()) {
								fillStackp = null;
								goto l2;
							}
						}
					} else {
						if ((fillPixelValueAtXy(nowX2, nowY2)) == 0) {
							in1 = 1;
							jobX1 = nowX2;
						}
					}
				}
				if (in1) {
					pushPointXy(findLeftAtXy(jobX1, nowY2), nowY2);
					if (failed()) {
						fillStackp = null;
						goto l2;
					}
				}
				fillStackp = fillStackp;
			l2:	/* end pushPoints:previousLeft:previousRight:direction: */;
				if (failed()) {
					return null;
				}
			}
		}
	}
}

int pushPointXy(int x, int y) {
	if (!(fillStackp < fillStackSize)) {
		return success(0);
	}
	fillStackp += 1;
	;
	longAtput(((((char *) fillStack)) + 4) + (fillStackp << 2), (x << 16) | y);
}

int pushPointspreviousLeftpreviousRightdirection(int y, int pLeft, int pRight, int dir) {
    int nowY;
    int in;
    int nowX;
    int jobX;

	jobX = null;
	nowY = y + dir;
	in = 0;
	for (nowX = pLeft; nowX <= pRight; nowX += 1) {
		if (in) {
			if ((fillPixelValueAtXy(nowX, nowY)) == 1) {
				in = 0;
				pushPointXy(findLeftAtXy(jobX, nowY), nowY);
				if (failed()) {
					return null;
				}
			}
		} else {
			if ((fillPixelValueAtXy(nowX, nowY)) == 0) {
				in = 1;
				jobX = nowX;
			}
		}
	}
	if (in) {
		pushPointXy(findLeftAtXy(jobX, nowY), nowY);
		if (failed()) {
			return null;
		}
	}
	return fillStackp;
}

/*
int storePointerUncheckedofObjectwithValue(int fieldIndex, int oop, int valuePointer) {
	return longAtput(((((char *) oop)) + 4) + (fieldIndex << 2), valuePointer);
}
*/
