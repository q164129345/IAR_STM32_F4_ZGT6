/*
 * 
 *
 */


#include "shell.h"

//声明外部变量
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
//定义一个结构体
_STRING_FORMAT_NETWORK_SETTINGS string_Buffer_For_Network_Setting;
//定义一个结构体指针
_STRING_FORMAT_NETWORK_SETTINGS *p_string_Buffer_For_Network_Setting = &string_Buffer_For_Network_Setting;

//用于查看某些值
char checknumber[30] = {0};
int32_t checksize = 100;
char test123[] = "8G";
int length[6] = {0};

/*
function: 
    16进制字符串转十进制整形

input: 

    
output:

    
note: https://typecodes.com/cseries/simplifychexstrtoint.htmll
*/
int HexStr2Integer( char * HexStr )
{
    int iResult = 0, iFlag = 1;

    //判断字符串是否合法
    if( NULL == HexStr || ( *HexStr == '+' && *(HexStr+1) == '\0' ) || ( *HexStr == '-' && *(HexStr+1) == '\0' ) 
        || ( *HexStr == 0x30 && *(HexStr+1) == 0x58 && *(HexStr+2) == '\0' )
        || ( *HexStr == 0x30 && *(HexStr+1) == 0x78 && *(HexStr+2) == '\0' ) )
    {
        return -1;
    }

    //指针变量p指向字符串的首位
    char * p = HexStr;

    //对以"+"、"-"号开头的十六进制字符串的处理
    if ( '-' == *p || '+' == *p )
    {
        if( *p == '-' )
            iFlag = -1;
        ++p;
    }
    //对"0x"或者"0X"开头的十六进制字符的处理
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
    整数转字符串

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

    //杞崲
    do
    {
        str[i++] = num%10+48;
        num /= 10;
            
    }
    while (num); //num涓嶄负0缁х画寰幆

    str[i] = '\0';


    //纭畾寮�濮嬭皟鏁寸殑浣嶇疆
    int j = 0;
    if(str[0] == '-')  //濡傛灉鏈夎礋鍙凤紝璐熷彿涓嶇敤璋冩暣
    {
        j = 1;
        ++i;
    }

    //瀵圭О浜ゆ崲
    for(;j<i/2;j++)
    {
        //瀵圭О浜ゆ崲涓ょ鐨勫�?
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
static void show_W5100_Default_Network_Settings(void)
{
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)w5100_Default_Network_Settings, W5100_NETWORK_SETTING_SIZE);
}


/*
funtion:
        
   
input: 
        

output:
        1：转换成功
        0: 转换失败


note: 
*/
static int8_t Command_Turn_To_Value(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting ,uint8_t *value_Network_Setting)
{
    
    //先判断一下命令是否正确，正确的话，进入转换
    if(Command[1] == 'i' || Command[1] == 's' || Command[1] == 'g' || Command[1] == 'm')
    {
        int32_t buffer[6] = {0};
        switch (Command[1])
        {
            //命令i，转换IP Address
            case 'i':
                    buffer[0] = atoi(string_Network_Setting->part1_IP_Address);
                    buffer[1] = atoi(string_Network_Setting->part2_IP_Address);
                    buffer[2] = atoi(string_Network_Setting->part3_IP_Address);
                    buffer[3] = atoi(string_Network_Setting->part4_IP_Address);

                    value_Network_Setting[14] = (uint8_t)buffer[0];
                    value_Network_Setting[15] = (uint8_t)buffer[1];
                    value_Network_Setting[16] = (uint8_t)buffer[2];
                    value_Network_Setting[17] = (uint8_t)buffer[3];
                break;
            //命令s, 转换Subnet MACK
            case 's':
                    buffer[0] = atoi(string_Network_Setting->part1_Subnet_Mask);
                    buffer[1] = atoi(string_Network_Setting->part2_Subnet_Mask);
                    buffer[2] = atoi(string_Network_Setting->part3_Subnet_Mask);
                    buffer[3] = atoi(string_Network_Setting->part4_Subnet_Mask);

                    value_Network_Setting[4] = (uint8_t)buffer[0];
                    value_Network_Setting[5] = (uint8_t)buffer[1];
                    value_Network_Setting[6] = (uint8_t)buffer[2];
                    value_Network_Setting[7] = (uint8_t)buffer[3]; 
                break;
            //命令g，转换Gateway 
            case 'g':
                    buffer[0] = atoi(string_Network_Setting->part1_Gateway);
                    buffer[1] = atoi(string_Network_Setting->part2_Gateway);
                    buffer[2] = atoi(string_Network_Setting->part3_Gateway);
                    buffer[3] = atoi(string_Network_Setting->part4_Gateway);

                    value_Network_Setting[0] = (uint8_t)buffer[0];
                    value_Network_Setting[1] = (uint8_t)buffer[1];
                    value_Network_Setting[2] = (uint8_t)buffer[2];
                    value_Network_Setting[3] = (uint8_t)buffer[3];  
                break;
            //命令m，转换MAC Address
            case 'm':
                //
                break;
        }
        return 1;
    }
    //不正确的话，返回转换失败
    else
    {
        return 0;
    }
}


