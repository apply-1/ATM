#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "bank.h"
#include "msgq.h"

Msg_ctos ctos = {};
Msg_stoc stoc = {};

void open_acc(void)
{
	printf("recv from %u:%s %lf\n",ctos.return_type,ctos.acc.card_id,ctos.acc.balance);
	//	处理具体任务
	int fd = open(ID_PATH,O_RDWR);
	char str[20] = {}; 
	read(fd,str,20);
	lseek(fd,0,SEEK_SET);
	ftruncate(fd,0);
	strcpy(ctos.acc.card_id,str);
	int no = atoi(str);
	no++;
	sprintf(str,"%d",no);
	write(fd,str,20);
	close(fd);
	
	char path[256];
	sprintf(path,"%s/%s",ACCOUNT_PATH,ctos.acc.card_id);
	creat(path,0644);
	fd = open(path,O_WRONLY);
	write(fd,&ctos.acc,sizeof(Account));
	close(fd);
	
	//	准备提示信息
	stoc.type = ctos.return_type;
	sprintf(stoc.info,"开户成功，你的账户是%s\n",ctos.acc.card_id);
}

int main(int argc,const char* argv[])
{
	printf("argv:%s\n",argv[0]);
	
	//	获取消息队列
	int msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	
	for(;;)
	{
		recv_msg(msgid,&ctos,sizeof(ctos),MSGTY_OPEN);
		open_acc();
		send_msg(msgid,&stoc,sizeof(Msg_stoc));
	}
}
