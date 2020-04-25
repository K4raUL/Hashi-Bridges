#include <Windows.h>
#include <tchar.h>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

int marginLeft;
int marginTop;
int cell;

// selected bridge params
int _i1_, _j1_;
int _i2_, _j2_;
bool isBridgeSelected;

HWND hwndGenButton;
HWND hwndClrButton;
HWND hwndSolButton;
HWND hwndEditW;
HWND hwndEditH;

RECT		 rField;
RECT		 rTimer;
int idTimer;
int min;
int sec;

bool showSolution;
bool gameOver;

WNDCLASSEX wcex;
static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Hashi bridges");
int **f;													// field array; 
															// 0			- empty tiles;
															// 1:8			- island tiles
															// -1, -2		- horizontal single/double bridges
															// -10, -20		- vertical single/double bridges

int **player;												// player field array
int f_generated = 0;

LRESULT CALLBACK WndProc(_In_ HWND   hWnd, _In_ UINT   message,	_In_ WPARAM wParam,	_In_ LPARAM lParam);
bool calcTrack(int M, int N, int **f, int **p, int cell, int mLeft, int mTop, int cursorX, int cursorY, int* i1, int* j1, int* i2, int* j2);
bool isPointinRect(int x, int y, int left, int top, int W, int H);
double distanceLinePoint(int x0, int y0, int x1, int y1, int x2, int y2, int cell);
bool CheckWin();

#endif