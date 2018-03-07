#ifndef VIEW_EXIT_H
#define VIEW_EXIT_H

#include <string>
#include <iostream>
#include <json/json.h>
#include <mysql/mysql.h>
#include "view.h"


using namespace std;


class view_exit:public view
{
public:
	void Delete_Online(Json::Value val,MYSQL * mpcon);

	void process(Json::Value val,int cli_fd);
	void responce();
private:
	string _reason;
	int _cli_fd;
};
#endif
