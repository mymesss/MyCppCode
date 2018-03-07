#include <iostream>
#include <memory.h>
#include <pthread.h>
#include "tcpsever.h"
#include "mpthread.h"
using namespace std;




tcpsever::tcpsever(char *ip,unsigned short port,int pth_num)
	:_pth(vector<Pmpthread>())
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
	//释放map
	delete _pth_num_map;
	//释放vector
	delete _sockpair_base;

	//释放堆上的线程对象
	vector<Pmpthread>::iterator it = _pth.begin();
	for(; it != _pth.end();++it)
	{
		delete *it;
	}
}

void listen_cb(int fd,short event,void *arg)
{
	//参数强转
	tcpsever * mthis = (tcpsever *)arg;
	//accept（sockfd）
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	int cli_fd = accept(fd,(struct sockaddr*)&caddr,(socklen_t *)&len);

	if (cli_fd != -1)
	{
		cout<<"client["<<cli_fd<<"],IP:"<<inet_ntoa(caddr.sin_addr)<<"  "<<ntohs(caddr.sin_port)<<endl;
		//在map中选出最合适（目前监听量最小）的线程
		map<int,int>::iterator it = mthis->_pth_num_map->begin();
		map<int,int>::iterator it_smil = it++;
		for(;it != mthis->_pth_num_map->end();++it)
		{
			//cout<<it->second<<"  ";
			if(it->second < it_smil->second)
				it_smil = it;
		}
		//cout<<endl;
		//向选出的线程的sockpair0端发送cli_fd
		send(it_smil->first,&cli_fd,sizeof(cli_fd),0);
	}
}

void tcpsever::run()
{
	//创建socketpair
	create_sock_pair();

	//启动线程
	create_pth();

	//将监听套接子libevent
	struct event * listen_event = event_new(_base,_listen_fd,EV_READ | EV_PERSIST,listen_cb,(void *)this);
	if(listen_event == NULL)
	{
		cerr<<"listen event_new fail;errno:"<<errno<<endl;
	}

	//加入libevent的监听队列
	event_add(listen_event,NULL);
	
	//循环监听
	event_base_dispatch(_base);
}

/*void tcpsever::create_pth()
{
	pthread_t id;
	vector<pair<int,int> >::iterator it = _sockpair_base->begin();
	for(;it != _sockpair_base->end();++it)
	{
		//线程创建，并传入sockpair1端
		if(pthread_create(&id,NULL,fun,(void *)(it->second)) == -1)
		{
			cerr<<"pthread_create fail;errno:"<<errno<<endl;
			return;
		}
	}

	//while(1);
}

void* fun(void * argv)
{
	int sockpair_fd = (int)argv;
	cout<<sockpair_fd<<endl;

	//while(1);
}
*/


void tcpsever::create_pth()
{
	vector<pair<int,int> >::iterator it = _sockpair_base->begin();
	for(;it != _sockpair_base->end();++it)
	{
		//在堆上new一个线程对象 给_pth保存堆上的地址，便于delete
		//线程对象构造需要sockpair_fd
		_pth.push_back(new Mpthread(it->second));
	}
}


void sock_0_cb(int fd,short event,void* arg)
{

	//强转参数
	map<int,int> *pth_num_map = (map<int,int> *)arg;
	
	//recv   当前对应线程的监听量num?
/*	struct msghdr msgr;
	struct iovec iovr[1];
	char recv_buff[20];
*/
	//创建接受msg消息的结构体
/*	memset(&msgr,0,sizeof(msgr));
	msgr.msg_name = NULL;
	msgr.msg_namelen = 0;
	iovr[0].iov_base = &recv_buff;
	iovr[0].iov_len = sizeof(recv_buff);
	msgr.msg_iov = iovr;
	msgr.msg_iovlen = 1;
*/
	int num;
	//接收消息
	if(recv(fd,&num,sizeof(int),0) == -1)
	{
		cerr<<"recvmsg fail;errno:"<<errno<<endl;
		return;
	}
	

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
