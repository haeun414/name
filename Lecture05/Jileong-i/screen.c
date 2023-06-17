#include "screen.h"
#include <Windows.h>

int screenWidth = 70;
int screenHeight = 20;

int getScreenWidth()
{
	return screenWidth;
}

int getScreenHeight()
{
	return screenHeight;
}

void setScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

void setCursorPos(int x, int y)
{
	/* ���� 74 ���� 22*/
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void setCursorVisibility(int isVisible)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = isVisible;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void SetColor(unsigned short backgroundColor, unsigned short textColor)
{
	unsigned short color = textColor;
	color = color + (backgroundColor << 4);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ClearBuffer()
{
	int y = 0;
	while (y < screenHeight + 2)
	{
		int x = 0;
		while (x < screenWidth + 2)
		{
			if (y == 0 || y == (screenHeight + 1))
				ScreenBuffer[x + (y * (screenWidth + 3))] = '-';
			else
			{
				if (x == 0 || x == (screenWidth + 1))
					ScreenBuffer[x + (y * (screenWidth + 3))] = '|';
				else
					ScreenBuffer[x + (y * (screenWidth + 3))] = ' ';
			}
			x = x + 1;
		}
		ScreenBuffer[x + (y * (screenWidth + 3))] = '\n';
		y = y + 1;
	}
	ScreenBuffer[(y * (screenWidth + 3))] = '\0';
}

int WriteToBuffer(int x, int y, const char* str)
{
	int i = 0;

	while (i < strlen(str))
	{
		ScreenBuffer[x + (y * (screenWidth + 3)) + i] = str[i];
		i = i + 1;
	}

	return 0;
}