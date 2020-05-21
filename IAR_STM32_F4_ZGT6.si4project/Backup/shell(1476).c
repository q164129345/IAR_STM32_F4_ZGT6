/*
 * 用于与串口助手交�?
 *
 */


#include "shell.h"


uint8_t aTxMessage[1024] = "\r\n************* Easy Shell *************\r\n"
                       "Help Information: \r\n"
                       "-h    Show the help information \r\n"
                       "      for example: >>-h \r\n"
                       "\r\n"
                       "-i    Set IP address \r\n"
                       "      for example: >>-i 192.168.178.59 \r\n"
                       "\r\n"
                       "-m    Set MAC address  \r\n"
                       "      for example: >>-m 94:75:CE:2A:13:75 \r\n"
                       "\r\n"
                       "-s    Set subnet mask  \r\n"
                       "      for examlpe: >>-s 255.255.240.0 \r\n"
                       "\r\n"
                       "-g    Set Gateway IP address \r\n"
                       "      for example: >>-g 10.10.10.10 \r\n"
                       "\r\n"
                       "-p    Print the network configuration \r\n"
                       "      for example: >>-p \r\n";


extern uint8_t w5100_Default_Network_Settings[];
//����һ���ṹ��
_FORMAT_NETWORK_SETTINGS buffer_For_Setting;
//����һ���ṹ��ָ��
_FORMAT_NETWORK_SETTINGS *p_buffer_For_Setting = &buffer_For_Setting;




//���ڲ鿴ĳЩֵ
char checknumber[10] = {0};

/*
function: 
    ����ת�ַ���

input: 

    
output:

    
note: https://www.runoob.com/w3cnote/c-int2str.html
*/
int8_t* int_To_String(int32_t num, int8_t *str)
{
    int8_t i = 0;
    if(num < 0)
    {
        num = - num;
        str[i++] = '-';
    }

    //转换
    do
    {
        str[i++] = num%10+48;
        num /= 10;
            
    }
    while (num); //num不为0继续循环

    str[i] = '\0';


    //确定开始调整的位置
    int j = 0;
    if(str[0] == '-')  //如果有负号，负号不用调整
    {
        j = 1;
        ++i;
    }

    //对称交换
    for(;j<i/2;j++)
    {
        //对称交换两端的�?
        str[j] =  str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    }

    return str;

}



/*
function: 
    

input: 
    
output:
    
note: 
*/

void show_Shell_Title(void)
{
 
   //
}



/*
function: 
    

input: 
    
output:
    
note: 
*/
void show_W5100_Default_Network_Settings(void)
{
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)w5100_Default_Network_Settings, W5100_NETWORK_SETTING_SIZE);
}

/*
funtion:
   

input:

output:

note:
*/
void do_something(void)
{
    /* 转换Gateway IP */
    int_To_String(w5100_Default_Network_Settings[0],p_buffer_For_Setting->gate_Way_1);
    int_To_String(w5100_Default_Network_Settings[1],p_buffer_For_Setting->gate_Way_2);
    int_To_String(w5100_Default_Network_Settings[2],p_buffer_For_Setting->gate_Way_3);
    int_To_String(w5100_Default_Network_Settings[3],p_buffer_For_Setting->gate_Way_4);

    /* 转换Subnet Mask */
    int_To_String(w5100_Default_Network_Settings[4],p_buffer_For_Setting->subnet_Mask_1);
    int_To_String(w5100_Default_Network_Settings[5],p_buffer_For_Setting->subnet_Mask_2);
    int_To_String(w5100_Default_Network_Settings[6],p_buffer_For_Setting->subnet_Mask_3);
    int_To_String(w5100_Default_Network_Settings[7],p_buffer_For_Setting->subnet_Mask_4);

    /* 转换MAC Address */
    int_To_String(w5100_Default_Network_Settings[8],p_buffer_For_Setting->MAC_Address_1);
    int_To_String(w5100_Default_Network_Settings[9],p_buffer_For_Setting->MAC_Address_2);
    int_To_String(w5100_Default_Network_Settings[10],p_buffer_For_Setting->MAC_Address_3);
    int_To_String(w5100_Default_Network_Settings[11],p_buffer_For_Setting->MAC_Address_4);
    int_To_String(w5100_Default_Network_Settings[12],p_buffer_For_Setting->MAC_Address_5);
    int_To_String(w5100_Default_Network_Settings[13],p_buffer_For_Setting->MAC_Address_6);

    /* 转换IP Address */
    int_To_String(w5100_Default_Network_Settings[14],p_buffer_For_Setting->ip_Address_1);
    int_To_String(w5100_Default_Network_Settings[15],p_buffer_For_Setting->ip_Address_2);
    int_To_String(w5100_Default_Network_Settings[16],p_buffer_For_Setting->ip_Address_3);
    int_To_String(w5100_Default_Network_Settings[17],p_buffer_For_Setting->ip_Address_4);


    /* 打印出来 */
    printf("\r\n Gateway: %s.%s.%s.%s \r\n", p_buffer_For_Setting->gate_Way_1,p_buffer_For_Setting->gate_Way_2,p_buffer_For_Setting->gate_Way_3,p_buffer_For_Setting->gate_Way_4);
    printf("\r\n Subnet Mask: %s.%s.%s.%s \r\n",p_buffer_For_Setting->subnet_Mask_1,p_buffer_For_Setting->subnet_Mask_2,p_buffer_For_Setting->subnet_Mask_3,p_buffer_For_Setting->subnet_Mask_4);
    printf("\r\n Mack address: %s.%s.%s.%s.%s.%s \r\n",p_buffer_For_Setting->MAC_Address_1,p_buffer_For_Setting->MAC_Address_2,p_buffer_For_Setting->MAC_Address_3,p_buffer_For_Setting->MAC_Address_4,p_buffer_For_Setting->MAC_Address_5,p_buffer_For_Setting->MAC_Address_6);
    printf("\r\n Ip address: %s.%s.%s.%s \r\n",p_buffer_For_Setting->ip_Address_1,p_buffer_For_Setting->ip_Address_2,p_buffer_For_Setting->ip_Address_3,p_buffer_For_Setting->ip_Address_4);

}

/*
funtion:
    �������ڷ��͹���������
   

input:

output:

note:
*/
void analyze_User_Command(char *Command)
{
    char *ans ;
    char new_Command[20] = {0};
    //�����ַ���-��
    ans = strchr(Command,'-');
    //�ҵ��ַ���-��
    if(ans != NULL)
    {
        //�����ַ�'-'֮ǰ���ַ�����ȡ�ַ���-��֮����ַ�
        strcpy(new_Command,ans);
        if(new_Command[0] == '-')
        {
            printf("The new_Command is :%s \r\n",new_Command);
        }
        switch (new_Command[1])
        {
        case 'h':
            //������ʾ��Ϣ
            HAL_UART_Transmit_IT(&huart1, (uint8_t *)aTxMessage, sizeof(aTxMessage));
            break;
        case 'i':
            //����IP Address
            break;
        case 'm':
            //����MAC��ַ
            break;
        case 's':
            //����Subnet mask
            break;
        case 'g':
            //��������
            break;
        case 'p':
            //��ӡW5100��������
            break;
        default:
            //��ʾ������Ϣ
            //��ӡ������Ϣ
        }
        
    //ɸѡ��Щ�ַ���h i m s g p
    //h
    //i
    //m
    //s
    //g
    //p
    //���϶��ǵĻ�
    }
    //û���ҵ��ַ���-��
    else
    {
        printf("something is wrong,Di you type '-'? \r\n");
    }
}





