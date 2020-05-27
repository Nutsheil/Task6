#pragma once
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#define DefaultPensilColor 0x000000
#define DefaultPensilWeigth 1
#define DefaultEraserWeigth 10
#define DefaultBitmapSizeX 500
#define DefaultBitmapSizeY 500

typedef enum
{
	pensil, eraser, other
}selected_object;

typedef struct
{
	HBITMAP hBitmap;
	POINT FirstCoord, SecondCoord;
	selected_object object;
	RGBTRIPLE Pensil_Color;
	int Pensil_Width;
	int Eraser_Width;
	bool new_bitmap;
}My_Paint;

void Settings(My_Paint* paint);