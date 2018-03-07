#include <iostream>
#include <pthread.h>
#include <map>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <event.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "mpthread.h"
#include "contral.h"
using namespace std;

Mpthread::mpthread(int sock_1)
	:_event_map(map<int,struct event *>())
{
	_sock_1 = sock_1;
	_Mcontral = new Contral();
	//启动子线程
	_pth_id = pthread_create(&_pth_id,NULL,pth_run,(void *)this);
	assert(_pth_id != -1);

	//->pth_run(this)
}

Mpthread::~mpthread()
{
	delete _Mcontral;
}

void cli_cb(int fd,short event,void* arg)
{
	//参数强转
	Pmpthread mthis = (Pmpthread)arg;
	//recv   ->buff
	char recv_buff[256] = {0};
	int ret = recv(fd,recv_buff,sizeof(recv_buff) - 1,0);

	//对数据读完和客户端是否关闭进行处理
	if(ret == 0)
	{
		//cout<<"client["<<fd<<"] close!"<<endl;
		struct event * tmp_ev = mthis->_event_map[fd];
		//在事件map表删除fd
		mthis->_event_map.erase(fd);
		//在libevent中删除fd事件
		close(fd);
		event_del(tmp_ev);
		//这里不close fd，在view里面close
	}
	else if(ret < 0)
	{
		if( errno == EAGAIN || errno == EWOULDBLOCK )
		{
		
		}
		else
		{
			//cout<<"client["<<fd<<"] close!"<<endl;
			struct event * tmp_ev = mthis->_event_map[fd];
			//在事件map表删除fd
			mthis->_event_map.erase(fd);
			//在libevent中删除fd事件
			close(fd);
			event_del(tmp_ev);
		}
	}
	
	//buff->contral
	if(strlen(recv_buff) != 0)
		mthis->_Mcontral->process(recv_buff,fd);

}



void sock_1_cb(int fd,short event,void *arg)
{
	//强转
	Pmpthread mthis = (Pmpthread)arg;

	//recv   cli_fd
	int cli_fd;
	if(recv(fd,&cli_fd,sizeof(cli_fd),0) <= 0)
	{
		cerr<<"recv sock_1 fail;errno:"<<errno<<endl;
		return;
	}


	//将cli_fd加入libevent  -》cli_cb()
	struct event * cli_event = event_new(mthis->_base,cli_fd,EV_READ | EV_PERSIST,cli_cb,(void *)mthis); 
	
	event_add(cli_event,NULL);

	//将事件加入到_event_map
	mthis->_event_map[cli_fd] = cli_event;

	//send(fd,      _event_map.size(),)
	int cli_num = mthis->_event_map.size();
	send(fd,&cli_num,sizeof(cli_num),0);
}


void *pth_run(void *arg)
{
	//参数强转
	Pmpthread mthis = (Pmpthread)arg;
	//初始化子线程libevent
	mthis->_base = event_base_new();
	
	
	//将sock_1 加入到libevnet  ->sock_1_cb(  ,mthis, )
	struct event * sock_1_event = event_new(mthis->_base,mthis->_sock_1,EV_READ | EV_PERSIST,sock_1_cb,(void *)mthis);
	if(sock_1_event == NULL)
	{
		cerr<<"sock_1_event fail;errno:"<<errno<<endl;
		exit(-1);
	}

	event_add(sock_1_event,NULL);
	//启动循环监听
	event_base_dispatch(mthis->_base);
}
