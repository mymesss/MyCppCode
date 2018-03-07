#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event.h>
#include <errno.h>
#include <stdlib.h>
#include <map>
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

		/*class mypair
		{
		public:
			mypair(int *arr);
			mypair(const tcpsever::mypair & src);
			~mypair();
			int operator[](int index);
			int getVal(int index);
			int *_arr;
		};*/

	private:
		struct event_base* _base;//libevent
		int _listen_fd;//listenfd
		int _pth_num;//线程个数
		vector<pair<int,int> > *_sockpair_base;//socketpair
		map<int,int> *_pth_num_map;//线程监听数量的map表
}Tcpsever,*PTcpsever;


void sock_0_cb(int fd,short event,void *arg);
void* fun(void * argv);
