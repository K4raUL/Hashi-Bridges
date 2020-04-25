#include "Generator.h"
#include "MainWindow.h"
#include "Painter.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC			hdc0, hdc;
	HBITMAP		backbuffer;

	RECT		rcCli;
	
	int savedDC;

	LONG nWidth;
	LONG nHeight;
	HBRUSH hBrush;

	TCHAR buff[1024];
	char timeBuf[1024];

	switch (message)
	{
	case WM_CREATE:
		// Start the timer.  
		SetTimer(hWnd, idTimer = 1, 1000, NULL);
		return 0L;
	case WM_TIMER:
		sec += 1;
		sec %= 60;
		if (sec == 0) min += 1;

		//SetTimer(hWnd, idTimer = 1, 1000, NULL);
		InvalidateRect(hWnd, &rTimer, true);
		break;
	case WM_COMMAND:

		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			if (lParam == (int)hwndGenButton) {
				GetWindowText(hwndEditH, buff, 1024);
				M = _ttoi(buff);

				GetWindowText(hwndEditW, buff, 1024);
				N = _ttoi(buff);

				f = new int*[M];
				player = new int*[M];
				for (int i = 0; i < M; i++) {
					f[i] = new int[N];
					player[i] = new int[N];
					for (int j = 0; j < N; j++) {
						f[i][j] = 0;
						player[i][j] = 0;
					}
				}
				Generator(M, N, f);
				f_generated = 1;
				gameOver = 0;

				showSolution = 0;
				EnableWindow(hwndClrButton, false);
				EnableWindow(hwndSolButton, true);

				SetTimer(hWnd, idTimer = 1, 1000, NULL);
				min = 0;
				sec = 0;
			}
			else if (lParam == (int)hwndClrButton) {
				player = new int*[M];
				for (int i = 0; i < M; i++) {
					player[i] = new int[N];
					for (int j = 0; j < N; j++) {
						player[i][j] = 0;
					}
				}
				if (gameOver == 1) {
					gameOver = 0;
					EnableWindow(hwndSolButton, true);

					SetTimer(hWnd, idTimer = 1, 1000, NULL);
					min = 0;
					sec = 0;
				}
				EnableWindow(hwndClrButton, false);
			}
			else if (lParam == (int)hwndSolButton) {
				player = new int*[M];
				for (int i = 0; i < M; i++) {
					player[i] = new int[N];
					for (int j = 0; j < N; j++) {
						player[i][j] = 0;
					}
				}
				showSolution = 1;
				EnableWindow(hwndClrButton, false);
				EnableWindow(hwndSolButton, false);

				KillTimer(hWnd, 1);
			}

			GetClientRect(hWnd, &rcCli);
			InvalidateRect(hWnd, &rcCli, true);
			break;
		case EN_CHANGE:
			
			break;
		default:
			break;
		}

	case WM_PAINT: case WM_SIZE:
		hdc0 = BeginPaint(hWnd, &ps);

		GetClientRect(WindowFromDC(hdc0), &rcCli);
		nWidth = rcCli.right - rcCli.left;
		nHeight = rcCli.bottom - rcCli.top;

		hdc = CreateCompatibleDC(hdc0);
		backbuffer = CreateCompatibleBitmap(hdc0, nWidth, nHeight);
		savedDC = SaveDC(hdc);

		SelectObject(hdc, backbuffer);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rcCli, hBrush);
		DeleteObject(hBrush);

		/*if (message == WM_SIZE)*/ calcTransform(M, N, nWidth, nHeight, &cell, &marginLeft, &marginTop);
		
		rField.left = marginLeft - cell/4.;
		rField.top = marginTop - cell / 4.;
		rField.bottom = marginTop + M*cell + cell / 4.;
		rField.right = marginLeft + N*cell + cell / 4.;

		rTimer.left = nWidth / 2. - nWidth / 20.;
		rTimer.top = marginTop / 3. - marginTop / 5.;
		rTimer.right = nWidth / 2. + nWidth / 20.;
		rTimer.bottom = marginTop / 3. + marginTop / 5.;

		// timer
		if (gameOver) {
			KillTimer(hWnd, 1);
			EnableWindow(hwndSolButton, false);

			SetTextColor(hdc, RGB(0, 153, 0));
			DrawText(hdc, timeBuf, wsprintf(timeBuf, "WIN!"), &rTimer, DT_CENTER | DT_SINGLELINE | DT_BOTTOM);
		}
		else SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, timeBuf, wsprintf(timeBuf, "%02d:%02d", min, sec), &rTimer, DT_CENTER | DT_SINGLELINE | DT_TOP);
		SetTextColor(hdc, RGB(0, 0, 0));

		// draw game field
		if (f_generated == 1) DrawField(M, N, f, player, hdc, hdc0, nWidth, nHeight, cell, marginLeft, marginTop, isBridgeSelected, showSolution, _i1_, _j1_, _i2_, _j2_);
						
		BitBlt(hdc0, 0, 0, nWidth, nHeight, hdc, 0, 0, SRCCOPY);
		RestoreDC(hdc, savedDC);

		DeleteObject(backbuffer);
		DeleteDC(hdc);
		DeleteDC(hdc0);

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (isBridgeSelected) {
			//if (f[_i1_][_j1_] == player[_i1_][_j1_] || f[_i2_][_j2_] == player[_i2_][_j2_]) break;
			if (_i1_ == _i2_) {
				for (int j = _j2_ + 1; j < _j1_; j++) player[_i1_][j] = -((-player[_i1_][j] + 1) % 3);
				player[_i1_][_j1_] += player[_i1_][_j2_ + 1] ? 1 : -2;
				player[_i2_][_j2_] += player[_i1_][_j2_ + 1] ? 1 : -2;
			}
			else if (_j1_ == _j2_) {
				for (int i = _i2_ + 1; i < _i1_; i++) player[i][_j1_] = -((-player[i][_j1_] + 10) % 30);
				player[_i1_][_j1_] += player[_i2_ + 1][_j1_] ? 1 : -2;
				player[_i2_][_j2_] += player[_i2_ + 1][_j1_] ? 1 : -2;
			}
			gameOver = CheckWin();
			
			EnableWindow(hwndClrButton, true);
			InvalidateRect(hWnd, &rField, true);
			InvalidateRect(hWnd, &rTimer, true);
		}
		break;
	case WM_MOUSEMOVE:
		isBridgeSelected = calcTrack(M, N, f, player, cell, marginLeft, marginTop, LOWORD(lParam), HIWORD(lParam), &_i1_, &_j1_, &_i2_, &_j2_);
		//if (isBridgeSelected) {
			//GetClientRect(hWnd, &rField);
			InvalidateRect(hWnd, &rField, true);
		//}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		
		for (int i = 0; i < M; i++) {
			delete[] f[i];
			delete[] player[i];
		}
		delete[] f;
		delete[] player;

		break;
	case WM_ERASEBKGND:
		return 1;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

