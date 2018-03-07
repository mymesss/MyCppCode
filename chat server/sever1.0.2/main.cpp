#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include "tcpsever.h"
using namespace std;

int main(int argc,char **argv)
{
	if(argc < 4)
	{
		cerr<<"arg not enough;errno:"<<errno<<endl;
		return 0;
	}

	//解析参数   ip   port   pth_num
	char * ip = argv[1];
	unsigned short port = atoi(argv[2]);
	int pth_num = atoi(argv[3]);
	
	//构造服务器对象
	tcpsever ser(ip,port,pth_num);

	//运行服务器
	ser.run();

	return 0;
}
