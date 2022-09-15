#include <string.h>
#include <stdlib.h>
#include <getch.h>
#include "bank.h"
#include "msgq.h"
#include "tools.h"

int msgid;
Msg_ctos ctos = {};
Msg_stoc stoc = {};

int main_menu(void)
{
	system("clear");
	puts("*******欢迎使用莫尼顿银行*******");
	puts("1、开户        2、销户");
	puts("3、登录        4、解锁");
	puts("5、退卡");
	puts("***************************");
	return get_cmd('1','5');
}

int second_menu(void)
{
	system("clear");
	puts("*******欢迎使用莫尼顿银行*******");
	puts("1、查询        2、取钱");
	puts("3、存钱        4、转账");
	puts("5、修改密码     6、退出");
	puts("***************************");
	return get_cmd('1','6');
}

void open_acc(void)
{
	printf("请输入\n身份证号:");
	my_fgets(ctos.acc.id,19);
	printf("密码:");
	my_fgets(ctos.acc.pass,7);
	printf("要存入的金额:");
	scanf("%lf",&ctos.acc.balance);
	ctos.acc.is_lock = 0;
	
	ctos.type = MSGTY_OPEN;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
}

void destory_acc(void)
{
	printf("请输入\n帐号:");
	my_fgets(ctos.acc.card_id,20);
	printf("密码:");
	my_fgets(ctos.acc.pass,7);
	
	ctos.type = MSGTY_DESTORY;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	printf("%s\n",stoc.info);
	if(strcmp(stoc.info,"是否确认注销(y/n)\n") == 0)
	{
		int ret = getch();
		if(ret == 'y' || ret == 'Y')
		{
			ctos.type = 48;
			ctos.acc.is_lock = 9;
			send_msg(msgid,&ctos,sizeof(Msg_ctos));
			msg_show("已注销该帐号",1);
		}
	}
}

void query_acc(void)
{
	ctos.type = MSGTY_QUERY;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	ptintf("%s\n",stoc.info);
	anykey_continue();
}

void take_acc(void)
{
	printf("请输入\n要取出的金额:");
	scanf("%lf",&ctos.acc.balance);
	
	ctos.type = MSGTY_TAKE;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
}

void save_acc(void)
{
	printf("请输入\n要存入的金额:");
	scanf("%lf",&ctos.acc.balance);
	
	ctos.type = MSGTY_SAVE;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
}

void transfer_acc(void)
{
	printf("请输入\n对方帐号:");
	my_fgets(ctos.acc.id,20);
	printf("要转的金额:");
	scanf("%lf",&ctos.acc.balance);

	ctos.type = MSGTY_TRANSFER;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
}

void repass_acc(void)
{
	printf("请输入\n原密码:");
	my_fgets(ctos.acc.pass,7);
	printf("新密码:");
	my_fgets(ctos.acc.id,7);

	ctos.type = MSGTY_REPASS;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
}

void login_acc(void)
{
	printf("请输入\n帐号:");
	my_fgets(ctos.acc.card_id,20);
	printf("密码:");
	my_fgets(ctos.acc.pass,7);
	
	ctos.type = MSGTY_LOGIN;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
	
	//	判断登录是否成功
	if(strcmp(stoc.info,"登录成功") == 0)
	{
		for(;;)
		{
			switch(second_menu())
			{
				case '1':	query_acc();	break;
				case '2':	take_acc();	break;
				case '3':	save_acc();	break;
				case '4':	transfer_acc();	break;
				case '5':	repass_acc();	break;
				case '6':	return;	break;
			}
		}
	}
}

void unlock_acc(void)
{
	printf("请输入\n帐号:");
	my_fgets(ctos.acc.card_id,20);
	printf("身份证号:");
	my_fgets(ctos.acc.id,19);
	
	ctos.type = MSGTY_UNLOCK;
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	msg_show(stoc.info,1);
}

int main(int argc,const char* argv[])
{
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	ctos.return_type = getpid();
	for(;;)
	{
		switch(main_menu())
		{
			case '1':	open_acc();	break;
			case '2':	destory_acc();	break;
			case '3':	login_acc();	break;
			case '4':	unlock_acc();	break;
			case '5':	return 0;	break;
		}
	}
	
/*
	//	测试
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	Msg_ctos ctos = {};
	Msg_stoc stoc = {};
	strcpy(ctos.acc.card_id,"1001");
	ctos.acc.balance = 10.1;
	
	ctos.type = MSGTY_OPEN;
	ctos.return_type = getpid();
	
	send_msg(msgid,&ctos,sizeof(Msg_ctos));
	
	recv_msg(msgid,&stoc,sizeof(Msg_stoc),ctos.return_type);
	printf("recieve:%s\n",stoc.info);
*/

}
