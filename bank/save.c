#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"
#include "msgq.h"

Msg_ctos ctos = {};
Msg_stoc stoc = {};

void save_acc(void)
{
	printf("recv from %u:%s %lf\n",ctos.return_type,ctos.acc.card_id,ctos.acc.balance);
	//	处理具体任务
	char path[256];
	sprintf(path,"%s/%s",ACCOUNT_PATH,ctos.acc.card_id);
	int fd = open(path,O_RDWR);
	Account account = {};
	read(fd,&account,sizeof(account));
			
	account.balance += ctos.acc.balance;
	lseek(fd,0,SEEK_SET);
	ftruncate(fd,0);
	write(fd,&account,sizeof(account));
	sprintf(stoc.info,"已存入%lf元，当前余额:%lf元\n",ctos.acc.balance,account.balance);
	close(fd);
	
	//	准备提示信息
	stoc.type = ctos.return_type;
}

int main(int argc,const char* argv[])
{
	printf("argv:%s\n",argv[0]);
	
	//	获取消息队列
	int msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	
	for(;;)
	{
		recv_msg(msgid,&ctos,sizeof(ctos),MSGTY_SAVE);
		save_acc();
		send_msg(msgid,&stoc,sizeof(Msg_stoc));
	}
}
