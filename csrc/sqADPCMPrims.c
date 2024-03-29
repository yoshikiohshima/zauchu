/* Automatically generated from Squeak on (29 March 1999 7:15:37 am ) */

#include "sq.h"


/* Memory Access Macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)

/*** Imported Functions/Variables ***/
extern int stackValue(int);
extern int successFlag;

/* allows accessing Strings in both C and Smalltalk */
#define asciiValue(c) c

/*** Variables ***/

/*** Function Prototypes ***/
int primADPCMCodecprivateDecodeMono(void);
int primADPCMCodecprivateDecodeStereo(void);
int primADPCMCodecprivateEncodeMono(void);
int primADPCMCodecprivateEncodeStereo(void);

int primADPCMCodecprivateDecodeMono(void) {
    int rcvr;
    int count;
    int step;
    int delta;
    int predictedDelta;
    int i;
    int bit;
    int result;
    int remaining;
    int shift;
    int result1;
    int remaining1;
    int shift1;
    int result2;
    int remaining2;
    int shift2;
    int predicted;
    int index;
    int deltaSignMask;
    int deltaValueMask;
    int deltaValueHighBit;
    int frameSizeMask;
    int currentByte;
    int bitPosition;
    int byteIndex;
    unsigned char *encodedBytes;
    short int *samples;
    int sampleIndex;
    int bitsPerSample;
    short int *stepSizeTable;
    short int *indexTable;

	rcvr = stackValue(1);
	count = checkedIntegerValueOf(stackValue(0));
	predicted = fetchIntegerofObject(0, rcvr);
	index = fetchIntegerofObject(1, rcvr);
	deltaSignMask = fetchIntegerofObject(2, rcvr);
	deltaValueMask = fetchIntegerofObject(3, rcvr);
	deltaValueHighBit = fetchIntegerofObject(4, rcvr);
	frameSizeMask = fetchIntegerofObject(5, rcvr);
	currentByte = fetchIntegerofObject(6, rcvr);
	bitPosition = fetchIntegerofObject(7, rcvr);
	byteIndex = fetchIntegerofObject(8, rcvr);
	encodedBytes = fetchArrayofObject(9, rcvr);
	encodedBytes -= 1;
	samples = fetchArrayofObject(10, rcvr);
	samples -= 1;
	sampleIndex = fetchIntegerofObject(12, rcvr);
	bitsPerSample = fetchIntegerofObject(13, rcvr);
	stepSizeTable = fetchArrayofObject(14, rcvr);
	stepSizeTable -= 1;
	indexTable = fetchArrayofObject(15, rcvr);
	indexTable -= 1;
	if (!(successFlag)) {
		return null;
	}
	for (i = 1; i <= count; i += 1) {
		if ((i & frameSizeMask) == 1) {
			/* begin nextBits: */
			result = 0;
			remaining = 16;
			while (1) {
				shift = remaining - bitPosition;
				result += ((shift < 0) ? ((unsigned) currentByte >> -shift) : ((unsigned) currentByte << shift));
				if (shift > 0) {
					remaining -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					predicted = result;
					goto l1;
				}
			}
		l1:	/* end nextBits: */;
			if (predicted > 32767) {
				predicted -= 65536;
			}
			/* begin nextBits: */
			result1 = 0;
			remaining1 = 6;
			while (1) {
				shift1 = remaining1 - bitPosition;
				result1 += ((shift1 < 0) ? ((unsigned) currentByte >> -shift1) : ((unsigned) currentByte << shift1));
				if (shift1 > 0) {
					remaining1 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining1;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					index = result1;
					goto l2;
				}
			}
		l2:	/* end nextBits: */;
			samples[sampleIndex += 1] = predicted;
		} else {
			/* begin nextBits: */
			result2 = 0;
			remaining2 = bitsPerSample;
			while (1) {
				shift2 = remaining2 - bitPosition;
				result2 += ((shift2 < 0) ? ((unsigned) currentByte >> -shift2) : ((unsigned) currentByte << shift2));
				if (shift2 > 0) {
					remaining2 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining2;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					delta = result2;
					goto l3;
				}
			}
		l3:	/* end nextBits: */;
			step = stepSizeTable[index + 1];
			predictedDelta = 0;
			bit = deltaValueHighBit;
			while (bit > 0) {
				if ((delta & bit) > 0) {
					predictedDelta += step;
				}
				step = ((unsigned) step >> 1);
				bit = ((unsigned) bit >> 1);
			}
			predictedDelta += step;
			if ((delta & deltaSignMask) > 0) {
				predicted -= predictedDelta;
			} else {
				predicted += predictedDelta;
			}
			if (predicted > 32767) {
				predicted = 32767;
			} else {
				if (predicted < -32768) {
					predicted = -32768;
				}
			}
			index += indexTable[(delta & deltaValueMask) + 1];
			if (index < 0) {
				index = 0;
			} else {
				if (index > 88) {
					index = 88;
				}
			}
			samples[sampleIndex += 1] = predicted;
		}
	}
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(0, rcvr, predicted);
	storeIntegerofObjectwithValue(1, rcvr, index);
	storeIntegerofObjectwithValue(6, rcvr, currentByte);
	storeIntegerofObjectwithValue(7, rcvr, bitPosition);
	storeIntegerofObjectwithValue(8, rcvr, byteIndex);
	storeIntegerofObjectwithValue(12, rcvr, sampleIndex);
	pop(1);
}

