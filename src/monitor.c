/*
 * monitor.c
 *
 *  Created on: 2013年8月21日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "monitor.h"
#define MAXBUFFER 64
/*
 * 监听线程,负责接送来自客户端的响应
 */
//int pth_listenServer();

/*
 * 监听串口传感器线程
 */
int pth_serialMonitor();

/*
 *发送飞信的线程
 */
int pth_sendFetion();

/*
 * 监听端口线程功能函数
 */
//int listenServer();

/*
 * 监听串口线程功能函数
 */
int serialMonitor();

/*
 * 发送飞信线程的功能函数
 */
int sendFetion();

/*************************************/
struct ip_list* ip_list=NULL;	//保存(多个)网卡的广播地址
extern char serialPort[];
extern char fetionUser[];
extern char fetionPasswd[];
extern char fetionTo[];
extern char fetionContent[];
extern int MSGRepeat;
extern int deviceId;
extern int sensorId;
extern char API_KEY[];
struct yee_switch sw;
int sign=0;	//send为1时，线程发送飞信
int start_monitor()
{
	Init_MonitorConf();	//读取配置文件
	pth_serialMonitor();//监听串口的线程

	pthread_t p_t=pth_sendFetion();	//负责发送飞信的线程
	pthread_join(p_t,NULL);
	return 0;
}



int pth_serialMonitor()
{
	pthread_t p_t;
	if(pthread_create(&p_t,NULL,(void*)serialMonitor,NULL)){
		debug("创建监听线程失败!\n");
		exit(1);
	}
	return 0;
}

int serialMonitor()
{
	serial *s;
	while(serial_open(&s, serialPort, 9600)){	//打开串口出错，2秒后重试
		debug("open serial port error!\n");
		sleep(2);
	}

	struct HCSR501 hcsr501;
	while(1){
		while(get_hcsr501(s,&hcsr501)){	//读取串口数据出错，1秒后重试
			debug("hcsr501 error\n");
			sleep(1);
		}

		if(0 == hcsr501.state){	//HCSR501 ：没反应
			usleep(500000);
			continue;
		}
		else if(1 == hcsr501.state){	//HCSR501:有反应
			debug("检测到异常!\n");
			while(get_switch(deviceId,sensorId,API_KEY,&sw)){
				debug("获取开关状态出错\n");
			}
			if(sw.state){
				debug("监控开关状态：开！\n");
				//feixin_count+=MSGRepeat;
				sign=1;
				debug("sign=%d\n",sign);
				sleep(30);	//检测到异常后标记发送飞信，然后30内不再检测
			}
			else{
				debug("监控开关状态：关！\n");
				sleep(3);
			}


		}

	}
	return 0;
}

int pth_sendFetion()
{
	pthread_t p_t;
	if(pthread_create(&p_t,NULL,(void*)sendFetion,NULL)){
		debug("创建监听线程失败!\n");
		exit(1);
	}
	return p_t;
}

int sendFetion()
{
	int ret;
	while(1){
	//	debug("sign:%d\n",sign);
		if(sign == 1){
			//debug("user:%s passwd:%s to:%s content:%s\n",fetionUser,fetionPasswd,
			//		fetionTo,fetionContent);
			ret=sendFetionByPHPAPI(fetionUser,fetionPasswd,fetionTo,fetionContent,MSGRepeat);
			//debug("ret:%d\n",ret);
			if(ret == 0){//发送成功
				sign=0;
				usleep(50000);
			}
		}
		sleep(1);
	}
	return 0;
}
