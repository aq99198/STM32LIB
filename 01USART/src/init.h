#pragma once

typedef struct core_t {
    serialPort_t *mainport;
    serialPort_t *flexport;
    serialPort_t *gpsport;
    serialPort_t *telemport;
    serialPort_t *rcvrport;
    uint8_t numRCChannels;                  // number of rc channels as reported by current input driver
    bool useServo;                          // feature SERVO_TILT or wing/airplane mixers will enable this
    uint8_t numServos;                      // how many total hardware servos we have. used by mixer
} core_t;

void SysInit(void);
