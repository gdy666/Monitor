/*
 * feixin.h
 *
 *  Created on: 2013年8月18日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef FEIXIN_H_
#define FEIXIN_H_
#include "curlutil.h"

/*
 * 通过飞信发送消息
 * 参数：
 * user:飞信帐号
 * passwd:飞信密码
 * to:对方手机(飞信好友)
 * content：内容
 * 返回值为0时发送成功，返回值为1时发送失败。
 */
#define FEITION_URL "http://yeso.me/fetion/feixin.php"
int send_feition(
		const char* user,
		const char*passwd,
		const char* to,
		const char* content);

#endif /* FEIXIN_H_ */
