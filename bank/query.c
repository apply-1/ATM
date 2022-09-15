#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"
#include "msgq.h"

Msg_ctos ctos = {};
Msg_stoc stoc = {};

void query_acc(void)
{
	printf("recv from %u:%s %lf\n",ctos.return_type,ctos.acc.card_id,ctos.acc.balance);
	//	处理具体任务
	char path[256];
	sprintf(path,"%s/%s",ACCOUNT_PATH,ctos.acc.card_id);
	int fd = open(path,O_RDWR);
	Account account = {};
	read(fd,&account,sizeof(account));
			
	sprintf(stoc.info,"卡号:%s\n身份证:%s\n余额:%lf\n",account.card_id,account.id,account.balance);
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
		recv_msg(msgid,&ctos,sizeof(ctos),MSGTY_QUERY);
		query_acc();
		send_msg(msgid,&stoc,sizeof(Msg_stoc));
	}
}
