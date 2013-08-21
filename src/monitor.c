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
int pth_listenServer();

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
int listenServer();

/*
 * 监听串口线程功能函数
 */
int serialMonitor();

/*
 * 发送飞信线程的功能函数
 */
int sendFetion();

extern char serial_port[];
/*************************************/
char sign=0;	//标记，如果接收到客户端响应，标记为1，服务端处理后置零
char feixin_count=0;	//需要发送的飞信短信条数
struct ip_list* ip_list=NULL;	//保存(多个)网卡的广播地址
int start_monitor()
{
	pth_listenServer();	//监听端口线程,负责接送来自客户端的响应

	while(get_broadip(&ip_list) <= 0){//获取（多个）网卡的广播地址
		debug("无法获取到任何网卡的广播地址\n");
		sleep(2);
	}

	pth_serialMonitor();//监听串口的线程

	pth_sendFetion();	//负责发送飞信的线程

}

int pth_listenServer()
{
	pthread_t p_t;
	if(pthread_create(&p_t,NULL,(void*)listenServer,NULL)){
		debug("创建监听线程失败!\n");
		exit(1);
	}
}

int listenServer()
{
	int sockfd; /* socket descriptors */
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size;
	int num;
	char recvmsg[MAXBUFFER]; /* buffer for message */
	char fromCli[] = "I'm here!";
	/* Creating UDP socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		/* handle exception */
		perror("Creating UDPsocket failed.");
		exit(1);
	}

	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(MONI_PORT);
	server.sin_addr.s_addr = htonl (INADDR_ANY);

	//应用了 SO_REUSEADDR 选项之后，bind API 函数将允许地址的立即重用，防止出现端口忙程序启动失败的情况
	int on=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
	/* handle exception */
	perror("Bind error.");
	exit(1);
	}
	debug("Bind success!\n");
	sin_size=sizeof(struct sockaddr_in);
	while (1) {
		num = recvfrom(sockfd,recvmsg,MAXBUFFER,0,(struct sockaddr *)&client,&sin_size);

		if (num < 0){
			perror("recvfrom error\n");
			//exit(1);
			continue;
		}

		recvmsg[num] = '\0';
	//	debug("You got a message from %s\n",inet_ntoa(client.sin_addr) ); /* prints client's IP */

		if(strcmp(recvmsg,fromCli)==0){	//来自客户端的响应
			sign=1; //标记位
		}

	}

	close(sockfd); /* close listenfd */
}

int pth_serialMonitor()
{
	pthread_t p_t;
	if(pthread_create(&p_t,NULL,(void*)serialMonitor,NULL)){
		debug("创建监听线程失败!\n");
		exit(1);
	}
}

int serialMonitor()
{
	serial *s;
	while(serial_open(&s, serial_port, 9600)){	//打开串口出错，2秒后重试
		debug("open port error!\n");
		sleep(2);
	}

	struct HCSR501 hcsr501;
	while(1){
		while(get_hcsr501(s,&hcsr501)){	//读取串口数据出错，1秒后重试
			debug("test\n");
			sleep(1);
		}

		if(0 == hcsr501.state){	//HCSR501 ：没反应
			usleep(500000);
			continue;
		}
		else if(1 == hcsr501.state){	//HCSR501:有反应
			debug("有贼！\n");
		//	feixin_count=3;
			//send_broadcast(ip_list);
			sleep(10);
		}

	}
}

int pth_sendFetion()
{
	pthread_t p_t;
	if(pthread_create(&p_t,NULL,(void*)sendFetion,NULL)){
		debug("创建监听线程失败!\n");
		exit(1);
	}
}

int sendFetion()
{
	while(1){
		if(feixin_count>0){
			while(send_feition("13527029922","cheng989","18007596682","异常！")){
				usleep(100000);
			}
			feixin_count--;
		}
	}
}
