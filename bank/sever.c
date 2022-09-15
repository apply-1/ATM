#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "msgq.h"
#include "bank.h"

pid_t subp[9];
int msgid;

void sigint(int num)
{
	for(int i=0; i<9; i++)
	{
		kill(subp[i],9);
	}
	del_msg(msgid);
	exit(0);
}

int main()
{
	signal(SIGINT,sigint);
	
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);	
	
	//	启动子进程
	const char* path[9] = {
		"./bin/open","./bin/destory","./bin/login",
		"./bin/unlock","./bin/query","./bin/take",
		"./bin/save","./bin/transfer","./bin/repass"
	};
	
	for(int i=0; i<9; i++)
	{
		subp[i] = vfork();
		if(subp[i] == 0)
		{
			execl(path[i],path[i],NULL);
		}
	}
	
	//	等待子进程结束
	while(-1 == wait(NULL));
}
