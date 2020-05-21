/*
 * 用于与串口助手交�?
 *
 */


#include "shell.h"

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
_FORMAT_NETWORK_SETTINGS buffer_For_Setting;
//����һ���ṹ��ָ��
_FORMAT_NETWORK_SETTINGS *p_buffer_For_Setting = &buffer_For_Setting;
//����һЩ�����š��ַ�������ʽ��IP Address
char part1_IP_Address[4] = {0};
char part2_IP_Address[4] = {0};
char part3_IP_Address[4] = {0};
char part4_IP_Address[4] = {0};

//���ڲ鿴ĳЩֵ
char checknumber[16] = {0};
int checksize = 0;
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
    /* ת�� Gateway IP */
    int_To_String(w5100_Default_Network_Settings[0],p_buffer_For_Setting->gate_Way_1);
    int_To_String(w5100_Default_Network_Settings[1],p_buffer_For_Setting->gate_Way_2);
    int_To_String(w5100_Default_Network_Settings[2],p_buffer_For_Setting->gate_Way_3);
    int_To_String(w5100_Default_Network_Settings[3],p_buffer_For_Setting->gate_Way_4);

    /* ת�� Subnet Mask */
    int_To_String(w5100_Default_Network_Settings[4],p_buffer_For_Setting->subnet_Mask_1);
    int_To_String(w5100_Default_Network_Settings[5],p_buffer_For_Setting->subnet_Mask_2);
    int_To_String(w5100_Default_Network_Settings[6],p_buffer_For_Setting->subnet_Mask_3);
    int_To_String(w5100_Default_Network_Settings[7],p_buffer_For_Setting->subnet_Mask_4);

    /* ת�� MAC Address */
    int_To_String(w5100_Default_Network_Settings[8],p_buffer_For_Setting->MAC_Address_1);
    int_To_String(w5100_Default_Network_Settings[9],p_buffer_For_Setting->MAC_Address_2);
    int_To_String(w5100_Default_Network_Settings[10],p_buffer_For_Setting->MAC_Address_3);
    int_To_String(w5100_Default_Network_Settings[11],p_buffer_For_Setting->MAC_Address_4);
    int_To_String(w5100_Default_Network_Settings[12],p_buffer_For_Setting->MAC_Address_5);
    int_To_String(w5100_Default_Network_Settings[13],p_buffer_For_Setting->MAC_Address_6);

    /* ת�� IP Address */
    int_To_String(w5100_Default_Network_Settings[14],p_buffer_For_Setting->ip_Address_1);
    int_To_String(w5100_Default_Network_Settings[15],p_buffer_For_Setting->ip_Address_2);
    int_To_String(w5100_Default_Network_Settings[16],p_buffer_For_Setting->ip_Address_3);
    int_To_String(w5100_Default_Network_Settings[17],p_buffer_For_Setting->ip_Address_4);


    /* ���ַ�����ӡ������ */
    printf("\r\n The network configuaration : ");
    printf("\r\n Gateway: %s.%s.%s.%s \r\n", p_buffer_For_Setting->gate_Way_1,p_buffer_For_Setting->gate_Way_2,p_buffer_For_Setting->gate_Way_3,p_buffer_For_Setting->gate_Way_4);
    printf("\r\n Subnet Mask: %s.%s.%s.%s \r\n",p_buffer_For_Setting->subnet_Mask_1,p_buffer_For_Setting->subnet_Mask_2,p_buffer_For_Setting->subnet_Mask_3,p_buffer_For_Setting->subnet_Mask_4);
    printf("\r\n Mack address: %s.%s.%s.%s.%s.%s \r\n",p_buffer_For_Setting->MAC_Address_1,p_buffer_For_Setting->MAC_Address_2,p_buffer_For_Setting->MAC_Address_3,p_buffer_For_Setting->MAC_Address_4,p_buffer_For_Setting->MAC_Address_5,p_buffer_For_Setting->MAC_Address_6);
    printf("\r\n Ip address: %s.%s.%s.%s \r\n",p_buffer_For_Setting->ip_Address_1,p_buffer_For_Setting->ip_Address_2,p_buffer_For_Setting->ip_Address_3,p_buffer_For_Setting->ip_Address_4);

}


/*
funtion:
    �������ڷ��͹���������
   
input: �ַ���(char *)

output:

note: ��ʹ��int8_t������Ϊstrcpy��C�ٷ��⴦��int8_t�ᾯ��
      ����stm32�������ͣ�int8_tʵ����typedef signed char���ͣ���char����
*/
int8_t analyze_IP_Address_Command(char const *Command)
{
    //����ָ���ţ�IP_Address����'.'��Ϊ�ָ���
    char const s[2] = '.';
    //����һ��ָ�룬ָ�򱻷ָ��λ��
    char *token;
    //����һ�����飬���IP Address, IP Address���ֻ��15���ַ�,Ԥ��һ��λ�ô��'\0'.
    char new_Command[16] = {0};

    //Command��������:"-i 192.168.178.100"
    //new_Command��������: "192.168.178.100"
    
    //�ж�
    //-i��һλ�Ƿ�Ϊ'�ո�'
    //'�ո�'��һλ�Ƿ����ַ���0-9
    if(Command[2] == ' ' && isdigit(Command[3])) 
    {
        //����-i+�ո���ȡIP Address������"192.168.178.100".
        //ʹ��strncpy���Ա�֤new_Command�ĵ�16λ��'\0'.
        strncpy(new_Command,Command +3,15);
        //���Ƹ�ȫ�ֱ��������ڹ۲�
        strncpy(checknumber,new_Command,15);

        //��ȡ��һ���ֵ��ַ���
        token = strtok(new_Command,s);
        if(token != NULL)
        {
           //��ȡ��һ�����ַ�����ǰ��λ
           //��ΪIP Address�ĵ�һ���ֲ��ܳ���3λ��
           strncpy(part1_IP_Address,token,3);
           printf("\r\n %s \r\n",part1_IP_Address);
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
           strncpy(part2_IP_Address,token,3);
           printf("\r\n %s \r\n",part2_IP_Address);
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
           strncpy(part3_IP_Address,token,3);
           printf("\r\n %s \r\n",part3_IP_Address);
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
           strncpy(part4_IP_Address,token,3);
           printf("\r\n %s \r\n",part4_IP_Address);
        }
        else //�Ҳ������Ĳ����ַ���
        {
           return 0;
        }
        
        //��ȡ���岿�ֵ��ַ���
        token = strtok(NULL,s);
        //��������ĸ�ʽ����Ӧ���е��岿�֡�
        if(token == NULL)
        {
            printf("do not find the part 5!!!");
            return 1;
        }
        else
        {
            return 0;
        }
    }
    //-i�ĺ�һλ����'�ո�' �� '�ո�'��һλ�����ַ���0-9�Ļ�������0.
    //���Թ�'�ո�'��һλ�����ַ���0-9�Ļ�����ִ�������else����
    else
    {
        return 0;
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
    char new_Command[20] = {0};
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
            //��������IP Address������
            analyze_IP_Address_Command(new_Command);
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





