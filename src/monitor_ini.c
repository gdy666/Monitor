/*
 * monitor_ini.c
 *
 *  Created on: 2013年8月23日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */


#include "monitor_ini.h"

#define MonitorConf_URL "/etc/Monitor.conf"	//配置文件路径
#define SESSION	"Monitor_Config"			//配置文件中开始标记

char serialPort[32]="";	//串口文件路径
char fetionUser[12]="";		//飞信帐号
char fetionPasswd[64]="";		//飞信秘密
char fetionTo[12]="";			//飞信收件人
char fetionContent[128]="";	//飞信内容
int MSGRepeat;				//每次检测到异常时短信重复的条数
int deviceId;			//Yeelink平台的设备ID
int sensorId;			//Yeelink平台的开关传感器ID
char API_KEY[33]="";			//Yeelink平台的API_KEY
void Init_MonitorConf()
{
	char* buf=load_file(MonitorConf_URL);

	get_string(buf,SESSION,"serialPort","",serialPort,sizeof(serialPort));
	get_string(buf,SESSION,"fetionUser","",fetionUser,sizeof(fetionUser));
	get_string(buf,SESSION,"fetionPasswd","",fetionPasswd,sizeof(fetionPasswd));
	get_string(buf,SESSION,"fetionTo","",fetionTo,sizeof(fetionTo));
	get_string(buf,SESSION,"fetionContent","",fetionContent,sizeof(fetionContent));
	get_string(buf,SESSION,"API_KEY","",API_KEY,sizeof(API_KEY));
	MSGRepeat=get_int(buf,SESSION,"MSGRepeat",1);
	deviceId=get_int(buf,SESSION,"deviceId",0);
	sensorId=get_int(buf,SESSION,"sensorId",0);

	free(buf);

}
