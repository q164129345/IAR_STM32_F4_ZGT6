#include "VTS_Protocol.h"
#include "socket_tcp.h"


/* test */
uint8_t *test_String;
uint8_t Position;


/* CMD list */
const char get_Firmware_Version[] = "v";
const char get_Status[] = "$";
const char set_Step_Mode[] = "g";
const char set_Phase_Current[] = "i";
const char set_Phase_Current_Standstill[] = "r";
const char set_Limit_Switch_Behaviour[] = "l";
const char set_Encoder_Type[] = ":CL_rotenc_type=";

/* from socket.c */
//extern char packet_Buffer_For_Tcp_1[PACKET_BUFFER_LEN];
//extern char packet_Buffer_For_Tcp_2[PACKET_BUFFER_LEN];

uint8_t protocol_Command[20];
uint8_t protocol_Parameter[20];



/*
function:
    ���ַ���s1�в����ַ���s2���ҳ��ֵ�λ�ã�������һ��ָ���λ�õ�ָ��

note: 
*/
uint8_t* my_Strrstr(const uint8_t *s1,const uint8_t *s2)
{
    uint8_t *last;
    uint8_t *current;

    /* ��ָ���ʼ��Ϊ�����Ѿ��ҵ���ǰһ��ƥ��λ�� */
    last = NULL;

    /* ֻ�ڵ�2���ַ�����Ϊ��ʱ�Ž��в��ң����s2Ϊ�գ�����NULL */
    
     




}




_VTS_Protocol_NumberTypeDef analyze_Tcp_Protocol_Command(int Socket,uint8_t *packet)
{
    _VTS_Protocol_NumberTypeDef feedback = DO_NOTHING;
    //uint16_t rx_bytes = get_Tcp_Packet(Socket,NULL,packet,PACKET_BUFFER_LEN);
    uint8_t D_packet[] = "adfsadfsdf:CL_rotenc_type=3";
    strcpy((char *)packet,(char *)D_packet);
    uint16_t rx_bytes = strlen((char *)packet);
    
    
    if(rx_bytes > 0 && rx_bytes < VTS_MAXIMUM_PROTOCOL_LENGTH)
    {
        uint8_t *p_String_Start;
        p_String_Start =(uint8_t *) strstr((char *)packet,set_Encoder_Type);
        Position = p_String_Start - packet;
        test_String = p_String_Start;
        
    }
    else
    {
       return feedback;       
    }   
}
















