#ifndef TOOLS_H
#define TOOLS_H

//打印提示信息
void msg_show(const char* msg,float sec);

//	按任意键继续
void anykey_continue(void);

int get_cmd(char start,char end);

//输入字符串并将末尾的\n变为\0
void my_fgets(char* str,int size);	




#endif//TOOLS_H
