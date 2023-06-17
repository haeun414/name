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


//���ӽ�����Ʈ�� 0�϶� ������ ���� ����
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

//0���� �Ӹ�
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

				// Ű �Է� ó��
				if (_kbhit()) // Ű �Է��� �ִ��� Ȯ��
				{
					char key = _getch(); // ���� Ű�� ������
					if (key == 'a')
					{
						// �������� 90�� ȸ��
						direction = (direction + 3) % 4; // ���� ���⿡�� 3�� ���ϰ� 4�� ���� �������� ���ο� �������� ����
					}
					else if (key == 'd')
					{
						// ���������� �̵�// ���������� 90�� ȸ��
						direction = (direction + 1) % 4; // ���� ���⿡�� 1�� ���ϰ� 4�� ���� �������� ���ο� �������� ����
					}

					// �� �̵� ó��
					MoveSnake(); // ���������� �̵�

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

							// Ű �Է� ó��
							if (_kbhit()) // Ű �Է��� �ִ��� Ȯ��
							{
								char key = _getch(); // ���� Ű�� ������
								if (key == 'a')
								{
									// �������� 90�� ȸ��
									direction = (direction + 3) % 4; // ���� ���⿡�� 3�� ���ϰ� 4�� ���� �������� ���ο� �������� ����
								}
								else if (key == 'd')
								{
									// ���������� �̵�// ���������� 90�� ȸ��
									direction = (direction + 1) % 4; // ���� ���⿡�� 1�� ���ϰ� 4�� ���� �������� ���ο� �������� ����
								}

								// �� �̵� ó��
								MoveSnake(); // ���������� �̵�
							}
						}
						Release();
					}
					else if (userInput == '2')
					{
						GameState = 0; // GameState�� 0���� �����Ͽ� ���� ����
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
	printf("*                ������ ����                *\n");
	printf("*             (Snake Bite Game)             *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("* 1. ���� ����                              *\n");
	printf("* 2. ���� ����                              *\n");
	printf("* 3. ���� ����(ESC)                         *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");

	return;
}

void printDescriptionScreen()
{
	printf("*********************************************\n");
	printf("          ������ ���� �ʿ��Ѱ���?            \n");
	printf("       ���� ȭ������ ���ư����?(Y/N)        \n");
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
	printf("*                1.�ٽ� ����                *\n");
	printf("*         2. ���� ȭ������ ���ư���         *\n");
	printf("*               3. ���� ����                *\n");
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

	// ��� �ʱ�ȭ
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

	//GameObject �ʱ�ȭ

	//��� �ʱ�ȭ
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

	//�� ���׾Ƹ� �ʱ�ȭ
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

//dir : �¿�

void MoveSnake()
{
	int screenWidth = 70;
	int screenHeight = 20;

	//�����̵� �κ�
	int i = GameStartSnakeLength - 1;
	while (i > 0)
	{
		snakeSkin[i].posX = snakeSkin[i - 1].posX;
		snakeSkin[i].posY = snakeSkin[i - 1].posY;
		i--;
	}
	//�Ӹ� �̵��κ�
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

	//�׵θ����� �浹 üũ
	if (snakeSkin[0].posX == 0 || snakeSkin[0].posX == screenWidth + 1 ||
		snakeSkin[0].posY == 0 || snakeSkin[0].posY == screenHeight + 1)
	{
		gameOver = true;
		return;
	}

	//�� �Ӹ��� ����� �浹 üũ
	if (snakeSkin[0].posX == apple.posX && snakeSkin[0].posY == apple.posY)
	{
		//����� ���� ���
		//���� ���̸� �÷���
		snakeSkin[GameStartSnakeLength].posX = snakeSkin[GameStartSnakeLength - 1].posX;
		snakeSkin[GameStartSnakeLength].posY = snakeSkin[GameStartSnakeLength - 1].posY;
		snakeSkin[GameStartSnakeLength].isActive = 1;
		GameStartSnakeLength++;

		//���ο� ��� ����
		InitApple();
	}

	//�� �Ӹ��� ������ �浹 üũ
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