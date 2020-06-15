#ifndef __VTS_PROTOCOL_H__
#define __VTS_PROTOCOL_H__

#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "socket_tcp.h"

/* According to the length of Nanotec drvier protocol, the maximum length is not more than 50 */
#define VTS_MAXIMUM_PROTOCOL_LENGTH 50


typedef enum VTS_Protocol_Number
{
    DO_NOTHING                       = 0,
    GET_FIRMWARE_VERSION             = 1,
    GET_STATUS                       = 2,
    SET_STEP_MODE                    = 3,
    SET_PHASE_CURRENT                = 4,
    SET_PHASE_CURRENT_STANDSTILL     = 5,
    SET_LIMIT_SWITCH_BEHAVIOUR       = 6,
    SET_ENCODER_TYPE                 = 7,
    SET_ENCODER_NUMBER_OF_INCREMENTS = 8,
    MASK_IOS                         = 9,
    SET_IO_POLARITY                  = 10,
    GET_INPUT_POLARITY               = 11,
    SET_DIGITAL_INPUT_FUNCTION       = 12,
    SET_DIGITAL_OUTPUT_FUNCTION      = 13,
    GET_DIGITAL_OUTPUT_FUNCTION      = 14,
    SET_OUTPUTS                      = 15,
    PROHIBIT_CONTINUATION_RECORD     = 16,
    GET_STEP_MODE                    = 17,
    GET_PHASE_CURRENT                = 18,
    GET_PHASE_CURRENT_STANDSTILL     = 19,
    RESET_POSITION_ERROR             = 20,
    GET_LIMIT_SWITCH_BEHAVIOUR       = 21,
    GET_ENCODER_TYPE                 = 22,
    GET_ENCODER_NUMBER_OF_INCREMENTS = 23,
    GET_DIGITAL_INPUT_FUNCTION       = 24,
    GET_IOS                          = 25,
    START_MOTOR                      = 26,
    STOP_MOTOR                       = 27,
    SET_MAXIMUM_ACCELLERATION        = 28,
    GET_MAXIMUM_ACCELLERATION        = 29,
    SET_MAXIMUM_DECELLERATION        = 30,
    GET_MAXIMUM_DECELLERATION        = 31,
    SET_MAXIMUM_ACCELLERATION_JERK   = 32,
    GET_MAXIMUM_ACCELLERATION_JERK   = 33,
    SET_MAXIMUM_DECELLERATION_JERK   = 34,
    GET_MAXIMUM_DECELLERATION_JERK   = 35,
    GET_STEP                         = 36,
    GET_ENCODER_STEP                 = 37,
    SET_TRAVEL_DISTANCE_STEPS        = 38,
    GET_TRAVEL_DISTANCE_STEPS        = 39,
    SET_DIRECTION_OF_ROTATION        = 40,
    GET_DIRECTION_OF_ROTATION        = 41,
    SET_POSITIONING_MODE             = 42,
    GET_POSITIONING_MODE             = 43,
    SET_RAMP_MODE                    = 44,
    GET_RAMP_MODE                    = 45,
    SET_MINIMUM_VELOCITY             = 46,
    GET_MINIMUM_VELOCITY             = 47,
    SET_MAXIMUM_VELOCITY             = 48,
    GET_MAXIMUM_VELOCITY             = 49
    
}_VTS_Protocol_NumberTypeDef;


_VTS_Protocol_NumberTypeDef analyze_Tcp_Protocol_Command(int Socket,uint8_t *packet);



#endif

