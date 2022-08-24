/**
 * @file myOutput.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * feature
 * - on
 * - off
 * - pulse
 * - pwm
 */

#ifndef __MY_OUTPUT_H
#define __MY_OUTPUT_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*v_outputCallbackFunc)(void);
typedef void (*v_outputDataCallbackFunc)(char *);
typedef void (*v_outputOnOffCallbackFunc)(uint8_t);
typedef uint32_t (*u32_outputCallbackFunc)(void);

typedef enum {
    OP_STT_OFF = 0,
    OP_STT_ON,
    OP_STT_DATA,
    OP_STT_UNKNOWN
} output_status_t;

typedef enum {
    OP_MODE_UNKNOWN = 0, /* mode output disable */
    OP_MODE_ON_OFF,      /* mode output active and inactive only */
    OP_MODE_PULSE,       /* mode output with a number pulse, period and duty cycle modificable */
    OP_MODE_PWM          /* mode output with pulse forever, period and duty cycle modificable*/
} output_mode_t;

typedef struct
{
    uint8_t mode;
    uint8_t activeLevel;
    uint8_t pulse;
    uint16_t period;
    uint16_t duty;
    v_outputCallbackFunc ptrInitFn;
    u32_outputCallbackFunc ptrGetCurrentTimeMs;
} output_info_t;

typedef struct
{
    uint8_t initSuccessFlag;
    uint8_t pulseCounter;
    output_status_t state;
    output_status_t lastState;
    uint32_t startTime;
    uint32_t runtime;
} output_proccess_t;

typedef struct
{
    v_outputOnOffCallbackFunc ptrOnOffFn;
    v_outputDataCallbackFunc ptrDataFn;
} output_event_t;

typedef struct
{
    output_info_t outputInfo;
    output_proccess_t outputProccess;
    output_event_t outputEvent;

} myOutput;

void outputTick(myOutput *output);
void setOutputConfig(myOutput *output, uint8_t activeLevel, v_outputCallbackFunc initOutput, u32_outputCallbackFunc getCurrentTimeMs);
void setOutputOnOff(myOutput *output, output_status_t state);
void setOutputPulse(myOutput *output, uint8_t numOfPulse, uint16_t period, uint16_t duty);
void setOutputPwm(myOutput *output, uint16_t period, uint16_t duty);

void outputOnOff(myOutput *output, v_outputOnOffCallbackFunc newFunction);
void outputData(myOutput *output, v_outputDataCallbackFunc newFunction);

#ifdef __cplusplus
}
#endif
#endif