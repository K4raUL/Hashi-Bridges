#include "Painter.h"

HPEN gridpen;
HPEN bridgepen;
HPEN HLpen;

void calcTransform(int M, int N, int nWidth, int nHeight, int *cell, int *mLeft, int *mTop)
{
	if (M >= N) {
		*mTop = 85;
		*cell = (nHeight - 2 * (*mTop)) / (M - 1);
		*mLeft = (nWidth - (*cell)*(N - 1)) / 2;
	}
	else {
		*mLeft = 50;
		*cell = (nWidth - 2 * (*mLeft)) / (N - 1);
		*mTop = (nHeight - (*cell)*(M - 1)) / 2;
	}
}

void DrawField(int M, int N, int** f, int **player, HDC hdc, HDC hdc0, int nWidth, int nHeight, int cell, int marginLeft, int marginTop, bool isBS, bool showSol, int i1, int j1, int i2, int j2)
{

	gridpen = CreatePen(PS_DOT, 1, RGB(153, 153, 153));
	bridgepen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HLpen = CreatePen(PS_DOT, 2, RGB(0, 153, 0));

	DrawGrid(M, N, hdc, cell, marginTop, marginLeft);

	if (showSol) DrawBridges(M, N, f, hdc, marginLeft, marginTop, cell);
	else DrawBridges(M, N, player, hdc, marginLeft, marginTop, cell);
	if (isBS) bridgeHL(hdc, player, cell, marginLeft, marginTop, i1, j1, i2, j2);

	DrawIslands(M, N, f, player, hdc, marginLeft, marginTop, cell);

	DeleteObject(gridpen);
	DeleteObject(bridgepen);
	DeleteObject(HLpen);
}

void DrawGrid(int M, int N, HDC hdc, int cell, int marginTop, int marginLeft)
{
	LPPOINT lppt = NULL;
	SelectObject(hdc, gridpen);

	for (int i = 0; i < N; i++) {
		MoveToEx(hdc, marginLeft + i*cell, marginTop, lppt);
		LineTo(hdc, marginLeft + i*cell, marginTop + (M - 1)*cell);
	}
	for (int i = 0; i < M; i++) {
		MoveToEx(hdc, marginLeft, marginTop + i*cell, lppt);
		LineTo(hdc, marginLeft + (N - 1)*cell, marginTop + i*cell);
	}
}

void DrawBridges(int M, int N, int** f, HDC hdc, int marginLeft, int marginTop, int cell)
{
	LPPOINT lppt = NULL;

	SelectObject(hdc, bridgepen);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (f[j][i] < 0) {
				if (f[j][i] == -1) {
					MoveToEx(hdc, marginLeft + i*cell, marginTop + j*cell, lppt);
					if (i + 1 < N)	LineTo(hdc, marginLeft + (i + 1)*cell, marginTop + j*cell);
					if (i - 1 >= 0)	LineTo(hdc, marginLeft + (i - 1)*cell, marginTop + j*cell);
				}
				else if (f[j][i] == -10) {
					MoveToEx(hdc, marginLeft + i*cell, marginTop + j*cell, lppt);
					if (j + 1 < M)	LineTo(hdc, marginLeft + i*cell, marginTop + (j + 1)*cell);
					if (j - 1 >= 0)	LineTo(hdc, marginLeft + i*cell, marginTop + (j - 1)*cell);
				}
				else if (f[j][i] == -2) {
					MoveToEx(hdc, marginLeft + i*cell, marginTop + j*cell - cell / 20., lppt);
					if (i + 1 < N)	LineTo(hdc, marginLeft + (i + 1)*cell, marginTop + j*cell - cell / 20.);
					if (i - 1 >= 0)	LineTo(hdc, marginLeft + (i - 1)*cell, marginTop + j*cell - cell / 20.);

					MoveToEx(hdc, marginLeft + i*cell, marginTop + j*cell + cell / 20., lppt);
					if (i + 1 < N)	LineTo(hdc, marginLeft + (i + 1)*cell, marginTop + j*cell + cell / 20.);
					if (i - 1 >= 0)	LineTo(hdc, marginLeft + (i - 1)*cell, marginTop + j*cell + cell / 20.);
				}
				else if (f[j][i] == -20) {
					MoveToEx(hdc, marginLeft + i*cell - cell / 20., marginTop + j*cell, lppt);
					if (j + 1 < M)	LineTo(hdc, marginLeft + i*cell - cell / 20., marginTop + (j + 1)*cell);
					if (j - 1 >= 0)	LineTo(hdc, marginLeft + i*cell - cell / 20., marginTop + (j - 1)*cell);

					MoveToEx(hdc, marginLeft + i*cell + cell / 20., marginTop + j*cell, lppt);
					if (j + 1 < M)	LineTo(hdc, marginLeft + i*cell + cell / 20., marginTop + (j + 1)*cell);
					if (j - 1 >= 0)	LineTo(hdc, marginLeft + i*cell + cell / 20., marginTop + (j - 1)*cell);
				}
			}
		}
	}
}

