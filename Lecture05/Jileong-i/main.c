#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "screen.h"
#include "util.h"
#include <conio.h>
#include <stdbool.h>
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3

int direction = DIRECTION_RIGHT;;
int Initialize();
int Release();
int DrawBuffer(int DeltaTime);
void MoveSnake();
void printStartScreen();
void printDescriptionScreen();
void printGameOverScreen();
int IsInsideRect(int x, int y, int rectLeft, int rectTop, int rectRight, int rectBottom);
bool gameOver = false;
int DeltaTimeSum = 0;


//게임스테이트가 0일때 무조건 게임 종료
int GameState = 0;
int GameStartSnakeLength;
int ActiveSnakeNum;

#define MAX_SNAKE_LENGTH 1400
struct GameObject
{
	int posX;
	int posY;
	int isActive;
};

//0번이 머리
struct GameObject snakeSkin[MAX_SNAKE_LENGTH];
struct GameObject apple;

int main()
{
	int userInput = 0;

	while (1)
	{
		system("cls");
		GameState = 0;
		
		printStartScreen();
		userInput = _getch();

		if (userInput == '1')
		{
			system("cls");
			Initialize();

			unsigned long time_end = GetTickCount();
			unsigned long deltaTime = GetTickCount();

			GameState = 1;

			while (GameState)
			{
				deltaTime = GetTickCount() - time_end;
				time_end = GetTickCount();

				DrawBuffer(deltaTime);

				if (deltaTime < 33)
					Sleep(33 - deltaTime);

				// 키 입력 처리
				if (_kbhit()) // 키 입력이 있는지 확인
				{
					char key = _getch(); // 눌린 키를 가져옴
					if (key == 'a')
					{
						// 왼쪽으로 90도 회전
						direction = (direction + 3) % 4; // 현재 방향에서 3을 더하고 4로 나눈 나머지를 새로운 방향으로 설정
					}
					else if (key == 'd')
					{
						// 오른쪽으로 이동// 오른쪽으로 90도 회전
						direction = (direction + 1) % 4; // 현재 방향에서 1을 더하고 4로 나눈 나머지를 새로운 방향으로 설정
					}

					// 뱀 이동 처리
					MoveSnake(); // 오른쪽으로 이동

				}
				if (gameOver)
				{
					printGameOverScreen();
					userInput = _getch();

					if (userInput == '1')
					{
						system("cls");
						Initialize();

						unsigned long time_end = GetTickCount();
						unsigned long deltaTime = GetTickCount();

						GameState = 1;

						while (GameState == 1)
						{
							deltaTime = GetTickCount() - time_end;
							time_end = GetTickCount();

							DrawBuffer(deltaTime);

							if (deltaTime < 33)
								Sleep(33 - deltaTime);

							// 키 입력 처리
							if (_kbhit()) // 키 입력이 있는지 확인
							{
								char key = _getch(); // 눌린 키를 가져옴
								if (key == 'a')
								{
									// 왼쪽으로 90도 회전
									direction = (direction + 3) % 4; // 현재 방향에서 3을 더하고 4로 나눈 나머지를 새로운 방향으로 설정
								}
								else if (key == 'd')
								{
									// 오른쪽으로 이동// 오른쪽으로 90도 회전
									direction = (direction + 1) % 4; // 현재 방향에서 1을 더하고 4로 나눈 나머지를 새로운 방향으로 설정
								}

								// 뱀 이동 처리
								MoveSnake(); // 오른쪽으로 이동
							}
						}
						Release();
					}
					else if (userInput == '2')
					{
						GameState = 0; // GameState를 0으로 설정하여 게임 종료
						/*break;*/
					}
					else if (userInput == '3')
					{
						return 0;
					}
				}
			}
			Release();
		}
		else if (userInput == '2')
		{
			system("cls");
			printDescriptionScreen();

			char choice = ' ';
			while (choice != 'Y' && choice != 'N')
			{
				choice = _getch();

				if (choice == 'Y' || choice == 'y')
				{
					system("cls");
					break;
				}
				else if (choice == 'N' || choice == 'n')
				{
					return 0;
				}
			}
		}
		else if (userInput == '3' || userInput == 27)
		{
			return 0;
		}
	}
	return 0;
}

