
#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#define NGX_MAX_ALLOC_FROM_POOL 4*1024
#define MAXFAIL 4


typedef struct ngx_pool_large_s  ngx_pool_large_t;

//�ڴ�ش���ڴ����Ϣ
struct ngx_pool_large_s {
	ngx_pool_large_t     *next;
	void                 *alloc;
};

typedef struct ngx_pool_s  ngx_pool_t;

//�ڴ��С���ڴ�����ݽṹ����
typedef struct {
	unsigned char               *last;
	unsigned char               *end;
	ngx_pool_t                  *next;
	size_t                      failed;   //4��
} ngx_pool_data_t;

//�ڴ�ص�ͷ����Ϣ
struct ngx_pool_s {
	ngx_pool_data_t       d;
	size_t                max;
	ngx_pool_t           *current;
	ngx_pool_large_t     *large;
};

/*
1.�ܹ���֪ngx���ڴ�ع���
2.CPUѰַΪʲôҪ�����ڴ���룿
*/

class NgxPool
{
public:
	//�����ڴ��
	void ngx_create_pool(size_t size);
	//�ڴ濪��   �ڴ����
	void* ngx_palloc(size_t size);
	//�ڴ濪��   �������ڴ����
	void* ngx_pnalloc(size_t size);	
	//�����ڴ��
	void ngx_reset_pool();
	//�����ڴ��
	void ngx_destroy_pool();
	//���ڴ�ع黹�ڴ�
	void ngx_pfree(void *p);

	~NgxPool();
private:
	ngx_pool_t *_pool;


	NgxPool(const NgxPool& src);
	void operator=();
	//�������ڴ��
	void *ngx_palloc_block(size_t size);
	//С�ڴ����
	void * ngx_palloc_small(size_t size, bool align);
	//���ڴ����
	void * ngx_palloc_large(size_t size);
};

#endif