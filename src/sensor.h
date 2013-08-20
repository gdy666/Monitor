/*
 * sensor.h
 *	传感器读写操作
 *  Created on: 2013年8月11日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef SENSOR_H_
#include "serialib.h"


//DHT11温湿度传感器的数据
struct DHT11{
	int temp;	//温度
	int rehum;	//相对湿度
};

//热释电红外传感器
struct HCSR501{
	char state;
};
/*
 *读取DHT11温湿度传感器的数据,正常读取时函数返回0
 */
int get_dht11(serial *s,struct DHT11* data);

//int HCSR501_readDate

/*
 * 读取热释电红外传感器HCSR501的数据，正常读区时函数返回0
 */
int get_hcsr501(serial *s,struct HCSR501* data);


#endif /* SENSOR_H_ */
