#include "mazenode.h"
#include "Stack.h"

#include <iostream>
using namespace std;

class Maze
{
public:
	Maze(int row, int col)
	  {
		  _row = row;
		  _col = col;
		  _pMaze = new MazeNode * [row];

		  for (int i = 0;i < row;i++)
		  {
			  _pMaze[i] = new MazeNode[col];
		  }

		  //生成二维数组MazeNode
	  }

	  ~Maze()
	  {
		  for (int i = 0;i < _row;i++)
		  {
			  delete []_pMaze[i];
		  }

		  delete []_pMaze;
	  }

	  void setMazeNode(int row, int col, int data)
	  {
		  _pMaze[row][col].set_node(row,col,data);
	  }

	  void adjustPath()
	  {
		  for (int row = 0;row < _row;row++)
		  {
			  for (int col = 0;col < _col;col++)
			  {
				  if(_pMaze[row][col].get_value() == 0)
				  {
					  if (col < _col-1 && _pMaze[row][col+1].get_value() == 0) //东
					  {
						  _pMaze[row][col].set_state(WAY_EAST,WAY_ENABLE);
					  }

					  if (col > 0 && _pMaze[row][col-1].get_value() == 0) //西
					  {
						  _pMaze[row][col].set_state(WAY_WEST,WAY_ENABLE);
					  }

					  if (row < _row-1 && _pMaze[row+1][col].get_value() == 0) //南
					  {
						  _pMaze[row][col].set_state(WAY_SOURTH,WAY_ENABLE);
					  }

					  if (row > 0 && _pMaze[row - 1][col].get_value() == 0) //北
					  {
						  _pMaze[row][col].set_state(WAY_NORTH,WAY_ENABLE);
					  }
				  }
			  }
		  }
	  }


	  int GetWay(int row,int col)
	  {
		  for (int i = 0; i < WAY_NUMBER;i++)
		  {
			  if (_pMaze[row][col].get_state(i) == WAY_ENABLE)
			  {
				  return i;
			  }
		  }

		  return -1;
	  }

	  void findMazePath()
	  {
		  int row = 0;
		  int col = 0;
		  while (row != (_row-1) || col != (_col-1))
		  {
			  int way = GetWay(row,col);
			  if (way == -1)
			  {
				  if (row == 0 && col == 0)
				  {
					  return;
				  }

				  _stack.Pop(&row,&col);
				  continue;
			  }

			  if (way == WAY_EAST)
			  {
				  _stack.Push(row,col);
				  _pMaze[row][col].set_state(way,WAY_DISABLE);
				  _pMaze[row][col+1].set_state(WAY_WEST,WAY_DISABLE);
				  col = col+1;
			  }
			  else if (way == WAY_SOURTH)
			  {
				  _stack.Push(row,col);
				  _pMaze[row][col].set_state(way,WAY_DISABLE);
				  _pMaze[row+1][col].set_state(WAY_NORTH,WAY_DISABLE);
				  row = row+1;
			  }
			  else if (way == WAY_WEST)
			  {
				  _stack.Push(row,col);
				  _pMaze[row][col].set_state(way,WAY_DISABLE);
				  _pMaze[row][col-1].set_state(WAY_EAST,WAY_DISABLE);
				  col = col-1;
			  }
			  else if (way == WAY_NORTH)
			  {
				  _stack.Push(row,col);
				  _pMaze[row][col].set_state(way,WAY_DISABLE);
				  _pMaze[row-1][col].set_state(WAY_SOURTH,WAY_DISABLE);
				  row = row - 1;
			  }
		  }
		  _stack.Push(row,col);
	  }

	  void showMazePath()
	  {
		  int row = 0;
		  int col = 0;
		  _stack.Pop(&row,&col);
		  if (row != 4 && col != 4)
		  {
			  cout<<"没有正确路线！"<<endl;
			  return;
		  }

		 
		  while (row != -1 && col != -1)
		  {
			  _pMaze[row][col].set_value(2);
			  _stack.Pop(&row,&col); 
		  }

		  cout<<endl;
		  cout<<"正确路线如下："<<endl;
		  for (int i = 0;i < _row;i++)
		  {
			  for (int j = 0;j < _col;j++)
			  {
				  cout<<_pMaze[i][j].get_value()<<" ";
			  }
			  cout<<endl;
		  }
	  }

	  void set_state(int row,int col,int way,int way_num)
	  {
		  _pMaze[row][col].set_state(way,way_num);
	  }

private:
	Maze(const Maze&src);
	void operator=(const Maze &src);

	MazeNode **_pMaze;
	int _row;
	int _col;
	Stack _stack;
};


int main()
{
	int row,col,data;

	cout<<"请输入迷宫行列数："<<endl;
	cin>>row>>col;

	cout<<"请输入迷宫路径："<<endl;

	Maze maze(row,col);

	for (int i = 0;i < row;i++)
	{
		for (int j = 0;j < col;j++)
		{
			cin>>data;
			maze.setMazeNode(i,j,data);
		}
	}

	maze.adjustPath();
	maze.findMazePath();
	maze.showMazePath();

	return 0;
}