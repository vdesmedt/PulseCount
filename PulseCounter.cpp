#define DEBUG
#include "PulseCounter.h"

PulseCounter::PulseCounter(int pin) {
  int i;
  bounce.attach(pin);
  for(i = 0 ; i <= KEEP_N_LAST_PULSE ; i++) {
    pulses[i] = 0;
  }
  pulseIndex = -1;
  pulseCount = 0;
}

void PulseCounter::update() {
  if(bounce.update() && bounce.read() == HIGH)
    addPulse();
}

void PulseCounter::addPulse() {
  pulseIndex = moveNext(pulseIndex);
  if(pulses[pulseIndex] != 0)
    free(pulses[pulseIndex]);
  pulses[pulseIndex] = new MillisExt(millis());
  pulseCount++;
}

int PulseCounter::moveNext(int i) {
	return ++i = KEEP_N_LAST_PULSE ? 0 : i; 
}

int PulseCounter::movePrevious(int i) {
  return --i < 0 ? KEEP_N_LAST_PULSE-1 : i;
}

double PulseCounter::getPulseRate(unsigned long interval) {
  MillisExt now(millis());
  MillisExt lowerBound(now-interval);

#ifdef DEBUG 
  Serial.print("Now :"); Serial.print(now.print()); 
  Serial.print(" - lowerBound :"); Serial.print(lowerBound.print());
  Serial.print(" - pulseIndex :"); Serial.println(pulseIndex);
  for(int j = 0 ; j < KEEP_N_LAST_PULSE ; j++) {
    Serial.print(" ["); Serial.print(j); Serial.print("]="); 
    if(pulses[j]==0)
      Serial.print("nil");
    else
      Serial.print(pulses[j]->print());
  }
  Serial.println("");
#endif

  if(pulses[pulseIndex] == 0 || *pulses[pulseIndex] < lowerBound) {//No measures in last interval
#ifdef DEBUG
    Serial.println("No data found in last interval. returning 0.");
#endif
    return 0;
  }

  int pulseCountDuringInterval = 1;
  int i = movePrevious(pulseIndex);
#ifdef DEBUG
  Serial.print("-");
#endif
  while(i != pulseIndex && pulses[i] !=0 && *pulses[i] > lowerBound) {
    i = movePrevious(i);
#ifdef DEBUG
  Serial.print("-");
#endif
    pulseCountDuringInterval++;
  }
  if(pulseCountDuringInterval == KEEP_N_LAST_PULSE) //If more than KEEP_N_LAST_PULSE happend during interval.
  {
#ifdef DEBUG
  Serial.print("+");
#endif
    i = moveNext(i);
  }

  double ellapsedTime = (now-(pulses[i]==0?0:*pulses[i])).getTotalMillisecond();
  double rate =  ((double)pulseCountDuringInterval * 60 * 1000) / ellapsedTime;
#ifdef DEBUG
  Serial.print("Found ");Serial.print(pulseCountDuringInterval); Serial.print(" pulse(s) on an interval of "); Serial.print(ellapsedTime); Serial.print("ms -> rate ="); Serial.println(rate);
#endif
  return rate;
}

unsigned long PulseCounter::getPulseCount() {
  return pulseCount;
}
