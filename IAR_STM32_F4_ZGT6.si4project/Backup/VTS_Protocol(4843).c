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
    从字符串s1中查找字符串s2最右出现的位置，并返回一个指向该位置的指针

note: 
*/
uint8_t* my_Strrstr(const uint8_t *s1,const uint8_t *s2)
{
    uint8_t *last;
    uint8_t *current;

    /* 把指针初始化为我们已经找到的前一次匹配位置 */
    last = NULL;

    /* 只在第2个字符串不为空时才进行查找，如果s2为空，返回NULL */
    
     




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
















