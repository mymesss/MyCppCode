#include <iostream>
#include <string>
//#include <vector>
using namespace std;


#if 0

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

	vector<T> operator=(const vector<T> &src)
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

		delete []oldPvec;  //释放原容器数据
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



	//迭代器一般都被设计成容器类的嵌套类
	/*class iterator
	{
	public:
		iterator(char * ptr = NULL)
			:_ptr(ptr){}
		bool operator!=(const iterator& src)
		{
			return this->_ptr != src._ptr;
		}

		void operator++()
		{
			++_ptr;
		}

		char& operator*()
		{
			return *_ptr;
		}

	private:
		char * _ptr;
	};

	iterator begin()
	{
		return iterator(mptr);
	}

	iterator end()
	{
		return iterator(mptr+strlen(mptr));
	}*/

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
	myStrVec1.push_back("nihao6");

	vector<CMyString>::iterator it = myStrVec1.begin();
	for (;it != myStrVec1.end();++it)
	{
		cout<<*it<<endl;
	}


	//CMyString str1;
	//CMyString str2 = "hello world";   // char*   =>   string
	//CMyString str3 = str2;
	//CMyString str4 = str3 + " ahaha!";
	//CMyString str5 = str4 + str3;
	//CMyString str6 = "test:" + str5;

	//// bool  operator>(const CMyString &src)
	//if(str5 > str6)  // <  == 
	//{
	//}

	//int size = str6.size();
	//for(int i=0; i<size; ++i)
	//{
	//	//  char  operator[](int index)
	//	cout<<str6[i];
	//}
	//cout<<endl;

	//// string   =>   char*     operator char*(){}
	//char buffer[1024] = {0};
	//strcpy(buffer, str6.c_str());
	//cout<<buffer<<endl;
	//cout<<str6<<endl;
	//cin>>str6;
	//cout<<str6<<endl;

	//CMyString str10 = "hello world";
	//int addr1 = (int)str10.c_str();
	//cout<<hex<<addr1<<endl;

	//CMyString str11 = str10;
	//addr1 = (int)str11.c_str();
	//cout<<hex<<addr1<<endl;

	////begin()返回容器首元素的迭代器
	////end()返回末尾元素后继位置的迭代器
	//CMyString::iterator it = str11.begin();
	//for(; it != str11.end(); ++it)
	//{
	//	cout<<*it<<" ";
	//}
	//cout<<endl;

	/*

	int size;   字符串的长度    12
	int ressize;   还有多少预留空间   1
	"hello orld";    13

	char buffer[]="hello world";   容器
	buffer[index];

	char *p = buffer;    

	指针   =》   数组
	迭代器	=》	  容器
	*/  


	//还有没有其他方式，在我们不知道对象底层数据结构的时候，
	//能够用统一的方式对对象的数据进行迭代访问
	//

	//
	//CMyString str1 = "hello";
	//CMyString::iterator it = str1.begin();
	//for (; it != str1.end();++it)
	//{
	//	cout<<*it<<" ";
	//}

	//cout<<endl;



	return 0;
}

#endif
