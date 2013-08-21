/*
 * monitor_base.h
 *
 *  Created on: 2013年8月20日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef MONITOR_BASE_H_
#define MONITOR_BASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>



//存储IP列表的结构体
struct ip_list{
	char* ip;
	struct ip_list* next;
};

/*
 * 获取到本地所有网卡的广播地址
 * 保存到ip_list中
 * 正常获取返回网卡数量，异常返回-1
 */
int get_broadip(struct ip_list** ip_list);
/*
 * 发送广播
 * 参数
 * ipaddr:广播地址
 * content:广播内容
 */
int send_broadcast(const char* ipaddr,int port,const char* content);

/*
 * 清空ip_list申请的空间
 */
int free_iplist(struct ip_list* ip_list);

#endif /* MONITOR_BASE_H_ */
