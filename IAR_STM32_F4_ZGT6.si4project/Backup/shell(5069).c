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
char checknumber[20] = {0};
int32_t checksize = 0;
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
        将解析出来的IP_Address命令转换为uint8_t类型,存放到数组.
   
input: 
        

output:
        1：转换成功
        0: 转换失败


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
        将解析出来的Subnet Mask命令转换为uint8_t类型,存放到数组.
   
input: 
        

output:
        1：转换成功
        0: 转换失败


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
        将解析出来的Gateway_IP命令转换为uint8_t类型,存放到数组.
   
input: 
        

output:
        1：转换成功
        0: 转换失败


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
    解析串口发送过来的命令,并将命令拆散四部分，存放到_STRING_FORMAT_NETWORK_SETTINGS结构体
    这个函数只能用于解析-i -s -g这三个命令
    注意：解析-m命令需要使用另外一个函数
   
input: 
    入口参数1: _STRING_FORMAT_NETWORK_SETTINGS结构体

output:
    1 解析成功
    0 解析失败

note: 不使用int8_t，是因为strcpy等C官方库处理int8_t会警告
      查了stm32变量类型，int8_t实际是typedef signed char类型，即char类型
*/
int8_t analyze_IP_Subnet_Gateway_Command(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *network_Setting)
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
        //定义四个数组，存放被拆散后的四个部分
        char buf_Part1[4] = {0};
        char buf_Part2[4] = {0};
        char buf_Part3[4] = {0};
        char buf_Part4[4] = {0};

        //Command的内容如:"-i 192.168.178.100"
        //                "-g 10.10.10.10"
        //                "-s 255.255.240.0"
        //new_Command的内容如: "192.168.178.100"
        
        //判断
        //-i后一位是否为'空格'
        //'空格'后一位是否是字符串0-9
        if(Command[2] == ' ' && isdigit(Command[3])) 
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
        //-i的后一位不是'空格' 或 '空格'后一位不是字符串0-9的话，返回0.
        //测试过'空格'后一位不是字符串0-9的话，会执行这里的else代码
        else
        {
            return 0;
        }

        //拆散的四个部分，存放到结构体里面
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
        
        //判断
        //-m后一位是否为'空格'
        if(Command[2] == ' ')
        {
            //砍掉-i+空格，提取IP Address，例如"192.168.178.100".
            //使用strncpy复制29位字符串，保证第30位是'\0'
            strncpy(new_Command,Command +3,29);
            //复制给全局变量，易于观察
            //这里观察的代码会引起一个问题，明天需要继续检查一下。。。。。。
            //memcpy(checknumber,new_Command,29);

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
       //-i的后一位不是'空格' 或 '空格'后一位不是字符串0-9的话，返回0.
       //测试过'空格'后一位不是字符串0-9的话，会执行这里的else代码
        else
        {
            return 0;
        }

        //拆散的六个部分，存放到结构体里面
        memcpy(network_Setting->part1_MAC_Address,buf_Part1,3);
        memcpy(network_Setting->part2_MAC_Address,buf_Part2,3);
        memcpy(network_Setting->part3_MAC_Address,buf_Part3,3);
        memcpy(network_Setting->part4_MAC_Address,buf_Part4,3);
        memcpy(network_Setting->part5_MAC_Address,buf_Part5,3);
        memcpy(network_Setting->part6_MAC_Address,buf_Part6,3);
        
        return 1;
    }


    //没有匹配到命令
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
            break;
        
        case 'i':
            //解析-i命令
            if( 1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //解析成功
                //将字符串转换为整数
                IP_Address_Command_Turn_To_Value(p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
                //写入IP地址到EEPROM                
            }
            break;
            
        case 'm':
            //解析-m命令
            if( 1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //解析成功
                //将字符串转换为整数
            }
            break;
            
        case 's':
            //设置Subnet mask
            //if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,part1_Subnet_Mask,part2_Subnet_Mask,part3_Subnet_Mask,part4_Subnet_Mask))
            if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //解析成功
                //将字符串转换为整数
                subnet_Mask_Command_Turn_To_Value(p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
                //写入Subnet Mask到EEPROM
            }            
            break;
            
        case 'g':
            //设置网关
            //if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,part1_Gateway_IP, part2_Gateway_IP, part3_Gateway_IP, part4_Gateway_IP));
            if(1 == analyze_IP_Subnet_Gateway_Command(new_Command,p_string_Buffer_For_Network_Setting))
            {
                //解析成功
                //将字符串转换为整数
                gateway_IP_Command_Turn_To_Value(p_string_Buffer_For_Network_Setting,w5100_Network_Settings);
            }
            break;
            
        case 'p':
            //打印W5100网络设置
            do_something();
            break;
            
        default:
            //提示错误信息
            //先用printf调试，后续改为DMA发送，提高效率
            printf("Type wrong command ,please type -h to show the command list.");
            //打印帮助信息
            break; //这里不加break的话，IAR编译器会警告.
        }
        
    //筛选这些字符：h i m s g p
    //h
    //i
    //m
    //s
    //g
    //p
    //以上都是的话
    }
    //没有找到字符‘-’
    else
    {
        printf("something is wrong,Di you type '-'? \r\n");
    }
}



// 16进制字符串转整形
// 来源：https://typecodes.com/cseries/simplifychexstrtoint.html
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




