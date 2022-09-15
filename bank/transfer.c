#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"
#include "msgq.h"

Msg_ctos ctos = {};
Msg_stoc stoc = {};

void transfer_acc(void)
{
	printf("recv from %u:%s %lf\n",ctos.return_type,ctos.acc.card_id,ctos.acc.balance);
	//	处理具体任务
	char path[256];
	sprintf(path,"%s/%s",ACCOUNT_PATH,ctos.acc.id);
	int you_fd = open(path,O_RDWR);
	if(you_fd < 0)
	{
		sprintf(stoc.info,"对方帐号不存在\n");
	}
	else
	{
		char path2[256];
		sprintf(path2,"%s/%s",ACCOUNT_PATH,ctos.acc.card_id);
		int my_fd = open(path2,O_RDWR);
		Account you_account = {};				//	对方的
		read(you_fd,&you_account,sizeof(you_account));
			
		Account my_account = {};
		read(my_fd,&my_account,sizeof(my_account));		//	自己的


		if(my_account.balance < ctos.acc.balance)
		{
			sprintf(stoc.info,"你的账户余额不足\n");
		}
		else
		{
			you_account.balance += ctos.acc.balance;
			my_account.balance -= ctos.acc.balance;
			lseek(you_fd,0,SEEK_SET);
			ftruncate(you_fd,0);
			write(you_fd,&you_account,sizeof(you_account));
			lseek(my_fd,0,SEEK_SET);
			ftruncate(my_fd,0);
			write(my_fd,&my_account,sizeof(my_account));
			sprintf(stoc.info,"转账成功\n");
		}
		close(my_fd);
		close(you_fd);
	}
	
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
		recv_msg(msgid,&ctos,sizeof(ctos),MSGTY_TRANSFER);
		transfer_acc();
		send_msg(msgid,&stoc,sizeof(Msg_stoc));
	}
}
