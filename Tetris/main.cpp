#include "Data.h"
#include <string.h>
#include "resource.h"
//#include <GdiPlus.h>
//#pragma comment(lib, "gdiplus.lib")
//using namespace Gdiplus;
#define ID_TIME 0

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Teris") ;
	MSG          msg ;
	HWND hWnd;
    HMENU hMenu;

	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX); 
	wcex.style         = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc   = (WNDPROC)WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance,(LPCTSTR)IDI_APPLICATION);
	wcex.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = szAppName;
	wcex.hIconSm       = LoadIcon(hInstance,(LPCTSTR)IDI_APPLICATION);
    hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	if(!RegisterClassEx(&wcex)) return FALSE;
	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,
		szAppName,
		TEXT("Teris Demo"),
		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		900,
		NULL,
		hMenu,
		hInstance,
		NULL);
	if(!hWnd) return FALSE;
	ShowWindow (hWnd, SW_NORMAL) ;
	UpdateWindow (hWnd) ;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

RECT rect;

void MoveAI(HWND hwnd, Teris &teris)
{
	AI_OFFSET pos;
	pos = teris.AI(teris);
// 	if (teris.GetGameState())
// 	{
// 		return;
// 	}
	while (teris.getNowShape()!=pos.shape)
	{
		int shape = teris.getNowShape();
		SendMessage(hwnd, WM_KEYDOWN, VK_UP, 0);
		if (shape==teris.getNowShape())//代表所在位置无法翻转
		{
			return;
		}
	}
	int X;
	while (teris.getNowPtX()!=pos.offsetX)
	{
		X = teris.getNowPtX();
		if (teris.getNowPtX()<pos.offsetX)
		{
			SendMessage(hwnd, WM_KEYDOWN, VK_RIGHT, 0);
		}
		else
			SendMessage(hwnd, WM_KEYDOWN, VK_LEFT, 0);
		if (X==teris.getNowPtX())//表示无法移动到计算的位置
			break;
	}
	int Y;
	while (teris.getNowPtY()!=pos.offsetY)
	{
		Y = teris.getNowPtY();
		SendMessage(hwnd, WM_KEYDOWN, VK_DOWN, 0);
		if (Y==teris.getNowPtY())//表示无法移动到计算的位置，翻转后Y轴位置改变
			break;
	}
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	rect.bottom = 18;
	rect.left = 0;
	rect.right = 12;
	rect.top = 0;
	HDC hdc;
	PAINTSTRUCT ps;
	static int cX, cY;
	static POINT pPos = {0, 0};
	static Teris teris(rect);
	static BOOL fState = FALSE;
	static BOOL fCheck = FALSE;
	static RECT rect1;
	rect1.left = 0;
	rect1.top = 0;

	switch(message)
	{
	case WM_CREATE:
		SetTimer(hwnd, ID_TIME, 500, NULL);
		MoveAI(hwnd, teris);

		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_FILE_RESTART:
			teris.InitGame();
			InvalidateRect(hwnd, NULL, TRUE);
			MoveAI(hwnd, teris);
			SetTimer(hwnd, ID_TIME, 500, NULL);
			return 0;
		}
		return 0;
    case WM_ERASEBKGND:
		return TRUE;
		
	case WM_SIZE:
		cX = LOWORD(lParam);
		cY = HIWORD(lParam);
		rect1.right = cX;
		rect1.bottom = cY;
		return 0;
	case WM_TIMER:
		SendMessage(hwnd, WM_KEYDOWN, VK_DOWN, 0);
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_DOWN:
			if (teris.Down())
			{
				fState = TRUE;
			}
			else
			{
				SendMessage(hwnd, WM_KEYUP, 0, 0);
				if (fCheck)
				{
					teris.Check();
					fState = TRUE;
					if (!teris.GetGameState())
					{
						MoveAI(hwnd, teris);
					}
				}
			}
			break;
		case VK_UP:
			if (teris.Up())
			{
				fState = TRUE;
			}
			break;
		case VK_LEFT:
			if (teris.Left())
			{
				fCheck = FALSE;
				fState = TRUE;
			}
			break;
		case VK_RIGHT:
			if (teris.Right())
			{
				fCheck = FALSE;
				fState = TRUE;
			}
			break;
		default:
			break;
		}
		if (fState)
		{
			InvalidateRect(hwnd, NULL, TRUE);
			fState = FALSE;
		}
		return 0;
	case WM_KEYUP:
		if (!fCheck)
		{
			fCheck = TRUE;
		}
		return 0;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			HBITMAP hBitMap;
			HDC hdcBack = CreateCompatibleDC(hdc);
			hBitMap = CreateCompatibleBitmap(hdc, cX, cY);
			SelectObject(hdcBack, hBitMap);
			if (teris.GetGameState())
			{
				SetBkMode(hdcBack, TRANSPARENT);
				SetTextColor(hdcBack, RGB(250, 135, 250));
				TextOut(hdcBack, cX/2, cY/2, TEXT("Game Over"), 9);
				KillTimer(hwnd, ID_TIME);
			}
			else
			{
				SelectObject(hdcBack, CreatePen(PS_SOLID, 1, RGB(24, 105, 173)));
				SelectObject(hdcBack, CreateSolidBrush(RGB(255, 255, 255)));
				Rectangle(hdcBack, 0, 0, (rect.right)*40, (rect.bottom)*40);

				DeleteObject(SelectObject(hdcBack, CreateSolidBrush(RGB(13, 135, 239))));

				for (int loop1=0; loop1<=3; ++loop1)
				{
					for (int loop2=0; loop2<=3; ++loop2)
					{
						if (teris.ptr->pos[loop1][loop2])
						{
							Rectangle(hdcBack, (teris.nowPt.x+loop2)*40, (teris.nowPt.y+loop1)*40, 
								(teris.nowPt.x+loop2)*40+40, (teris.nowPt.y+loop1)*40+40);
						}
					}
				}
				for (int loop1=0; loop1<rect.bottom-rect.top; ++loop1)
				{
					for (int loop2=0; loop2<rect.right-rect.left; ++loop2)
					{
						if (teris.pDC[loop1][loop2])
						{
							Rectangle(hdcBack, loop2*40, loop1*40, loop2*40+40, loop1*40+40);
						}
					}
				}

				for (int loop1=0; loop1<=3; ++loop1)
				{
					for (int loop2=0; loop2<=3; ++loop2)
					{
						if (teris.nextPtr->pos[loop1][loop2])
						{
							Rectangle(hdcBack, (loop2)*40+550, (loop1)*40+80, 
								(loop2)*40+40+550, (loop1)*40+40+80);
						}
					}
				}

				DeleteObject(SelectObject(hdcBack, (HPEN)GetStockObject(BLACK_PEN)));
				DeleteObject(SelectObject(hdcBack, (HBRUSH)GetStockObject(WHITE_BRUSH)));
				SetBkMode(hdcBack, TRANSPARENT);
				SetTextColor(hdcBack, RGB(250, 135, 250));
				TextOut(hdcBack, 500, 50, TEXT("下一方块："), 5);
				TCHAR score[20];
				wsprintf(score, TEXT("当前成绩： %d分"), teris.GetScore());
				TextOut(hdcBack, 500, 250, score, strlen((char*)score));
			//	TextOut(hdcBack, 500, 300, TEXT("蛋仔版^_^"), 6);
			}
			// COPY后备缓冲
			BitBlt (hdc, 0, 0, cX, cY, 
				hdcBack, 0, 0, SRCCOPY);
			DeleteObject (hBitMap);
			DeleteDC (hdcBack);
			EndPaint(hwnd, &ps);
			return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}