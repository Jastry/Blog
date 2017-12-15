#ifndef __COMM_H__
#define __COMM_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>

//const char *pathname = ".";
//const int proj_id = 0x233;
/*
 * 打开一个已有的消息队列，如果不存在，则返回失败
 */
int GetMsg();

/*
 * 销毁一个存在的消息队列
 */
int DestoryMsg();

/*
 * 网消息队列中写消息
 * */
int SendMsg();

/*
 * 从消息队列中取数据
 */
int RecvMsg();
#endif
