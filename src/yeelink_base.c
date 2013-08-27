/*
 * yeelink.c
 *
 *  Created on: 2013年8月12日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */
#include "yeelink_base.h"
#include "curlutil.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (strlen((char *)stream) + strlen((char *)ptr) > 128) return 0;
	strcat(stream, (char *)ptr);
	return size*nmemb;
}

int yeelink_post(const char* URL,const char* argv,const char* api_key,char* result)
{
	CURL *curl = NULL;
	CURLcode ret;
	struct curl_slist *headers = 0;
	char* header=(char*)malloc(sizeof(char)*128);

	sprintf(header,"U-ApiKey: %s",api_key);
	curl = curl_easy_init();
	headers=curl_slist_append(headers,header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(argv));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	ret = curl_easy_perform(curl);
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	return ret;
}

int yeelink_get(const char* URL,const char* api_key,char* result)
{
	CURL *curl = NULL;
	CURLcode ret;
	struct curl_slist *headers = 0;
	char* header=(char*)malloc(sizeof(char)*128);

	sprintf(header,"U-ApiKey: %s",api_key);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	ret = curl_easy_perform(curl);

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	return ret;
}

int edit_datapoint(int device_id,int sensor_id,const char* json,const char* API_KEY,char* result)
{
	char* url=(char*)malloc(sizeof(char)*128);
	struct curl_slist *headers = 0;
	int ret;

	sprintf(url,"%s/%d/sensor/%d/datapoints/ ",YEELINK_BASE_API_URL,
			device_id,sensor_id);
	ret=yeelink_post(url,json,API_KEY,result);
	debug("result:%s\n",result);
	curl_slist_free_all(headers);
	free(url);
	return ret;
}

int get_datapoint(int device_id,int sensor_id,const char* API_KEY,char* result)
{
	char* url=(char*)malloc(sizeof(char)*128);
	struct curl_slist *headers = 0;
	int ret;

	sprintf(url,"%s/%d/sensor/%d/datapoints/ ",YEELINK_BASE_API_URL,
			device_id,sensor_id);
	ret=yeelink_get(url,API_KEY,result);
	//debug("result:%s\n",result);
	curl_slist_free_all(headers);
	free(url);
	return ret;
}
