/*
 * main.c
 *
 *  Created on: 2013年8月20日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */


#include "fetion.h"
//#include "monitor.h"
//#include "yeelink.h"

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
	curl_global_init(CURL_GLOBAL_ALL);
	start_monitor();
//	while(1);
	curl_global_cleanup();
	return 0;
}
