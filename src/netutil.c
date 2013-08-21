/*
 * monitor_base.c
 *
 *  Created on: 2013年8月20日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "netutil.h"

int send_broadcast(const char* ipaddr,int port,const char* content)
{
	struct sockaddr_in s_addr;
	int sock;
	int addr_len;
	int len;
	//char buff[32];
	int yes;
	/*创建socket*/
	if((sock=socket(AF_INET,SOCK_DGRAM,0))==-1){
		debug("发送更新广播时创建socket出错");
		return 0;
	}
	/*设置通信方式对广播,即本程序发送的一个消息,网络上所有主机均可以收到*/
	yes=1;
	setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(yes));

	/*设置对方地址和端口信息*/
	s_addr.sin_family=AF_INET;
	s_addr.sin_port=htons(port);
	s_addr.sin_addr.s_addr=inet_addr(ipaddr);	//广播

	/*发送UDP消息*/
	addr_len=sizeof(s_addr);
	//strcpy(buff,"update info");
	int count=0;

	for(;count<3;count++){	//重复发送3次广播，防止广播丢失
		len=sendto(sock,content,strlen(content),0,(struct sockaddr*)&s_addr,addr_len);
		if(len<0){
			debug("\n\r 发送更新广播失败.\n\r");
			return -1;
		}
		debug("发送更新广播成功.:%s\n\r",ipaddr);
		usleep(50000);
	}

	close(sock);
	return 0;
}

/*
返回本机所有Ip的广播地址， "ip1, ip2, ..."格式
*/
int get_broadip(struct ip_list** ip_list)
{
	int fd, num;
    struct ifreq ifq[16];
    struct ifconf ifc;
    int i;
    char *ips, *tmp_ip;
    char *delim = ",";

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        fprintf(stderr, "socket failed\n");
        return -1;
    }
    ifc.ifc_len = sizeof(ifq);
    ifc.ifc_buf = (caddr_t)ifq;
    if(ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    {
        fprintf(stderr, "ioctl failed\n");
        return -1;
    }
    num = ifc.ifc_len / sizeof(struct ifreq);
    for(i=0;i<num;i++)
    {
	 if(ioctl(fd, SIOCGIFBRDADDR, (char *)&ifq[i]))
	    {
		fprintf(stderr, "ioctl failed\n");
		return -1;
	    }
	}

    close(fd);

    struct ip_list* next;
    int count=0;
    for(i=0; i<num; i++)
    {
        tmp_ip = inet_ntoa(((struct sockaddr_in*)(&ifq[i].ifr_broadaddr))-> sin_addr);
        if(strcmp(tmp_ip, "0.0.0.0") != 0)
        {
        	if((*ip_list)==NULL){
        		*ip_list=calloc(sizeof(struct ip_list),sizeof(char));
        		next=*ip_list;
        	}
        	else{
        		struct ip_list* temp=calloc(sizeof(struct ip_list),sizeof(char));
        		next->next=temp;
        		next=temp;
        	}
        	next->ip=(char *)malloc(16 * sizeof(char));
			sprintf(next->ip,"%s",tmp_ip);
			count++;
        };
    }
    return count;
}

int free_iplist(struct ip_list* ip_list)
{
	struct ip_list* next=ip_list;
	struct ip_list* temp;
	while(next){
		free(next->ip);
		temp=next->next;
		free(next);
		next=temp;
	}
}
