#include <iostream>
using namespace std;


class CircleQueue   //默认第_size-1个数据为无效数据
{
public:
	CircleQueue(int size=20);
	CircleQueue(const CircleQueue &src);
	void operator=(const CircleQueue &src);
	~CircleQueue();
	void addQue(int data);
	void delQue();
	int front();
	int back();
	int getlen();
	bool empty();
	bool full();
private:
	void resize();
	int *_pQueue;
	int _front;
	int _rear;
	int _size;
};
CircleQueue::CircleQueue(int size)
{
	_size = size;
	_front = 0;
	_rear = _front;
	_pQueue = new int [_size];
	memset(_pQueue,0,sizeof(int)*_size);
}

CircleQueue::CircleQueue(const CircleQueue &src)
{
	_pQueue = new int [src._size];
	memcpy(_pQueue,src._pQueue,sizeof(int)*src._size);
	_size = src._size;
	_front = src._front;
	_rear = src._rear;
}

CircleQueue::~CircleQueue()
{
	delete []_pQueue;
}

void CircleQueue::operator=(const CircleQueue &src)
{
	if (this == &src)
	{
		return;
	}

	delete []_pQueue;
	_pQueue = new int [src._size];
	memcpy(_pQueue,src._pQueue,sizeof(int)*src._size);
	_size = src._size;
	_front = src._front;
	_rear = src._rear;
}

void CircleQueue::addQue(int data)
{
	if (full())
	{
		resize();
	}

	_pQueue[_rear] = data;
	_rear = (_rear+1)%_size;
}

void CircleQueue::delQue()
{
	_rear = _front;
}

bool CircleQueue::full()
{
	return _front == (_rear+1)%_size;
}

bool CircleQueue::empty()
{
	return _front == _rear;
}

void CircleQueue::resize()
{
	int * p = _pQueue;
	_pQueue = new int [_size*2];
	memcpy(_pQueue,p,sizeof(int)*_size);
	delete []p;
	_size *= 2;
}

int CircleQueue::front()
{
	if (empty())
	{
		throw "front error\n";
	}
	int data = _pQueue[_front];
	_front = (_front+1)%_size;

	return data;
}

int CircleQueue::getlen()
{
	if (empty())
	{
		return 0;
	}
	else if (_front < _rear)
	{
		return _rear - _front;
	}
	else if (_front > _rear)
	{
		return (_size - (_front - _rear) - 1);
	}
}

int CircleQueue::back()
{
	if (empty())
	{
		throw "back error\n";
	}
	int data = _pQueue[_rear - 1];
	_rear = (_rear - 1)%_size;

	return data;
}








//用OOP实现带头结点单链表
struct Node
{
	Node(int data=0)
	{
		_data = data;
		_pnext = NULL;
	}
	int _data;
	Node *_pnext;
};
class LinkList
{
public:
	LinkList();
	~LinkList();
	void insertHead(int data);
	void insertTail(int data);
	void deleteData(int data);
	Node * createLoop(int data1,int data2);
	void show();
	//是否有环
	bool isLoop();
	//获取单链表环的第一个节点
	Node* getFirstLoopNode();
	//获取环的长度
	int getLoopLength();
private:
	Node *_head;
	Node *_tail;
	int _length;
	LinkList(const LinkList&);
	void operator=(const LinkList&);
};

LinkList::LinkList()
{
	_head = new Node;
	_tail = NULL;
	_length = 0;
}

LinkList::~LinkList()
{
	Node * p = _head->_pnext;
	while (_head->_pnext != NULL)
	{
		_head->_pnext = p->_pnext;
		delete p;
		p = _head->_pnext;
	}
	delete _head;
	_head = NULL;
}

void LinkList::insertHead(int data)
{
	Node * p = _head->_pnext;

	_head->_pnext = new Node;
	_head->_pnext->_pnext = p;
	_head->_pnext->_data = data;

	if (_tail == NULL)
	{
		_tail = _head->_pnext;
	}
	_length++;
}

void LinkList::insertTail(int data)
{
	if (_tail == NULL)
	{
		_tail = new Node;
		_head->_pnext = _tail;
	}
	else
	{
		_tail ->_pnext = new Node;
		_tail = _tail->_pnext;
	}
	_tail->_data = data;
	_length++;
}

void LinkList::deleteData(int data)
{
	Node * p = _head;
	while(p != NULL&&p->_pnext != NULL)
	{
		if(p->_pnext->_data == data)
		{
			Node * q = p->_pnext;
			p->_pnext = q->_pnext;
			delete q;
			_length--;
		}
		else
		{
			p = p->_pnext;
		}
	}
}

void LinkList::show()
{
	Node * p = _head->_pnext;

	while(p != NULL)
	{
		cout<<p->_data<<" ";
		p = p->_pnext;
	}

	cout<<endl;
}

int LinkList::getLoopLength()
{
	return _length;
}

bool LinkList::isLoop()
{
	Node * p = _head->_pnext;
	Node * q = _head->_pnext->_pnext;

	while (p != NULL && q != NULL)
	{
		if (p  == q)
		{
			return true;
		}

		p = p->_pnext;
		q = q->_pnext->_pnext;
	}

	return false;
}

Node * LinkList::getFirstLoopNode()
{
	Node * p = _head->_pnext;
	Node * q = _head->_pnext->_pnext;

	while (p != NULL && q != NULL)
	{
		if (p == q)
		{
			p = _head;
			while (p != NULL)
			{
				if (p == q)
				{
					return q;
				}
				p = p->_pnext;
				q = q->_pnext;
			}
		}
		p = p->_pnext;
		q = q->_pnext->_pnext;
	}
	return NULL;
}




Node * LinkList::createLoop(int data1,int data2)
{
	Node * p = _head->_pnext;
	Node * q = _head->_pnext;
	while(p != NULL && p->_data != data1)p = p->_pnext;
	while(q != NULL && q->_data != data2)q = q->_pnext;

	q->_pnext = p;

	return p;
}





int main()
{

	//CircleQueue que1;
	//for (int i = 0;i < 20;i++)
	//{
	//	que1.addQue(i);
	//}
	//CircleQueue que2(que1);
	//CircleQueue que3;
	//que3 = que2;
	//int n1 = que1.front();
	//int n2 = que2.front();
	//int n3 = que3.front();
	//cout<<n1<<" "<<n2<<" "<<n3<<endl;

	//n1 = que1.back();
	//n2 = que2.back();
	//n3 = que3.back();
	//cout<<n1<<" "<<n2<<" "<<n3<<endl;

	//n1 = que1.front();
	//n2 = que2.front();
	//n3 = que3.front();
	//cout<<n1<<" "<<n2<<" "<<n3<<endl;

	//n1 = que1.back();
	//n2 = que2.back();
	//n3 = que3.back();
	//cout<<n1<<" "<<n2<<" "<<n3<<endl;


	LinkList list;

	for (int i = 10; i < 30;i++)
	{
		list.insertTail(15);
	}

	list.deleteData(15);
	list.deleteData(29);
	list.show();

	//Node * p = list.createLoop(17,25);  //创建一个环起点为 数据17  
	//if (list.isLoop())
	//{
	//	cout<<"yes"<<endl;
	//}
	//else
	//{
	//	cout<<"no"<<endl;
	//}

	//Node * q = list.getFirstLoopNode();




	return 0;
}