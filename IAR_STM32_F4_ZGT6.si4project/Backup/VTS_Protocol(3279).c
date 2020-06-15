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

// VTS Scaner Main Borad ֻ����һ����λ���� 
const uint8_t set_Digital_Input_Function_A[] = ":port_in_a"; //12
const uint8_t set_Digital_Input_Function_B[] = ":port_in_b"; //13
const uint8_t set_Digital_Input_Function_C[] = ":port_in_c"; //14
const uint8_t set_Digital_Input_Function_D[] = ":port_in_d"; //15
const uint8_t set_Digital_Input_Function_E[] = ":port_in_e"; //16
const uint8_t set_Digital_Input_Function_F[] = ":port_in_f"; //17
// VTS Scaner Main Borad ֻ����һ����λ���� 
const uint8_t get_Digital_Input_Function_A[] = ":port_in_a"; //22
const uint8_t get_Digital_Input_Function_B[] = ":port_in_b"; //23
const uint8_t get_Digital_Input_Function_C[] = ":port_in_c"; //24
const uint8_t get_Digital_Input_Function_D[] = ":port_in_d"; //25
const uint8_t get_Digital_Input_Function_E[] = ":port_in_e"; //26
const uint8_t get_Digital_Input_Function_F[] = ":port_in_f"; //27

// VTS Scaner Main Borad û�������ź���� 
const uint8_t set_Digital_Output_Function_A[] = ":port_out_a"; //18
const uint8_t set_Digital_Output_Function_B[] = ":port_out_b"; //19
const uint8_t set_Digital_Output_Function_C[] = ":port_out_c"; //20
const uint8_t set_Digital_Output_Function_D[] = ":port_out_d"; //21
// VTS Scaner Main Borad û�������ź���� 
const uint8_t get_Digital_Output_Function_A[] = ":port_out_a"; //28
const uint8_t get_Digital_Output_Function_B[] = ":port_out_b"; //29
const uint8_t get_Digital_Output_Function_C[] = ":port_out_c"; //30
const uint8_t get_Digital_Output_Function_D[] = ":port_out_d"; //31
// VTS Scaner Main Borad û�������ź���� 
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

/* ��ű�����Ĳ��� */
uint8_t protocol_Parameter[20];


/*
function:
    ���ַ���s1�в����ַ���s2��β������ֵ
    
input:
    1.ȷ���ַ���s1��s2������'\0'��β!!!!
    
note:
    s1 = "dsafasdfasdf:CL_rotenc_type=30"
                                      
    s2 = ":CL_rotenc_type="

    return��ֵΪ28��s1[28]��'3'
*/
uint8_t mystr_find_Right_Side_Index(const uint8_t *s1,const uint8_t *s2)
{
    /* s1,s2�������ǿ�ָ�� */
    if(s1 == NULL || s2 == NULL)
    {
        return 0;
    }
    
    uint8_t s1_Length = strlen((char *)s1);
    uint8_t s2_Length = strlen((char *)s2);
    
    /* s1ֻ�ܱ�s2����s1��s2������ͬ��û������� */
    if(s1_Length <= s2_Length) return 0;

    uint8_t *left = NULL;
    uint8_t *right = NULL;
    /* �����ַ���s2��������ַ���s1���ĸ�λ��   */
    left =(uint8_t *)strstr((char *)s1,(char *)s2);
    
    /* �ַ���s2����s1���� */
    if(left == NULL) 
    {
        return 0;
    }
    
    right = left + s2_Length;
    return right-s1;
}


