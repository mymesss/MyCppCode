#include <iostream>
#include <sys/socket.h>
#include <json/json.h>
#include <mysql/mysql.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "view_register.h"
using namespace std;


bool view_register::Query_Usr(Json::Value val,MYSQL * mpcon)
{
	MYSQL_RES * mp_res;
	MYSQL_ROW mp_row;


	string cmd = "select * from usr where name = '";
	string name(val["name"].asString());
	cmd.insert(cmd.size(),name.c_str());
	cmd.push_back('\'');
	cmd.push_back(';');

	if(mysql_real_query(mpcon,cmd.c_str(),cmd.size()))
	{
		cerr<<"query name in usr fail;errno:"<<errno<<endl;
		exit(-1);
	}

	mp_res = mysql_store_result(mpcon);
	mp_row = mysql_fetch_row(mp_res);

	if(mp_row == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void view_register::Insert_Usr(Json::Value val,MYSQL * mpcon)
{
	string name = val["name"].asString();
	string pw = val["pw"].asString();

	string cmd = "insert into usr values('";
	cmd.insert(cmd.size(),name.c_str());
	cmd.insert(cmd.size(),"','");
	cmd.insert(cmd.size(),pw.c_str());
	cmd.insert(cmd.size(),"');");

	if(mysql_real_query(mpcon,cmd.c_str(),cmd.size()))
	{
		cerr<<"Insert usr fail;errno:"<<errno<<endl;
		return;
	}
}

void view_register::process(Json::Value val,int cli_fd)
{
	_cli_fd = cli_fd;

	//创建mysql指针
	MYSQL* mpcon = mysql_init((MYSQL *)0);

	//链接mysql
	if(!mysql_real_connect(mpcon,"127.0.0.1","root","961122",NULL,3306,NULL,0))
	{
		cerr<<"sql connect fail;errno:"<<errno<<endl;
		exit(-1);
	}

	//选择数据库
	if(mysql_select_db(mpcon,"myweichat"))
	{
		cerr<<"select database fail;errno:"<<errno<<endl;
		exit(-1);
	}

	//访问usr表，查看用户名是否存在
	if(!Query_Usr(val,mpcon))
	{
		//注册到usr
		Insert_Usr(val,mpcon);
		_reason = "Register success!\n";
	}
	else
	{
		_reason = "Register fail: The user already exists!\n";
	}
}


void view_register::responce()
{
	Json::Value val;
	val["reason"] = _reason.c_str();
	send(_cli_fd,val.toStyledString().c_str(),val.toStyledString().size(),0);
	_reason.clear();
}
