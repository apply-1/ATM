#ifndef MSGQ_H
#define MSGQ_H
#include <stdio.h>

int get_msg(const char *pathname,int proj_id);

//	magsz提供消息包总字节数
int send_msg(int msqid, const void *msgp, size_t msgsz);

//	magsz提供消息包总字节数
int recv_msg(int msqid, void *msgp, size_t msgsz, long msgtyp);

int del_msg(int msqid);

#endif//MSGQ_H