int primADPCMCodecprivateDecodeStereo(void) {
    int rcvr;
    int count;
    int deltaLeft;
    int deltaRight;
    int predictedLeft;
    int stepLeft;
    int stepRight;
    int predictedRight;
    int predictedDeltaLeft;
    int predictedDeltaRight;
    int indexLeft;
    int indexRight;
    int i;
    int bit;
    int result;
    int remaining;
    int shift;
    int result1;
    int remaining1;
    int shift1;
    int result2;
    int remaining2;
    int shift2;
    int result3;
    int remaining3;
    int shift3;
    int result4;
    int remaining4;
    int shift4;
    int result5;
    int remaining5;
    int shift5;
    short int *predicted;
    short int *index;
    int deltaSignMask;
    int deltaValueMask;
    int deltaValueHighBit;
    int frameSizeMask;
    int currentByte;
    int bitPosition;
    int byteIndex;
    unsigned char *encodedBytes;
    short int *samples;
    short int *rightSamples;
    int sampleIndex;
    int bitsPerSample;
    short int *stepSizeTable;
    short int *indexTable;

	rcvr = stackValue(1);
	count = checkedIntegerValueOf(stackValue(0));
	predicted = fetchArrayofObject(0, rcvr);
	predicted -= 1;
	index = fetchArrayofObject(1, rcvr);
	index -= 1;
	deltaSignMask = fetchIntegerofObject(2, rcvr);
	deltaValueMask = fetchIntegerofObject(3, rcvr);
	deltaValueHighBit = fetchIntegerofObject(4, rcvr);
	frameSizeMask = fetchIntegerofObject(5, rcvr);
	currentByte = fetchIntegerofObject(6, rcvr);
	bitPosition = fetchIntegerofObject(7, rcvr);
	byteIndex = fetchIntegerofObject(8, rcvr);
	encodedBytes = fetchArrayofObject(9, rcvr);
	encodedBytes -= 1;
	samples = fetchArrayofObject(10, rcvr);
	samples -= 1;
	rightSamples = fetchArrayofObject(11, rcvr);
	rightSamples -= 1;
	sampleIndex = fetchIntegerofObject(12, rcvr);
	bitsPerSample = fetchIntegerofObject(13, rcvr);
	stepSizeTable = fetchArrayofObject(14, rcvr);
	stepSizeTable -= 1;
	indexTable = fetchArrayofObject(15, rcvr);
	indexTable -= 1;
	if (!(successFlag)) {
		return null;
	}
	predictedLeft = predicted[1];
	predictedRight = predicted[2];
	indexLeft = index[1];
	indexRight = index[2];
	for (i = 1; i <= count; i += 1) {
		if ((i & frameSizeMask) == 1) {
			/* begin nextBits: */
			result = 0;
			remaining = 16;
			while (1) {
				shift = remaining - bitPosition;
				result += ((shift < 0) ? ((unsigned) currentByte >> -shift) : ((unsigned) currentByte << shift));
				if (shift > 0) {
					remaining -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					predictedLeft = result;
					goto l1;
				}
			}
		l1:	/* end nextBits: */;
			/* begin nextBits: */
			result1 = 0;
			remaining1 = 6;
			while (1) {
				shift1 = remaining1 - bitPosition;
				result1 += ((shift1 < 0) ? ((unsigned) currentByte >> -shift1) : ((unsigned) currentByte << shift1));
				if (shift1 > 0) {
					remaining1 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining1;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					indexLeft = result1;
					goto l2;
				}
			}
		l2:	/* end nextBits: */;
			/* begin nextBits: */
			result2 = 0;
			remaining2 = 16;
			while (1) {
				shift2 = remaining2 - bitPosition;
				result2 += ((shift2 < 0) ? ((unsigned) currentByte >> -shift2) : ((unsigned) currentByte << shift2));
				if (shift2 > 0) {
					remaining2 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining2;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					predictedRight = result2;
					goto l3;
				}
			}
		l3:	/* end nextBits: */;
			/* begin nextBits: */
			result3 = 0;
			remaining3 = 6;
			while (1) {
				shift3 = remaining3 - bitPosition;
				result3 += ((shift3 < 0) ? ((unsigned) currentByte >> -shift3) : ((unsigned) currentByte << shift3));
				if (shift3 > 0) {
					remaining3 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining3;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					indexRight = result3;
					goto l4;
				}
			}
		l4:	/* end nextBits: */;
			if (predictedLeft > 32767) {
				predictedLeft -= 65536;
			}
			if (predictedRight > 32767) {
				predictedRight -= 65536;
			}
			samples[sampleIndex += 1] = predictedLeft;
			rightSamples[sampleIndex] = predictedRight;
		} else {
			/* begin nextBits: */
			result4 = 0;
			remaining4 = bitsPerSample;
			while (1) {
				shift4 = remaining4 - bitPosition;
				result4 += ((shift4 < 0) ? ((unsigned) currentByte >> -shift4) : ((unsigned) currentByte << shift4));
				if (shift4 > 0) {
					remaining4 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining4;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					deltaLeft = result4;
					goto l5;
				}
			}
		l5:	/* end nextBits: */;
			/* begin nextBits: */
			result5 = 0;
			remaining5 = bitsPerSample;
			while (1) {
				shift5 = remaining5 - bitPosition;
				result5 += ((shift5 < 0) ? ((unsigned) currentByte >> -shift5) : ((unsigned) currentByte << shift5));
				if (shift5 > 0) {
					remaining5 -= bitPosition;
					currentByte = encodedBytes[byteIndex += 1];
					bitPosition = 8;
				} else {
					bitPosition -= remaining5;
					currentByte = currentByte & ((((bitPosition - 8) < 0) ? ((unsigned) 255 >> -(bitPosition - 8)) : ((unsigned) 255 << (bitPosition - 8))));
					deltaRight = result5;
					goto l6;
				}
			}
		l6:	/* end nextBits: */;
			stepLeft = stepSizeTable[indexLeft + 1];
			stepRight = stepSizeTable[indexRight + 1];
			predictedDeltaLeft = predictedDeltaRight = 0;
			bit = deltaValueHighBit;
			while (bit > 0) {
				if ((deltaLeft & bit) > 0) {
					predictedDeltaLeft += stepLeft;
				}
				if ((deltaRight & bit) > 0) {
					predictedDeltaRight += stepRight;
				}
				stepLeft = ((unsigned) stepLeft >> 1);
				stepRight = ((unsigned) stepRight >> 1);
				bit = ((unsigned) bit >> 1);
			}
			predictedDeltaLeft += stepLeft;
			predictedDeltaRight += stepRight;
			if ((deltaLeft & deltaSignMask) > 0) {
				predictedLeft -= predictedDeltaLeft;
			} else {
				predictedLeft += predictedDeltaLeft;
			}
			if ((deltaRight & deltaSignMask) > 0) {
				predictedRight -= predictedDeltaRight;
			} else {
				predictedRight += predictedDeltaRight;
			}
			if (predictedLeft > 32767) {
				predictedLeft = 32767;
			} else {
				if (predictedLeft < -32768) {
					predictedLeft = -32768;
				}
			}
			if (predictedRight > 32767) {
				predictedRight = 32767;
			} else {
				if (predictedRight < -32768) {
					predictedRight = -32768;
				}
			}
			indexLeft += indexTable[(deltaLeft & deltaValueMask) + 1];
			if (indexLeft < 0) {
				indexLeft = 0;
			} else {
				if (indexLeft > 88) {
					indexLeft = 88;
				}
			}
			indexRight += indexTable[(deltaRight & deltaValueMask) + 1];
			if (indexRight < 0) {
				indexRight = 0;
			} else {
				if (indexRight > 88) {
					indexRight = 88;
				}
			}
			samples[sampleIndex += 1] = predictedLeft;
			rightSamples[sampleIndex] = predictedRight;
		}
	}
	predicted[1] = predictedLeft;
	predicted[2] = predictedRight;
	index[1] = indexLeft;
	index[2] = indexRight;
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(6, rcvr, currentByte);
	storeIntegerofObjectwithValue(7, rcvr, bitPosition);
	storeIntegerofObjectwithValue(8, rcvr, byteIndex);
	storeIntegerofObjectwithValue(12, rcvr, sampleIndex);
	pop(1);
}

