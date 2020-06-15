#include "VTS_Protocol.h"
#include "socket_tcp.h"


/* test */
uint8_t *test_String;
uint8_t Position;


/* CMD list */
const uint8_t get_Firmware_Version[] = "v";   //1
const uint8_t get_Status[] = "$";             //2
const uint8_t set_Step_Mode[] = "g";          //3
const uint8_t set_Phase_Current[] = "i";      //4
const uint8_t set_Phase_Current_Standstill[] = "r";     //5 
const uint8_t set_Limit_Switch_Behaviour[] = "l";       //6
const uint8_t set_Encoder_Type[] = ":CL_rotenc_type=";  //7
const uint8_t set_Encoder_Number_Of_Increments[] = ":CL_rotenc_inc+"; //8
const uint8_t mask_IOS[] = "L";  //9
const uint8_t set_IO_Polarity[] = "h"; //10
const uint8_t get_Input_Polarity[] = "Zh";  //11

// VTS Scaner Main Borad 只用了一个限位开关 
const uint8_t set_Digital_Input_Function_A[] = ":port_in_a"; //12
const uint8_t set_Digital_Input_Function_B[] = ":port_in_b"; //13
const uint8_t set_Digital_Input_Function_C[] = ":port_in_c"; //14
const uint8_t set_Digital_Input_Function_D[] = ":port_in_d"; //15
const uint8_t set_Digital_Input_Function_E[] = ":port_in_e"; //16
const uint8_t set_Digital_Input_Function_F[] = ":port_in_f"; //17
// VTS Scaner Main Borad 只用了一个限位开关 
const uint8_t get_Digital_Input_Function_A[] = ":port_in_a"; //22
const uint8_t get_Digital_Input_Function_B[] = ":port_in_b"; //23
const uint8_t get_Digital_Input_Function_C[] = ":port_in_c"; //24
const uint8_t get_Digital_Input_Function_D[] = ":port_in_d"; //25
const uint8_t get_Digital_Input_Function_E[] = ":port_in_e"; //26
const uint8_t get_Digital_Input_Function_F[] = ":port_in_f"; //27

// VTS Scaner Main Borad 没有数字信号输出 
const uint8_t set_Digital_Output_Function_A[] = ":port_out_a"; //18
const uint8_t set_Digital_Output_Function_B[] = ":port_out_b"; //19
const uint8_t set_Digital_Output_Function_C[] = ":port_out_c"; //20
const uint8_t set_Digital_Output_Function_D[] = ":port_out_d"; //21
// VTS Scaner Main Borad 没有数字信号输出 
const uint8_t get_Digital_Output_Function_A[] = ":port_out_a"; //28
const uint8_t get_Digital_Output_Function_B[] = ":port_out_b"; //29
const uint8_t get_Digital_Output_Function_C[] = ":port_out_c"; //30
const uint8_t get_Digital_Output_Function_D[] = ":port_out_d"; //31
// VTS Scaner Main Borad 没有数字信号输出 
const uint8_t set_Outputs[] = "Y"; //32

