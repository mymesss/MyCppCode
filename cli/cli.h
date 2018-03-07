#ifndef CLI_H
#define CLI_H
#include <string>
#include "public.h"

using namespace std;


class cli
{
public:
	cli(const char * ip,int port);
	void cli_run();
	void reason_Out();
	void cli_Exit();
	void cli_Register();
	bool cli_Login();
	void cli_TalkOne();
	void cli_TalkMore();

private:
	string _ser_ip;
	int _ser_port;
	int _cli_fd;
	string _reason;
	string _name;
	string _pw;
};




#endif
