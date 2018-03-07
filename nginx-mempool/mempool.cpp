#include "mempool.h"

#include <iostream>
using namespace std;

void NgxPool::ngx_create_pool(size_t size)
{
	size = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;//最大1G
	_pool = (ngx_pool_s *)malloc(size); 
	if (_pool == NULL)
	{
		;//throw
	}

	_pool->d.end = (unsigned char*)_pool + size;
	_pool->d.last = (unsigned char *)_pool + sizeof(ngx_pool_s);
	_pool->d.next = NULL;
	_pool->d.failed = 0;

	size = size - sizeof(ngx_pool_s);
	_pool->max = size;
	_pool->current = _pool;
	_pool->large = NULL;
}


void* NgxPool::ngx_pnalloc(size_t size)
{
	if (_pool == NULL)
	{
		return;
	}
	if (size > _pool->max)
	{
		return ngx_palloc_large(size);
	}
	else
		return ngx_palloc_small(size,0);
}

void* NgxPool::ngx_palloc(size_t size)
{
	if (_pool == NULL)
	{
		return;
	}
	if (size > _pool->max)
	{
		return ngx_palloc_large(size);
	}
	else
		return ngx_palloc_small(size,1);
}

void *NgxPool::ngx_palloc_block(size_t size)
{
	if (_pool == NULL)
	{
		return;
	}
	size_t poolSize  = (size_t)_pool->d.end - (size_t)_pool;
	ngx_pool_t *p = _pool->current;
	while (p->d.next != NULL)p = p->d.next;

	p->d.next = (ngx_pool_t *)malloc(poolSize);
	p = p->d.next;
	p->d.end = (unsigned char *)p + poolSize;
	p->d.last = (unsigned char *)p + sizeof(ngx_pool_t);
	p->d.next = NULL;
	p->d.failed = 0;

	void *m = p->d.last;

	int a = sizeof(unsigned long);
	m =  (unsigned char *) (((uintptr_t) (m) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1)); //内存对齐

	p->d.last = (unsigned char *)m + size;
	return m;
}


void * NgxPool::ngx_palloc_small(size_t size, bool align)
{
	void *m;
	ngx_pool_t *p = _pool->current;
	while (p != NULL)
	{
		if (p->d.failed > MAXFAIL)
		{
			p = p->d.next;
			if(p != NULL)
			{
				_pool->current = p;
			}
			continue;
		}

		if (p->d.last + size <= p->d.end)
		{
			m = p->d.last;
			if (align)
			{
				int a = sizeof(unsigned long);
				m =  (unsigned char *) (((uintptr_t) (m) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1)); //内存对齐
			}

			p->d.last = (unsigned char*)m + size;
			return m;
		}
		else
		{
			++ p->d.failed;
		}

		p = p->d.next;
	}
	return ngx_palloc_block(size);

}

void * NgxPool::ngx_palloc_large(size_t size)
{
	if (_pool == NULL)
	{
		return;
	}
	int n = 0;
	void *p;
	ngx_pool_large_t *large;

	p = malloc(size);
	for (large = _pool->large;large;large = large->next)
	{
		if (large->alloc == NULL)
		{
			large->alloc = p;
			return p;
		}

		if (n++ > 3)
		{
			break;
		}
	}

	large = (ngx_pool_large_t *)malloc(sizeof(ngx_pool_large_t));
	if (large == NULL)
	{
		free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = _pool->large;
	_pool->large = large;

	return p;
}


void NgxPool::ngx_reset_pool()
{
	if (_pool == NULL)
	{
		return;
	}
	ngx_pool_t        *p;
	ngx_pool_large_t  *l;

	for (l = _pool->large; l; l = l->next) 
	{
		if (l->alloc) 
		{
			free(l->alloc);
		}
	}

	for (p = _pool; p; p = p->d.next) 
	{
		p->d.last = (unsigned char *) p + sizeof(ngx_pool_t);
		p->d.failed = 0;
	}

	_pool->current = _pool;
	_pool->large = NULL;
}

void NgxPool::ngx_destroy_pool()
{
	if (_pool == NULL)
	{
		return;
	}
	ngx_pool_t          *p, *n;
	ngx_pool_large_t    *l;

	for (l = _pool->large; l; l = l->next) 
	{
		if (l->alloc != NULL)
		{
			free(l->alloc);
		}
	}

	for (p = _pool, n = _pool->d.next; /* void */; p = n, n = n->d.next)
	{
		free(p);
		if (n == NULL) 
		{
			break;
		}
	}
	_pool = NULL;

}

void NgxPool::ngx_pfree(void *p)
{
	if (_pool == NULL)
	{
		return;
	}
	ngx_pool_large_t  *l;

	for (l = _pool->large; l; l = l->next)
	{
		if (p == l->alloc) 
		{
			free(l->alloc);
			l->alloc = NULL;
			return;
		}
	}
	return;
}

NgxPool::~NgxPool()
{
	if (_pool != NULL)
		ngx_destroy_pool();
}