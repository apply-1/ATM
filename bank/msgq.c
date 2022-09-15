#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msgq.h"

int get_msg(const char *pathname,int proj_id)
{
	int msgid = msgget(ftok(pathname,proj_id),IPC_CREAT|0644);
	if(msgid < 0)
	{
		perror("msgget");
	}
	return msgid;
}

//	magsz提供消息包总字节数
int send_msg(int msqid, const void *msgp, size_t msgsz)
{
	int ret = msgsnd(msqid,msgp,msgsz-sizeof(long),0);
	if(ret < 0)
	{
		perror("msgsnd");
	}
	return ret;
}

//	magsz提供消息包总字节数
int recv_msg(int msqid, void *msgp, size_t msgsz, long msgtyp)
{
	int ret = msgrcv(msqid,msgp,msgsz-sizeof(long),msgtyp,0);
	if(ret <= 0)
	{
		perror("msgrcv");
	}
	return ret;
}

int del_msg(int msqid)
{
	int ret = msgctl(msqid,IPC_RMID,NULL);
	if(ret)
	{
		perror("msgctl");
	}
	return ret;
}

