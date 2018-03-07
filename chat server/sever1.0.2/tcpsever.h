#ifndef TCPSEVER_H
#define TCPSEVER_H


#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event.h>
#include <errno.h>
#include <stdlib.h>
#include <map>
#include "mpthread.h"
using namespace std;

typedef class tcpsever
{
	public:
		tcpsever();
		tcpsever(char *ip,unsigned short port,int pth_num);
		~tcpsever();
		void run();
		void create_sock_pair();
		void create_pth();

	private:
		friend void listen_cb(int fd,short event,void *arg);
		friend void sock_0_cb(int fd,short event,void* arg);
		struct event_base* _base;//libevent
		int _listen_fd;//listenfd
		int _pth_num;//线程个数
		vector<pair<int,int> > *_sockpair_base;//socketpair
		map<int,int> *_pth_num_map;//线程监听数量的map表
		vector<Pmpthread> _pth;//保存堆上的线程对象指针
}Tcpsever,*PTcpsever;


void sock_0_cb(int fd,short event,void *arg);
void listen_cb(int fd,short event,void *arg);


#endif


