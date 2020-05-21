/*
 * 
 *
 */


#include "shell.h"

//�����ⲿ����
extern uint8_t w5100_Network_Settings[];

uint8_t aTxMessage[1024] = "\r\n************* Easy Shell *************\r\n"
                       "Command List: \r\n"
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
_STRING_FORMAT_NETWORK_SETTINGS string_Buffer_For_Network_Setting;
//����һ���ṹ��ָ��
_STRING_FORMAT_NETWORK_SETTINGS *p_string_Buffer_For_Network_Setting = &string_Buffer_For_Network_Setting;

//���ڲ鿴ĳЩֵ
char checknumber[20] = {0};
int32_t checksize = 0;
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

}

/*
funtion:
        ������������IP_Address����ת��Ϊuint8_t����,��ŵ�����.
   
input: 
        

output:
        1��ת���ɹ�
        0: ת��ʧ��


note: 
*/
int8_t IP_Address_Command_Turn_To_Value(_STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting ,uint8_t *value_Network_Setting)
{
    int32_t buffer[4] = {0};
    buffer[0] = atoi(string_Network_Setting->part1_IP_Address);
    buffer[1] = atoi(string_Network_Setting->part2_IP_Address);
    buffer[2] = atoi(string_Network_Setting->part3_IP_Address);
    buffer[3] = atoi(string_Network_Setting->part4_IP_Address);

    value_Network_Setting[14] = (uint8_t)buffer[0];
    value_Network_Setting[15] = (uint8_t)buffer[1];
    value_Network_Setting[16] = (uint8_t)buffer[2];
    value_Network_Setting[17] = (uint8_t)buffer[3];

    return 1;
}

/*
funtion:
        ������������Subnet Mask����ת��Ϊuint8_t����,��ŵ�����.
   
input: 
        

output:
        1��ת���ɹ�
        0: ת��ʧ��


note: 
*/
int8_t subnet_Mask_Command_Turn_To_Value(_STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting,uint8_t *value_Network_Setting)
{
    int32_t buffer[4] = {0};
    buffer[0] = atoi(string_Network_Setting->part1_Subnet_Mask);
    buffer[1] = atoi(string_Network_Setting->part2_Subnet_Mask);
    buffer[2] = atoi(string_Network_Setting->part3_Subnet_Mask);
    buffer[3] = atoi(string_Network_Setting->part4_Subnet_Mask);

    value_Network_Setting[4] = (uint8_t)buffer[0];
    value_Network_Setting[5] = (uint8_t)buffer[1];
    value_Network_Setting[6] = (uint8_t)buffer[2];
    value_Network_Setting[7] = (uint8_t)buffer[3];

    return 1;
    
}

/*
funtion:
        ������������Gateway_IP����ת��Ϊuint8_t����,��ŵ�����.
   
input: 
        

output:
        1��ת���ɹ�
        0: ת��ʧ��


note: 
*/
int8_t gateway_IP_Command_Turn_To_Value(_STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting,uint8_t *value_Network_Setting)
{
    int32_t buffer[4] = {0};
    buffer[0] = atoi(string_Network_Setting->part1_Gateway);
    buffer[1] = atoi(string_Network_Setting->part2_Gateway);
    buffer[2] = atoi(string_Network_Setting->part3_Gateway);
    buffer[3] = atoi(string_Network_Setting->part4_Gateway);

    value_Network_Setting[0] = (uint8_t)buffer[0];
    value_Network_Setting[1] = (uint8_t)buffer[1];
    value_Network_Setting[2] = (uint8_t)buffer[2];
    value_Network_Setting[3] = (uint8_t)buffer[3];

    return 1;
    
}




