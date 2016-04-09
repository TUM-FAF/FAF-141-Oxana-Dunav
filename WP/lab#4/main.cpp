#include <windows.h>
#include "ball.h"
#include <time.h>
#define IDT_TIMER 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Ball *balls[50];
char szClassName[ ] = "WP lab4";

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           "WP Lab 4",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           725,
           725,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );

    ShowWindow (hwnd, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}


LRESULT CALLBACK WndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;
	static HDC hDCMem;
	static PAINTSTRUCT ps;
	static RECT rect;
	static HBRUSH hBrush;
	static HBITMAP hBMMem;
	static HANDLE hOld;
	static int nBalls = 0;
	static int timerSpeed = 20;

    POINT coord;

	switch(msg)
	{
	case WM_CREATE:

		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		hDCMem = CreateCompatibleDC(hDC);
		hBMMem = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		hOld = SelectObject(hDCMem, hBMMem);
		srand (time(NULL));
        for (int i = 0; i < 3; i++) {
            coord.x = rand()% 500;
            coord.y = rand()% 500;
            balls[nBalls] = new Ball(coord, 5 + coord.x % 5);
            nBalls++;
        }
		SetTimer(hWnd, IDT_TIMER, timerSpeed, NULL);
		break;

	case WM_SIZE:

		SelectObject(hDCMem, hOld);
		DeleteObject(hBMMem);
		DeleteDC(hDCMem);

		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		hDCMem = CreateCompatibleDC(hDC);
		hBMMem = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		hOld = SelectObject(hDCMem, hBMMem);
		break;

	case WM_LBUTTONDOWN:

		coord.x = LOWORD(lParam);
		coord.y = HIWORD(lParam);

		balls[nBalls] = new Ball(coord, 5 + coord.x % 5);
		nBalls++;
		break;

	case WM_MOUSEWHEEL:
		if((short)HIWORD(wParam) < 0)
		{
			timerSpeed += 10;
			if (timerSpeed > 500)
                timerSpeed = 500;
		}
		else
		{
			timerSpeed -= 10;
			if(timerSpeed < 0)
                timerSpeed = 1;
		}

		KillTimer(hWnd, IDT_TIMER);
		SetTimer(hWnd, IDT_TIMER, timerSpeed, NULL);
		break;

	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		for(int i = 0; i < nBalls - 1; i++)
		{
			for(int j = i + 1; j < nBalls; j++)
			{
				checkBallCollision(balls[i], balls[j]);
			}
		}

		FillRect(hDCMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		for(int i = 0; i < nBalls; i++)
		{
			balls[i]->moveBall(hDCMem, rect, hBrush);
		}

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, hDCMem, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:

		SelectObject(hDCMem, hOld);
		DeleteObject(hBMMem);
		DeleteObject(hDCMem);
		KillTimer(hWnd, IDT_TIMER);

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
