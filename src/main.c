/*
 * main.c
 *
 *  Created on: 2013年8月20日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */


#include "feixin.h"
#include "monitor_base.h"

void debug(const char *fmt, ...)
{
	#ifdef DEBUG
		 va_list ap;
		 va_start(ap, fmt);
		 vprintf(fmt, ap);
		 va_end(ap);
	#endif
}

int main()
{
	//send_feition("13527029922","cheng989","18007596682","this is a test.");
	struct ip_list* ip_list=NULL;
	get_broadip(&ip_list);
	struct ip_list* next=ip_list;
	while(next){
		printf("IP:%s\n",next->ip);
		next=next->next;
	}
}
