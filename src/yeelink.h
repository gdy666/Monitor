/*
 * yeelink.h
 *
 *  Created on: 2013年8月22日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef YEELINK_H_
#define YEELINK_H_
#include "yeelink_base.h"
#include <stdlib.h>

struct yee_switch{
	char timestamp[20];
	int state;
};

/*
 * 获取开关信息
 * 成功获取返回0
 */
int get_switch(int device_id,int sensor_id,const char* API_KEY,struct yee_switch*);
#endif /* YEELINK_H_ */
