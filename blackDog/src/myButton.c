/**
 * @file mybutton.c
 * @author Tran Quang Khuong (khuong.tranquang@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "mybutton.h"

void inputSetDebounce(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.debounceTimeMs = timeMs;
}
void inputSetClickTime(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.clickTimeMs = timeMs;
}
void inputSetPressTime(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.pressTimeMs = timeMs;
}
void inputSetLongPressTime(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.longPressTimeMs = timeMs;
}

void inputConfig(myButton* button, uint8_t activeLevel, v_callbackFunc inputInit, u8_callbackFunc getInputStatus, u32_callbackFunc getCurrentTickMs) {
    // button->attachClick = inputAttachClick;
    if (button->_proccessInfo.debounceTimeMs == 0) button->_proccessInfo.debounceTimeMs = 50;
    if (button->_proccessInfo.clickTimeMs == 0) button->_proccessInfo.clickTimeMs = 400;
    if (button->_proccessInfo.endActionTimeMs == 0) button->_proccessInfo.endActionTimeMs = 600;
    if (button->_proccessInfo.pressTimeMs == 0) button->_proccessInfo.pressTimeMs = 1000;
    if (button->_proccessInfo.longPressTimeMs == 0) button->_proccessInfo.longPressTimeMs = 5000;
    button->_inputInfo.status.inputConfigSuccess = 0;
    button->_inputInfo.status.activeLevel = activeLevel;
    if ((inputInit) && (getInputStatus) && (getCurrentTickMs)) {
        // button->_inputInfo.ptrInitFn = inputInit;
        inputInit();
        // button->_inputInfo.ptrInitFn();
        button->_inputInfo.ptrGetInputStatusFn = getInputStatus;
        button->_inputInfo.ptrGetCurrentTickMsFn = getCurrentTickMs;
        button->_inputInfo.status.inputConfigSuccess = 1;
    }
}

void inputTick(myButton* button) {
    if (!(button->_inputInfo.status.inputConfigSuccess)) return;

    button->_inputInfo.status.logicState = button->_inputInfo.ptrGetInputStatusFn();

    if (button->_inputInfo.status.logicLastState != button->_inputInfo.status.logicState) {
        button->_inputInfo.status.logicLastState = button->_inputInfo.status.logicState;
        button->_proccessInfo.startTimeMs = button->_inputInfo.ptrGetCurrentTickMsFn();
    }

    button->_proccessInfo.actionTimeMs = button->_inputInfo.ptrGetCurrentTickMsFn() - button->_proccessInfo.startTimeMs;

    if (button->_inputInfo.status.logicState == button->_inputInfo.status.activeLevel) {
        if (button->_proccessInfo.actionTimeMs > button->_proccessInfo.debounceTimeMs) {
            if (button->_proccessInfo.logicLastState != button->_inputInfo.status.logicState) {
                button->_proccessInfo.logicLastState = button->_inputInfo.status.logicState;
                button->_proccessInfo.multiClickCounter++;
                switch (button->_proccessInfo.multiClickCounter) {
                    case 0: {
                        button->_proccessInfo.event = EVENT_STT_UNKNOWN;
                        break;
                    }
                    case 1: {
                        button->_proccessInfo.event = EVENT_STT_CLICK;
                        break;
                    }
                    case 2: {
                        button->_proccessInfo.event = EVENT_STT_DOUBLE_CLICK;
                        break;
                    }
                    default:
                        button->_proccessInfo.event = EVENT_STT_MULTI_CLICK;
                        break;
                }
            } else {
                if (button->_proccessInfo.multiClickCounter <= 1) {
                    if ((button->_proccessInfo.actionTimeMs > button->_proccessInfo.pressTimeMs) && (button->_proccessInfo.actionTimeMs < button->_proccessInfo.longPressTimeMs)) {
                        button->_proccessInfo.event = EVENT_STT_PRESS;
                    } else if ((button->_proccessInfo.actionTimeMs > button->_proccessInfo.longPressTimeMs) && (button->_proccessInfo.event != EVENT_STT_LONG_PRESS)) {
                        button->_proccessInfo.event = EVENT_STT_LONG_PRESS;
                        if (button->_event.ptrLongPressFn) button->_event.ptrLongPressFn();
                    }
                }
            }
        }
    } else {
        if (button->_proccessInfo.actionTimeMs > button->_proccessInfo.debounceTimeMs) {
            button->_proccessInfo.logicLastState = button->_inputInfo.status.logicState;
            if (button->_proccessInfo.actionTimeMs > button->_proccessInfo.endActionTimeMs) {
                button->_proccessInfo.lastMultiClickCounter = button->_proccessInfo.multiClickCounter;
                button->_proccessInfo.multiClickCounter = 0;
                if (button->_proccessInfo.event != EVENT_STT_LONG_PRESS) {
                    switch (button->_proccessInfo.event) {
                        case EVENT_STT_CLICK: {
                            if (button->_event.ptrClickFn) button->_event.ptrClickFn();
                            break;
                        }
                        case EVENT_STT_DOUBLE_CLICK: {
                            if (button->_event.ptrDoubleClickFn) button->_event.ptrDoubleClickFn();
                            break;
                        }
                        case EVENT_STT_MULTI_CLICK: {
                            if (button->_event.ptrMultiClickFn) button->_event.ptrMultiClickFn();
                            break;
                        }
                        case EVENT_STT_PRESS: {
                            if (button->_event.ptrPressFn) button->_event.ptrPressFn();
                            break;
                        }

                        default:
                            break;
                    }
                } else {
                    if (button->_event.ptrLongPressReleaseFn) button->_event.ptrLongPressReleaseFn();
                }
                button->_proccessInfo.event = EVENT_STT_UNKNOWN;
            }
        }
    }
}

void inputAttachClick(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrClickFn = newFunction;
}
void inputAttachDoubleClick(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrDoubleClickFn = newFunction;
}
void inputAttachMultiClick(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrMultiClickFn = newFunction;
}
void inputAttachPress(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrPressFn = newFunction;
}
void inputAttachLongPress(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrLongPressFn = newFunction;
}
void inputAttachDepress(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrLongPressReleaseFn = newFunction;
}