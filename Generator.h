#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

extern int curNodeX, curNodeY;
extern int M, N;

std::vector<int*> getNeighbours(int x, int y, int** f);
bool isNotOutOfMap(int x, int y);
std::vector<std::vector<int>> getNearestCells(int x, int y);
int** Generator(int M, int N, int** ff);
void recNode(int x, int y, int** f, int depth);
bool noAdjacentIslands(int x, int y, int** f);

#endif