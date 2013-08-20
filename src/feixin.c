/*
 * feixin.c
 *
 *  Created on: 2013年8月18日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */
#include "feixin.h"

int send_feition(const char* user,const char* passwd,const char* to,const char* content)
{
	char* argv=(char*)malloc(sizeof(char)*512);
	char* result=(char*)calloc(256,sizeof(char));
	int state=0;

	sprintf(argv,"user=%s&passwd=%s&to=%s&msg=%s",
			user,passwd,to,content);

	post_with_argv(FEITION_URL,argv,result);
	if(!strstr(result,"success")){
		state=1;
	}

	free(argv);
	free(result);
	return state;
}