/*
funtion:
    解析串口发送过来的命令,并将命令拆散四部分，存放到_STRING_FORMAT_NETWORK_SETTINGS结构体
    这个函数只能用于执行-i -s -g -m这四个命令
   
input: 
    入口参数1: _STRING_FORMAT_NETWORK_SETTINGS结构体

output:
    1 执行成功
    0 执行失败

note: 不使用int8_t，是因为strcpy等C官方库处理int8_t会警告
      查了stm32变量类型，int8_t实际是typedef signed char类型，即char类型
*/
static int8_t excute_Command(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *network_Setting)
{
    //是-i,-s,-g命令
    if(Command[1] == 'i' || Command[1] == 'g' || Command[1] == 's')
    {
        
        //定义分割符号
        //IP_Address与Subnet Mask与Gateway IP是以'.'作为分隔符
        char const s[2] = ".";
        //定义一个指针，指向被分割的位置
        char *token;
        //定义一个数组
        char new_Command[20] = {0};
        //定义四个数组，存放将被拆散后的四个部分
        char buf_Part1[4] = {0};
        char buf_Part2[4] = {0};
        char buf_Part3[4] = {0};
        char buf_Part4[4] = {0};

        /*
         *Command的内容如:"-i 192.168.178.100"
         *                "-g 10.10.10.10"
         *                "-s 255.255.240.0"
         *new_Command的内容如: "192.168.178.100"
         *                     "10.10.10.10"
         *                     "255.255.240.0"
         *     
         *实际上，以下这段代码的目的是：
         *1.将命令拆分成四个块
         *2.在拆块的时候，也检查了格式是否正确。（比如IP Address不可能被拆成五块,“-i 192.168.178.22.10”这样的命令是错误的。
         */

        //判断
        //-i后一位是否为'空格'
        if(Command[2] == ' ') 
        {
            //砍掉-i+空格，提取IP Address，例如"192.168.178.100".
            //使用strncpy复制19位字符串，保证第20位是'\0'
            strncpy(new_Command,Command +3,19);
            //复制给全局变量，易于观察
            strncpy(checknumber,new_Command,19);

            //获取第一部分的字符串
            token = strtok(new_Command,s);
            if(token != NULL)
            {
               //提取第一部分字符串的前三位
               //因为每一部分都不能超过三位数
               strncpy(buf_Part1,token,3);
            }
            else //找不到第一部分字符串
            {   
               return 0;
            }
            
            //获取第二部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第二部分字符串的前三位
               //因为每一部分都不能超过三位数
               strncpy(buf_Part2,token,3);
            }
            else //找不到第二部分字符串
            {
               return 0;
            }
            
            //获取第三部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第三部分字符串的前三位
               //因为每一部分都不能超过三位数
               strncpy(buf_Part3,token,3);
            }
            else //找不到第三部分字符串
            {
               return 0;
            }
            
            //获取第四部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第四部分字符串的前三位
               //因为每一部分都不能超过三位数
               strncpy(buf_Part4,token,3);
            }
            else //找不到第四部分字符串
            {
               return 0;
            }
            
            //获取第五部分的字符串
            token = strtok(NULL,s);
            //按照命令的格式，不应该有第五部分。
            if(token != NULL) return 0;
        }
        //没有找到空格的话
        else
        {
            return 0;
        }
        

       /*
        * 拆散的四个部分，进行语法检查
        *
        */
       //获取每一部分的字符串长度.
       length[0] = strlen(buf_Part1);
       length[1] = strlen(buf_Part2);
       length[2] = strlen(buf_Part3);
       length[3] = strlen(buf_Part4);

       //字符串的长度 = 合法字符串的长度，证明语法没有问题
       //IP Address,Subnet,Gatway限定在"1234567890"
       if(length[0] == strspn(buf_Part1,"1234567890") &&
          length[1] == strspn(buf_Part2,"1234567890") &&
          length[2] == strspn(buf_Part3,"1234567890") &&
          length[3] == strspn(buf_Part4,"1234567890"))
       //语法正确，命令存放到结构体里
       {
         //根据命令，存放到对应的位置
         switch (Command[1])
          {
            //-i命令
            case 'i':
                    memcpy(network_Setting->part1_IP_Address,buf_Part1,4);
                    memcpy(network_Setting->part2_IP_Address,buf_Part2,4);
                    memcpy(network_Setting->part3_IP_Address,buf_Part3,4);
                    memcpy(network_Setting->part4_IP_Address,buf_Part4,4);
                    break;
            //-s命令
            case 's':
                    memcpy(network_Setting->part1_Subnet_Mask,buf_Part1,4);
                    memcpy(network_Setting->part2_Subnet_Mask,buf_Part2,4);
                    memcpy(network_Setting->part3_Subnet_Mask,buf_Part3,4);
                    memcpy(network_Setting->part4_Subnet_Mask,buf_Part4,4);
                    break;
            //-g命令
            case 'g':
                    memcpy(network_Setting->part1_Gateway,buf_Part1,4);
                    memcpy(network_Setting->part2_Gateway,buf_Part2,4);
                    memcpy(network_Setting->part3_Gateway,buf_Part3,4);
                    memcpy(network_Setting->part4_Gateway,buf_Part4,4);
                    break;
          }
          return 1;
        }
        //语法有问题，执行失败
        else
        {
          return 0;
        }
    }

    
    //是-m命令
    if(Command[1] == 'm')
    {      
        //定义分割符号
        //IP_Address与Subnet Mask与Gateway IP是以'.'作为分隔符
        char const s[2] = ":";
        //定义一个指针，指向被分割的位置
        char *token;
        //定义一个数组
        char new_Command[30] = {0};
        //定义六个数组，存放被拆散后的六个部分
        char buf_Part1[3] = {0};
        char buf_Part2[3] = {0};
        char buf_Part3[3] = {0};
        char buf_Part4[3] = {0};
        char buf_Part5[3] = {0};
        char buf_Part6[3] = {0};

        //Command的内容如:"-m CF:D9:F4:AA:D1:7A"

        //new_Command的内容如: "CF:D9:F4:AA:D1:7A",即去掉了-m与'空格'

        /*
         *Command的内容如:"-m CF:D9:F4:AA:D1:7A"
         *new_Command的内容如: "CF:D9:F4:AA:D1:7A",即去掉了-m与'空格'
 
         *     
         *实际上，以下这段代码的目的是：
         *1.将命令拆分成六个块
         *2.在拆块的时候，也检查了格式是否正确。（比如MAC Address不可能被拆成七个块,也不可能少于六个块)
         */

        //判断
        //-m后一位是否为'空格'
        if(Command[2] == ' ')
        {
            //砍掉-i+空格，提取IP Address，例如"192.168.178.100".
            //使用strncpy复制29位字符串，保证第30位是'\0'
            strncpy(new_Command,Command +3,29);
            //复制给全局变量，易于观察
            memcpy(checknumber,new_Command,29);

            //获取第一部分的字符串
            token = strtok(new_Command,s);
            if(token != NULL)
            {
               //提取第一部分字符串的前二位
               //因为每一部分都不能超过二位数
               strncpy(buf_Part1,token,2);
            }
            else //找不到第一部分字符串
            {   
               return 0;
            }

            //获取第二部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第二部分字符串的前两位
               //因为每一部分都不能超过二位数
               strncpy(buf_Part2,token,2);
            }
            else //找不到第二部分字符串
            {
               return 0;
            }

            //获取第三部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第三部分字符串的前两位
               //因为每一部分都不能超过二位数
               strncpy(buf_Part3,token,2);
            }
            else //找不到第三部分字符串
            {
               return 0;
            }

            //获取第四部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第四部分字符串的前两位
               //因为每一部分都不能超过二位数
               strncpy(buf_Part4,token,2);
            }
            else //找不到第四部分字符串
            {
               return 0;
            }

            //获取第五部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第五部分字符串的前两位
               //因为每一部分都不能超过二位数
               strncpy(buf_Part5,token,2);
            }
            else //找不到第五部分字符串
            {
               return 0;
            }

            //获取第六部分的字符串
            token = strtok(NULL,s);
            if(token != NULL) 
            {
               //提取第六部分字符串的前两位
               //因为每一部分都不能超过二位数
               strncpy(buf_Part6,token,2);
            }
            else //找不到第六部分字符串
            {
               return 0;
            }

            //获取第七部分的字符串
            token = strtok(NULL,s);
            //按照命令的格式，不应该有第七部分。
            if(token != NULL) return 0;

        }
        //-m的后一位不是'空格' 返回0.
        else
        {
            return 0;
        }


       /*
        * 拆散的六个部分，进行语法检查
        *
        */
       //获取每一部分的字符串长度.
       length[0] = strlen(buf_Part1);
       length[1] = strlen(buf_Part2);
       length[2] = strlen(buf_Part3);
       length[3] = strlen(buf_Part4);
       length[4] = strlen(buf_Part5);
       length[5] = strlen(buf_Part6);

       //字符串的长度 = 合法字符串的长度，证明语法没有问题
       //MAC Address限定在"1234567890ABCDEF"
       if(length[0] == strspn(buf_Part1,"1234567890ABCDEF") &&
          length[1] == strspn(buf_Part2,"1234567890ABCDEF") &&
          length[2] == strspn(buf_Part3,"1234567890ABCDEF") &&
          length[3] == strspn(buf_Part4,"1234567890ABCDEF") &&
          length[4] == strspn(buf_Part5,"1234567890ABCDEF") &&
          length[5] == strspn(buf_Part6,"1234567890ABCDEF"))
       //语言正确，命令存放到结构体
       {
            
            memcpy(network_Setting->part1_MAC_Address,buf_Part1,3);
            memcpy(network_Setting->part2_MAC_Address,buf_Part2,3);
            memcpy(network_Setting->part3_MAC_Address,buf_Part3,3);
            memcpy(network_Setting->part4_MAC_Address,buf_Part4,3);
            memcpy(network_Setting->part5_MAC_Address,buf_Part5,3);
            memcpy(network_Setting->part6_MAC_Address,buf_Part6,3);
            return 1;
       }
       //语法有问题，执行失败
       else
       {
            return 0;
       }
    }




    
    //没有匹配到-i -g -s -m命令,执行失败
    return 0; 
}


