#ifndef CONTRAL_H
#define CONTRAL_H

#include <iostream>
#include <map>
#include <json/json.h>
#include "view.h"
using namespace std;
typedef class contral
{
	public:
		contral();
		~contral();

		void process(char *buff,int cli_fd);
	private:
		map<int,view* > _map;//model

}Contral,*Pcontral;
#endif
