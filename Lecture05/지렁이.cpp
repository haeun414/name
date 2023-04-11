#include <iostream>
#include <conio.h>
#include <Windows.h>

// game_state == 0일 때
int print_title_screen()
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
	printf("* 3. 게임 랭킹 보기                         *\n");
	printf("* 4. 게임 종료(ESC)                         *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");

	return 0;
}

//game_state == 1일 때
void gotoxy(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int print_game_screen(int stage_width, int stage_height)
{
	gotoxy(stage_width, stage_height);

	for (int i = 0; i < stage_width; i++)
	{
		for (int j = 0; j < stage_height; j++)
		{
			if (i == 0 || i == stage_width - 1)
			{
				printf("*");
			}
			else if (j == 0 || j == stage_height - 1)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}

//game_state == 2일 때
int print_introducion_screen()
{
	printf("*********************************************\n");
	printf("시작 화면으로 돌아갈까요?(Y/N)\n");

	return 0;
}

int main()
{
	int game_state = 0;
	int game_is_running = 1;
	int n1, n2;
	bool game_1 = false;
	bool game_2 = false;

	while (game_is_running)
	{
		char key_input = '0';
		switch (game_state)
		{
		case 0:
			if (!game_1)
			{
				print_title_screen();
				while (key_input != '1' && key_input != '2' && key_input != '4' && key_input != 27)
				{
					key_input = _getch();
				}
				game_1 = true;
			}
			if (key_input == '1')
			{
				game_state = 1;
			}
			else if (key_input == '2')
			{
				game_1 = false;
				game_state = 2;
			}
			else if (key_input == '4' || key_input == 27)
			{
				game_is_running = 0;
			}
			break;
		case 1:
			printf("좌표를 입력하세요: \n");
			scanf_s("%d%d", &n1, &n2);
			print_game_screen(n1, n2);
			game_state = 5;
			break;
		case 2:
			if (!game_2)
			{
				print_introducion_screen();
				while (key_input != 'y' && key_input != 'n')
				{
					key_input = _getch();
				}
				game_2 = true;
			}
			switch (key_input)
			{
			case 'y':
				game_1 = false;
				game_2 = false;
				game_state = 0;
				break;
			case 'n':
				game_1 = false;
				game_2 = false;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	return 0;
}