#include <iostream>
#include <string>
using namespace std;

/*
ÔËËã·ûÖØÔØ


*/


#if 0


class CMyString
{
public:
	CMyString(char *ptr = NULL)
	{
		if (ptr == NULL)
		{
			mptr = new char[1];
			*mptr = 0;
		}
		else
		{
			mptr = new char[strlen(ptr)+1];
			strcpy(mptr,ptr);
		}
	}
	CMyString(const CMyString &src)
	{
		mptr = new char[strlen(src.c_str())+1];
		strcpy(mptr,src.c_str());
	}
	CMyString& operator=(const CMyString &src)
	{
		if(this == &src)
			return *this;

		delete []mptr;
		mptr = new char[src.size()+1];
		strcpy(mptr,src.c_str());

		return *this;
	}
	~CMyString()
	{
		delete []mptr;
		mptr = NULL;
	}

	bool operator>(const CMyString &src)
	{
		return strcmp(mptr,src.c_str()) > 0;
	}
	bool operator<(const CMyString &src)
	{
		return strcmp(mptr,src.c_str()) < 0;
	}
	bool operator==(const CMyString &src)
	{
		return strcmp(mptr,src.c_str()) == 0;
	}

	int size()const
	{
		return strlen(mptr);
	}
	char& operator[](int index)   // cout<<str6[i];  str6[i] = 'a';
	{
		if (index > strlen(mptr))
			throw "";

		return *(mptr+index);
	}
	const char* c_str()const
	{
		return mptr;
	}


private:
	char *mptr;

	friend CMyString operator+(const CMyString &lhs, const CMyString &rhs);
	friend istream& operator>>(istream &in, CMyString &src);
	friend ostream& operator<<(ostream &out,const CMyString &src);
};
CMyString operator+(const CMyString &lhs, const CMyString &rhs)
{
	char * ptr = new char[lhs.size()+rhs.size()+1];
	strcat(ptr,lhs.c_str());
	strcat(ptr,rhs.c_str());

	CMyString tmp = ptr;
	delete []ptr;

	return tmp;
}

ostream& operator<<(ostream &out, const CMyString &src)
{
	out<<src.c_str()<<endl;

	return out;
}
istream& operator>>(istream &in, CMyString &src)
{
	char str[1024] = {0};
	in>>str;
	cin.getline(str,1024);
	cin.get();

	src = CMyString(str);

	return in;
}

int main(int argc, char* argv[])
{
	CMyString str1;
	CMyString str2 = "hello world";   // char*   =>   string
	CMyString str3 = str2;
	CMyString str4 = str3 + " ahaha!";
	CMyString str5 = str4 + str3;
	CMyString str6 = "test:" + str5;

	// bool  operator>(const CMyString &src)
	if(str5 > str6)  // <  == 
	{
	}

	int size = str6.size();
	for(int i=0; i<size; ++i)
	{
		//  char  operator[](int index)
		cout<<str6[i];
	}
	cout<<endl;

	// string   =>   char*     operator char*(){}
	char buffer[1024] = {0};
	strcpy(buffer, str6.c_str());
	cout<<buffer<<endl;
	cout<<str6<<endl;
	cin>>str6;
	cout<<str6<<endl;

	return 0;
}

#endif