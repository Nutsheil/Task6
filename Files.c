#include "Files.h"

void Save(HBITMAP image)
{
	OPENFILENAME ofn;
	WCHAR fileName[260];

	BITMAP bitmap;
	BITMAPFILEHEADER bitmapfileheader;
	LPBITMAPINFO lpbitmapinfo;
	HDC hdc;
	LPBYTE pixels;
	DWORD DataSize;
	WORD BitCount;
	HANDLE file;
	DWORD dwTemp;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"BMP\0*.BMP\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;
	GetSaveFileName(&ofn);

	GetObject(image, sizeof(BITMAP), (LPSTR)&bitmap);

	BitCount = (WORD)bitmap.bmPlanes * bitmap.bmBitsPixel;

	DataSize = (bitmap.bmWidth * BitCount + 15) / 8 * bitmap.bmHeight;

	lpbitmapinfo = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER));

	lpbitmapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbitmapinfo->bmiHeader.biWidth = bitmap.bmWidth;
	lpbitmapinfo->bmiHeader.biHeight = bitmap.bmHeight;
	lpbitmapinfo->bmiHeader.biPlanes = 1;
	lpbitmapinfo->bmiHeader.biBitCount = BitCount;
	lpbitmapinfo->bmiHeader.biCompression = 0;
	lpbitmapinfo->bmiHeader.biSizeImage = DataSize;
	lpbitmapinfo->bmiHeader.biXPelsPerMeter = 0;
	lpbitmapinfo->bmiHeader.biYPelsPerMeter = 0;
	lpbitmapinfo->bmiHeader.biClrImportant = 0;

	bitmapfileheader.bfType = 0x4d42;
	bitmapfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bitmapfileheader.bfReserved1 = 0;
	bitmapfileheader.bfReserved2 = 0;
	bitmapfileheader.bfSize = bitmapfileheader.bfOffBits + DataSize;

	pixels = (LPBYTE)malloc(DataSize);

	hdc = GetDC(0);
	GetDIBits(hdc, image, 0, (WORD)bitmap.bmHeight, pixels, lpbitmapinfo, DIB_RGB_COLORS);
	ReleaseDC(0, hdc);

	file = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, &bitmapfileheader, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);
	WriteFile(file, lpbitmapinfo, sizeof(BITMAPINFOHEADER) + lpbitmapinfo->bmiHeader.biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);
	WriteFile(file, pixels, DataSize, &dwTemp, NULL);

	CloseHandle(file);
	free(pixels);
	free(lpbitmapinfo);
}

HBITMAP Open()
{
	OPENFILENAME ofn;
	WCHAR fileName[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"BMP\0*.BMP\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	return LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}