/*
funtion:
    �������ڷ��͹���������,���������ɢ�Ĳ��֣���ŵ�_STRING_FORMAT_NETWORK_SETTINGS�ṹ��
    �������ֻ�����ڽ���-i -s -g����������
    ע�⣺����-m������Ҫʹ������һ������
   
input: 
    ��ڲ���1: _STRING_FORMAT_NETWORK_SETTINGS�ṹ��

output:
    1 �����ɹ�
    0 ����ʧ��

note: ��ʹ��int8_t������Ϊstrcpy��C�ٷ��⴦��int8_t�ᾯ��
      ����stm32�������ͣ�int8_tʵ����typedef signed char���ͣ���char����
*/
//int8_t analyze_IP_Subnet_Gateway_Command(char const *Command,char *part1,char *part2,char *part3,char *part4)
int8_t analyze_IP_Subnet_Gateway_Command(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *network_Setting)
{
    //��-i,-s,-g����
    if(Command[1] == 'i' || Command[1] == 'g' || Command[1] == 's')
    {
        
        //����ָ����
        //IP_Address��Subnet Mask��Gateway IP����'.'��Ϊ�ָ���
        char const s[2] = ".";
        //����һ��ָ�룬ָ�򱻷ָ��λ��
        char *token;
        //����һ������
        char new_Command[20] = {0};
        //�����ĸ����飬��ű���ɢ����ĸ�����
        char buf_Part1[4] = {0};
        char buf_Part2[4] = {0};
        char buf_Part3[4] = {0};
        char buf_Part4[4] = {0};

        //Command��������:"-i 192.168.178.100"
        //                "-g 10.10.10.10"
        //                "-s 255.255.240.0"
        //new_Command��������: "192.168.178.100"
        
        //�ж�
        //-i��һλ�Ƿ�Ϊ'�ո�'
        //'�ո�'��һλ�Ƿ����ַ���0-9
        if(Command[2] == ' ' && isdigit(Command[3])) 
        {
            //����-i+�ո���ȡIP Address������"192.168.178.100".
            //ʹ��strncpy����19λ�ַ�������֤��20λ��'\0'
            strncpy(new_Command,Command +3,19);
            //���Ƹ�ȫ�ֱ��������ڹ۲�
            strncpy(checknumber,new_Command,19);

            //��ȡ��һ���ֵ��ַ���
            token = strtok(new_Command,s);
            if(token != NULL)
            {
               //��ȡ��һ�����ַ�����ǰ��λ
               //��ΪIP Address�ĵ�һ���ֲ��ܳ���3λ��
               strncpy(buf_Part1,token,3);
            }
            else //�Ҳ�����һ�����ַ���
            {   
               return 0;
            }
            
            //��ȡ�ڶ����ֵ��ַ���
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //��ȡ�ڶ������ַ�����ǰ��λ
               //��ΪIP Address�ĵڶ����ֲ��ܳ���3λ��
               strncpy(buf_Part2,token,3);
            }
            else //�Ҳ����ڶ������ַ���
            {
               return 0;
            }
            
            //��ȡ�������ֵ��ַ���
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //��ȡ���������ַ�����ǰ��λ
               //��ΪIP Address�ĵ������ֲ��ܳ���3λ��
               strncpy(buf_Part3,token,3);
            }
            else //�Ҳ������������ַ���
            {
               return 0;
            }
            
            //��ȡ���Ĳ��ֵ��ַ���
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //��ȡ���Ĳ����ַ�����ǰ��λ
               //��ΪIP Address�ĵ��Ĳ��ֲ��ܳ���3λ��
               strncpy(buf_Part4,token,3);
            }
            else //�Ҳ������Ĳ����ַ���
            {
               return 0;
            }
            
            //��ȡ���岿�ֵ��ַ���
            token = strtok(NULL,s);
            //��������ĸ�ʽ����Ӧ���е��岿�֡�
            if(token != NULL) return 0;

        
        }
        //-i�ĺ�һλ����'�ո�' �� '�ո�'��һλ�����ַ���0-9�Ļ�������0.
        //���Թ�'�ո�'��һλ�����ַ���0-9�Ļ�����ִ�������else����
        else
        {
            return 0;
        }

        //��ɢ���ĸ����֣���ŵ��ṹ������
        switch (Command[1])
        {
            case 'i':
                memcpy(network_Setting->part1_IP_Address,buf_Part1,4);
                memcpy(network_Setting->part2_IP_Address,buf_Part2,4);
                memcpy(network_Setting->part3_IP_Address,buf_Part3,4);
                memcpy(network_Setting->part4_IP_Address,buf_Part4,4);
                break;
            case 's':
                memcpy(network_Setting->part1_Subnet_Mask,buf_Part1,4);
                memcpy(network_Setting->part2_Subnet_Mask,buf_Part2,4);
                memcpy(network_Setting->part3_Subnet_Mask,buf_Part3,4);
                memcpy(network_Setting->part4_Subnet_Mask,buf_Part4,4);
                break;
            case 'g':
                memcpy(network_Setting->part1_Gateway,buf_Part1,4);
                memcpy(network_Setting->part2_Gateway,buf_Part2,4);
                memcpy(network_Setting->part3_Gateway,buf_Part3,4);
                memcpy(network_Setting->part4_Gateway,buf_Part4,4);
                break;
        }
        return 1;
    }
    
    //��-m����
    if(Command[1] == 'm')
    {      
        //����ָ����
        //IP_Address��Subnet Mask��Gateway IP����'.'��Ϊ�ָ���
        char const s[2] = ":";
        //����һ��ָ�룬ָ�򱻷ָ��λ��
        char *token;
        //����һ������
        char new_Command[20] = {0};
        //�����ĸ����飬��ű���ɢ����ĸ�����
        char buf_Part1[4] = {0};
        char buf_Part2[4] = {0};
        char buf_Part3[4] = {0};
        char buf_Part4[4] = {0};

        //Command��������:"-i 192.168.178.100"
        //                "-g 10.10.10.10"
        //                "-s 255.255.240.0"
        //new_Command��������: "192.168.178.100"
        
        //�ж�
        //-i��һλ�Ƿ�Ϊ'�ո�'
        //'�ո�'��һλ�Ƿ����ַ���0-9




        return 1;
    }
    
}








