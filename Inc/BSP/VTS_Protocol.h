#ifndef __VTS_PROTOCOL_H__
#define __VTS_PROTOCOL_H__

#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "socket_tcp.h"

/* According to the length of Nanotec drvier protocol, the maximum length is not more than 50 */
#define VTS_MAXIMUM_PROTOCOL_LENGTH 40


typedef enum VTS_Protocol_Number
{
    PROTOCOL_ERR0R                   = 0,  
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
    
    SET_DIGITAL_INPUT_FUNCTION_A     = 12,
    SET_DIGITAL_INPUT_FUNCTION_B     = 13,
    SET_DIGITAL_INPUT_FUNCTION_C     = 14,
    SET_DIGITAL_INPUT_FUNCTION_D     = 15,
    SET_DIGITAL_INPUT_FUNCTION_E     = 16,
    SET_DIGITAL_INPUT_FUNCTION_F     = 17,
    SET_DIGITAL_OUTPUT_FUNCTION_A    = 18,
    SET_DIGITAL_OUTPUT_FUNCTION_B    = 19,
    SET_DIGITAL_OUTPUT_FUNCTION_C    = 20,
    SET_DIGITAL_OUTPUT_FUNCTION_D    = 21,
    GET_DIGITAL_INPUT_FUNCTION_A     = 22,
    GET_DIGITAL_INPUT_FUNCTION_B     = 23,
    GET_DIGITAL_INPUT_FUNCTION_C     = 24,
    GET_DIGITAL_INPUT_FUNCTION_D     = 25,
    GET_DIGITAL_INPUT_FUNCTION_E     = 26,
    GET_DIGITAL_INPUT_FUNCTION_F     = 27,
    GET_DIGITAL_OUTPUT_FUNCTION_A    = 28,
    GET_DIGITAL_OUTPUT_FUNCTION_B    = 29,
    GET_DIGITAL_OUTPUT_FUNCTION_C    = 30,
    GET_DIGITAL_OUTPUT_FUNCTION_D    = 31,

    SET_OUTPUTS                      = 32,
    PROHIBIT_CONTINUATION_RECORD     = 33,
    GET_STEP_MODE                    = 34,
    GET_PHASE_CURRENT                = 35,
    GET_PHASE_CURRENT_STANDSTILL     = 36,
    RESET_POSITION_ERROR             = 37,
    GET_LIMIT_SWITCH_BEHAVIOUR       = 38,
    GET_ENCODER_TYPE                 = 39,
    GET_ENCODER_NUMBER_OF_INCREMENTS = 40,
    GET_IOS                          = 41,
    START_MOTOR                      = 42,
    STOP_MOTOR                       = 43,
    SET_MAXIMUM_ACCELLERATION        = 44,
    GET_MAXIMUM_ACCELLERATION        = 45,
    SET_MAXIMUM_DECELLERATION        = 46,
    GET_MAXIMUM_DECELLERATION        = 47,
    SET_MAXIMUM_ACCELLERATION_JERK   = 48,
    GET_MAXIMUM_ACCELLERATION_JERK   = 49,
    SET_MAXIMUM_DECELLERATION_JERK   = 50,
    GET_MAXIMUM_DECELLERATION_JERK   = 51,
    GET_STEP                         = 52,
    GET_ENCODER_STEP                 = 53,
    SET_TRAVEL_DISTANCE_STEPS        = 54,
    GET_TRAVEL_DISTANCE_STEPS        = 55,
    SET_DIRECTION_OF_ROTATION        = 56,
    GET_DIRECTION_OF_ROTATION        = 57,
    SET_POSITIONING_MODE             = 58,
    GET_POSITIONING_MODE             = 59,
    SET_RAMP_MODE                    = 60,
    GET_RAMP_MODE                    = 61,
    SET_MINIMUM_VELOCITY             = 62,
    GET_MINIMUM_VELOCITY             = 63,
    SET_MAXIMUM_VELOCITY             = 64,
    GET_MAXIMUM_VELOCITY             = 65
    
}_VTS_Protocol_NumberTypeDef;

uint8_t mystr_find_Right_Side_Index(const uint8_t *s1,const uint8_t *s2);
_VTS_Protocol_NumberTypeDef analyze_Tcp_Protocol_Command(int Socket,uint8_t *packet);



#endif

