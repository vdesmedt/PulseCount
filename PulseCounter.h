#ifndef PulseCounter_h
#define PulseCounter_h 

#include <MillisExt.h>
#include <Bounce2.h>

class PulseCounter {
public:
	PulseCounter(int pin);
	void update();
	void addPulse();
	double getPulseRate(unsigned long interval);
	unsigned long getPulseCount();
private:
	static const int KEEP_N_LAST_PULSE = 20;
	MillisExt* pulses[KEEP_N_LAST_PULSE];
	Bounce bounce;
	int pulseIndex;
	unsigned long pulseCount;
	int moveNext(int i);
	int movePrevious(int i);
};

#endif