/*
funtion:
    �������ڷ��͹���������
   
input: �ַ���(char *)

output:

note: ��ʹ��int8_t������Ϊstrcpy��C�ٷ��⴦��int8_t�ᾯ��
      ����stm32�������ͣ�int8_tʵ����typedef signed char���ͣ���char����
*/
void analyze_User_Command(char const *Command)
{
    char *ans ;
    char new_Command[30] = {0};
    //�����ַ���-��
    ans = strchr(Command,'-');
    //�ҵ��ַ���-��
    if(ans != NULL)
    {
        //�����ַ�'-'֮ǰ���ַ�����ȡ�ַ���-�����ַ���-��֮����ַ�
        strcpy(new_Command,ans);

        //����new_Command
        switch (new_Command[1])
        {
        case 'h':
            //������ʾ��Ϣ
            HAL_UART_Transmit_IT(&huart1, (uint8_t *)aTxMessage, sizeof(aTxMessage));
            break;
        
        case 'i':
            //����-i����
            if( 1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //�����ɹ�
                //���ַ���part1_IP_Address - part4_IP_Addressת��Ϊ����
                IP_Address_Command_Turn_To_Value(p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
                //д��IP��ַ��EEPROM                
            }
            break;
            
        case 'm':
            //����MAC��ַ
            break;
            
        case 's':
            //����Subnet mask
            //if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,part1_Subnet_Mask,part2_Subnet_Mask,part3_Subnet_Mask,part4_Subnet_Mask))
            if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //�����ɹ�
                //���ַ���part1_Subnet_Mask - part4_Subnet_Maskת��Ϊ����
                subnet_Mask_Command_Turn_To_Value(p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
                //д��Subnet Mask��EEPROM
            }            
            break;
            
        case 'g':
            //��������
            //if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,part1_Gateway_IP, part2_Gateway_IP, part3_Gateway_IP, part4_Gateway_IP));
            if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //�����ɹ�
                //���ַ���part1_Gateway_IP - part4_Gateway_IPת��Ϊ����
                gateway_IP_Command_Turn_To_Value(p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
            }
            break;
            
        case 'p':
            //��ӡW5100��������
            do_something();
            break;
            
        default:
            //��ʾ������Ϣ
            //����printf���ԣ�������ΪDMA���ͣ����Ч��
            printf("Type wrong command ,please type -h to show the command list.");
            //��ӡ������Ϣ
            break; //���ﲻ��break�Ļ���IAR�������ᾯ��.
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





