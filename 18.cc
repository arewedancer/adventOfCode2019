#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <unordered_map>
using namespace std;
#define key_size 26
#define board_size 81
int real_key_size;
int real_board_size;

int getPos(pair<int,int> pos)
{
	return pos.first * board_size + pos.second;
};

vector<vector<char>> board;
std::pair<int, int> start;
unordered_map<char, pair<int,int>> key_map;
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
				{
					maxc = std::max(maxc, c);
					key_map[c] = make_pair(row, col);
				};
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
		real_board_size = board.size();
		real_key_size = maxc - 'a';
	};
};

bool mark[board_size][board_size];
bool keys[key_size];

int h[board_size * board_size];
int getDistance(int i, int j, char c)
{
	return abs(i - key_map[c].first) + abs(j - key_map[c].second);
};

int getDistance(int i, int j)
{
	int dist = 20000;	
	for(auto k = key_map.begin(); k != key_map.end(); ++k)
	{
		dist = min(dist, getDistance(i,j, k->first));
	};
	return dist;
};
void calculateH()
{
	for(int i = 0; i < board_size; ++i )
		for(int j = 0; j < board_size; ++j)
		{
			char c = board[i][j];
			if ( c == '.')
			{
				h[getPos(make_pair(i,j))] = getDistance(i,j);
			}
			else if ( isalpha(c))
			{
				if (islower(c))
					h[getPos(make_pair(i,j))] = 0;
				else
					h[getPos(make_pair(i,j))] = getDistance(i,j, c);
			}
		};
};

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
			for (char door : doors)
			{
				std::cout << door << ",";
			};
			std::cout << c ;
			std::cout << "  step=" << step << " " ;
			doors.clear();
		}
		else
		{
			doors.push_back(c);
		};
	};

	// down
	if (pos.second + 1 < board_size && board[pos.first][pos.second + 1] != '#' && !mark[pos.first][pos.second+1])
		DFS(make_pair(pos.first, pos.second+1), step+1, doors);
	// left
	if (pos.first - 1 >=0 && board[pos.first-1][pos.second] != '#' && !mark[pos.first-1][pos.second])
		DFS(make_pair(pos.first-1, pos.second), step+1, doors);
	// right
	if (pos.first + 1 < board_size && board[pos.first+1][pos.second] != '#' && !mark[pos.first+1][pos.second])
		DFS(make_pair(pos.first+1, pos.second), step+1, doors);
	// up
	if (pos.second - 1 >=0 && board[pos.first][pos.second -1] != '#' && !mark[pos.first][pos.second-1])
		DFS(make_pair(pos.first, pos.second-1), step+1, doors);
	std::cout << std::endl;

};

/*	function reconstruct_path(cameFrom, current)
total_path := {current}
while current in cameFrom.Keys:
current := cameFrom[current]
total_path.prepend(current)
return total_path
*/
// h(A) = m-distance between A and a
// h(a) = 0
// h(.) = smallest m-distance to any keys
// goal = pickup_all_keys


vector<int> openSet;
int gScore[81*81];
int fScore[81*81];
unordered_map<int, int> cameFrom;

#define INFI 20000

void init_score(int* score)
{
	for(int i = 0; i < board_size; ++i)
		for(int j = 0; j < board_size; ++j)
			score[getPos(make_pair(i,j))] =  INFI;
};

int getLowest()
{
	int minScore = 20000;
	int idx;
	for(int i = 0; i < openSet.size(); ++i)
	{
		if ( fScore[openSet[i]] < minScore )
			idx = i;
	};
	return idx;
};

pair<int,int> decode(int pos)
{
	return make_pair(pos/board_size, pos % board_size);
};

void Astar()
{

	// A* finds a path from start to goal.
	// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
	//function A_Star(start, goal, h)
	// The set of discovered nodes that may need to be (re-)expanded.
	// Initially, only the start node is known.
	//openSet := {start}
	openSet.push_back(getPos(start));

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	//cameFrom := an empty map

	// For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
	//gScore := map with default value of Infinity
	//gScore[start] := 0
	init_score(gScore);
	gScore[getPos(start)] = 0;

	// For node n, fScore[n] := gScore[n] + h(n).
	//fScore := map with default value of Infinity
	init_score(fScore);
	//fScore[start] := h(start)
	fScore[getPos(start)] = h[getPos(start)];

	//while openSet is not empty
	while (openSet.size() != 0)
	{
		if (pickup_all_keys() ) break;
		//current := the node in openSet having the lowest fScore[] value
		int current_idx = getLowest();
		int current = openSet[current_idx];

		pair<int,int> pos = decode(current);
		if (isalpha(board[pos.first][pos.second]) && islower(board[pos.first][pos.second]))
		{
			keys[board[pos.first][pos.second]] = 1;
			std::cout << board[pos.first][pos.second] << std::endl;
		};

		//if current = goal
		//    return reconstruct_path(cameFrom, current)

		//openSet.Remove(current)
		openSet.erase(openSet.begin() + current_idx);
		vector<int> neighbors;
		if (pos.second + 1 < board_size && board[pos.first][pos.second + 1] != '#')
			neighbors.push_back(getPos(make_pair(pos.first, pos.second+1)));
		if (pos.first - 1 >=0 && board[pos.first - 1][pos.second] != '#')
			neighbors.push_back(getPos(make_pair(pos.first - 1, pos.second)));
		if (pos.first + 1 < board_size && board[pos.first + 1][pos.second] != '#')
			neighbors.push_back(getPos(make_pair(pos.first + 1, pos.second)));
		if (pos.second - 1 >=0 && board[pos.first][pos.second - 1] != '#')
			neighbors.push_back(getPos(make_pair(pos.first, pos.second-1)));
		//for each neighbor of current
		// d(current,neighbor) is the weight of the edge from current to neighbor
		// tentative_gScore is the distance from start to the neighbor through current
		for (int neighbor : neighbors)
		{
			//tentative_gScore := gScore[current] + d(current, neighbor)
			int tentative_gScore = gScore[current];
			//if tentative_gScore < gScore[neighbor]
			if (tentative_gScore < gScore[neighbor])
			{
				// This path to neighbor is better than any previous one. Record it!
				//cameFrom[neighbor] := current
				//gScore[neighbor] := tentative_gScore
				//fScore[neighbor] := gScore[neighbor] + h(neighbor)
				cameFrom[neighbor] = tentative_gScore;
				fScore[neighbor] = gScore[neighbor] + h[neighbor];
				//if neighbor not in openSet
				auto it = find(openSet.begin(), openSet.end(), neighbor);
				if ( it == openSet.end() )
					openSet.push_back(neighbor);
			};
		};
	};

	// Open set is empty but goal was never reached
	//return failure
};

int main()
{
	scan("data/18.txt", board);
	for(int i =0; i < board_size ; i++ )
		for(int j =0; j < board_size ; j++ )
			mark[i][j] = 0;

	init_keys();
	std::vector<char> doors;
	//DFS(start, 0, doors);
	calculateH();
	Astar();
};
