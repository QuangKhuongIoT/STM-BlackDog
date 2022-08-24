#include "myOutput.h"

/**
 * @brief set configuration information of ouput channel
 *
 * @param output name of output object
 * @param activeLevel logic level when activated
 * @param initOutputFunc user output init function
 * @param getCurrentTimeMsFunc user get system time in milisecond
 */
void setOutputConfig(myOutput *output, uint8_t activeLevel, v_outputCallbackFunc initOutputFunc, u32_outputCallbackFunc getCurrentTimeMsFunc) {
    output->outputInfo.activeLevel = activeLevel;
    output->outputInfo.ptrInitFn = initOutputFunc;
    output->outputInfo.ptrGetCurrentTimeMs = getCurrentTimeMsFunc;
    if ((initOutputFunc) && (getCurrentTimeMsFunc)) {
        output->outputProccess.initSuccessFlag = 1;
    } else {
        output->outputProccess.initSuccessFlag = 0;
    }
}

/**
 * @brief Set the Output On Off object
 *
 * @param output name of output object
 * @param state output state
 */
void setOutputOnOff(myOutput *output, output_status_t state) {
    output->outputInfo.mode = OP_MODE_ON_OFF;
    if (output->outputEvent.ptrOnOffFn) {
        output->outputEvent.ptrOnOffFn(state);
    }
}

/**
 * @brief Set the Output Pulse object
 *
 * @param output name of output object
 * @param numOfPulse number of pulse
 * @param period output pulse period
 * @param duty output pulse duty
 */
void setOutputPulse(myOutput *output, uint8_t numOfPulse, uint16_t period, uint16_t duty) {
    output->outputInfo.mode = OP_MODE_PULSE;
    output->outputProccess.pulseCounter = numOfPulse;
    output->outputInfo.period = period;
    output->outputInfo.duty = duty;
}

/**
 * @brief Set the Output Pwm object
 *
 * @param output name of output object
 * @param period output pulse period
 * @param duty output pulse duty
 */
void setOutputPwm(myOutput *output, uint16_t period, uint16_t duty) {
    output->outputInfo.mode = OP_MODE_PWM;
    output->outputInfo.period = period;
    output->outputInfo.duty = duty;
}

/**
 * @brief
 *
 * @param output name of output object
 * @param newFunction user output data function
 */
void outputOnOff(myOutput *output, v_outputOnOffCallbackFunc newFunction) {
    output->outputEvent.ptrOnOffFn = newFunction;
}

/**
 * @brief
 *
 * @param output name of output object
 * @param newFunction user output data function
 */
void outputData(myOutput *output, v_outputDataCallbackFunc newFunction) {
    output->outputEvent.ptrDataFn = newFunction;
}

/**
 * @brief
 *
 * @param output name of output object
 */
void outputTick(myOutput *output) {
    if (output->outputProccess.initSuccessFlag == 0) return;
    // if (output->outputProccess.state != output->outputProccess.lastState) {
    //     output->outputProccess.state = output->outputProccess.lastState;
    //     output->outputProccess.startTime = output->outputInfo.ptrGetCurrentTimeMs;
    // }
    switch (output->outputInfo.mode) {
        case OP_MODE_PWM:
        case OP_MODE_PULSE: {
            if ((output->outputInfo.mode == OP_MODE_PULSE) && (output->outputProccess.pulseCounter == 0)) return;
            output->outputProccess.runtime = output->outputInfo.ptrGetCurrentTimeMs() - output->outputProccess.startTime; /* get runtime in current period */
            if (output->outputProccess.runtime < output->outputInfo.duty) {
                if (output->outputProccess.lastState != output->outputInfo.activeLevel) {
                    output->outputProccess.lastState = output->outputInfo.activeLevel;
                    if (output->outputEvent.ptrOnOffFn) output->outputEvent.ptrOnOffFn(!output->outputInfo.activeLevel);
                }
            } else {
                if (output->outputProccess.lastState != !(output->outputInfo.activeLevel)) {
                    output->outputProccess.lastState = !(output->outputInfo.activeLevel);
                    if (output->outputEvent.ptrOnOffFn) output->outputEvent.ptrOnOffFn(output->outputInfo.activeLevel);
                }
                if (output->outputProccess.runtime >= output->outputInfo.period) {
                    if (output->outputInfo.mode == OP_MODE_PULSE) output->outputProccess.pulseCounter--;
                    output->outputProccess.startTime = output->outputInfo.ptrGetCurrentTimeMs();
                }
            }
            break;
        }

        default:
            break;
    }
}