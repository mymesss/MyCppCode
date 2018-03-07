#include <iostream>
using namespace std;









#if 0
template<typename T>
class CGrid
{
public:
	CGrid<T>(int w = 10,int h = 10);
	~CGrid<T>();
	CGrid(const CGrid<T> &src);//��ͨ���������Ա��������ͬ���ͣ�

	template<typename E>
	CGrid(const CGrid<E> &src) //���������Ա����ģ�壨��ͬ���ͣ�
	{
		mWidth = src.mWidth;
		mHeight = src.mHeight;

		mpCells = new T *[mWidth];
		for(int i = 0;i < mWidth;i++)
		{
			mpCells[i] = new T[mHeight];
			for (int j = 0;j < mHeight;j++)
			{
				mpCells[i][j] = src.mpCells[i][j];
			}
		}
	}
	void operator=(const CGrid<T> &src);//��ͨ��ֵ��Ա����

	template<typename E>
	void operator=(const CGrid<E> &src)//��ֵ��Ա����ģ��
	{
		for (int i = 0;i < mWidth;i++)
		{
			delete []mpCells[i];
		}

		delete []mpCells;
		mpCells = new T *[src.mWidth];

		for (int i = 0;i < src.mWidth;i++)
		{
			mpCells[i] = new T[src.mHeight];
			for (int j = 0;j < src.mHeight;j++)
			{
				mpCells[i][j] = src.mpCells[i][j];
			}
		}

		mWidth = src.mWidth;
		mHeight = src.mHeight;
	}
	int getHeight()const;
	int getWidth()const;
	T getElement(int x,int y)const;
	void setElenent(int x,int y,const T &val);
private:

	T **mpCells;
	int mWidth;
	int mHeight;

	//������ͬ��ʵ���������Ϊ��Ԫ��
	template<typename E>
	friend class CGrid;
};

template<typename T>
CGrid<T>::CGrid(int w,int h)
	:mHeight(h)
	,mWidth(w)
{
	mpCells = new T*[w];
	for (int i = 0;i < w;i++)
	{
		mpCells[i] = new T[h];
		for (int j = 0;j < h;j++)
		{
			mpCells[i][j] = T();
		}
	}
}

template<typename T>
CGrid<T>::~CGrid()
{
	for (int i = 0;i < mWidth;i++)
	{
		delete [] mpCells[i];
	}

	delete []mpCells;
}

//������Ĭ�����ɵĳ�Ա���������죬�������������죬��ֵ��ʡ�Բ�дʱ���ڵ���ʱҲ����ȥ��ģ�����ɶ�Ӧ�ĳ�Ա������ֻ���ñ�����Ĭ�ϵ�
//ֻ�е������ĳ�Ա�����Ĳ����������õĳ�Ա�����Ĳ������Ͳ�ͬʱ������ģ��ȥ���ɶ�Ӧ�ķ���
template<typename T>
CGrid<T>::CGrid(const CGrid<T> &src)
{
	mWidth = src.mWidth;
	mHeight = src.mHeight;

	mpCells = new T *[mWidth];
	for(int i = 0;i < mWidth;i++)
	{
		mpCells[i] = new T[mHeight];
		for (int j = 0;j < mHeight;j++)
		{
			mpCells[i][j] = src.mpCells[i][j];
		}
	}
}



template<typename T>
void CGrid<T>::operator=(const CGrid<T> &src)//ȫ�ͷţ���new
{
	if (this == &src)return;
	for (int i = 0;i < mWidth;i++)
	{
		delete []mpCells[i];
	}
	delete []mpCells;
	mpCells = new T *[src.mWidth];

	for (int i = 0;i < src.mWidth;i++)
	{
		mpCells[i] = new T[src.mHeight];
		for (int j = 0;j < src.mHeight;j++)
		{
			mpCells[i][j] = src.mpCells[i][j];
		}
	}

	mWidth = src.mWidth;
	mHeight = src.mHeight;
}

int main()
{
	CGrid<int> intcg;
	CGrid<long> longcg1;
	CGrid<long> longcg2(intcg);
	longcg1 = intcg;

	return 0;
}


#endif

#if 0
template <typename T>
class Link
{
public:
	Link()
	{
		mphead = new Node();
		mphead->mdata = 0;
		mphead->pnext = NULL;
	}

private:

	class Node
	{
	public:
		Node(T data = T(),T * next = T*())
		{
			mdata = data;
			pnext = next;
		}
		T mdata;
		Node * pnext;
	};

	Node * mphead;
};



template<typename T>
Link<T>::Link()
{
	mphead = new Node();
}

template<typename T>
Link<T>::~Link()
{
	Node * p = mphead;
	while (p != NULL)
	{
		mphead = mphead->pnext;
		delete p;
		p = mphead;
	}
}

template<typename T>
typename Link<T>::Node * Link<T>::getNodeAddr(T val)
{
	Link<T>::Node * p = mphead->pnext;
	while (p != NULL)
	{
		if (p->mdata == val)
		{
			return p;
		}
	}

	return NULL;
}


int main()
{

	return 0;
}


#endif