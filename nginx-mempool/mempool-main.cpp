#include "mempool.h"
#include <iostream>
using namespace std;


int main()
{
	NgxPool mypool;
	mypool.ngx_create_pool(4*1024);
	int * p = (int *)mypool.ngx_palloc(5*1024);

	for (int i = 0;i < 6;++i)
	{
		p[i] = i;
		cout<<p[i]<<endl;
	}

	mypool.ngx_pfree(p);
	mypool.ngx_reset_pool();
	mypool.ngx_destroy_pool();//在析构里面会调用destory

}