#include <iostream>
#include <sys/socket.h>
#include <json/json.h>
#include <mysql/mysql.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "view_exit.h"

using namespace std;





void view_exit::Delete_Online(Json::Value val,MYSQL * mpcon)
{
	string name = val["name"].asString();
	if(!name.empty())
	{
		_reason = "Bye\n";
	}
	else
	{
		string cmd = "delete from online where name = '";
		cmd.insert(cmd.size(),name.c_str());
		cmd.insert(cmd.size(),"';");

		if(mysql_real_query(mpcon,cmd.c_str(),cmd.size()))
		{
			cerr<<"Delete Online fail;errno:"<<errno<<endl;
			return;
		}

		_reason = "Bye ";
		_reason.insert(_reason.size(),name.c_str());
		_reason.push_back('\n');
	}
}


void view_exit::process(Json::Value val,int cli_fd)
{
	_cli_fd = cli_fd;


	MYSQL * mpcon = mysql_init((MYSQL *)0);

	if(!mysql_real_connect(mpcon,"127.0.0.1","root","961122",NULL,3306,NULL,0))
	{
		cerr<<"sql connect fail;errno:"<<errno<<endl;
		return;
	}

	if(mysql_select_db(mpcon,"myweichat"))
	{
		cerr<<"select database fail;errno:"<<errno<<endl;
		return;
	}


	Delete_Online(val,mpcon);
}


void view_exit::responce()
{
	Json::Value val;
	val["reason"] = _reason.c_str();
	send(_cli_fd,val.toStyledString().c_str(),val.toStyledString().size(),0);
	_reason.clear();
}
