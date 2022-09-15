#include <stdio.h>
#include <string.h>
#include <getch.h>
#include "tools.h"

//打印提示信息
void msg_show(const char* msg,float sec)
{
	printf("%s",msg);
	fflush(stdout);
	usleep(sec*1000000);
}

//	按任意键继续
void anykey_continue(void)
{
	puts("按任意键继续...");
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	getch();
}

int get_cmd(char start,char end)
{
	puts("请输入指令：");
	for(;;)
	{
		int cmd = getch();
		if(start <= cmd && end >= cmd)
		{
			printf("%c\n",cmd);
			return cmd;
		}
	}
}

//输入字符串并将末尾的\n变为\0
void my_fgets(char* str,int size)	
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	fgets(str,size,stdin);
	if(strlen(str) != size-1)
		str[strlen(str)-1]='\0';	//将\n置为\0 
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}
