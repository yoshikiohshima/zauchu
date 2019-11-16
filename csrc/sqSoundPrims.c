/* Automatically generated from Squeak on (12 January 2000 6:48:40 pm ) */

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
int primFMSoundmixSampleCountintostartingAtleftVolrightVol(void);
int primLoopedSampledSoundmixSampleCountintostartingAtleftVolrightVol(void);
int primPluckedSoundmixSampleCountintostartingAtleftVolrightVol(void);
int primReverbSoundapplyReverbTostartingAtcount(void);
int primSampledSoundmixSampleCountintostartingAtleftVolrightVol(void);

int primFMSoundmixSampleCountintostartingAtleftVolrightVol(void) {
    int rcvr;
    int n;
    short int *aSoundBuffer;
    int startIndex;
    int leftVol;
    int rightVol;
    int sample;
    int s;
    int sliceIndex;
    int offset;
    int lastIndex;
    int i;
    int doingFM;
    int scaledVol;
    int scaledVolIncr;
    int scaledVolLimit;
    int count;
    short int *waveTable;
    int scaledWaveTableSize;
    int scaledIndex;
    int scaledIndexIncr;
    int normalizedModulation;
    int scaledOffsetIndex;
    int scaledOffsetIndexIncr;

	rcvr = stackValue(5);
	n = checkedIntegerValueOf(stackValue(4));
	aSoundBuffer = arrayValueOf(stackValue(3));
	aSoundBuffer -= 1;
	startIndex = checkedIntegerValueOf(stackValue(2));
	leftVol = checkedIntegerValueOf(stackValue(1));
	rightVol = checkedIntegerValueOf(stackValue(0));
	scaledVol = fetchIntegerofObject(3, rcvr);
	scaledVolIncr = fetchIntegerofObject(4, rcvr);
	scaledVolLimit = fetchIntegerofObject(5, rcvr);
	count = fetchIntegerofObject(7, rcvr);
	waveTable = fetchArrayofObject(8, rcvr);
	waveTable -= 1;
	scaledWaveTableSize = fetchIntegerofObject(9, rcvr);
	scaledIndex = fetchIntegerofObject(10, rcvr);
	scaledIndexIncr = fetchIntegerofObject(11, rcvr);
	normalizedModulation = fetchIntegerofObject(14, rcvr);
	scaledOffsetIndex = fetchIntegerofObject(15, rcvr);
	scaledOffsetIndexIncr = fetchIntegerofObject(16, rcvr);
	if (!(successFlag)) {
		return null;
	}
	doingFM = (normalizedModulation != 0) && (scaledOffsetIndexIncr != 0);
	lastIndex = (startIndex + n) - 1;
	for (sliceIndex = startIndex; sliceIndex <= lastIndex; sliceIndex += 1) {
		sample = ((int) (scaledVol * (waveTable[(((int) scaledIndex >> 15)) + 1])) >> 15);
		if (doingFM) {
			offset = normalizedModulation * (waveTable[(((int) scaledOffsetIndex >> 15)) + 1]);
			scaledOffsetIndex = (scaledOffsetIndex + scaledOffsetIndexIncr) % scaledWaveTableSize;
			if (scaledOffsetIndex < 0) {
				scaledOffsetIndex += scaledWaveTableSize;
			}
			scaledIndex = ((scaledIndex + scaledIndexIncr) + offset) % scaledWaveTableSize;
			if (scaledIndex < 0) {
				scaledIndex += scaledWaveTableSize;
			}
		} else {
			scaledIndex = (scaledIndex + scaledIndexIncr) % scaledWaveTableSize;
		}
		if (leftVol > 0) {
			i = (2 * sliceIndex) - 1;
			s = (aSoundBuffer[i]) + (((int) (sample * leftVol) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		if (rightVol > 0) {
			i = 2 * sliceIndex;
			s = (aSoundBuffer[i]) + (((int) (sample * rightVol) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		if (scaledVolIncr != 0) {
			scaledVol += scaledVolIncr;
			if (((scaledVolIncr > 0) && (scaledVol >= scaledVolLimit)) || ((scaledVolIncr < 0) && (scaledVol <= scaledVolLimit))) {
				scaledVol = scaledVolLimit;
				scaledVolIncr = 0;
			}
		}
	}
	count -= n;
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(3, rcvr, scaledVol);
	storeIntegerofObjectwithValue(4, rcvr, scaledVolIncr);
	storeIntegerofObjectwithValue(7, rcvr, count);
	storeIntegerofObjectwithValue(10, rcvr, scaledIndex);
	storeIntegerofObjectwithValue(15, rcvr, scaledOffsetIndex);
	pop(5);
}

int primLoopedSampledSoundmixSampleCountintostartingAtleftVolrightVol(void) {
    int rcvr;
    int n;
    short int *aSoundBuffer;
    int startIndex;
    int leftVol;
    int rightVol;
    int sampleIndex;
    int s;
    int sliceIndex;
    int leftVal;
    int m;
    int rightVal;
    int lastIndex;
    int i;
    int isInStereo;
    int compositeLeftVol;
    int compositeRightVol;
    int nextSampleIndex;
    int scaledVol;
    int scaledVolIncr;
    int scaledVolLimit;
    int count;
    int releaseCount;
    short int *leftSamples;
    short int *rightSamples;
    int lastSample;
    int loopEnd;
    int scaledLoopLength;
    int scaledIndex;
    int scaledIndexIncr;

	rcvr = stackValue(5);
	n = checkedIntegerValueOf(stackValue(4));
	aSoundBuffer = arrayValueOf(stackValue(3));
	aSoundBuffer -= 1;
	startIndex = checkedIntegerValueOf(stackValue(2));
	leftVol = checkedIntegerValueOf(stackValue(1));
	rightVol = checkedIntegerValueOf(stackValue(0));
	scaledVol = fetchIntegerofObject(3, rcvr);
	scaledVolIncr = fetchIntegerofObject(4, rcvr);
	scaledVolLimit = fetchIntegerofObject(5, rcvr);
	count = fetchIntegerofObject(7, rcvr);
	releaseCount = fetchIntegerofObject(8, rcvr);
	leftSamples = fetchArrayofObject(10, rcvr);
	leftSamples -= 1;
	rightSamples = fetchArrayofObject(11, rcvr);
	rightSamples -= 1;
	lastSample = fetchIntegerofObject(16, rcvr);
	loopEnd = fetchIntegerofObject(17, rcvr);
	scaledLoopLength = fetchIntegerofObject(18, rcvr);
	scaledIndex = fetchIntegerofObject(19, rcvr);
	scaledIndexIncr = fetchIntegerofObject(20, rcvr);
	if (!(successFlag)) {
		return null;
	}
	isInStereo = leftSamples != rightSamples;
	compositeLeftVol = ((int) (leftVol * scaledVol) >> 15);
	compositeRightVol = ((int) (rightVol * scaledVol) >> 15);
	i = (2 * startIndex) - 1;
	lastIndex = (startIndex + n) - 1;
	for (sliceIndex = startIndex; sliceIndex <= lastIndex; sliceIndex += 1) {
		sampleIndex = ((int) (scaledIndex += scaledIndexIncr) >> 9);
		if ((sampleIndex > loopEnd) && (count > releaseCount)) {
			sampleIndex = ((int) (scaledIndex -= scaledLoopLength) >> 9);
		}
		if ((nextSampleIndex = sampleIndex + 1) > lastSample) {
			if (sampleIndex > lastSample) {
				count = 0;
				if (!(successFlag)) {
					return null;
				}
				storeIntegerofObjectwithValue(3, rcvr, scaledVol);
				storeIntegerofObjectwithValue(4, rcvr, scaledVolIncr);
				storeIntegerofObjectwithValue(7, rcvr, count);
				storeIntegerofObjectwithValue(19, rcvr, scaledIndex);
				pop(6);
				pushInteger(null);
				return null;
			}
			if (scaledLoopLength == 0) {
				nextSampleIndex = sampleIndex;
			} else {
				nextSampleIndex = (((int) (scaledIndex - scaledLoopLength) >> 9)) + 1;
			}
		}
		m = scaledIndex & 511;
		rightVal = leftVal = ((int) (((leftSamples[sampleIndex]) * (512 - m)) + ((leftSamples[nextSampleIndex]) * m)) >> 9);
		if (isInStereo) {
			rightVal = ((int) (((rightSamples[sampleIndex]) * (512 - m)) + ((rightSamples[nextSampleIndex]) * m)) >> 9);
		}
		if (leftVol > 0) {
			s = (aSoundBuffer[i]) + (((int) (compositeLeftVol * leftVal) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		i += 1;
		if (rightVol > 0) {
			s = (aSoundBuffer[i]) + (((int) (compositeRightVol * rightVal) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		i += 1;
		if (scaledVolIncr != 0) {
			scaledVol += scaledVolIncr;
			if (((scaledVolIncr > 0) && (scaledVol >= scaledVolLimit)) || ((scaledVolIncr < 0) && (scaledVol <= scaledVolLimit))) {
				scaledVol = scaledVolLimit;
				scaledVolIncr = 0;
			}
			compositeLeftVol = ((int) (leftVol * scaledVol) >> 15);
			compositeRightVol = ((int) (rightVol * scaledVol) >> 15);
		}
	}
	count -= n;
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(3, rcvr, scaledVol);
	storeIntegerofObjectwithValue(4, rcvr, scaledVolIncr);
	storeIntegerofObjectwithValue(7, rcvr, count);
	storeIntegerofObjectwithValue(19, rcvr, scaledIndex);
	pop(5);
}

int primPluckedSoundmixSampleCountintostartingAtleftVolrightVol(void) {
    int rcvr;
    int n;
    short int *aSoundBuffer;
    int startIndex;
    int leftVol;
    int rightVol;
    int average;
    int scaledThisIndex;
    int scaledNextIndex;
    int sample;
    int s;
    int sliceIndex;
    int lastIndex;
    int i;
    int scaledVol;
    int scaledVolIncr;
    int scaledVolLimit;
    int count;
    short int *ring;
    int scaledIndex;
    int scaledIndexIncr;
    int scaledIndexLimit;

	rcvr = stackValue(5);
	n = checkedIntegerValueOf(stackValue(4));
	aSoundBuffer = arrayValueOf(stackValue(3));
	aSoundBuffer -= 1;
	startIndex = checkedIntegerValueOf(stackValue(2));
	leftVol = checkedIntegerValueOf(stackValue(1));
	rightVol = checkedIntegerValueOf(stackValue(0));
	scaledVol = fetchIntegerofObject(3, rcvr);
	scaledVolIncr = fetchIntegerofObject(4, rcvr);
	scaledVolLimit = fetchIntegerofObject(5, rcvr);
	count = fetchIntegerofObject(7, rcvr);
	ring = fetchArrayofObject(8, rcvr);
	ring -= 1;
	scaledIndex = fetchIntegerofObject(9, rcvr);
	scaledIndexIncr = fetchIntegerofObject(10, rcvr);
	scaledIndexLimit = fetchIntegerofObject(11, rcvr);
	if (!(successFlag)) {
		return null;
	}
	lastIndex = (startIndex + n) - 1;
	scaledThisIndex = scaledNextIndex = scaledIndex;
	for (sliceIndex = startIndex; sliceIndex <= lastIndex; sliceIndex += 1) {
		scaledNextIndex = scaledThisIndex + scaledIndexIncr;
		if (scaledNextIndex >= scaledIndexLimit) {
			scaledNextIndex = 32768 + (scaledNextIndex - scaledIndexLimit);
		}
		average = ((int) ((ring[((int) scaledThisIndex >> 15)]) + (ring[((int) scaledNextIndex >> 15)])) >> 1);
		ring[((int) scaledThisIndex >> 15)] = average;
		sample = ((int) (average * scaledVol) >> 15);
		scaledThisIndex = scaledNextIndex;
		if (leftVol > 0) {
			i = (2 * sliceIndex) - 1;
			s = (aSoundBuffer[i]) + (((int) (sample * leftVol) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		if (rightVol > 0) {
			i = 2 * sliceIndex;
			s = (aSoundBuffer[i]) + (((int) (sample * rightVol) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		if (scaledVolIncr != 0) {
			scaledVol += scaledVolIncr;
			if (((scaledVolIncr > 0) && (scaledVol >= scaledVolLimit)) || ((scaledVolIncr < 0) && (scaledVol <= scaledVolLimit))) {
				scaledVol = scaledVolLimit;
				scaledVolIncr = 0;
			}
		}
	}
	scaledIndex = scaledNextIndex;
	count -= n;
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(3, rcvr, scaledVol);
	storeIntegerofObjectwithValue(4, rcvr, scaledVolIncr);
	storeIntegerofObjectwithValue(7, rcvr, count);
	storeIntegerofObjectwithValue(9, rcvr, scaledIndex);
	pop(5);
}

int primReverbSoundapplyReverbTostartingAtcount(void) {
    int rcvr;
    short int *aSoundBuffer;
    int startIndex;
    int n;
    int j;
    int delayedLeft;
    int delayedRight;
    int sliceIndex;
    int i;
    int out;
    int tapIndex;
    int tapGain;
    int *tapDelays;
    int *tapGains;
    int tapCount;
    int bufferSize;
    int bufferIndex;
    short int *leftBuffer;
    short int *rightBuffer;

	rcvr = stackValue(3);
	aSoundBuffer = arrayValueOf(stackValue(2));
	aSoundBuffer -= 1;
	startIndex = checkedIntegerValueOf(stackValue(1));
	n = checkedIntegerValueOf(stackValue(0));
	tapDelays = fetchArrayofObject(7, rcvr);
	tapDelays -= 1;
	tapGains = fetchArrayofObject(8, rcvr);
	tapGains -= 1;
	tapCount = fetchIntegerofObject(9, rcvr);
	bufferSize = fetchIntegerofObject(10, rcvr);
	bufferIndex = fetchIntegerofObject(11, rcvr);
	leftBuffer = fetchArrayofObject(12, rcvr);
	leftBuffer -= 1;
	rightBuffer = fetchArrayofObject(13, rcvr);
	rightBuffer -= 1;
	if (!(successFlag)) {
		return null;
	}
	for (sliceIndex = startIndex; sliceIndex <= ((startIndex + n) - 1); sliceIndex += 1) {
		delayedLeft = delayedRight = 0;
		for (tapIndex = 1; tapIndex <= tapCount; tapIndex += 1) {
			i = bufferIndex - (tapDelays[tapIndex]);
			if (i < 1) {
				i += bufferSize;
			}
			tapGain = tapGains[tapIndex];
			delayedLeft += tapGain * (leftBuffer[i]);
			delayedRight += tapGain * (rightBuffer[i]);
		}
		j = (2 * sliceIndex) - 1;
		out = (aSoundBuffer[j]) + (((int) delayedLeft >> 15));
		if (out > 32767) {
			out = 32767;
		}
		if (out < -32767) {
			out = -32767;
		}
		aSoundBuffer[j] = out;
		leftBuffer[bufferIndex] = out;
		j += 1;
		out = (aSoundBuffer[j]) + (((int) delayedRight >> 15));
		if (out > 32767) {
			out = 32767;
		}
		if (out < -32767) {
			out = -32767;
		}
		aSoundBuffer[j] = out;
		rightBuffer[bufferIndex] = out;
		bufferIndex = (bufferIndex % bufferSize) + 1;
	}
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(11, rcvr, bufferIndex);
	pop(3);
}

int primSampledSoundmixSampleCountintostartingAtleftVolrightVol(void) {
    int rcvr;
    int n;
    short int *aSoundBuffer;
    int startIndex;
    int leftVol;
    int rightVol;
    int sampleIndex;
    int sample;
    int s;
    int lastIndex;
    int outIndex;
    int i;
    int overflow;
    int scaledVol;
    int scaledVolIncr;
    int scaledVolLimit;
    int count;
    short int *samples;
    int samplesSize;
    int scaledIndex;
    int indexHighBits;
    int scaledIncrement;

	rcvr = stackValue(5);
	n = checkedIntegerValueOf(stackValue(4));
	aSoundBuffer = arrayValueOf(stackValue(3));
	aSoundBuffer -= 1;
	startIndex = checkedIntegerValueOf(stackValue(2));
	leftVol = checkedIntegerValueOf(stackValue(1));
	rightVol = checkedIntegerValueOf(stackValue(0));
	scaledVol = fetchIntegerofObject(3, rcvr);
	scaledVolIncr = fetchIntegerofObject(4, rcvr);
	scaledVolLimit = fetchIntegerofObject(5, rcvr);
	count = fetchIntegerofObject(7, rcvr);
	samples = fetchArrayofObject(8, rcvr);
	samples -= 1;
	samplesSize = fetchIntegerofObject(10, rcvr);
	scaledIndex = fetchIntegerofObject(11, rcvr);
	indexHighBits = fetchIntegerofObject(12, rcvr);
	scaledIncrement = fetchIntegerofObject(13, rcvr);
	if (!(successFlag)) {
		return null;
	}
	lastIndex = (startIndex + n) - 1;
	outIndex = startIndex;
	sampleIndex = indexHighBits + (((unsigned) scaledIndex) >> 16);
	while ((sampleIndex <= samplesSize) && (outIndex <= lastIndex)) {
		sample = ((int) ((samples[sampleIndex]) * scaledVol) >> 15);
		if (leftVol > 0) {
			i = (2 * outIndex) - 1;
			s = (aSoundBuffer[i]) + (((int) (sample * leftVol) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		if (rightVol > 0) {
			i = 2 * outIndex;
			s = (aSoundBuffer[i]) + (((int) (sample * rightVol) >> 15));
			if (s > 32767) {
				s = 32767;
			}
			if (s < -32767) {
				s = -32767;
			}
			aSoundBuffer[i] = s;
		}
		if (scaledVolIncr != 0) {
			scaledVol += scaledVolIncr;
			if (((scaledVolIncr > 0) && (scaledVol >= scaledVolLimit)) || ((scaledVolIncr < 0) && (scaledVol <= scaledVolLimit))) {
				scaledVol = scaledVolLimit;
				scaledVolIncr = 0;
			}
		}
		scaledIndex += scaledIncrement;
		if (scaledIndex >= 536870912) {
			overflow = ((unsigned) scaledIndex) >> 16;
			indexHighBits += overflow;
			scaledIndex -= overflow << 16;
		}
		sampleIndex = indexHighBits + (((unsigned) scaledIndex) >> 16);
		outIndex += 1;
	}
	count -= n;
	if (!(successFlag)) {
		return null;
	}
	storeIntegerofObjectwithValue(3, rcvr, scaledVol);
	storeIntegerofObjectwithValue(4, rcvr, scaledVolIncr);
	storeIntegerofObjectwithValue(7, rcvr, count);
	storeIntegerofObjectwithValue(11, rcvr, scaledIndex);
	storeIntegerofObjectwithValue(12, rcvr, indexHighBits);
	pop(5);
}
