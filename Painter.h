#ifndef PAINTER_H
#define PAINTER_H

#include <Windows.h>
#include <tchar.h>

extern HPEN gridpen;
extern HPEN bridgepen;
extern HPEN HLpen;

void calcTransform(int M, int N, int nWidth, int nHeight, int *cell, int *mLeft, int *mTop);
void DrawField(int M, int N, int** f, int **player, HDC hdc, HDC hdc0, int nWidth, int nHeight, int cell, int marginLeft, int marginTop, bool isBS, bool showSol, int i1, int j1, int i2, int j2);
void DrawGrid(int M, int N, HDC hdc, int cell, int marginTop, int marginLeft);
void bridgeHL(HDC hdc, int **p, int cell, int marginLeft, int marginTop, int i1, int j1, int i2, int j2);
void DrawIslands(int M, int N, int** f, int** p, HDC hdc, int marginLeft, int marginTop, int cell);
void DrawBridges(int M, int N, int** f, HDC hdc, int marginLeft, int marginTop, int cell);

void DrawBridgeManual();

#endif
