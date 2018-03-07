
#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#define NGX_MAX_ALLOC_FROM_POOL 4*1024
#define MAXFAIL 4


typedef struct ngx_pool_large_s  ngx_pool_large_t;

//内存池大块内存的信息
struct ngx_pool_large_s {
	ngx_pool_large_t     *next;
	void                 *alloc;
};

typedef struct ngx_pool_s  ngx_pool_t;

//内存池小块内存的数据结构类型
typedef struct {
	unsigned char               *last;
	unsigned char               *end;
	ngx_pool_t                  *next;
	size_t                      failed;   //4次
} ngx_pool_data_t;

//内存池的头部信息
struct ngx_pool_s {
	ngx_pool_data_t       d;
	size_t                max;
	ngx_pool_t           *current;
	ngx_pool_large_t     *large;
};

/*
1.能够熟知ngx的内存池管理？
2.CPU寻址为什么要进行内存对齐？
*/

class NgxPool
{
public:
	//创建内存池
	void ngx_create_pool(size_t size);
	//内存开辟   内存对齐
	void* ngx_palloc(size_t size);
	//内存开辟   不考虑内存对齐
	void* ngx_pnalloc(size_t size);	
	//重置内存池
	void ngx_reset_pool();
	//销毁内存池
	void ngx_destroy_pool();
	//给内存池归还内存
	void ngx_pfree(void *p);

	~NgxPool();
private:
	ngx_pool_t *_pool;


	NgxPool(const NgxPool& src);
	void operator=();
	//开辟新内存块
	void *ngx_palloc_block(size_t size);
	//小内存分配
	void * ngx_palloc_small(size_t size, bool align);
	//大内存分配
	void * ngx_palloc_large(size_t size);
};

#endif