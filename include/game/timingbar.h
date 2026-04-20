#ifndef TIMINGBAR_H
#define TIMINGBAR_H

typedef enum {
    TIMING_BAR_RUNNING,
    TIMING_BAR_WIN,
    TIMING_BAR_LOSE

} TimingBarResult;

extern TimingBarResult timingBarResult;

#endif