void printStartScreen()
{
	printf("*********************************************\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*                지렁이 게임                *\n");
	printf("*             (Snake Bite Game)             *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("* 1. 게임 시작                              *\n");
	printf("* 2. 게임 설명                              *\n");
	printf("* 3. 게임 종료(ESC)                         *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");

	return;
}

void printDescriptionScreen()
{
	printf("*********************************************\n");
	printf("          설명이 굳이 필요한가요?            \n");
	printf("       시작 화면으로 돌아갈까요?(Y/N)        \n");
	printf("*********************************************\n");
}

void printGameOverScreen()
{
	printf("*********************************************\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*                 GAME OVER                 *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*                1.다시 시작                *\n");
	printf("*         2. 시작 화면으로 돌아가기         *\n");
	printf("*               3. 게임 종료                *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");

	return;
}

void InitApple()
{
	int rectLeft = 5;
	int rectTop = 5;
	int rectRight = 64;
	int rectBottom = 16;

	// 사과 초기화
	srand(time(NULL));
	do
	{
		apple.posX = rectLeft + rand() % (rectRight - rectLeft + 1);
		apple.posY = rectTop + rand() % (rectBottom - rectTop + 1);
	} while (!IsInsideRect(apple.posX, apple.posY, rectLeft, rectTop, rectRight, rectBottom));

	apple.isActive = 1;
}

int IsInsideRect(int x, int y, int rectLeft, int rectTop, int rectRight, int rectBottom)
{
	return (x >= rectLeft && x <= rectRight && y >= rectTop && y <= rectBottom);
}

int Initialize()
{
	GameState = 1;
	GameStartSnakeLength = 3;
	ActiveSnakeNum = GameStartSnakeLength;
	setScreenSize(70, 20);
	setCursorVisibility(0);
	SetColor(0b1000, 0b1111);
	ClearBuffer();

	//GameObject 초기화

	//사과 초기화
	InitApple();
	int i = 0;
	while (i < GameStartSnakeLength)
	{
		if (snakeSkin[i].posX == apple.posX
			&& snakeSkin[i].posY == apple.posY)
		{
			InitApple();
			i = 0;
		}
		else
		{
			i = i + 1;
		}
	}

	//뱀 몸뚱아리 초기화
	i = 0;
	while (i < MAX_SNAKE_LENGTH)
	{
		snakeSkin[i].posX = 35;
		snakeSkin[i].posY = 10;
		snakeSkin[i].isActive = 0;
		i = i + 1;
	}

	i = 0;
	while (i < GameStartSnakeLength)
	{
		snakeSkin[i].posX = 35 - i;
		snakeSkin[i].posY = 10;
		snakeSkin[i].isActive = 1;
		i = i + 1;
	}

	direction = DIRECTION_RIGHT;

	return 0;
}

int Release()
{
	return 0;
}

//dir : 좌우

void MoveSnake()
{
	int screenWidth = 70;
	int screenHeight = 20;

	//몸통이동 부분
	int i = GameStartSnakeLength - 1;
	while (i > 0)
	{
		snakeSkin[i].posX = snakeSkin[i - 1].posX;
		snakeSkin[i].posY = snakeSkin[i - 1].posY;
		i--;
	}
	//머리 이동부분
	switch (direction)
	{
	case DIRECTION_UP:
		snakeSkin[0].posY--;
		break;
	case DIRECTION_RIGHT:
		snakeSkin[0].posX++;
		break;
	case DIRECTION_DOWN:
		snakeSkin[0].posY++;
		break;
	case DIRECTION_LEFT:
		snakeSkin[0].posX--;
		break;
	}

	//테두리와의 충돌 체크
	if (snakeSkin[0].posX == 0 || snakeSkin[0].posX == screenWidth + 1 ||
		snakeSkin[0].posY == 0 || snakeSkin[0].posY == screenHeight + 1)
	{
		gameOver = true;
		return;
	}

	//뱀 머리와 사과의 충돌 체크
	if (snakeSkin[0].posX == apple.posX && snakeSkin[0].posY == apple.posY)
	{
		//사과를 먹은 경우
		//뱀의 길이를 늘려줌
		snakeSkin[GameStartSnakeLength].posX = snakeSkin[GameStartSnakeLength - 1].posX;
		snakeSkin[GameStartSnakeLength].posY = snakeSkin[GameStartSnakeLength - 1].posY;
		snakeSkin[GameStartSnakeLength].isActive = 1;
		GameStartSnakeLength++;

		//새로운 사과 생성
		InitApple();
	}

	//뱀 머리와 몸통의 충돌 체크
	for (int i = 1; i < GameStartSnakeLength; ++i)
	{
		if (snakeSkin[0].posX == snakeSkin[i].posX && snakeSkin[0].posY == snakeSkin[i].posY)
		{
			gameOver = true;
			return;
		}
	}
}

int DrawBuffer(int DeltaTime)
{
	setCursorPos(0, 0);
	SetColor(0b1111, 0b0000);
	printf("%s", ScreenBuffer);

	DeltaTimeSum = DeltaTimeSum + DeltaTime;
	if (DeltaTimeSum >= 500)
	{
		MoveSnake();
		DeltaTimeSum = 0;
	}

	//apple
	setCursorPos(apple.posX, apple.posY);
	SetColor(0b1111, 0b0100);
	printf("*");

	//snake
	int i = 0;
	while (i < GameStartSnakeLength)
	{
		setCursorPos(snakeSkin[i].posX, snakeSkin[i].posY);
		SetColor(0b1111, 0b0010);
		printf("O");
		i = i + 1;
	}

	SetColor(0b0000, 0b1111);
	setCursorPos(0, 22);
	if (DeltaTime != 0)
		printf("time : %d                 \nfps :%d                  \n", DeltaTime, 1000 / DeltaTime);

	return GameState;
}