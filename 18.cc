#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <cctype>
#include <cmath>
using namespace std;
#define key_size 26

vector<vector<char>> board;
std::pair<int, int> start;
void scan(string file_name, vector<vector<char>>& board)
{
	string line;
	ifstream myfile(file_name);
	int row = 0;
	char maxc = 0;
	if (myfile.is_open())
	{
		while ( getline(myfile,line) )
		{
			int col = 0;
			vector<char> tmp;
			for(char c : line)
			{
				if ( isalpha(c) && islower(c) )
					maxc = std::max(maxc, c);
				if ( c == '@' )
				{
					start = make_pair(row, col);
				};
				tmp.push_back(c);
				col++;
			};
			board.push_back(tmp);
			row++;
		}
		myfile.close();
	}
};

bool mark[81][81];
bool keys[26];

void init_keys()
{
	for(int i = 0; i < key_size; ++i)
	{
		keys[i] = false;
	};
};

bool pickup_all_keys()
{
	for(int i = 0; i < key_size; ++i)
	{
		if (!keys[i]) return false;
	};
	return true;
}
void DFS(pair<int,int> pos, int step, vector<char> doors)
{
  mark[pos.first][pos.second] = 1;
	if (pickup_all_keys()) return;
	char c = board[pos.first][pos.second];
	if (isalpha(c))
	{
		if ( islower(c) )
		{
			keys[c-97] = true;
			//print out all doors (dependencies)
			std::cout << "--- ";
			for (char door : doors)
			{
				std::cout << door << ",";
			};
			std::cout << "-->" << c ;
			std::cout << "  step=" << step;
			doors.clear();
		}
		else
		{
			doors.push_back(c);
		};
	};
	step++;
	
	vector<pair<int,int>> dirs;
	// left
	if (pos.first - 1 >=0 && board[pos.first-1][pos.second] != '#' && !mark[pos.first-1][pos.second])
		DFS(make_pair(pos.first-1, pos.second), step, doors);
	if (pos.first + 1 < 81 && board[pos.first+1][pos.second] != '#' && !mark[pos.first+1][pos.second])
		DFS(make_pair(pos.first+1, pos.second), step, doors);
	if (pos.second - 1 >=0 && board[pos.first][pos.second -1] != '#' && !mark[pos.first][pos.second-1])
		DFS(make_pair(pos.first, pos.second-1), step, doors);
	if (pos.second + 1 < 81 && board[pos.first][pos.second + 1] != '#' && !mark[pos.first][pos.second+1])
		DFS(make_pair(pos.first, pos.second+1), step, doors);
	std::cout << std::endl;

};

int main()
{
	scan("data/18.txt", board);
	for(int i =0; i < 81 ; i++ )
	for(int j =0; j < 81 ; j++ )
		mark[i][j] = 0;

	init_keys();
	std::vector<char> doors;
	DFS(start, 0, doors);
	cout << "debug";
};
