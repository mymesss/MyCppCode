#include <iostream>
#include <json/json.h>
#include <errno.h>
#include "contral.h"
#include "public.h"
#include "view_login.h"


using namespace std;


contral::contral()
{
	//_map.insert(make_pair(MSG_TYPE_REGISTER,new view_register()));
	_map.insert(make_pair(2,new view_login()));
	//_map.insert(make_pair(MSG_TYPE_EXIT,new view_exit()));
}

contral::~contral()
{
	map<int,view*>::iterator it = _map.begin();
	for(; it != _map.end();++it)
	{
		delete it->second;
	}
}

void contral::process(char *buff,int cli_fd)
{
	//Json解析    -》TYPE
	Json::Value val;
	//读取信息的Reader
	Json::Reader read;
	if(-1 == read.parse(buff,val))
	{
		cerr<<"json read fail;errno:"<<errno<<endl;
		return;
	}

	//在map中调用消息类型对应的view对象的process
	_map[val["TYPE"].asInt()]->process(val,cli_fd);
	//_map[val["TYPE"].asInt()]->responce();
	_map[val["TYPE"].asInt()]->responce();

}