int CALLBACK WinMain(	_In_ HINSTANCE hInstance,	_In_opt_ HINSTANCE hPrevInstance,	_In_ LPSTR     lpCmdLine,	_In_ int       nCmdShow	)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		850, 850,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Windows Desktop Guided Tour"), NULL);
		return 1;
	}

	hwndGenButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Generate",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		80,        // Button width
		30,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndClrButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Clear",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		100,         // x position 
		10,         // y position 
		80,        // Button width
		30,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndSolButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Solution",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		190,         // x position 
		10,         // y position 
		80,        // Button width
		30,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndEditW = CreateWindow("EDIT",
		"10", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		10, 45, 35, 20, hWnd, NULL, hInstance, NULL);

	hwndEditH = CreateWindow("EDIT",
		"10", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		54, 45, 35, 20, hWnd, NULL, hInstance, NULL);

	EnableWindow(hwndClrButton, false);
	EnableWindow(hwndSolButton, true);

	showSolution = 0;
	gameOver = 0;
	f		= new int*[M];
	player	= new int*[M];
	for (int i = 0; i < M; i++) {
		f[i]		= new int[N];
		player[i]	= new int[N];
		for (int j = 0; j < N; j++) {
			f[i][j]		 = 0;
			player[i][j] = 0;
		}
	}
	Generator(M, N, f);
	f_generated = 1;

	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);

	RECT rcCli;
	GetClientRect(hWnd, &rcCli);
	InvalidateRect(hWnd, &rcCli, true);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

