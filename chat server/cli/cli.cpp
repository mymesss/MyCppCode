#include <iostream>
#include <json/json.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "public.h"
#include "cli.h"


using namespace std;


cli::cli(const char * ip,int port)
	:_ser_ip(ip)
	 ,_ser_port(port)
{
	_cli_fd = socket(AF_INET,SOCK_STREAM,0);
	if( -1 == _cli_fd)
	{
		cerr<<"_cli_fd create fail;errno:"<<errno<<endl;
		exit(-1);
	}
	cout<<_cli_fd<<endl;

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(_ser_port);
	saddr.sin_addr.s_addr = inet_addr(_ser_ip.c_str());

	int ret = connect(_cli_fd,(struct sockaddr *)&saddr,sizeof(saddr));

	if(ret == -1)
	{
		cerr<<"connect ser fail;errno:"<<errno<<endl;
		exit(-1);
	}
}

void cli::cli_Register()
{
	string name;
	string pw;
	cout<<"Input usr name:"<<endl;
	cin>>name;
	cout<<"Input usr password:"<<endl;
	cin>>pw;

	Json::Value val_r;
	val_r["TYPE"] = REGISTER;
	val_r["name"] = name.c_str();
	val_r["pw"] = name.c_str();

	if(send(_cli_fd,val_r.toStyledString().c_str(),val_r.toStyledString().size(),0) == -1)
	{
		cerr<<"Register send fail;errno:"<<errno<<endl;
		return;
	}

	char buff[128] = {0};
	if(recv(_cli_fd,buff,sizeof(buff) - 1,0) == -1)
	{
		cerr<<"Register recv fail;errno:"<<errno<<endl;
		return;
	}

	_reason = buff;

}

bool cli::cli_Login()
{
	char buff[128] = {0};
	cout<<"Input Your usrname:"<<endl;
	cin>>_name;
	memset(buff,0,128);
	cout<<"Input Your password:"<<endl;
	cin>>_pw;

	Json::Value val_s;
	val_s["TYPE"] = LOGIN;
	val_s["name"] = _name.c_str();
	val_s["pw"] = _pw.c_str();

	int ret = send(_cli_fd,val_s.toStyledString().c_str(),val_s.toStyledString().size(),0);
	if(ret == -1)
	{
		cerr<<"cli login send fail;errno:"<<errno<<endl;
		exit(-1);
	}
	memset(buff,0,128);

	ret = recv(_cli_fd,buff,sizeof(buff) - 1,0);
	if(ret == -1)
	{
		cerr<<"cli login recv fail;errno:"<<errno<<endl;
		exit(-1);
	}
	
	_reason = buff;
	Json::Value val_r;
	Json::Reader read;
	if(read.parse(buff,val_r) == -1)
	{
		cerr<<"json read val_r fail;errno:"<<errno<<endl;
		exit(-1);
	}

	if(strncmp(val_r["reason"].asString().c_str(),"welcome",7) == 0)
	{
		return true;
	}
	else
	{
		//清理登录不成功的数据
		_name.clear();
		_pw.clear();
		return false;
	}
}



void cli::cli_TalkOne()
{}


void cli::cli_TalkMore()
{}


void cli::cli_Exit()
{
	Json::Value val;
	val["TYPE"] = EXIT;
	val["name"] = _name.c_str();//

	if(send(_cli_fd,val.toStyledString().c_str(),val.toStyledString().size(),0) == -1)
	{
		cerr<<"cli_exit send fail;errno:"<<errno<<endl;
		return;
	}

	char buff[64] = {0};
	if(recv(_cli_fd,buff,strlen(buff) - 1,0) == -1)
	{
		cerr<<"cli_exit recv fail;errno:"<<errno<<endl;
		return;
	}
	_reason = buff;
}
void cli::reason_Out()
{
	Json::Reader read;
	Json::Value reason;
	if(read.parse(_reason.c_str(),reason) == -1)
	{
		cerr<<"json read on reason_out fail;errno:"<<errno<<endl;
		exit(-1);
	}

	cout<<reason["reason"].asString()<<endl;
	_reason.clear();
}


void cli::cli_run()
{
	int num;
	int flag = 1;
	while(true)
		{
			while(flag == 1)
			{
				cout<<"1 register\t2 login\t0 exit"<<endl;
				cin>>num;
	
				switch(num)
				{
					case REGISTER:
						cli_Register();
						reason_Out();
						break;
					case LOGIN:
						if(cli_Login())
							flag = 0;
						reason_Out();
						break;
					case EXIT:
						cli_Exit();
						reason_Out();
						flag = -1;
						break;
					default:
						cout<<"Input errno"<<endl;
						break;
				}
			}


			if(flag == -1)break;


			//启动线程
			flag = 1;

			while(flag)
			{
				cout<<"3 chat\t4 more talk\t0 exit"<<endl;
				num;

				cin>>num;
				switch(num)
				{
					case TALK_ONE:
						cli_TalkOne();
						break;
					case TALK_MORE:
						cli_TalkMore();
						break;
					case EXIT:
						cli_Exit();
						reason_Out();
						flag = 0;
						break;
					default:
						cout<<"Input Errno! Please Try Again:"<<endl;
						break;
				}
			}

			flag = 1;
		}
}
