#include <iostream>
#include <string.h>
using namespace std;

class Person
{
public:

	Person(int id = 0, const char * name = "\0") :_id(id)
	{
		strcpy(_name, name);
	}
	int getId()const{return _id;}

	void show()
	{
		cout<<"Id:"<<_id<<"    "<<"Name:"<<_name<<endl;
	}
private:
	int _id;
	char _name[20];
};

////////////////////////////////////////////////////
template<typename T>
class CHashFunc
{
public:
	CHashFunc(int size)
		:_bucketSize(size){}
	int getHash(const T &val)
	{
		return val%_bucketSize;
	}
	void setHash(int size)
	{
		_bucketSize = size;
	}
private:
	int _bucketSize;
};


template<typename K, typename V>
class CHashTable
{
public:
	CHashTable(int size = 3, double threshold = 0.75)
		:_size(size)
		,_threshold(threshold)
		,_use_size(0)
		,_hashFunc(size)
	{
		pHashTable = new Node[size];
	}

	bool isFull()const
	{
		int useSize = 0;
		for (int i = 0;i < _size;i++)
		{
			Node * pcur = pHashTable[i]._pnext;
			if (pcur != NULL)
			{
				useSize++;
			}
		}

		_use_size = useSize;
		double load = _use_size * 1.0 / _size;
		cout<<"当前哈希表的长度:"<<_size<<" 负载因子是:"<<load<<endl;
		return load >= _threshold;
	}

	void put(const K &key, const V &val)
	{
		/*if (isFull())
		{
			reSize();
		}*/

		cout<<"Put id:"<<key<<endl;
		int hashIndex = _hashFunc.getHash(key);
		Node *pcur = pHashTable[hashIndex]._pnext;
		//不允许key重复，如果key重复，则更新它的value值
		while (pcur != NULL)
		{
			if (pcur->_key == key)
			{
				pcur->_value = val;
				return;
			}
			pcur = pcur->_pnext;
		}

		Node * qcur = new Node(key,val);
		qcur->_pnext = pHashTable[hashIndex]._pnext;
		pHashTable[hashIndex]._pnext = qcur;

		if (isFull())
		{
			reSize();
		}

	}

	//删除键为key的Node元素
	void remove(const K &key)
	{
		int hashIndex = _hashFunc.getHash(key);
		Node *pcur = pHashTable[hashIndex]._pnext;
		Node *qcur = pHashTable+hashIndex;

		while(pcur != NULL)
		{
			if (key == pcur->_key)
			{
				qcur->_pnext = pcur->_pnext;
				delete pcur;
				cout<<"Remove id:"<<key<<endl;
				return;
			}
			qcur = pcur;
			pcur = pcur->_pnext;
		}	
	}

	//获取键为key的元素
	V get(const K &key)
	{
		int hashIndex = _hashFunc.getHash(key);
		Node * pcur = pHashTable[hashIndex]._pnext;

		while (pcur != NULL)
		{
			if (key == pcur->_key)
			{
				return pcur->_value;
			}
			pcur = pcur->_pnext;
		}
	}
private:
	struct Node
	{
		Node(K key = K(), V val = V())
			:_key(key), _value(val),_pnext(NULL){}
		K _key;
		V _value;
		Node *_pnext;
	};

	Node *pHashTable;
	double _threshold;
	int _size;
	mutable int _use_size;
	CHashFunc<K> _hashFunc;

	static size_t hash_size(size_t num_elems)
	{
		static size_t sizes[] =
		{
			3, 13, 23, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
			49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
			12582917, 25165843
		};
		const size_t nth_size = sizeof(sizes)/sizeof(size_t) - 1;
		for(size_t i = 0; i < nth_size; ++i)
			if (num_elems < sizes[i])
				return sizes[i];
		return sizes[nth_size];
	}

	//哈希表的增长，原来的元素全部要冲洗hash的
	void reSize()
	{
		int oldsSize = _size;
		Node * oldHashTable = pHashTable;

		_size = hash_size(_size);
		pHashTable = new Node[_size];
		_hashFunc.setHash(_size);

		for (int i = 0;i < oldsSize;i++)
		{
			Node * pcur = oldHashTable[i]._pnext;
			while(pcur != NULL)
			{
				put(pcur->_key,pcur->_value);
				oldHashTable[i]._pnext = pcur->_pnext;
				delete pcur;
				pcur = oldHashTable[i]._pnext;
			}
		}
		delete [] oldHashTable;
	}
};


int main(int argc, char* argv[])
{
	CHashTable<int, Person> hashTable;
	hashTable.put(1001, Person(1001, "zhang san"));
	hashTable.put(1004, Person(1004, "gao yang"));
	hashTable.put(1006, Person(1006, "liu shuo"));
	hashTable.put(1006, Person(1006, "liu"));
	hashTable.put(1002, Person(1002, "zhang san"));
	hashTable.put(1005, Person(1005, "gao yang"));
	hashTable.put(1007, Person(1007, "liu shuo"));
	hashTable.remove(1001);
	Person per1(hashTable.get(1005));
	per1.show();

	return 0;
}