const uint8_t prohibit_Continuation_Record[] = "N0"; //33
const uint8_t get_Step_Mode[] = "Zg";  //34
const uint8_t get_Phase_Current[] = "Zi";  //35
const uint8_t get_phase_Current_Standstill[] = "Zr"; //36
const uint8_t reset_Position_Error[] = "D";  //37
const uint8_t get_Limit_Switch_Behaviour[] = "Zl";  //38
const uint8_t get_Encoder_Type[] = ":CL_rotenc_type";   //39
const uint8_t get_Encoder_Number_Of_Increments[] = ":CL_rotenc_inc"; //40
const uint8_t get_IOs[] = "ZY";  //41
const uint8_t start_Motor[] = "A";  //42
const uint8_t stop_Motor[] = "S";  //43
const uint8_t set_Maximum_Accelleration[] = "b";   //44
const uint8_t get_Maximum_Accelleration[] = "Zb";  //45
const uint8_t set_Maximum_Decelleration[] = "B";   //46
const uint8_t get_Maximum_Decelleration[] = "ZB";  //47
const uint8_t set_Maximum_Accelleration_Jerk[] = ":b";   //48
const uint8_t get_Maximum_Accelleration_Jerk[] = "Z:b";  //49
const uint8_t set_Maximum_Decelleration_Jerk[] = ":B";   //50
const uint8_t get_Maximum_Decelleration_Jerk[] = "Z:B";  //51
const uint8_t get_Step[] = "C"; //52
const uint8_t get_Encoder_Step[] = "I"; //53
const uint8_t set_Travel_Distance_Steps[] = "s";  //54
const uint8_t get_Travel_Distance_Steps[] = "Zs"; //55
const uint8_t set_Direction_Of_Rotation[] = "d+"; //56
const uint8_t get_Direction_Of_Rotation[] = "Zd"; //57
const uint8_t set_Positioning_Mode[] = "p=";  //58
const uint8_t get_Positioning_Mode[] = "Zp";  //59
const uint8_t set_Ramp_Mode[] = ":ramp_mode"; //60
const uint8_t get_Ramp_Mode[] = ":ramp_mode"; //61
const uint8_t set_Minimum_Velocity[] = "u+";  //62
const uint8_t get_Minimum_Velocity[] = "Zu";  //63
const uint8_t set_Maximum_Velocity[] = "o+";  //64
const uint8_t get_Maximum_Velocity[] = "Zo";  //65

/* 存放报文里的参数 */
uint8_t protocol_Parameter[20];


/*
function:
    从字符串s1中查找字符串s2的尾巴索引值
    
input:
    1.确保字符串s1与s2都是以'\0'结尾!!!!
    
note:
    s1 = "dsafasdfasdf:CL_rotenc_type=30"
                                      
    s2 = ":CL_rotenc_type="

    return的值为28，s1[28]是'3'
*/
uint8_t mystr_find_Right_Side_Index(const uint8_t *s1,const uint8_t *s2)
{
    /* s1,s2都不能是空指针 */
    if(s1 == NULL || s2 == NULL)
    {
        return 0;
    }
    
    uint8_t s1_Length = strlen((char *)s1);
    uint8_t s2_Length = strlen((char *)s2);
    
    /* s1只能比s2长，s1与s2长度相同是没有意义的 */
    if(s1_Length <= s2_Length) return 0;

    uint8_t *left = NULL;
    uint8_t *right = NULL;
    /* 查找字符串s2的左侧在字符串s1的哪个位置   */
    left =(uint8_t *)strstr((char *)s1,(char *)s2);
    
    /* 字符串s2不在s1里面 */
    if(left == NULL) 
    {
        return 0;
    }
    
    right = left + s2_Length;
    return right-s1;
}


