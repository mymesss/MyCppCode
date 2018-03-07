#include <iostream>
#include <memory.h>
#include <pthread.h>
#include "tcpsever.h"
using namespace std;


/*tcpsever::mypair::mypair(int* arr = NULL)
{
	_arr = new int[2];
	if(arr != NULL)
	{
		int i = 0;
		for(;i < 2; ++i)
		{
			_arr[i] = arr[i];
		}
	}
}

tcpsever::mypair::~mypair()
{
	delete[] _arr;
}

tcpsever::mypair::mypair(const tcpsever::mypair & src)
{
	int i = 0;
	for(;i < 2;++i)
	{
		_arr[i] = src._arr[i];
	}
}



int tcpsever::mypair::operator[](int index)
{
	cout<<"mypair::operator[]"<<endl;
	if(index <= 1 && index >= 0)return _arr[index];
	return -1;
}

int tcpsever::mypair::getVal(int index)
{
	if(index <= 1 && index >= 0)
	{
		return _arr[index];
	}

	return -1;
}

*/
tcpsever::tcpsever(char *ip,unsigned short port,int pth_num)
{
	//创建map对象
	_pth_num_map = new map<int,int>;
	_sockpair_base = new vector<pair<int,int> >;
	//创建服务器

	//创建套接字listefd
	int listefd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == listefd)
	{
		cerr<<"fd create file;errno:"<<errno<<endl;
		return;
	}

	//bind  绑定套接字
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);

	if(-1 == bind(listefd,(struct sockaddr*)&saddr,sizeof(saddr)))
	{
		cerr<<"bind fail;errno:"<<errno<<endl;
		return;
	}

	//创建监听队列
	if(-1 == listen(listefd,5))
	{
		cerr<<"listen fail;errno:"<<errno<<endl;
		return;
	}
	
	//将套接字和线程数给对象保存
	_listen_fd = listefd;
	_pth_num = pth_num;

	//初始化libevent
	_base = event_base_new();
}


tcpsever::~tcpsever()
{
	delete _pth_num_map;
}

void tcpsever::run()
{
	//创建socketpair
	create_sock_pair();

	//启动线程
	create_pth();

	//将监听套接子libevent
	
	//循环监听
}

void tcpsever::create_pth()
{
	pthread_t id;
	vector<pair<int,int> >::iterator it = _sockpair_base->begin();
	for(;it != _sockpair_base->end();++it)
	{
		if(pthread_create(&id,NULL,fun,(void *)(it->second)) == -1)
		{
			cerr<<"pthread_create fail;errno:"<<errno<<endl;
			return;
		}
	}

	while(1);
}

void* fun(void * argv)
{
	int sockpair_fd = (int)argv;
	cout<<sockpair_fd<<endl;

	while(1);
}

void sock_0_cb(int fd,short event,void* arg)
{

	//强转参数
	map<int,int> *pth_num_map = (map<int,int> *)arg;
	
	//recv   当前对应线程的监听量num?
	struct msghdr msgr;
	struct iovec iovr[1];
	char recv_buff[20];

	//创建接受msg消息的结构体
	memset(&msgr,0,sizeof(msgr));
	msgr.msg_name = NULL;
	msgr.msg_namelen = 0;
	iovr[0].iov_base = &recv_buff;
	iovr[0].iov_len = sizeof(recv_buff);
	msgr.msg_iov = iovr;
	msgr.msg_iovlen = 1;

	//接收消息
	if(recvmsg(fd,&msgr,0) == -1)
	{
		cerr<<"recvmsg fail;errno:"<<errno<<endl;
		return;
	}
	
	int num = atoi(recv_buff);

	//更新到map表
	(*pth_num_map)[fd] =  num;
}

void tcpsever::create_sock_pair()
{
	//申请socketpair
	int socketpair_fd[2] = {-1};
	for(int i = 0; i < _pth_num;++i)
	{
		//socketpair(int [2])
		if(socketpair(AF_UNIX,SOCK_STREAM,0,socketpair_fd) == -1)
		{
			cerr<<"socketpair fail;errno:"<<errno<<endl;
			return;
		}

		//将创建的socketpair添加到vector
		_sockpair_base->push_back(make_pair(socketpair_fd[0],socketpair_fd[1]));
		//将双端管道的0端存到map表并进行计数归0
		_pth_num_map->insert(make_pair(socketpair_fd[0],0));

		//将socketpair0端加入到libevent并注册回调函数
		struct event * listen_event_pair = event_new(_base,socketpair_fd[0],EV_READ | EV_PERSIST,sock_0_cb,(void *)_pth_num_map);

		event_add(listen_event_pair,NULL);

		//启动循环监听  event_dispatch();
		//event_base_dispatch(_base);
	}
}
