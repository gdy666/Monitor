/*
 * monitor.h
 *
 *  Created on: 2013年8月21日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <pthread.h>
#include "serialib.h"
#include "sensor.h"
#include "fetion.h"
#include "yeelink.h"
#include "monitor_ini.h"

#define MONI_PORT 1110	//监听的端口，
#define CLI_PORT 1111	//客户端接收广播的端口
#define SER_ORDER "who is here?"
#define CLI_REPLY "I'm here!"

//新线程中启动监控程序
int start_monitor();

#endif /* MONITOR_H_ */