/*
funtion:
    解析串口发送过来的命令
   
input: 字符串(char *)

output:

note: 不使用int8_t，是因为strcpy等C官方库处理int8_t会警告
      查了stm32变量类型，int8_t实际是typedef signed char类型，即char类型
*/
void analyze_User_Command(char const *Command)
{
    char *ans ;
    char new_Command[40] = {0};
    //查找字符‘-’
    ans = strchr(Command,'-');
    //找到字符‘-’
    if(ans != NULL)
    {
        //丢掉字符'-'之前的字符，提取字符‘-’和字符‘-’之后的字符
        strcpy(new_Command,ans);

        //分析new_Command
        switch (new_Command[1])
        {
        case 'h':
            //发送提示信息
            HAL_UART_Transmit_IT(&huart1, (uint8_t *)aTxMessage, sizeof(aTxMessage));
            checksize = strspn(test123,"1234567890ABCDEF");
            break;
        
        case 'i':
        case 'm':
        case 's':
        case 'g':
            //解析命令
            if( 1 == excute_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //解析成功
                //将字符串转换为整数
                Command_Turn_To_Value(new_Command,p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
                //写入IP地址到EEPROM                
            }
            break; 
        case 'p':
            //打印W5100网络设置
            break;
            
        default:
            //提示错误信息
            //先用printf调试，后续改为DMA发送，提高效率
            printf("Type wrong command ,please type -h to show the command list.");
            //打印帮助信息
            break; //这里不加break的话，IAR编译器会警告.
        }
        
    }
    //没有找到字符‘-’
    else
    {
        printf("something is wrong,Di you type '-'? \r\n");
    }
}



