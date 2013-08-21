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

#include "netutil.h"
#include <pthread.h>
#include "serialib.h"
#include "sensor.h"
#include "feixin.h"

#define MONI_PORT 1110	//监听的端口，客户端也用这个端口作为接送广播

//新线程中启动监控程序
int start_monitor();

#endif /* MONITOR_H_ */
