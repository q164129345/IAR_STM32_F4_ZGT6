/*
 * ʵ�ּ򵥵�shell
 * Ŀ����ʵ����ʹ�ô�������д���������ã��浽RAM�
 */


#include "shell.h"
#include "f4_flash.h"
//�����ⲿ����
extern uint8_t w5100_Network_Settings[];

uint8_t aTxMessage[1024] =  "\r\n                                      \r\n"
                            "\r\n************* Easy Shell *************\r\n"
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
                            "      for example: >>-p \r\n"
                            "\r\n"
                            "-w    Write the network configuration to EEPROM \r\n"
                            "      for example: >>-w \r\n";


extern uint8_t w5100_Default_Network_Settings[];
//����һ���ṹ��
_STRING_FORMAT_NETWORK_SETTINGS string_Buffer_For_Network_Setting;
//����һ���ṹ��ָ��
_STRING_FORMAT_NETWORK_SETTINGS *p_string_Buffer_For_Network_Setting = &string_Buffer_For_Network_Setting;

//���ڲ鿴ĳЩֵ
char checknumber[30] = {0};


const uint8_t TEXT_Buffer[] = {"STM32 FLASH TEST"};
uint8_t Read_TEXT_Buffer[20] = {0};
#define TEXT_LENTH sizeof(TEXT_Buffer)  //����
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0) //������Ա�֤�ַ����ĳ���һ���ܱ�4����

#define FLASH_SAVE_ADDR ADDR_FLASH_SECTOR_9   //��д����9


/*
function: 
    16�����ַ���תʮ��������
    ���紮������:"90"����ô�ú�������ʮ����144
input: 
    char *
    
output:
    int
    
note: https://typecodes.com/cseries/simplifychexstrtoint.htmll
*/
int HexStr2Integer( char * HexStr )
{
    int iResult = 0, iFlag = 1;

    //�ж��ַ����Ƿ�Ϸ�
    if( NULL == HexStr || ( *HexStr == '+' && *(HexStr+1) == '\0' ) || ( *HexStr == '-' && *(HexStr+1) == '\0' ) 
        || ( *HexStr == 0x30 && *(HexStr+1) == 0x58 && *(HexStr+2) == '\0' )
        || ( *HexStr == 0x30 && *(HexStr+1) == 0x78 && *(HexStr+2) == '\0' ) )
    {
        return -1;
    }

    //ָ�����pָ���ַ�������λ
    char * p = HexStr;

    //����"+"��"-"�ſ�ͷ��ʮ�������ַ����Ĵ���
    if ( '-' == *p || '+' == *p )
    {
        if( *p == '-' )
            iFlag = -1;
        ++p;
    }
    //��"0x"����"0X"��ͷ��ʮ�������ַ��Ĵ���
    else if( ( *p == 0x30 && *(p+1) == 0x58 ) || ( *p == 0x30 && *(p+1) == 0x78 ) )
    {
        p += 2;
    }

    while( *p != '\0' )
    {
        if ( *p >= 48 && *p <= 57 )
            iResult = ( *p - 48 )+ ( iResult<<4 );
        else if ( *p >= 65 && *p <= 70 )
            iResult = ( *p - 65 + 10 )+ ( iResult<<4 );
        else if ( *p >= 97 && *p <= 102 )
            iResult = ( *p - 97 + 10 )+ ( iResult<<4 );
        else
            return -2;

        ++p;
    }
    return iFlag*iResult;
}


/*
function: 
    

input: 
    
output:
    
note: 
*/
static void show_W5100_Default_Network_Settings(void)
{
  //��ӡ�Ѿ����յ���IP Address
  printf("\r\n IP Address: %d.%d.%d.%d",w5100_Network_Settings[14],
                                        w5100_Network_Settings[15],
                                        w5100_Network_Settings[16],
                                        w5100_Network_Settings[17]);
  //��ӡ�Ѿ����յ���Subnet mask
  printf("\r\n Subnet mask: %d.%d.%d.%d",w5100_Network_Settings[4],
                                         w5100_Network_Settings[5],
                                         w5100_Network_Settings[6],
                                         w5100_Network_Settings[7]);
  //��ӡ�Ѿ����յ���Gateway IP Address
  printf("\r\n Gateway IP Address: %d.%d.%d.%d",w5100_Network_Settings[0],
                                                w5100_Network_Settings[1],
                                                w5100_Network_Settings[2],
                                                w5100_Network_Settings[3]);
  //��ӡ�Ѿ����յ���MAC Address
  printf("\r\n MAC Address: %X:%X:%X:%X:%X:%X",w5100_Network_Settings[8],
                                               w5100_Network_Settings[9],
                                               w5100_Network_Settings[10],
                                               w5100_Network_Settings[11],
                                               w5100_Network_Settings[12],
                                               w5100_Network_Settings[13]);
  printf("\r\n The above is the network configuration you entered.");
  printf("\r\n If you need to write to EEPROM , please type -w!");
  
}


