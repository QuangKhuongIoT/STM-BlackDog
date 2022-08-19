/**
 * @file myButton.h
 * @author Tran Quang Khuong (khuong.tranquang@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __MY_BUTTON_H
#define __MY_BUTTON_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

struct _mybutton;

typedef void (*v_callbackFunc)(void);
typedef uint32_t (*u32_callbackFunc)(void);
typedef uint8_t (*u8_callbackFunc)(void);
typedef void (*v_callbackEvent)(struct _mybutton *, v_callbackFunc);

typedef enum {
    EVENT_STT_UNKNOWN,
    EVENT_STT_CLICK,
    EVENT_STT_DOUBLE_CLICK,
    EVENT_STT_MULTI_CLICK,
    EVENT_STT_PRESS,
    EVENT_STT_LONG_PRESS,
    EVENT_STT_DEPRESS
} event_state_t;

typedef struct
{
    struct {
        uint8_t activeLevel : 1;            /* Logic level when active action happen */
        uint8_t logicState : 1;             /* Logic of current status */
        uint8_t logicLastState : 1;         /* Logic of last status */
        uint8_t inputLastState : 1;         /* input last status */
        uint8_t inputConfigSuccess : 1;     /* Flag of initialization successful */
        uint8_t spare : 3;                  /* Spare bits */
    } status;                               /* Input status info */
    v_callbackFunc ptrInitFn;               /* Function pointer for user in order to configuring input */
    u8_callbackFunc ptrGetInputStatusFn;    /* Function pointer for user tranfer the input status */
    u32_callbackFunc ptrGetCurrentTickMsFn; /* Function pointer for user tranfer the current system tick counter*/
} input_info_t;

typedef struct
{
    uint8_t logicLastState;         /* Logic of recent event */
    uint32_t startTimeMs;           /* Timer value at the begining of logic changing */
    uint32_t actionTimeMs;          /* Timer counter for current logic state status */
    uint32_t activeTimeMs;          /* Timer counter for recent active logic state status */
    uint32_t inactiveTimeMs;        /* Timer counter for recent deactive logic state status */
    uint16_t endActionTimeMs;       /* Timer value to detect an action was finished */
    uint16_t debounceTimeMs;        /* Setting value to remove noise effect */
    uint16_t clickTimeMs;           /* Setting value to detect a click action */
    uint16_t pressTimeMs;           /* Setting value to detect a press action */
    uint16_t longPressTimeMs;       /* Setting value to detect a long press action */
    uint16_t multiClickCounter;     /* Counter value contains the number of click actions to return the event respectively */
    uint16_t lastMultiClickCounter; /* Counter value contains the recent number of click actions */
    event_state_t event;            /* Recent event detected */
} process_info_t;

typedef struct
{
    v_callbackFunc ptrClickFn;            /* Click trigger event function pointer */
    v_callbackFunc ptrDoubleClickFn;      /* Double click trigger event function pointer */
    v_callbackFunc ptrMultiClickFn;       /* Multiple click trigger event function pointer */
    v_callbackFunc ptrPressFn;            /* Press trigger event function pointer */
    v_callbackFunc ptrLongPressFn;        /* Long press trigger event function pointer */
    v_callbackFunc ptrLongPressReleaseFn; /* Long press released trigger event function pointer */
} event_t;

typedef struct _mybutton {
    input_info_t _inputInfo;
    process_info_t _proccessInfo;
    event_t _event;

    v_callbackEvent attachClick;
} myButton;

void inputConfig(myButton *button, uint8_t activeLevel, v_callbackFunc inputInit, u8_callbackFunc getInputStatus, u32_callbackFunc getCurrentTickMs);
void inputTick(myButton *button);
void inputAttachClick(myButton *button, v_callbackFunc newFunction);
void inputAttachDoubleClick(myButton *button, v_callbackFunc newFunction);
void inputAttachMultiClick(myButton *button, v_callbackFunc newFunction);
void inputAttachPress(myButton *button, v_callbackFunc newFunction);
void inputAttachLongPress(myButton *button, v_callbackFunc newFunction);
void inputAttachDepress(myButton *button, v_callbackFunc newFunction);

void inputSetDebounce(myButton *button, uint16_t timeMs);
void inputSetClickTime(myButton *button, uint16_t timeMs);
void inputSetPressTime(myButton *button, uint16_t timeMs);
void inputSetLongPressTime(myButton *button, uint16_t timeMs);
uint8_t inputGetStatus(myButton *button);
uint16_t inputGetClickCounter(myButton *button);
uint32_t inputGetActiveTime(myButton *button);
uint32_t inputGerDeactiveTime(myButton *button);
#ifdef __cplusplus
}
#endif

#endif
