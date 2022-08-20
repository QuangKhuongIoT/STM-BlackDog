/**
 * @file mybutton.c
 * @author Tran Quang Khuong (khuong.tranquang@gmail.com)
 * @brief
 * @version v1.0.0
 * @date 2022-08-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "mybutton.h"
/**
 * @brief function to set debounce time
 *
 * @param button button need to be set debounce time
 * @param timeMs time debounce in ms
 */
void inputSetDebounce(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.debounceTimeMs = timeMs;
}

/**
 * @brief function to set click time
 *
 * @param button button need to be set click time
 * @param timeMs time click in ms
 */
void inputSetClickTime(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.clickTimeMs = timeMs;
}

/**
 * @brief function to set press time
 *
 * @param button button need to be set press time
 * @param timeMs time press in ms
 */
void inputSetPressTime(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.pressTimeMs = timeMs;
}

/**
 * @brief function to set long press time
 *
 * @param button button need to be set long press time
 * @param timeMs time long press in ms
 */
void inputSetLongPressTime(myButton* button, uint16_t timeMs) {
    button->_proccessInfo.longPressTimeMs = timeMs;
}

/**
 * @brief function get current button level
 *
 * @param button button to get info
 * @return uint8_t level
 */
uint8_t inputGetStatus(myButton* button) {
    return button->_inputInfo.status.logicState;
}

/**
 * @brief function to get multi click counter
 *
 * @param button button to get info
 * @return uint16_t counter value
 */
uint16_t inputGetClickCounter(myButton* button) {
    return button->_proccessInfo.lastMultiClickCounter;
}

/**
 * @brief function to get recent button active time
 *
 * @param button button to get info
 * @return uint32_t recent active time in ms
 */
uint32_t inputGetActiveTime(myButton* button) {
    return button->_proccessInfo.activeTimeMs;
}

/**
 * @brief function to get recent button inactive time
 *
 * @param button button to get info
 * @return uint32_t recent inactive time in ms
 */
uint32_t inputGetDeactiveTime(myButton* button) {
    return button->_proccessInfo.inactiveTimeMs;
}

/**
 * @brief to config input as a button
 *
 * @param button name
 * @param activeLevel level when this input active [0:1]
 * @param inputInit function pointer to input init function
 * @param getInputStatus function pointer to get input status function
 * @param getCurrentTickMs function pointer to get current system tick ms function
 */
void inputConfig(myButton* button, uint8_t activeLevel, v_callbackFunc inputInit, u8_callbackFunc getInputStatus, u32_callbackFunc getCurrentTickMs) {
    button->attachClick = inputAttachClick;
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

/**
 * @brief function button proccess, put it in main loop
 *
 * @param button button name to checking
 */
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
            button->_proccessInfo.activeTimeMs = button->_proccessInfo.actionTimeMs;
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
            button->_proccessInfo.inactiveTimeMs = button->_proccessInfo.actionTimeMs;
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
/**
 * @brief add function callback for click event
 *
 * @param button button added callback
 * @param newFunction function callback
 */
void inputAttachClick(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrClickFn = newFunction;
}

/**
 * @brief add function callback for double click event
 *
 * @param button button added callback
 * @param newFunction function callback
 */
void inputAttachDoubleClick(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrDoubleClickFn = newFunction;
}

/**
 * @brief add function callback for multi click event
 *
 * @param button button added callback
 * @param newFunction function callback
 */
void inputAttachMultiClick(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrMultiClickFn = newFunction;
}

/**
 * @brief add function callback for press event
 *
 * @param button button added callback
 * @param newFunction function callback
 */
void inputAttachPress(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrPressFn = newFunction;
}

/**
 * @brief add function callback for long press event
 *
 * @param button button added callback
 * @param newFunction function callback
 */
void inputAttachLongPress(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrLongPressFn = newFunction;
}

/**
 * @brief add function callback for long press release event
 *
 * @param button button added callback
 * @param newFunction function callback
 */
void inputAttachDepress(myButton* button, v_callbackFunc newFunction) {
    button->_event.ptrLongPressReleaseFn = newFunction;
}