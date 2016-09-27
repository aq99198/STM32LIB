#pragma once

extern uint16_t acc_1G;
extern int16_t accData[3];
extern int16_t gyroData[3];
extern int16_t heading;
extern int16_t angle[2];
extern int16_t gyroADC[3], accADC[3], accSmooth[3], magADC[3];
extern int16_t gyroData[3];

enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
    AUX1,
    AUX2,
    AUX3,
    AUX4
};

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

void imuInit(void);
void annexCode(void);
void computeIMU(void);
void blinkLED(uint8_t num, uint8_t wait, uint8_t repeat);
int getEstimatedAltitude(void);