void bridgeHL(HDC hdc, int **p, int cell, int marginLeft, int marginTop, int i1, int j1, int i2, int j2)
{
	int startX = j1*cell + marginLeft;
	int startY = i1*cell + marginTop;
	int finishX = j2*cell + marginLeft;
	int finishY = i2*cell + marginTop;
	
	LPPOINT lppt = NULL;
	int shift = cell / 20.;

	SelectObject(hdc, HLpen);

	if (j1 == j2) {
		if (p[i2 + 1][j1] == -20) {
			MoveToEx(hdc, startX-shift, startY, lppt);
			LineTo(hdc, finishX-shift, finishY);
			MoveToEx(hdc, startX+shift, startY, lppt);
			LineTo(hdc, finishX+shift, finishY);

			return;
		}
	}
	if (i1 == i2) {
		if (p[i1][j2 + 1] == -2) {
			MoveToEx(hdc, startX, startY - shift, lppt);
			LineTo(hdc, finishX, finishY - shift);
			MoveToEx(hdc, startX, startY + shift, lppt);
			LineTo(hdc, finishX, finishY + shift);

			return;
		}
	}
	
	MoveToEx(hdc, startX, startY, lppt);
	LineTo(hdc, finishX, finishY);
}

void DrawIslands(int M, int N, int** f, int** p, HDC hdc, int marginLeft, int marginTop, int cell)
{
	char buf[3];
	HFONT hFont = CreateFont(cell / 3., cell / 7., 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
	HBRUSH hbrushWin = CreateSolidBrush(RGB(80, 255, 80));
	HBRUSH hbrushFail = CreateSolidBrush(RGB(255, 80, 80));
	HBRUSH hbrushDef = CreateSolidBrush(RGB(255, 255, 255));

	SelectObject(hdc, hFont);
	SelectObject(hdc, bridgepen);

	SetBkMode(hdc, TRANSPARENT);
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (f[j][i] > 0) {
				if (f[j][i] == p[j][i]) SelectObject(hdc, hbrushWin);
				else if (f[j][i] < p[j][i]) SelectObject(hdc, hbrushFail);
				else SelectObject(hdc, hbrushDef);

				Ellipse(hdc, marginLeft + i*cell - cell / 4., marginTop + j*cell - cell / 4., marginLeft + i*cell + cell / 4., marginTop + j*cell + cell / 4.);

				RECT rect;
				rect.left = marginLeft + i*cell - cell / 4.;
				rect.top = marginTop + j*cell - cell / 4.;
				rect.right = marginLeft + i*cell + cell / 4.;
				rect.bottom = marginTop + j*cell + cell / 4.;

				DrawText(hdc, buf, wsprintf(buf, "%d", f[j][i]), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER /*| DT_NOCLIP*/);
				//FillRect(hdc, &rect, hbrushWin);
			}
		}
	}

	SetBkMode(hdc, OPAQUE);

	DeleteObject(hFont);
	DeleteObject(hbrushDef);
	DeleteObject(hbrushWin);
	DeleteObject(hbrushFail);
}

void DrawBridgeManual()
{

}