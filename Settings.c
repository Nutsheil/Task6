#include "Settings.h"

HWND hWnd;
HWND PensilColor[3];
HWND PensilWidth, EraserWidth;
///////////////////////////////////// прототипы функций
LRESULT CALLBACK WndProc2(HWND, UINT, UINT, LONG);

ATOM registerMyClass(HINSTANCE hInstance, LPCWSTR szClassName, WNDPROC WndProcParam);

HWND createMyWindow(LPCWSTR windowName, LPCWSTR szClassName, WNDPROC WndProcParam, HWND hParentWindow, RECT rect);
/////////////////////////////////////

My_Paint MYPAINT;

My_Paint Settings()
{
	hWnd = createMyWindow("Settings", "Window2", WndProc2, NULL, (RECT) { 200, 200, 400, 260 });

	for(int i=0;i<3;i++)
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

	return MYPAINT;
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
			MYPAINT.Pensil_Color.rgbtRed = _wtoi(number[0]);
			GetWindowTextW(PensilColor[1], number[1], 3);
			MYPAINT.Pensil_Color.rgbtGreen = _wtoi(number[1]);
			GetWindowTextW(PensilColor[2], number[2], 3);
			MYPAINT.Pensil_Color.rgbtBlue = _wtoi(number[2]);

			GetWindowText(PensilWidth, number[3], 3);
			MYPAINT.Pensil_Weigth = _wtoi(number[3]);
			GetWindowText(EraserWidth, number[4], 3);
			MYPAINT.Eraser_Weigth = _wtoi(number[4]);

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