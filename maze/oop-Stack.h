
#ifndef _STACK_H_

#define _STACK_H_

#include <iostream>
using namespace std;


struct Node
{
public:
	Node(int row = -1,int col = -1)
	{
		_row = row;
		_col = col;
		_pnext = NULL;
	}


	void set_row(int row)
	{
		_row = row;
	}

	void set_col(int col)
	{
		_col = col;
	}
	void set_pnext(Node * p)
	{
		_pnext = p;
	}
	
	int get_row()
	{
		return _row;
	}

	int get_col()
	{
		return _col;
	}

	Node * get_pnext()
	{
		return _pnext;
	}

	
private:
	int _row;
	int _col;
	Node * _pnext;
};


class Stack
{
public:
	Stack()
	{
		_head = new Node;
		_top = NULL;
	}

	~Stack()
	{
		while(_top != NULL)
		{
			_head->set_pnext(_top->get_pnext());
			delete _top;
			_top=_head->get_pnext();
		}

		delete _head;
		_head = NULL;
	}

	void Push(int row,int col)
	{
		Node * p = new Node(row,col);
		insert_head(p);
		_top = p;
	}

	void Pop(int *row,int *col)
	{
		if (empty())
		{
			*row = -1;
			*col = -1;
			return;
		}

		*row = _top->get_row();
		*col = _top->get_col();
		_head->set_pnext(_top->get_pnext());
		delete _top;
		_top = _head->get_pnext();
	}

	void getTop(int * row,int * col)
	{
		if (empty())
		{
			*row = -1;
			*col = -1;
			return;
		}
		*row = _top->get_row();
		*col = _top->get_col();
	}

	bool empty()
	{
		return _top == NULL;
	}

private:
	void insert_head(Node * p)
	{
		Node * q = _head ->get_pnext();
		_head ->set_pnext(p);
		p->set_pnext(q);
	}

	void operator=(const Stack & src);

	Stack(const Stack &src);
	Node * _top;
	Node * _head;
};


#endif	_STACK_H_