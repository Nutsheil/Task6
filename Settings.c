#include "Settings.h"

HWND hWnd;
HWND PensilColor[3];
HWND PensilWidth, EraserWidth;
///////////////////////////////////// ��������� �������
LRESULT CALLBACK WndProc2(HWND, UINT, UINT, LONG);

ATOM registerMyClass(HINSTANCE hInstance, LPCWSTR szClassName, WNDPROC WndProcParam);

HWND createMyWindow(LPCWSTR windowName, LPCWSTR szClassName, WNDPROC WndProcParam, HWND hParentWindow, RECT rect);
/////////////////////////////////////

RGBTRIPLE MyPensilColor;
int MyPensilWidth, MyEraserWidth;

void Settings(RGBTRIPLE* PColor, int* PWidth, int* EWidth)
{
	hWnd = createMyWindow("Settings", "Window2", WndProc2, NULL, (RECT) { 200, 200, 400, 260 });

	for (int i = 0; i < 3; i++)
		PensilColor[i] = CreateWindow("EDIT", "", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 40, 10 + 30 * i, 60, 20, hWnd, NULL, NULL, NULL);

	PensilWidth = CreateWindow("EDIT", "", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 120, 100, 60, 20, hWnd, NULL, NULL, NULL);
	EraserWidth = CreateWindow("EDIT", "", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 120, 130, 60, 20, hWnd, NULL, NULL, NULL);

	HWND MyButton = CreateWindow("BUTTON", "Select", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130, 170, 80, 20, hWnd, 1, NULL, NULL);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	*PColor = MyPensilColor;
	*PWidth = MyPensilWidth;
	*EWidth = MyEraserWidth;
	//paint->Pensil_Color = MyPensilColor;
	//paint->Pensil_Width = MyPensilWidth;
	//paint->Eraser_Width = MyEraserWidth;
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1:
		{
			WCHAR number[5][4];
			int aaa;

			GetWindowTextW(PensilColor[0], number[0], 3);
			MyPensilColor.rgbtRed = _wtoi(number[0]);
			GetWindowTextW(PensilColor[1], number[1], 3);
			MyPensilColor.rgbtGreen = _wtoi(number[1]);
			GetWindowTextW(PensilColor[2], number[2], 3);
			MyPensilColor.rgbtBlue = _wtoi(number[2]);

			GetWindowTextW(PensilWidth, number[3], 3);
			MyPensilWidth = _wtoi(number[3]);
			GetWindowTextW(EraserWidth, number[4], 3);
			MyEraserWidth = _wtoi(number[4]);

			DestroyWindow(hWnd);
			break;
		}
		default:
			break;
		}
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 10, 10, "R:", 2);
		TextOut(hdc, 10, 40, "G:", 2);
		TextOut(hdc, 10, 70, "B:", 2);
		TextOut(hdc, 10, 100, "Pensil Weigth:", 14);
		TextOut(hdc, 10, 130, "Eraser Weigth:", 14);

		EndPaint(hWnd, &ps);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND createMyWindow(LPCWSTR windowName, LPCWSTR szClassName, WNDPROC WndProcParam, HWND hParentWindow, RECT rect)
{
	registerMyClass(NULL, szClassName, WndProcParam);

	HWND hWnd = CreateWindow(szClassName, windowName, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, hParentWindow, NULL, NULL, NULL);

	if (!hWnd) { return 0; }
	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
	return hWnd;
}

ATOM registerMyClass(HINSTANCE hInstance, LPCWSTR szClassName, WNDPROC WndProcParam)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = 0;
	wc.lpfnWndProc = WndProcParam;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	return RegisterClass(&wc);
}