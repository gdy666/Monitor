/*
 * yeelink.h
 *
 *  Created on: 2013年8月12日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef YEELINK_BASE_H_
#define YEELINK_BASE_H_

#define YEELINK_BASE_API_URL "http://api.yeelink.net/v1.0/device"

/*
 * 编辑数据点
 */
int edit_datapoint(int device_id,int sensor_id,const char* json,const char* API_KEY,char* result);

int get_datapoint(int device_id,int sensor_id,const char* API_KEY,char* result);

#endif /* YEELINK_H_ */
