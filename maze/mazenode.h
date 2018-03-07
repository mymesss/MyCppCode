#include <iostream>
#include "Stack.h"
using namespace std;

#pragma once
const int WAY_NUMBER = 4;

const int WAY_EAST = 0;
const int WAY_SOURTH = 1;
const int WAY_WEST = 2;
const int WAY_NORTH = 3;

const int WAY_ENABLE = 0;  // 路径可以行走
const int WAY_DISABLE = 1; // 路径不能行走


struct MazeNode
{
public:

	MazeNode()
	{
		for(int i=0; i<WAY_NUMBER; ++i)
		{
			_state[i] = WAY_DISABLE;
		}
	}

	int get_value()
	{
		return _value;
	}

	void set_value(int data)
	{
		_value = data;
	}

	void set_row(int row)
	{
		_row = row;
	}

	void set_col(int col)
	{
		_col = col;
	}

	int get_row()
	{
		return _row;
	}

	int get_col()
	{
		return _col;
	}

	void set_state(int num,int way_value)
	{
		_state[num] = way_value;
	}

	int get_state(int way)
	{
		return _state[way];
	}

	void set_node(int row,int col,int data)
	{
		_row = row;
		_col = col;
		_value = data;
	}

private:
	int _value;
	int _row;
	int _col;
	int _state[WAY_NUMBER];
};