int primADPCMCodecprivateEncodeMono(void) {
    int rcvr;
    int count;
    int step;
    int sign;
    int diff;
    int delta;
    int predictedDelta;
    int i;
    int bit;
    int p;
    int j;
    int diff1;
    int bestIndex;
    int buf;
    int bufBits;
    int bitsAvailable;
    int shift;
    int buf1;
    int bufBits1;
    int bitsAvailable1;
    int shift1;
    int buf2;
    int bufBits2;
    int bitsAvailable2;
    int shift2;
    int predicted;
    int index;
    int deltaSignMask;
    int deltaValueHighBit;
    int frameSizeMask;
    int currentByte;
    int bitPosition;
    int byteIndex;
    unsigned char *encodedBytes;
    short int *samples;
    int sampleIndex;
    int bitsPerSample;
    short int *stepSizeTable;
    short int *indexTable;

	rcvr = stackValue(1);
	count = checkedIntegerValueOf(stackValue(0));
	predicted = fetchIntegerofObject(0, rcvr);
	index = fetchIntegerofObject(1, rcvr);
	deltaSignMask = fetchIntegerofObject(2, rcvr);
	deltaValueHighBit = fetchIntegerofObject(4, rcvr);
	frameSizeMask = fetchIntegerofObject(5, rcvr);
	currentByte = fetchIntegerofObject(6, rcvr);
	bitPosition = fetchIntegerofObject(7, rcvr);
	byteIndex = fetchIntegerofObject(8, rcvr);
	encodedBytes = fetchArrayofObject(9, rcvr);
	encodedBytes -= 1;
	samples = fetchArrayofObject(10, rcvr);
	samples -= 1;
	sampleIndex = fetchIntegerofObject(12, rcvr);
	bitsPerSample = fetchIntegerofObject(13, rcvr);
	stepSizeTable = fetchArrayofObject(14, rcvr);
	stepSizeTable -= 1;
	indexTable = fetchArrayofObject(15, rcvr);
	indexTable -= 1;
	if (!(successFlag)) {
		return null;
	}
	step = stepSizeTable[1];
	for (i = 1; i <= count; i += 1) {
		if ((i & frameSizeMask) == 1) {
			predicted = samples[sampleIndex += 1];
			if ((p = predicted) < 0) {
				p += 65536;
			}
			/* begin nextBits:put: */
			buf = p;
			bufBits = 16;
			while (1) {
				bitsAvailable = 8 - bitPosition;
				shift = bitsAvailable - bufBits;
				currentByte += ((shift < 0) ? ((unsigned) buf >> -shift) : ((unsigned) buf << shift));
				if (shift < 0) {
					encodedBytes[byteIndex += 1] = currentByte;
					bitPosition = 0;
					currentByte = 0;
					buf = buf & (((((0 - shift) < 0) ? ((unsigned) 1 >> -(0 - shift)) : ((unsigned) 1 << (0 - shift)))) - 1);
					bufBits -= bitsAvailable;
				} else {
					bitPosition += bufBits;
					goto l1;
				}
			}
		l1:	/* end nextBits:put: */;
			if (i < count) {
				/* begin indexForDeltaFrom:to: */
				diff1 = (samples[sampleIndex + 1]) - predicted;
				if (diff1 < 0) {
					diff1 = 0 - diff1;
				}
				bestIndex = 63;
				for (j = 1; j <= 62; j += 1) {
					if (bestIndex == 63) {
						if ((stepSizeTable[j]) >= diff1) {
							bestIndex = j;
						}
					}
				}
				index = bestIndex;
			}
			/* begin nextBits:put: */
			buf1 = index;
			bufBits1 = 6;
			while (1) {
				bitsAvailable1 = 8 - bitPosition;
				shift1 = bitsAvailable1 - bufBits1;
				currentByte += ((shift1 < 0) ? ((unsigned) buf1 >> -shift1) : ((unsigned) buf1 << shift1));
				if (shift1 < 0) {
					encodedBytes[byteIndex += 1] = currentByte;
					bitPosition = 0;
					currentByte = 0;
					buf1 = buf1 & (((((0 - shift1) < 0) ? ((unsigned) 1 >> -(0 - shift1)) : ((unsigned) 1 << (0 - shift1)))) - 1);
					bufBits1 -= bitsAvailable1;
				} else {
					bitPosition += bufBits1;
					goto l2;
				}
			}
		l2:	/* end nextBits:put: */;
		} else {
			sign = 0;
			diff = (samples[sampleIndex += 1]) - predicted;
			if (diff < 0) {
				sign = deltaSignMask;
				diff = 0 - diff;
			}
			delta = 0;
			predictedDelta = 0;
			bit = deltaValueHighBit;
			while (bit > 0) {
				if (diff >= step) {
					delta += bit;
					predictedDelta += step;
					diff -= step;
				}
				step = ((unsigned) step >> 1);
				bit = ((unsigned) bit >> 1);
			}
			predictedDelta += step;
			if (sign > 0) {
				predicted -= predictedDelta;
			} else {
				predicted += predictedDelta;
			}
			if (predicted > 32767) {
				predicted = 32767;
			} else {
				if (predicted < -32768) {
					predicted = -32768;
				}
			}
			index += indexTable[delta + 1];
			if (index < 0) {
				index = 0;
			} else {
				if (index > 88) {
					index = 88;
				}
			}
			step = stepSizeTable[index + 1];
			/* begin nextBits:put: */
			buf2 = sign | delta;
			bufBits2 = bitsPerSample;
			while (1) {
				bitsAvailable2 = 8 - bitPosition;
				shift2 = bitsAvailable2 - bufBits2;
				currentByte += ((shift2 < 0) ? ((unsigned) buf2 >> -shift2) : ((unsigned) buf2 << shift2));
				if (shift2 < 0) {
					encodedBytes[byteIndex += 1] = currentByte;
					bitPosition = 0;
					currentByte = 0;
					buf2 = buf2 & (((((0 - shift2) < 0) ? ((unsigned) 1 >> -(0 - shift2)) : ((unsigned) 1 << (0 - shift2)))) - 1);
					bufBits2 -= bitsAvailable2;
				} else {
					bitPosition += bufBits2;
					goto l3;
				}
			}
		l3:	/* end nextBits:put: */;
		}
	}
	if (bitPosition > 0) {
		encodedBytes[byteIndex += 1] = currentByte;
	}
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(0, rcvr, predicted);
	storeIntegerofObjectwithValue(1, rcvr, index);
	storeIntegerofObjectwithValue(6, rcvr, currentByte);
	storeIntegerofObjectwithValue(7, rcvr, bitPosition);
	storeIntegerofObjectwithValue(8, rcvr, byteIndex);
	storeIntegerofObjectwithValue(12, rcvr, sampleIndex);
	pop(1);
}

int primADPCMCodecprivateEncodeStereo(void) {
    int rcvr;
    int count;

	rcvr = stackValue(1);
	count = checkedIntegerValueOf(stackValue(0));
	if (!(successFlag)) {
		return null;
	}
	success(0);
	if (!(successFlag)) {
		return null;
	}
	pop(1);
}