/*
function:
    从Socket接收报文，存放到packet里。接着分析报文的内容，return报文的类别(_VTS_Protocol_NumberTypeDef枚举了所有的类别)
    
input:
    
note:

*/
_VTS_Protocol_NumberTypeDef analyze_Tcp_Protocol_Command(
                                              int Socket,      /* 只能使用宏定义SOCKET_0       与SOCKET_1 */
                                              uint8_t *packet                                   
                                              )                                                           
{
    _VTS_Protocol_NumberTypeDef feedback; 
    //uint16_t rx_bytes = get_Tcp_Packet(Socket,NULL,packet,PACKET_BUFFER_LEN);

    //测试代码，后续需要删除
    uint8_t D_packet[] = ":CL_rotenc_type=3";
    strcpy((char *)packet,(char *)D_packet);
    uint16_t rx_bytes = strlen((char *)D_packet);
    
    /* VTS Protocol里每一条报文的长度不会超过40字节 */
    if(rx_bytes > VTS_MAXIMUM_PROTOCOL_LENGTH)
    {
        return feedback = PROTOCOL_ERR0R;
    }

    /* 分析TCP报文 */
    
    /* 以':'字符开头的报文         */
    if(packet[0] == ':')
    {
        /* 设置编码器的类型 */
        if(strstr((char *)packet,(char *)set_Encoder_Type) != NULL)
        {
            return feedback = SET_ENCODER_TYPE;
        }
        /* 设置编码器旋转一周反馈的脉冲数量 */
        else if(strstr((char *)packet,(char *)set_Encoder_Number_Of_Increments) != NULL)
        {
            return feedback = SET_ENCODER_NUMBER_OF_INCREMENTS;
        }
        /* 设置最大加速度jerk */
        else if(packet[1] == set_Maximum_Accelleration_Jerk[1])
        {
            return SET_MAXIMUM_ACCELLERATION_JERK;
        }
        
    }
    /* 不以':'字符开头的报文 */
    else
    {
        /* 有部分报文都是以字符'Z'开头 */
        /* 一共15条这样的报文         */
        if(packet[0] == 'Z')
        {
            /* 1.获取IO优先级 */
            if(packet[1] == get_Input_Polarity[1])
            {
                return feedback = GET_INPUT_POLARITY;
            }
            /* 2.获取脉冲模式 */
            else if(packet[1] == get_Step_Mode[1])
            {
                return feedback = GET_STEP_MODE;
            }
            /* 3.获取相电流 */
            else if(packet[1] == get_Phase_Current[1])
            {
                return feedback = GET_PHASE_CURRENT;
            }
            /* 4.获取相电流（保持）*/
            else if(packet[1] == get_phase_Current_Standstill[1])
            {
                return feedback = GET_PHASE_CURRENT_STANDSTILL;
            }
            /* 5.获取限位开关行为 */
            else if(packet[1] == get_Limit_Switch_Behaviour[1])
            {
                return feedback = GET_LIMIT_SWITCH_BEHAVIOUR;
            }
            /* 6.获取IOs */
            else if(packet[1] == get_IOs[1])
            {
                return feedback = GET_IOS;
            }
            /* 7.获取最大加速度 */
            else if(packet[1] == get_Maximum_Accelleration[1])
            {
                return feedback = GET_MAXIMUM_ACCELLERATION;
            }
            /* 8.获取最大减速度 */
            else if(packet[1] == get_Maximum_Decelleration[1])
            {
                return feedback = GET_MAXIMUM_DECELLERATION;
            }
            /* 9.获取最大加速度jerk */
            else if(packet[1] == get_Maximum_Accelleration_Jerk[1])
            {
                return feedback = GET_MAXIMUM_ACCELLERATION_JERK;
            }
            /* 10.获取最大减速度jerk */
            else if(packet[1] == get_Maximum_Decelleration_Jerk[1])
            {
                return feedback = GET_MAXIMUM_DECELLERATION_JERK;
            }
            /* 11.获取行程距离的脉冲数 */
            else if(packet[1] == get_Travel_Distance_Steps[1])
            {
                return feedback = GET_TRAVEL_DISTANCE_STEPS;
            }
            /* 12.获取旋转方向 */
            else if(packet[1] == get_Direction_Of_Rotation[1])
            {
                return feedback = GET_DIRECTION_OF_ROTATION;
            }
            /* 13.获取位置模式 */
            else if(packet[1] == get_Positioning_Mode[1])
            {
                return feedback = GET_POSITIONING_MODE;
            }
            /* 14.获取最低速度 */
            else if(packet[1] == get_Minimum_Velocity[1])
            {
                return feedback = GET_MINIMUM_VELOCITY;
            }
            /* 15.获取最高速度 */
            else if(packet[1] == get_Maximum_Velocity[1])
            {
                return feedback = GET_MAXIMUM_VELOCITY;
            }
            /* 匹配不上 */
            else
            {
                return feedback = PROTOCOL_ERR0R;
            }  
        }
        else
        {  
            switch(packet[0])
            {
                /* 1.获取固件版本       */
                case get_Firmware_Version[0]:
                     return feedback = GET_FIRMWARE_VERSION; 
                     break;
                /* 2.获取状态 */
                case get_Status[0]:
                     return feedback = GET_STATUS;
                     break;
                /* 3.设置脉冲模式       */
                case set_Step_Mode[0]:
                     return feedback = SET_STEP_MODE;
                     break;
                /* 4.设置相电流 */
                /* 5.设置相电流(保持) */    
                /* 6.设置限位开关的行为 */




            }





        
            /* 1.获取固件版本       */
            if(packet[0] == get_Firmware_Version[0])
            {
               return feedback = GET_FIRMWARE_VERSION; 
            }
            /* 2.获取状态 */
            else if(packet[0] == get_Status[0])
            {
               return feedback = GET_STATUS;
            }
            /* 3.设置脉冲模式       */
            else if(packet[0] == set_Step_Mode[0])
            {
               return feedback = SET_STEP_MODE;
            }
            /* 4.设置相电流 */
            else if(packet[0] == set_Phase_Current[0])
            {
               return feedback = SET_PHASE_CURRENT;
            }
            /* 5.设置相电流(保持) */
            else if(packet[0] == set_Phase_Current_Standstill[0])
            {
               return feedback = SET_PHASE_CURRENT_STANDSTILL;
            }
            /* 6.设置限位开关的行为 */
            else if(packet[0] == set_Limit_Switch_Behaviour[0])
            {
               return feedback = SET_LIMIT_SWITCH_BEHAVIOUR;
            }
            /* 7.屏蔽IO？   */
            else if(packet[0] == mask_IOS[0])
            {
               return feedback = MASK_IOS;
            }
            /* 8.设置IO优先级 */
            else if(packet[0] == set_IO_Polarity[0])
            {
               return feedback = SET_IO_POLARITY;
            }
            /* 9.设置输出数字信号 */
            else if(packet[0] == set_Outputs[0])
            {
               return feedback = SET_OUTPUTS;
            }
            /* 10.禁止继续记录 */
            else if(packet[0] == prohibit_Continuation_Record[0])
            {
               return feedback = PROHIBIT_CONTINUATION_RECORD;
            }
            /* 11.复位错误 */
            else if(packet[0] == reset_Position_Error[0])
            {
               return feedback = RESET_POSITION_ERROR;
            }
            /* 12.启动电机 */
            else if(packet[0] == start_Motor[0])
            {
               return feedback = START_MOTOR;
            }
            /* 13.停止电机 */
            else if(packet[0] == stop_Motor[0])
            {
               return feedback = STOP_MOTOR;
            }
            /* 14.设置最大加速度 */
            else if(packet[0] == set_Maximum_Accelleration[0])
            {
               return feedback = SET_MAXIMUM_ACCELLERATION;
            }
            /* 15.设置最大减速度 */
            else if(packet[0] == set_Maximum_Decelleration[0])
            {
               return feedback = SET_MAXIMUM_DECELLERATION;
            }
            /* 16.获取脉冲数*/
            else if(packet[0] == get_Step[0])
            {
                return feedback = GET_STEP;
            }
            /* 17.获取编码器脉冲数 */
            else if(packet[0] == get_Encoder_Step[0])
            {
                return feedback = GET_ENCODER_STEP;
            }
            /* 18.设置行程距离脉冲数 */
            else if(packet[0] == set_Travel_Distance_Steps[0])
            {
                return feedback = SET_TRAVEL_DISTANCE_STEPS;
            }
            /* 19.设置旋转方向 */
            else if(packet[0] == set_Direction_Of_Rotation[0])
            {
                return feedback = SET_DIRECTION_OF_ROTATION;
            }
            /* 20.设置位置模式 */
            else if(packet[0] == set_Positioning_Mode[0])
            {
                return feedback = SET_POSITIONING_MODE;
            }
            /* 21.设置最低速度 */
            else if(packet[0] == set_Minimum_Velocity[0])
            {
                return feedback = SET_MINIMUM_VELOCITY;
            }
            /* 22.设置最高速度 */
            else if(packet[0] == set_Maximum_Velocity[0])
            {
                return feedback = SET_MAXIMUM_VELOCITY;
            }
            /* 匹配不上 */
            else
            {
                return feedback = PROTOCOL_ERR0R;
            }
        }
    }
  
}
















