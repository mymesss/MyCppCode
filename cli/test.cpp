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


	int i = 0;
	while(1)
	{
		new cli(argv[1],atoi(argv[2]));
	}

}
