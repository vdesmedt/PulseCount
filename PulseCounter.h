#ifndef PulseCounter_h
#define PulseCounter_h 

#include <Arduino.h> 
#include "MillisExt.h"
#endif

class PulseCounter {
public:
	PulseCounter();
	void addPulse();
	double getPulseRate(int interval);
	unsigned long getPulseCount();
private:
	static const int KEEP_N_LAST_PULSE = 20;
	MillisExt* pulses[KEEP_N_LAST_PULSE];
	int pulseIndex;
	unsigned long pulseCount;
	int moveNext(int i);
	int movePrevious(int i);
};