/*
 * yeelink.c
 *
 *  Created on: 2013年8月22日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "yeelink.h"

int get_switch(int device_id,int sensor_id,const char* API_KEY,struct yee_switch* sw)
{
	int state=0;
	char* result=(char*)calloc(256,sizeof(char));
	get_datapoint(device_id,sensor_id,API_KEY,result);
	char* p=NULL;
	if((p=strstr(result,"timestamp"))!=NULL){//获取成功
		p+=12;
		sscanf(p,"%[^\"]",sw->timestamp);
	//	debug("time:%s\n",sw->timestamp);
		p=strstr(p,"value");
		p+=7;
		sscanf(p,"%d",&(sw->state));
	}else{//获取出错
		state=1;
	}

	free(result);
	return state;
}
