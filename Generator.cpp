#include <Windows.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <iterator>
#include "Generator.h"

using namespace std;

int curNodeX, curNodeY;
int M = 10; 
int N = 10;

// ================================================ Generation params ================================================

int maxDepth		= 0/*(M + N)/5.*/;				// 0 to unlimited depth
bool mergingEnabled = true;							// enable/disable new bridges to connect with existing nodes
double branching	= 0.7;							// from 0 to 1, lesser values = nodes have less branches

// ===================================================================================================================


// return all neighbours of given island
std::vector<int*> getNeighbours(int x, int y, int** f)
{
	std::vector<int*> res;
	int *cell = new int[2];

	for (int i = x + 1; i < M; i++) {
		if (f[i][y] >= 1 && f[i][y] <= 8) {
			cell[0] = i;
			cell[1] = y;
			res.push_back(cell);
			break;
		}
	}

	for (int i = x - 1; i >= 0; i--) {
		if (f[i][y] >= 1 && f[i][y] <= 8) {
			cell[0] = i;
			cell[1] = y;
			res.push_back(cell);
			break;
		}
	}

	for (int j = y + 1; j < N; j++) {
		if (f[x][j] >= 1 && f[x][j] <= 8) {
			cell[0] = x;
			cell[1] = j;
			res.push_back(cell);
			break;
		}
	}

	for (int j = y - 1; j >= 0; j--) {
		if (f[x][j] >= 1 && f[x][j] <= 8) {
			cell[0] = x;
			cell[1] = j;
			res.push_back(cell);
			break;
		}
	}

	return res;
}

// check if given position is out of map
bool isNotOutOfMap(int x, int y)
{
	return x < M && y < N && x >= 0 && y >= 0;
}

// return all adjacent cells of given position
vector<vector<int>> getNearestCells(int x, int y)
{
	vector<vector<int>> res;
	res.clear();

	if (isNotOutOfMap(x + 1, y)) res.push_back({ x + 1, y });

	if (isNotOutOfMap(x - 1, y)) res.push_back({ x - 1, y });

	if (isNotOutOfMap(x, y + 1)) res.push_back({ x, y + 1 });

	if (isNotOutOfMap(x, y - 1)) res.push_back({ x, y - 1 });

	random_device rd;
	mt19937 g(rd());
	shuffle(res.begin(), res.end(), g);

	return res;
}

// recursive function to generate field
void recNode(int x, int y, int** f, int depth)
{
	vector<vector<int>>steps = getNearestCells(x, y);

	depth++;
	if (depth == maxDepth && depth != 0) return;

	// parse all 4 directions 
	for (int i = 0; i < steps.size(); i++) {
		int shiftX = (steps[i][0] - x);
		int shiftY = (steps[i][1] - y);

		int curCellX = x + shiftX;
		int curCellY = y + shiftY;
		if (f[curCellX][curCellY] != 0) continue;

		// branching with <=1 prob
		if ( ((double)rand() / RAND_MAX) > branching )	continue;

		// choose single or double bridge
		int bridge = rand() % 2 + 1;

		// check direction and filling list of all possible positions to create new node
		vector<vector<int>> newNodeList;
		newNodeList.clear();
		int kkk = 0;
		for (int k = curCellX + shiftX; k < M && k >= 0; k += (shiftX == 0 ? M : shiftX)) {
			for (int j = curCellY + shiftY; j < N && j >= 0; j += (shiftY == 0 ? N : shiftY)) {
				if (f[k][j] < 0) {											// stop on another bridge
					kkk = 1;
					break;
				}
				if (f[k][j] > 0) {											// stop on another node 
					if (mergingEnabled) newNodeList.push_back({ k, j });
					kkk = 1;
					break;
				}
				if (noAdjacentIslands(k, j, f)) newNodeList.push_back({ k, j });
			}
			if (kkk) break;
		}

		int nodes = newNodeList.size();
		if (nodes == 0) continue;
		
		// NEW NODE
		int newNode = rand() % nodes;
		curCellX = newNodeList[newNode][0];
		curCellY = newNodeList[newNode][1];

		// draw bridges between nodes
		if (shiftX) for (int k = min(x, curCellX) + abs(shiftX); k < max(x, curCellX); k += abs(shiftX)) f[k][y] = -10 * bridge;
		if (shiftY) for (int j = min(y, curCellY) + abs(shiftY); j < max(y, curCellY); j += abs(shiftY)) f[x][j] = -bridge;

		// increase node values
		f[x][y] += bridge;
		f[curCellX][curCellY] += bridge;
		if (f[curCellX][curCellY] > bridge) continue;
		
		recNode(curCellX, curCellY, f, depth);
	}
}

bool noAdjacentIslands(int x, int y, int** f)
{
	vector<vector<int>>check = getNearestCells(x, y);
	for (int j = 0; j < check.size(); j++) if (f[check[j][0]][check[j][1]] > 0) return false;
	return true;
}

int** Generator(int m, int n, int** ff)
{
	M = m;
	N = n;

	srand(time(NULL));

	curNodeX = rand() % M;
	curNodeY = rand() % N;

	//recNode(0, 1, ff, 0);
	recNode(curNodeX, curNodeY, ff, 0);

	return ff;
}