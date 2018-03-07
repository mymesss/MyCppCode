#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
#include <json/json.h>
#include <mysql/mysql.h>
#include <string>
#include "view.h"

using namespace std;

class view_register : public view
{
public:
	bool Query_Usr(Json::Value val,MYSQL * mpcon);
	void Insert_Usr(Json::Value val,MYSQL * mpcon);
	void process(Json::Value val,int cli_fd);
	void responce();
private:
	string _reason;
	int _cli_fd;
};





#endif