/*
funtion:
        
   
input: 
        

output:
        1��ת���ɹ�
        0: ת��ʧ��


note: 
*/
static int8_t Command_Turn_To_Value(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting ,uint8_t *value_Network_Setting)
{
    
    //���ж�һ�������Ƿ���ȷ����ȷ�Ļ�������ת��
    if(Command[1] == 'i' || Command[1] == 's' || Command[1] == 'g' || Command[1] == 'm')
    {
        int32_t buffer[6] = {0};
        switch (Command[1])
        {
            //����i��ת��IP Address
            case 'i':
                    //���ַ���תΪ����
                    //���Թ�һ������
                    //��������:-i 192.168.178.100
                    //value_Network_Setting��ֵΪ: 192 168 178 100
                    buffer[0] = atoi(string_Network_Setting->part1_IP_Address);
                    buffer[1] = atoi(string_Network_Setting->part2_IP_Address);
                    buffer[2] = atoi(string_Network_Setting->part3_IP_Address);
                    buffer[3] = atoi(string_Network_Setting->part4_IP_Address);
                    //����ת��
                    value_Network_Setting[14] = (uint8_t)buffer[0];
                    value_Network_Setting[15] = (uint8_t)buffer[1];
                    value_Network_Setting[16] = (uint8_t)buffer[2];
                    value_Network_Setting[17] = (uint8_t)buffer[3];
                break;
            //����s, ת��Subnet MACK
            case 's':
                    //���ַ���תΪ����
                    buffer[0] = atoi(string_Network_Setting->part1_Subnet_Mask);
                    buffer[1] = atoi(string_Network_Setting->part2_Subnet_Mask);
                    buffer[2] = atoi(string_Network_Setting->part3_Subnet_Mask);
                    buffer[3] = atoi(string_Network_Setting->part4_Subnet_Mask);
                    //����ת��
                    value_Network_Setting[4] = (uint8_t)buffer[0];
                    value_Network_Setting[5] = (uint8_t)buffer[1];
                    value_Network_Setting[6] = (uint8_t)buffer[2];
                    value_Network_Setting[7] = (uint8_t)buffer[3]; 
                break;
            //����g��ת��Gateway 
            case 'g':
                    //���ַ���תΪ����
                    buffer[0] = atoi(string_Network_Setting->part1_Gateway);
                    buffer[1] = atoi(string_Network_Setting->part2_Gateway);
                    buffer[2] = atoi(string_Network_Setting->part3_Gateway);
                    buffer[3] = atoi(string_Network_Setting->part4_Gateway);
                    //����ת��
                    value_Network_Setting[0] = (uint8_t)buffer[0];
                    value_Network_Setting[1] = (uint8_t)buffer[1];
                    value_Network_Setting[2] = (uint8_t)buffer[2];
                    value_Network_Setting[3] = (uint8_t)buffer[3];  
                break;
            //����m��ת��MAC Address
            case 'm':
                    //��ʮ�������ַ���ת��Ϊ����
                    //���Թ�һ������:
                    //������������:-m 94:54:CE:2A:13:75
                    //value_Network_Setting��ֵ��Ӧ��: 148 84 206 42 19 117
                    buffer[0] = HexStr2Integer(string_Network_Setting->part1_MAC_Address);
                    buffer[1] = HexStr2Integer(string_Network_Setting->part2_MAC_Address);
                    buffer[2] = HexStr2Integer(string_Network_Setting->part3_MAC_Address);
                    buffer[3] = HexStr2Integer(string_Network_Setting->part4_MAC_Address);
                    buffer[4] = HexStr2Integer(string_Network_Setting->part5_MAC_Address);
                    buffer[5] = HexStr2Integer(string_Network_Setting->part6_MAC_Address);
                    //����ת��
                    value_Network_Setting[8] = (uint8_t)buffer[0];
                    value_Network_Setting[9] = (uint8_t)buffer[1];
                    value_Network_Setting[10] = (uint8_t)buffer[2];
                    value_Network_Setting[11] = (uint8_t)buffer[3];
                    value_Network_Setting[12] = (uint8_t)buffer[4];
                    value_Network_Setting[13] = (uint8_t)buffer[5];   
                break;
        }
        return 1;
    }
    //����ȷ�Ļ�������ת��ʧ��
    else
    {
        return 0;
    }
}


