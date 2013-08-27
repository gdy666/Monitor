/*
 * curlutil.c
 *
 *  Created on: 2013年8月8日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 *		version:0.10
 */

#include "curlutil.h"

#define MAXBUFFER 409600
static const char* userAgent="Mozilla/5.0 (Linux; U; Android 4.1.2; zh-CN; MI 1SC Build/JZO54K) AppleWebKit/534.31 (KHTML, like Gecko) UCBrowser/9.2.4.329 U3/0.8.0 Mobile Safari/534.31";
static size_t get_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (strlen((char *)stream) + strlen((char *)ptr) > MAXBUFFER) return 0;
	strcat(stream, (char *)ptr);
	return size*nmemb;
}


int post_with_argv(const char* URL,const char* argv,char* result)
{
	CURL *curl = NULL;
	CURLcode ret;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(argv));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	ret = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	return ret;
}

int get_with_argv(const char* URL,const char* argv,char* result)
{
	memset(result,0,sizeof(result));
	CURL *curl = NULL;
	CURLcode ret;
	char* data = (char*) calloc(512,sizeof(char));

	strcat(data,URL);
	strcat(data,"?");
	strcat(data,argv);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_URL, data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);

	ret = curl_easy_perform(curl);

	free(data);
	curl_easy_cleanup(curl);
	return ret;
}

int postGetCookies(const char* URL,const char* argv,char* nnn)
{
	CURL *curl = NULL;
	CURLcode ret;
	char result[MAXBUFFER]="";
//	struct curl_slist *slist = NULL;
//	char cookies[1024]="";
//	sprintf(cookies,"Cookie: %s\r\n",cookie);
//	char url[64]="";
//	sscanf(URL,"%*[^/]%s",url);
//	printf("url:%s\n",url);
//	slist = curl_slist_append(slist, "POST /huc/user/space/login.do?m=submit&fr=space HTTP/1.1\r\n");
//	slist = curl_slist_append(slist, "Host: f.10086.cn\r\n");
//	slist = curl_slist_append(slist, cookies);
//	slist = curl_slist_append(slist, "Content-Type: application/x-www-form-urlencoded\r\n");
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(argv));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl,CURLOPT_USERAGENT,userAgent);
//	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist); // HEADER
	ret = curl_easy_perform(curl);

	debug("ret:%d \n",ret);
	struct curl_slist *cookies;
	struct curl_slist *nc;
	ret=curl_easy_getinfo(curl, CURLINFO_COOKIELIST, cookies);
	nc = cookies;
	int i = 1;

	while (nc) {
	    printf("[%d]: %s\n", i, nc->data);
	    nc = nc->next;
	    i++;
	  }



	curl_easy_cleanup(curl);
	return ret;
}
