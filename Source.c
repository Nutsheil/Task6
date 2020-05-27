#include "Files.h"
#include "Settings.h"

char szClassName[] = "Window1";
HWND hWnd;
HMENU hMenu;
///////////////////////////////////// прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

ATOM RegisterMyClass(HINSTANCE hInstance);

int CreateMyWindow(HINSTANCE hInstance, int nCmdShow);
/////////////////////////////////////

My_Paint MYPAINT;

void CreateMyMenu()
{
	hMenu = CreateMenu();
	HMENU hPopMenuFile = CreatePopupMenu();
	HMENU hPopMenuPensil = CreatePopupMenu();
	HMENU hPopMenuPensilWeigth = CreatePopupMenu();
	HMENU hPopMenuPensilColor = CreatePopupMenu();

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, "File");
	AppendMenu(hPopMenuFile, MF_STRING, 1, "New");
	AppendMenu(hPopMenuFile, MF_STRING, 2, "Open");
	AppendMenu(hPopMenuFile, MF_STRING, 3, "Save");

	AppendMenu(hMenu, MF_STRING, 4, "Pensil");
	AppendMenu(hMenu, MF_STRING, 5, "Eraser");
	AppendMenu(hMenu, MF_STRING, 6, "Settings");

	SetMenu(hWnd, hMenu);
	SetMenu(hWnd, hPopMenuFile);
}

void SetDeafultSetting()
{
	MYPAINT.Pensil_Color.rgbtRed = 0;
	MYPAINT.Pensil_Color.rgbtGreen = 0;
	MYPAINT.Pensil_Color.rgbtBlue = 0;
	MYPAINT.Pensil_Width = DefaultPensilWeigth;
	MYPAINT.Eraser_Width = DefaultEraserWeigth;
	MYPAINT.object = other;
	MYPAINT.new_bitmap = false;
}

void Draw(HDC hdc)
{
	if (MYPAINT.new_bitmap == true)
	{
		HBRUSH myBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, myBrush);
		Rectangle(hdc, -1, -1, DefaultBitmapSizeX + 1, DefaultBitmapSizeY + 1);
		DeleteObject(myBrush);
		MYPAINT.new_bitmap = false;
		return;
	}

	HPEN myPen = NULL;

	if (MYPAINT.object == pensil)
		myPen = CreatePen(PS_SOLID, MYPAINT.Pensil_Width, RGB(MYPAINT.Pensil_Color.rgbtRed, MYPAINT.Pensil_Color.rgbtGreen, MYPAINT.Pensil_Color.rgbtBlue));
	if (MYPAINT.object == eraser)
		myPen = CreatePen(PS_SOLID, MYPAINT.Eraser_Width, RGB(255, 255, 255));

	SelectObject(hdc, myPen);
	MoveToEx(hdc, MYPAINT.FirstCoord.x, MYPAINT.FirstCoord.y, NULL);
	LineTo(hdc, MYPAINT.SecondCoord.x, MYPAINT.SecondCoord.y);
	MYPAINT.FirstCoord = MYPAINT.SecondCoord;
	DeleteObject(myPen);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	CreateMyWindow(hInstance, nCmdShow);
	CreateMyMenu();

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		DestroyMenu(hMenu);
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1:
		{
			HDC hdc = GetDC(hWnd);
			MYPAINT.hBitmap = CreateCompatibleBitmap(hdc, DefaultBitmapSizeX, DefaultBitmapSizeY);
			MYPAINT.new_bitmap = true;
			InvalidateRect(hWnd, NULL, 0);
			break;
		}
		case 2:
		{
			MYPAINT.hBitmap = Open();
			break;
		}
		case 3:
		{
			Save(MYPAINT.hBitmap);
			break;
		}
		case 4:
		{
			MYPAINT.object = pensil;
			break;
		}
		case 5:
		{
			MYPAINT.object = eraser;
			break;
		}
		case 6:
		{
			Settings(&MYPAINT.Pensil_Color, &MYPAINT.Pensil_Width, &MYPAINT.Eraser_Width);
			break;
		}
		default:
			break;
		}
	}
	case WM_LBUTTONDOWN:
	{
		MYPAINT.FirstCoord.x = LOWORD(lParam);
		MYPAINT.FirstCoord.y = HIWORD(lParam);
		MYPAINT.SecondCoord = MYPAINT.FirstCoord;
		InvalidateRect(hWnd, NULL, 0);
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam & MK_LBUTTON)
		{
			MYPAINT.SecondCoord.x = LOWORD(lParam);
			MYPAINT.SecondCoord.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, 0);
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hdcC = CreateCompatibleDC(hdc);

		HBITMAP hOldBitmap = SelectObject(hdcC, MYPAINT.hBitmap);
		BITMAP bitmap;

		GetObject(MYPAINT.hBitmap, sizeof(bitmap), &bitmap);


		Draw(hdcC);


		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcC, 0, 0, SRCCOPY);

		SelectObject(hdcC, hOldBitmap);
		DeleteObject(hdcC);
		EndPaint(hWnd, &ps);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CreateMyWindow(HINSTANCE hInstance, int nCmdShow)
{
	RegisterMyClass(hInstance);

	hWnd = CreateWindow(szClassName, "PAINT", WS_OVERLAPPEDWINDOW, 0, 0, 900, 600, NULL, NULL, hInstance, NULL);

	if (!hWnd) { return 0; }
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

ATOM RegisterMyClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	return RegisterClass(&wc);
}