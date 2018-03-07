#ifndef MPTHREAD_H
#define MPTHREAD_H

#include <iostream>
#include <map>
#include <pthread.h>
#include "contral.h"

using namespace std;

void cli_cb(int fd,short event,void* arg);
void sock_1_cb(int fd,short event,void *arg);
void *pth_run(void *arg);

typedef class mpthread
{
	public:
		mpthread(int sock_1);
		~mpthread();

	private:
		friend void cli_cb(int fd,short event,void* arg);
		friend void sock_1_cb(int fd,short event,void *arg);
		friend void *pth_run(void *arg);

		int _sock_1;///socketpair1
		pthread_t _pth_id;//pthread_id
		struct event_base* _base;//libevent
		map<int,struct event*> _event_map;//保存事件的map
		Contral * _Mcontral;

}Mpthread,*Pmpthread;



#endif 