/*
function:
    ��Socket���ձ��ģ���ŵ�packet����ŷ������ĵ����ݣ�return���ĵ����(_VTS_Protocol_NumberTypeDefö�������е����)
    
input:
    
note:

*/
_VTS_Protocol_NumberTypeDef analyze_Tcp_Protocol_Command(
                                              int Socket,      /* ֻ��ʹ�ú궨��SOCKET_0       ��SOCKET_1 */
                                              uint8_t *packet                                   
                                              )                                                           
{
    _VTS_Protocol_NumberTypeDef feedback; 
    //uint16_t rx_bytes = get_Tcp_Packet(Socket,NULL,packet,PACKET_BUFFER_LEN);

    //���Դ��룬������Ҫɾ��
    uint8_t D_packet[] = ":CL_rotenc_type=3";
    strcpy((char *)packet,(char *)D_packet);
    uint16_t rx_bytes = strlen((char *)D_packet);
    
    /* VTS Protocol��ÿһ�����ĵĳ��Ȳ��ᳬ��40�ֽ� */
    if(rx_bytes > VTS_MAXIMUM_PROTOCOL_LENGTH)
    {
        return feedback = PROTOCOL_ERR0R;
    }

    /* ����TCP���� */
    
    /* ��':'�ַ���ͷ�ı���         */
    if(packet[0] == ':')
    {
        /* ���ñ����������� */
        if(strstr((char *)packet,(char *)set_Encoder_Type) != NULL)
        {
            return feedback = SET_ENCODER_TYPE;
        }
        /* ���ñ�������תһ�ܷ������������� */
        else if(strstr((char *)packet,(char *)set_Encoder_Number_Of_Increments) != NULL)
        {
            return feedback = SET_ENCODER_NUMBER_OF_INCREMENTS;
        }
        /* ���������ٶ�jerk */
        else if(packet[1] == set_Maximum_Accelleration_Jerk[1])
        {
            return SET_MAXIMUM_ACCELLERATION_JERK;
        }
        
    }
    /* ����':'�ַ���ͷ�ı��� */
    else
    {
        /* �в��ֱ��Ķ������ַ�'Z'��ͷ */
        /* һ��15�������ı���         */
        if(packet[0] == 'Z')
        {
            /* 1.��ȡIO���ȼ� */
            if(packet[1] == get_Input_Polarity[1])
            {
                return feedback = GET_INPUT_POLARITY;
            }
            /* 2.��ȡ����ģʽ */
            else if(packet[1] == get_Step_Mode[1])
            {
                return feedback = GET_STEP_MODE;
            }
            /* 3.��ȡ����� */
            else if(packet[1] == get_Phase_Current[1])
            {
                return feedback = GET_PHASE_CURRENT;
            }
            /* 4.��ȡ����������֣�*/
            else if(packet[1] == get_phase_Current_Standstill[1])
            {
                return feedback = GET_PHASE_CURRENT_STANDSTILL;
            }
            /* 5.��ȡ��λ������Ϊ */
            else if(packet[1] == get_Limit_Switch_Behaviour[1])
            {
                return feedback = GET_LIMIT_SWITCH_BEHAVIOUR;
            }
            /* 6.��ȡIOs */
            else if(packet[1] == get_IOs[1])
            {
                return feedback = GET_IOS;
            }
            /* 7.��ȡ�����ٶ� */
            else if(packet[1] == get_Maximum_Accelleration[1])
            {
                return feedback = GET_MAXIMUM_ACCELLERATION;
            }
            /* 8.��ȡ�����ٶ� */
            else if(packet[1] == get_Maximum_Decelleration[1])
            {
                return feedback = GET_MAXIMUM_DECELLERATION;
            }
            /* 9.��ȡ�����ٶ�jerk */
            else if(packet[1] == get_Maximum_Accelleration_Jerk[1])
            {
                return feedback = GET_MAXIMUM_ACCELLERATION_JERK;
            }
            /* 10.��ȡ�����ٶ�jerk */
            else if(packet[1] == get_Maximum_Decelleration_Jerk[1])
            {
                return feedback = GET_MAXIMUM_DECELLERATION_JERK;
            }
            /* 11.��ȡ�г̾���������� */
            else if(packet[1] == get_Travel_Distance_Steps[1])
            {
                return feedback = GET_TRAVEL_DISTANCE_STEPS;
            }
            /* 12.��ȡ��ת���� */
            else if(packet[1] == get_Direction_Of_Rotation[1])
            {
                return feedback = GET_DIRECTION_OF_ROTATION;
            }
            /* 13.��ȡλ��ģʽ */
            else if(packet[1] == get_Positioning_Mode[1])
            {
                return feedback = GET_POSITIONING_MODE;
            }
            /* 14.��ȡ����ٶ� */
            else if(packet[1] == get_Minimum_Velocity[1])
            {
                return feedback = GET_MINIMUM_VELOCITY;
            }
            /* 15.��ȡ����ٶ� */
            else if(packet[1] == get_Maximum_Velocity[1])
            {
                return feedback = GET_MAXIMUM_VELOCITY;
            }
            /* ƥ�䲻�� */
            else
            {
                return feedback = PROTOCOL_ERR0R;
            }  
        }
        else
        {  
            switch(packet[0])
            {
                /* 1.��ȡ�̼��汾       */
                case get_Firmware_Version[0]:
                     return feedback = GET_FIRMWARE_VERSION; 
                     break;
                /* 2.��ȡ״̬ */
                case get_Status[0]:
                     return feedback = GET_STATUS;
                     break;
                /* 3.��������ģʽ       */
                case set_Step_Mode[0]:
                     return feedback = SET_STEP_MODE;
                     break;
                /* 4.��������� */
                /* 5.���������(����) */    
                /* 6.������λ���ص���Ϊ */




            }





        
            /* 1.��ȡ�̼��汾       */
            if(packet[0] == get_Firmware_Version[0])
            {
               return feedback = GET_FIRMWARE_VERSION; 
            }
            /* 2.��ȡ״̬ */
            else if(packet[0] == get_Status[0])
            {
               return feedback = GET_STATUS;
            }
            /* 3.��������ģʽ       */
            else if(packet[0] == set_Step_Mode[0])
            {
               return feedback = SET_STEP_MODE;
            }
            /* 4.��������� */
            else if(packet[0] == set_Phase_Current[0])
            {
               return feedback = SET_PHASE_CURRENT;
            }
            /* 5.���������(����) */
            else if(packet[0] == set_Phase_Current_Standstill[0])
            {
               return feedback = SET_PHASE_CURRENT_STANDSTILL;
            }
            /* 6.������λ���ص���Ϊ */
            else if(packet[0] == set_Limit_Switch_Behaviour[0])
            {
               return feedback = SET_LIMIT_SWITCH_BEHAVIOUR;
            }
            /* 7.����IO��   */
            else if(packet[0] == mask_IOS[0])
            {
               return feedback = MASK_IOS;
            }
            /* 8.����IO���ȼ� */
            else if(packet[0] == set_IO_Polarity[0])
            {
               return feedback = SET_IO_POLARITY;
            }
            /* 9.������������ź� */
            else if(packet[0] == set_Outputs[0])
            {
               return feedback = SET_OUTPUTS;
            }
            /* 10.��ֹ������¼ */
            else if(packet[0] == prohibit_Continuation_Record[0])
            {
               return feedback = PROHIBIT_CONTINUATION_RECORD;
            }
            /* 11.��λ���� */
            else if(packet[0] == reset_Position_Error[0])
            {
               return feedback = RESET_POSITION_ERROR;
            }
            /* 12.������� */
            else if(packet[0] == start_Motor[0])
            {
               return feedback = START_MOTOR;
            }
            /* 13.ֹͣ��� */
            else if(packet[0] == stop_Motor[0])
            {
               return feedback = STOP_MOTOR;
            }
            /* 14.���������ٶ� */
            else if(packet[0] == set_Maximum_Accelleration[0])
            {
               return feedback = SET_MAXIMUM_ACCELLERATION;
            }
            /* 15.���������ٶ� */
            else if(packet[0] == set_Maximum_Decelleration[0])
            {
               return feedback = SET_MAXIMUM_DECELLERATION;
            }
            /* 16.��ȡ������*/
            else if(packet[0] == get_Step[0])
            {
                return feedback = GET_STEP;
            }
            /* 17.��ȡ������������ */
            else if(packet[0] == get_Encoder_Step[0])
            {
                return feedback = GET_ENCODER_STEP;
            }
            /* 18.�����г̾��������� */
            else if(packet[0] == set_Travel_Distance_Steps[0])
            {
                return feedback = SET_TRAVEL_DISTANCE_STEPS;
            }
            /* 19.������ת���� */
            else if(packet[0] == set_Direction_Of_Rotation[0])
            {
                return feedback = SET_DIRECTION_OF_ROTATION;
            }
            /* 20.����λ��ģʽ */
            else if(packet[0] == set_Positioning_Mode[0])
            {
                return feedback = SET_POSITIONING_MODE;
            }
            /* 21.��������ٶ� */
            else if(packet[0] == set_Minimum_Velocity[0])
            {
                return feedback = SET_MINIMUM_VELOCITY;
            }
            /* 22.��������ٶ� */
            else if(packet[0] == set_Maximum_Velocity[0])
            {
                return feedback = SET_MAXIMUM_VELOCITY;
            }
            /* ƥ�䲻�� */
            else
            {
                return feedback = PROTOCOL_ERR0R;
            }
        }
    }
  
}
















