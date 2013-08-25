/*
 * sensor.c
 *
 *  Created on: 2013年8月11日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */
#include "sensor.h"
#define DHT11_ORDER "DHT11"
//#include "includes.h"

int get_dht11(serial *s,struct DHT11* data)
{
	int state=0;
//	static int count_err=0;	//连续错误次数
	char buffer[128];

	serial_write(s,"DHT11");
	usleep(90000);
	serial_read(s, buffer, '\n', 128);
	debug("receive from serial's data length is :%d\n",strlen(buffer));
	debug("receive from serial's data content is :%s\n",buffer);

	//判定接收到的字符串长度，小于50失败
	if(strlen(buffer)>50){
		sscanf(buffer,"%*s%*s%*s%*s%d%*s%d",&data->temp,&data->rehum);
		debug("temp:%d rehum:%d\n",data->temp,data->rehum);
		if(data->rehum>100||data->rehum<0||data->temp<0||data->temp>80){
			state=1;
//			count_err++;	//连续错误累加
		}
	}else{
		state=1;
	}

//	if(count_err>=100){	//如果连续错误超过100次，仍然将函数返回值置0，目的是为了让程序跳出死循环
//		state=0;
//	}
//
//	if(state==0){
//		count_err=0;
//	}
	return state;
}

int get_hcsr501(serial *s,struct HCSR501* data)
{
	int state=0;
	char buffer[128];


	serial_write(s,"HCSR501");
	usleep(90000);
	serial_read(s, buffer, '\n', 128);
	sscanf(buffer,"%*s%*s%d",&data->state);
//	printf("data->state :%d \n",data->state);
	if(data->state!=0&&data->state!=1){
		state=1;
	}
	return state;
}
