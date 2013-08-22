/*
 * main.c
 *
 *  Created on: 2013年8月20日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */


#include "feixin.h"
#include "monitor.h"
#include "yeelink.h"

#define DEBUG

void debug(const char *fmt, ...)
{
	#ifdef DEBUG
		#include <stdarg.h>
		 va_list ap;
		 va_start(ap, fmt);
		 vprintf(fmt, ap);
		 va_end(ap);
	#endif
}

int main()
{
	start_monitor();
	while(1);
//	send_feition("13527029922","cheng989","18007596682","this is a test.");
//	struct ip_list* ip_list=NULL;
//	get_broadip(&ip_list);
//	struct ip_list* next=ip_list;
//	while(next){
//		printf("IP:%s\n",next->ip);
//		send_broadcast(next->ip,1111,"who is here?");
//		next=next->next;
//	}
//	free_iplist(ip_list);
//	char* result=(char*)calloc(512,sizeof(char));
//	get_datapoint(4417,6641,"6edcfb225b401db3bb165aa4c25a4d19",result);
//	struct yee_switch sw;
//	get_switch(4417,6641,"6edcfb225b401db3bb165aa4c25a4d19",&sw);
	return 0;
}
