#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include "cli.h"

using namespace std;



int main(int argc,char ** argv)
{
	if(argc < 3)
	{
		cerr<<"arg not enough;errno:"<<errno<<endl;
		return 0;
	}

	cli mycli(argv[1],atoi(argv[2]));
	mycli.cli_run();
}
