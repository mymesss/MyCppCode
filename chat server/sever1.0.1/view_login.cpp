#include <iostream>
#include <sys/socket.h>
#include <json/json.h>
#include <mysql/mysql.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "view_login.h"
using namespace std;

bool view_login::Query_Offline(Json::Value val,MYSQL * mpcon)
{

	MYSQL_RES * mp_res;
	MYSQL_ROW mp_row;
	
	//从Json包中读出名字
	string name = val["name"].asString();
	//创建命令字符串	
	name.push_back('\'');
	name.push_back(';');

	char cmd[128] = "select * from offline where name = '";
	strcat(cmd,name.c_str());

	//查找语句
	if(mysql_real_query(mpcon,cmd,strlen(cmd)))
	{
		cerr<<"query name in offline fail;errno:"<<errno<<endl;
		exit(-1);
	}

	//读取返回值
	mp_res = mysql_store_result(mpcon);
	if(mp_res != NULL)
	{
		//读取查询结果
		_reason.insert(_reason.size(),"Offline reason:\n");
		while(mp_row = mysql_fetch_row(mp_res))
		{
			_reason.insert(_reason.size(),mp_row[1]);
		}
		return true;
	}
	//无离线消息处理
	_reason.insert(_reason.size(),"No reason!\n");
	return false;
}

bool view_login::Query_Usr(Json::Value val,MYSQL * mpcon)
{
	MYSQL_RES * mp_res;
	MYSQL_ROW mp_row;
	//访问usr表   name pw
	//组合命令
	char cmd[128] = "select * from user where name = '";
	string name(val["name"].asString());
	name.push_back('\'');
	name.push_back(';');
	strcat(cmd,name.c_str());
	//执行查询name的命令
	if(mysql_real_query(mpcon,cmd,strlen(cmd)))
	{
		cerr<<"query name in usr fail;errno:"<<errno<<endl;
		exit(-1);
	}

	//接收指令的返回值
	mp_res = mysql_store_result(mpcon);

	//接收返回值的一行
	if(mp_res != NULL)	
	{
		mp_row = mysql_fetch_row(mp_res);
		if(strcmp(mp_row[1],val["pw"].asString().c_str()) == 0)
		{
			_reason.insert(_reason.size(),"welcome ");
			_reason.insert(_reason.size(),val["name"].asString());
			_reason.push_back('\n');

			return true;
		}
	}
	else
	{
		_reason.insert(_reason.size(),"No this user!\nPlease examine your input and try again!\n");
		return false;
	}
}

bool view_login::Query_Online(Json::Value val,MYSQL * mpcon)
{
	
	MYSQL_RES * mp_res;
	MYSQL_ROW mp_row;
	
	//从Json包中读出名字
	string name = val["name"].asString();
	//创建命令字符串	
	name.push_back('\'');
	name.push_back(';');

	char cmd[128] = "select * from online where name = '";
	strcat(cmd,name.c_str());

	//查找语句
	if(mysql_real_query(mpcon,cmd,strlen(cmd)))
	{
		cerr<<"query name in online fail;errno:"<<errno<<endl;
		exit(-1);
	}

	//读取返回值
	mp_res = mysql_store_result(mpcon);
	if(mp_res != NULL)
	{
		//读取查询结果
		_reason.insert(_reason.size(),"Offline reason:\n");
		if(mp_row = mysql_fetch_row(mp_res))
		{
			if(strcmp(val["name"].asString().c_str(),mp_row[1]) == 0)
			{
				//已在线处理
				_reason.insert(_reason.size(),"Don't repeat login\n");
				return false;
			}
		}
	}
	//不在线处理
	return true;
}

void view_login::Insert_Online(Json::Value val,MYSQL * mpcon)
{
	//从Json包中读出名字
	string name = val["name"].asString();

	//创建命令字符串	
	name.push_back('\'');
	name.push_back(')');
	name.push_back(';');
	string cmd = "insert into online values('";
	char cli_fd[10] = {0};
	sprintf(cli_fd,"%d",_cli_fd);
	cmd.insert(cmd.size(),cli_fd,strlen(cli_fd));
	cmd.insert(cmd.size(),"','");
	cmd.insert(cmd.size(),name.c_str());

	if(mysql_real_query(mpcon,cmd.c_str(),cmd.size()))
	{
		cerr<<"Insert Online fali;errno:"<<errno<<endl;
		return;
	}

}
void view_login::Delete_Offline(Json::Value val,MYSQL * mpcon)
{
	//从Json包中读出名字
	string name = val["name"].asString();

	//创建命令字符串	
	name.push_back('\'');
	name.push_back(';');
	string cmd = "delete from offline where name = '";
	cmd.insert(cmd.size(),name.c_str());

	if(mysql_real_query(mpcon,cmd.c_str(),cmd.size()))
	{
		cerr<<"Delete Offline fali;errno:"<<errno<<endl;
		return;
	}
}

void view_login::process(Json::Value val,int cli_fd)
{
	_cli_fd = cli_fd;

	//创建mysql指针
	MYSQL * mpcon = mysql_init((MYSQL *)0);


	//链接mysql 
	if(!mysql_real_connect(mpcon,"127.0.0.1","root","961122",NULL,3306,NULL,0))
	{
		cerr<<"sql connect fail;errno:"<<errno<<endl;
		return;
	}

	//选择数据库
	if(mysql_select_db(mpcon,"myweichat"))
	{
		cerr<<"select database fail;errno:"<<errno<<endl;
		return;
	}


	//访问在线表，是否重复登录
	if(Query_Online(val,mpcon))
	{
		//查看用户表，确认用户存在和密码正确
		if(Query_Usr(val,mpcon))
		{
			//访问 offline 并读取离线消息
			Query_Offline(val,mpcon);
			//在Offline中删除离线用户和消息
			Delete_Offline(val,mpcon);
			//在Online中添加用户
			Insert_Online(val,mpcon);
		}
	}
}



void view_login::responce()
{
	Json::Value val;
	val["reason"] = _reason.c_str();
	send(_cli_fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
}