bool calcTrack(int M, int N, int **f, int **p, int cell, int mLeft, int mTop, int cursorX, int cursorY, int* i1, int* j1, int* i2, int* j2)
{
	if (cursorX < mLeft || cursorX > (N - 1)*cell + mLeft) return false;
	if (cursorY < mTop  || cursorY > (M - 1)*cell + mTop)  return false;

	double delta = cell / 2;

	int leftj = (int)((cursorX - mLeft) / cell);
	int rightj = leftj + 1;
	int topi = (int)((cursorY - mTop) / cell);
	int bottomi = topi + 1;

	*i1 = -1;
	*j1 = -1;
	*i2 = -1;
	*j2 = -1;

	if (isPointinRect(cursorX, cursorY, leftj*cell + mLeft + delta / 2., topi*cell + mTop - delta / 2., cell - delta, delta)) {
		// i,j -> i,j+1
		for (int j = leftj+1; j < N; j++) {
			if (p[topi][j] < -2) return false;
			if (f[topi][j] > 0) {
				*i1 = topi;
				*j1 = j;
				break;
			}
		}
		
		for (int j = leftj; j >= 0; j--) {
			if (p[topi][j] < -2) return false;
			if (f[topi][j] > 0) {
				*i2 = topi;
				*j2 = j;
				break;
			}
		}
	}
	else if (isPointinRect(cursorX, cursorY, leftj*cell + mLeft + delta / 2., bottomi*cell + mTop - delta / 2., cell - delta, delta)) {
		// i+1,j -> i+1,j+1
		for (int j = leftj + 1; j < N; j++) {
			if (p[bottomi][j] < -2) return false;
			if (f[bottomi][j] > 0) {
				*i1 = bottomi;
				*j1 = j;
				break;
			}
		}

		for (int j = leftj; j >= 0; j--) {
			if (p[bottomi][j] < -2) return false;
			if (f[bottomi][j] > 0) {
				*i2 = bottomi;
				*j2 = j;
				break;
			}
		}
	}
	else if (isPointinRect(cursorX, cursorY, leftj*cell + mLeft - delta / 2., topi*cell + mTop + delta / 2., delta, cell - delta)) {
		// i,j -> i+1,j
		for (int i = topi+1; i < M; i++) {
			if (p[i][leftj] > -10 && p[i][leftj] < 0) return false;
			if (f[i][leftj] > 0) {
				*i1 = i;
				*j1 = leftj;
				break;
			}
		}

		for (int i = topi; i >= 0; i--) {
			if (p[i][leftj] > -10 && p[i][leftj] < 0) return false;
			if (f[i][leftj] > 0) {
				*i2 = i;
				*j2 = leftj;
				break;
			}
		}
	}
	else if (isPointinRect(cursorX, cursorY, rightj*cell + mLeft - delta / 2., topi*cell + mTop + delta / 2., delta, cell - delta)) {
		// i+1,j -> i+1,j+1
		for (int i = topi+1; i < M; i++) {
			if (p[i][rightj] > -10 && p[i][rightj] < 0) return false;
			if (f[i][rightj] > 0) {
				*i1 = i;
				*j1 = rightj;
				break;
			}
		}

		for (int i = topi; i >= 0; i--) {
			if (p[i][rightj] > -10 && p[i][rightj] < 0) return false;
			if (f[i][rightj] > 0) {
				*i2 = i;
				*j2 = rightj;
				break;
			}
		}
	}
	else {																			// otherwise calculate minimum distance
		double minDist = cell;
		int topNear = 0;
		int leftNear = 0;
		int bottomNear = 0;
		int rightNear = 0;

		double dist = distanceLinePoint(cursorX, cursorY, leftj*cell + mLeft, topi*cell + mTop, rightj*cell + mLeft, topi*cell + mTop, cell);
		if (dist < minDist) {
			minDist = dist;
			leftNear = leftj;
			topNear = topi;
			rightNear = rightj;
			bottomNear = topi;
		}
		dist = distanceLinePoint(cursorX, cursorY, leftj*cell + mLeft, topi*cell + mTop, leftj*cell + mLeft, bottomi*cell + mTop, cell);
		if (dist < minDist) {
			minDist = dist;
			leftNear = leftj;
			topNear = topi;
			rightNear = leftj;
			bottomNear = bottomi;
		}
		dist = distanceLinePoint(cursorX, cursorY, leftj*cell + mLeft, bottomi*cell + mTop, rightj*cell + mLeft, bottomi*cell + mTop, cell);
		if (dist < minDist) {
			minDist = dist;
			leftNear = leftj;
			topNear = bottomi;
			rightNear = rightj;
			bottomNear = bottomi;
		}
		dist = distanceLinePoint(cursorX, cursorY, rightj*cell + mLeft, topi*cell + mTop, rightj*cell + mLeft, bottomi*cell + mTop, cell);
		if (dist < minDist) {
			minDist = dist;
			leftNear = rightj;
			topNear = topi;
			rightNear = rightj;
			bottomNear = bottomi;
		}

		if (leftNear == rightNear) {
			for (int i = topNear + 1; i < M; i++) {
				if (p[i][leftNear] > -10 && p[i][leftNear] < 0) return false;
				if (f[i][leftNear] > 0) {
					*i1 = i;
					*j1 = leftNear;
					break;
				}
			}

			for (int i = topNear; i >= 0; i--) {
				if (p[i][leftNear] > -10 && p[i][leftNear] < 0) return false;
				if (f[i][leftNear] > 0) {
					*i2 = i;
					*j2 = leftNear;
					break;
				}
			}
		}
		else if (topNear == bottomNear) {
			for (int j = leftNear + 1; j < N; j++) {
				if (p[topNear][j] < -2) return false;
				if (f[topNear][j] > 0) {
					*i1 = topNear;
					*j1 = j;
					break;
				}
			}

			for (int j = leftNear; j >= 0; j--) {
				if (p[topNear][j] < -2) return false;
				if (f[topNear][j] > 0) {
					*i2 = topNear;
					*j2 = j;
					break;
				}
			}
		}
	}

	if (*i1 >= 0 && *j1 >= 0 && *i2 >= 0 && *j2 >= 0) return true;
	return false;
}

bool CheckWin()
{
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (f[i][j] > 0) {
				if (f[i][j] != player[i][j]) return false;
			}
		}
	}
	return true;
}

bool isPointinRect(int x, int y, int left, int top, int W, int H)
{
	return x > left && x < left + W && y > top && y < top + H;
}

double distanceLinePoint(int x0, int y0, int x1, int y1, int x2, int y2, int cell)
{
	return (double)abs( (y2-y1)*x0 - (x2-x1)*y0 + x2*y1 - y2*x1 ) / cell;
}