/*
funtion:
    �������ڷ��͹���������,���������ɢ�Ĳ��֣���ŵ�_STRING_FORMAT_NETWORK_SETTINGS�ṹ��
    �������ֻ������ִ��-i -s -g -m���ĸ�����
   
input: 
    ��ڲ���1: _STRING_FORMAT_NETWORK_SETTINGS�ṹ��

output:
    1 ִ�гɹ�
    0 ִ��ʧ��

note: ��ʹ��int8_t������Ϊstrcpy��C�ٷ��⴦��int8_t�ᾯ��
      ����stm32�������ͣ�int8_tʵ����typedef signed char���ͣ���char����
*/
static int8_t excute_Command(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *network_Setting)
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
        //�����ĸ����飬��Ž�����ɢ����ĸ�����
        char buf_Part1[4] = {0};
        char buf_Part2[4] = {0};
        char buf_Part3[4] = {0};
        char buf_Part4[4] = {0};

        /*
         *Command��������:"-i 192.168.178.100"
         *                "-g 10.10.10.10"
         *                "-s 255.255.240.0"
         *new_Command��������: "192.168.178.100"
         *                     "10.10.10.10"
         *                     "255.255.240.0"
         *     
         *ʵ���ϣ�������δ����Ŀ���ǣ�
         *1.�������ֳ��ĸ���
         *2.�ڲ���ʱ��Ҳ����˸�ʽ�Ƿ���ȷ��������IP Address�����ܱ�������,��-i 192.168.178.22.10�������������Ǵ���ġ�
         */

        //�ж�
        //-i��һλ�Ƿ�Ϊ'�ո�'
        if(Command[2] == ' ') 
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
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
        //û���ҵ��ո�Ļ�
        else
        {
            return 0;
        }
        

       /*
        * ��ɢ���ĸ����֣������﷨���
        *
        */
       //��ȡÿһ���ֵ��ַ�������.
       int8_t length[4] = {0};
       length[0] = strlen(buf_Part1);
       length[1] = strlen(buf_Part2);
       length[2] = strlen(buf_Part3);
       length[3] = strlen(buf_Part4);

       //�ַ����ĳ��� = �Ϸ��ַ����ĳ��ȣ�֤���﷨û������
       //IP Address,Subnet,Gatway�޶���"1234567890"
       if(length[0] == strspn(buf_Part1,"1234567890") &&
          length[1] == strspn(buf_Part2,"1234567890") &&
          length[2] == strspn(buf_Part3,"1234567890") &&
          length[3] == strspn(buf_Part4,"1234567890"))
       //�﷨��ȷ�������ŵ��ṹ����
       {
         //���������ŵ���Ӧ��λ��
         switch (Command[1])
          {
            //-i����
            case 'i':
                    memcpy(network_Setting->part1_IP_Address,buf_Part1,4);
                    memcpy(network_Setting->part2_IP_Address,buf_Part2,4);
                    memcpy(network_Setting->part3_IP_Address,buf_Part3,4);
                    memcpy(network_Setting->part4_IP_Address,buf_Part4,4);
                    break;
            //-s����
            case 's':
                    memcpy(network_Setting->part1_Subnet_Mask,buf_Part1,4);
                    memcpy(network_Setting->part2_Subnet_Mask,buf_Part2,4);
                    memcpy(network_Setting->part3_Subnet_Mask,buf_Part3,4);
                    memcpy(network_Setting->part4_Subnet_Mask,buf_Part4,4);
                    break;
            //-g����
            case 'g':
                    memcpy(network_Setting->part1_Gateway,buf_Part1,4);
                    memcpy(network_Setting->part2_Gateway,buf_Part2,4);
                    memcpy(network_Setting->part3_Gateway,buf_Part3,4);
                    memcpy(network_Setting->part4_Gateway,buf_Part4,4);
                    break;
          }
          return 1;
       }
       //�﷨�����⣬ִ��ʧ��
       else
       {
          return 0;
       }
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
        char new_Command[30] = {0};
        //�����������飬��ű���ɢ�����������
        char buf_Part1[3] = {0};
        char buf_Part2[3] = {0};
        char buf_Part3[3] = {0};
        char buf_Part4[3] = {0};
        char buf_Part5[3] = {0};
        char buf_Part6[3] = {0};

        //Command��������:"-m CF:D9:F4:AA:D1:7A"

        //new_Command��������: "CF:D9:F4:AA:D1:7A",��ȥ����-m��'�ո�'

        /*
         *Command��������:"-m CF:D9:F4:AA:D1:7A"
         *new_Command��������: "CF:D9:F4:AA:D1:7A",��ȥ����-m��'�ո�'
 
         *     
         *ʵ���ϣ�������δ����Ŀ���ǣ�
         *1.�������ֳ�������
         *2.�ڲ���ʱ��Ҳ����˸�ʽ�Ƿ���ȷ��������MAC Address�����ܱ�����߸���,Ҳ����������������)
         */

        //�ж�
        //-m��һλ�Ƿ�Ϊ'�ո�'
        if(Command[2] == ' ')
        {
            //����-i+�ո���ȡIP Address������"192.168.178.100".
            //ʹ��strncpy����29λ�ַ�������֤��30λ��'\0'
            strncpy(new_Command,Command +3,29);
            //���Ƹ�ȫ�ֱ��������ڹ۲�
            memcpy(checknumber,new_Command,29);

            //��ȡ��һ���ֵ��ַ���
            token = strtok(new_Command,s);
            if(token != NULL)
            {
               //��ȡ��һ�����ַ�����ǰ��λ
               //��Ϊÿһ���ֶ����ܳ�����λ��
               strncpy(buf_Part1,token,2);
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
               strncpy(buf_Part2,token,2);
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
               strncpy(buf_Part3,token,2);
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
               //��Ϊÿһ���ֶ����ܳ�����λ��
               strncpy(buf_Part4,token,2);
            }
            else //�Ҳ������Ĳ����ַ���
            {
               return 0;
            }

            //��ȡ���岿�ֵ��ַ���
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //��ȡ���岿���ַ�����ǰ��λ
               //��Ϊÿһ���ֶ����ܳ�����λ��
               strncpy(buf_Part5,token,2);
            }
            else //�Ҳ������岿���ַ���
            {
               return 0;
            }

            //��ȡ�������ֵ��ַ���
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //��ȡ���������ַ�����ǰ��λ
               //��Ϊÿһ���ֶ����ܳ�����λ��
               strncpy(buf_Part6,token,2);
            }
            else //�Ҳ������������ַ���
            {
               return 0;
            }

            //��ȡ���߲��ֵ��ַ���
            token = strtok(NULL,s);
            //��������ĸ�ʽ����Ӧ���е��߲��֡�
            if(token != NULL) return 0;

        }
        //-m�ĺ�һλ����'�ո�' ����0.
        else
        {
            return 0;
        }


       /*
        * ��ɢ���������֣������﷨���
        *
        */
       //��ȡÿһ���ֵ��ַ�������.
       int length[6] = {0}; 
        
       length[0] = strlen(buf_Part1);
       length[1] = strlen(buf_Part2);
       length[2] = strlen(buf_Part3);
       length[3] = strlen(buf_Part4);
       length[4] = strlen(buf_Part5);
       length[5] = strlen(buf_Part6);

       //�ַ����ĳ��� = �Ϸ��ַ����ĳ��ȣ�֤���﷨û������
       //MAC Address�޶���"1234567890ABCDEF"
       if(length[0] == strspn(buf_Part1,"1234567890ABCDEF") &&
          length[1] == strspn(buf_Part2,"1234567890ABCDEF") &&
          length[2] == strspn(buf_Part3,"1234567890ABCDEF") &&
          length[3] == strspn(buf_Part4,"1234567890ABCDEF") &&
          length[4] == strspn(buf_Part5,"1234567890ABCDEF") &&
          length[5] == strspn(buf_Part6,"1234567890ABCDEF"))
       //������ȷ�������ŵ��ṹ��
       {
            
            memcpy(network_Setting->part1_MAC_Address,buf_Part1,3);
            memcpy(network_Setting->part2_MAC_Address,buf_Part2,3);
            memcpy(network_Setting->part3_MAC_Address,buf_Part3,3);
            memcpy(network_Setting->part4_MAC_Address,buf_Part4,3);
            memcpy(network_Setting->part5_MAC_Address,buf_Part5,3);
            memcpy(network_Setting->part6_MAC_Address,buf_Part6,3);
            return 1;
       }
       //�﷨�����⣬ִ��ʧ��
       else
       {
            return 0;
       }
    }




    
    //û��ƥ�䵽-i -g -s -m����,ִ��ʧ��
    return 0; 
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
    char new_Command[40] = {0};
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
            STMFLASH_Write(FLASH_SAVE_ADDR,(uint32_t*)TEXT_Buffer,SIZE);
            break;
        
        case 'i':
        case 'm':
        case 's':
        case 'g':
            //��������
            if( 1 == excute_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //�����ɹ�
                //���ַ���ת��Ϊ����
                Command_Turn_To_Value(new_Command,p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
                //д��IP��ַ��EEPROM                
            }
            break; 
        case 'p':
            //��ӡW5100��������
            //show_W5100_Default_Network_Settings();
            STMFLASH_Read(FLASH_SAVE_ADDR,(uint32_t*)Read_TEXT_Buffer,SIZE);
            printf("%s",Read_TEXT_Buffer);
            break;
            
        default:
            //��ʾ������Ϣ
            //����printf���ԣ�������ΪDMA���ͣ����Ч��
            printf("Type wrong command ,please type -h to show the command list.");
            break; //���ﲻ��break�Ļ���IAR�������ᾯ��.
        }
        
    }
    //û���ҵ��ַ���-��
    else
    {
        printf("Something wrong,Di you type '-'? \r\n");
        printf("If you need some help , plese type -h");
    }
}

