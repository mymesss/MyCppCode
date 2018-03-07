#include <iostream>
#include <string>
using namespace std;



template<typename T>
class vector
{
public:
	vector(int size = 10)
		:_size(size)
		,_cur(0)
		,_top(size-1)
	{
		_pvec = new T[size];
		for (int i = 0;i < size;++i)
		{
			_pvec[i] = T();
		}
	}

	vector(const vector<T> & src)
	{
		_pvec = new T[src._size];
		_top = src._size - 1;
		_cur = 0;
		_size = src._size;
		for (int i = src._size-1;i > src._top;--i)
		{
			push_back(src._pvec[i]);
		}
	}

	vector<T>& operator=(const vector<T> &src)
	{
		if (this == &src)
			return *this;

		delete []_pvec;
		_pvec = new T[src._size];
		_top = src._size - 1;
		_cur = 0;
		_size = src._size;
		for (int i = src._size-1;i > src._top;--i)
		{
			push_back(src._pvec[i]);
		}

		return *this;
	}

	~vector()
	{
		delete []_pvec;
		_pvec = NULL;
	}

	void push_back(const T &val)
	{
		if (_cur == _size)
		{
			reSize();
		}
		_pvec[_top--] = val;
		++_cur;
	}

	void pop_back()
	{
		if(_cur == 0)
			return;

		++_top;
		--_cur;
	}

	void reSize()
	{
		T * oldPvec = _pvec;
		int oldCur = _cur;
		int oldSize = _size;
		int oldTop = _top;  //保留原容器数据

		_size *= 2;    //扩容
		_pvec = new T[_size];
		_top = _size-1;
		_cur = 0;  //创建新容器

		for (int i = oldSize-1; i > oldTop;--i)  //循环读取原数据
		{
			push_back(oldPvec[i]);
		}

		delete []oldPvec;  //释放原容器空间
	}



	class iterator
	{
	public:
		iterator(T * pv)
			:_pv(pv){}

		bool operator!=(const iterator& src)
		{
			return _pv != src._pv;
		}

		void operator++()
		{
			--_pv;
		}

		T& operator*()
		{
			return *_pv;
		}
	private:
		T * _pv;
	};

	iterator begin()
	{
		return iterator(_pvec+_size-1);
	}

	iterator end()
	{
		return iterator(_pvec+_top);
	}

private:
	T * _pvec;
	int _size;
	int _cur;
	int _top;
};


//int main()
//{
//
//	vector<int> intVec;
//	for (int i = 0;i < 10;++i)
//	{
//		intVec.push_back(rand()%100);
//	}
//
//	vector<int>::iterator it = intVec.begin();
//	for (;it != intVec.end();++it)
//	{
//		cout<<*it<<endl;
//	}
//	vector<char> charVec;
//	return 0;
//}





class CMyString
{
public:
	CMyString(char *ptr = NULL)
	{
		//cout<<this<<" CMyString()"<<endl;
		if(ptr == NULL)
		{
			mptr = new char[1];
			*mptr = 0;
		}
		else
		{
			mptr = new char[strlen(ptr) + 1];
			strcpy(mptr, ptr);
		}
	}
	CMyString(const CMyString &src)
	{
		//cout<<this<<" CMyString(const CMyString &src)"<<endl;
		mptr = new char[strlen(src.mptr) + 1];
		strcpy(mptr, src.mptr);
	}
	CMyString& operator=(const CMyString &src)
	{
		if(this == &src)
			return *this;

		delete []mptr;

		mptr = new char[strlen(src.mptr) + 1];
		strcpy(mptr, src.mptr);
		return *this;
	}
	~CMyString()
	{
		//cout<<this<<" ~CMyString()"<<endl;
		delete []mptr;
		mptr = NULL;
	}

	bool operator>(const CMyString &src)
	{
		return strcmp(mptr, src.mptr) > 0;
	}
	bool operator<(const CMyString &src)
	{
		return strcmp(mptr, src.mptr) < 0;
	}
	bool operator==(const CMyString &src)
	{
		return strcmp(mptr, src.mptr) == 0;
	}

	int size()const{return strlen(mptr);}
	char& operator[](int index)  // cout<<str6[i];  str6[i] = 'a';
	{
		return mptr[index];
	}
	const char* c_str()const{return mptr;}

private:
	char *mptr;

	friend CMyString operator+(const CMyString &lhs, const CMyString &rhs);
	friend istream& operator>>(istream &in, CMyString &src);
};
CMyString operator+(const CMyString &lhs, const CMyString &rhs)
{
	int size = lhs.size() + rhs.size() + 1;
	char *ptmp = new char[size];
	strcpy(ptmp, lhs.mptr);
	strcat(ptmp, rhs.mptr);

	CMyString tmp(ptmp);
	delete []ptmp;

	return tmp;
}
ostream& operator<<(ostream &out, const CMyString &src)
{
	out<<src.c_str();
	return out;
}
istream& operator>>(istream &in, CMyString &src)
{
	char buffer[1024] = {0};
	//cin >> buffer;  // scanf       gets          getchar
	cin.getline(buffer, 1024);  // cin.gets()    cin.get()
	src = buffer;
	return in;
}

int main(int argc, char* argv[])
{


	vector<CMyString> myStrVec1;
	myStrVec1.push_back("hello world");
	myStrVec1.push_back("nihao1");
	myStrVec1.push_back("hhhh");
	myStrVec1.push_back("lol");
	myStrVec1.push_back("pop");
	myStrVec1.push_back("ressss");
	myStrVec1.push_back("nihao3");
	myStrVec1.push_back("nihao2");
	myStrVec1.push_back("nihao4");
	myStrVec1.push_back("nihao5");
	myStrVec1.push_back("nihao6");  //11个
	myStrVec1.pop_back();

	vector<CMyString> myStrVec2 = myStrVec1;
	vector<CMyString> myStrVec3;
	myStrVec3 = myStrVec2;

	vector<CMyString>::iterator it = myStrVec1.begin();
	for (;it != myStrVec1.end();++it)
	{
		cout<<*it<<endl;
	}

	it = myStrVec2.begin();
	for (;it != myStrVec2.end();++it)
	{
		cout<<*it<<endl;
	}

	it = myStrVec3.begin();
	for (;it != myStrVec3.end();++it)
	{
		cout<<*it<<endl;
	}



	return 0;
}
