#ifndef VIEW_LOGIN
#define VIEW_LOGIN

#include <string>
#include <iostream>
#include <json/json.h>
#include <mysql/mysql.h>
#include "view.h"

using namespace std;
class view_login :public view
{
	public:
		bool Query_Offline(Json::Value val,MYSQL* mpcon);
		bool Query_Online(Json::Value val,MYSQL * mpcon);
		bool Query_Usr(Json::Value val,MYSQL * mpcon);
		void Insert_Online(Json::Value val,MYSQL * mpcon);
		void Delete_Offline(Json::Value val,MYSQL * mpcon);
		void process(Json::Value val,int cli_fd);
		void responce();
	private:
		string _reason;
		int _cli_fd;
};
#endif
