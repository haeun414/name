#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>

struct Point {
    int x;
    int y;
};

// 화면을 지우는 함수
void clearScreen()
{
    COORD cursorPosition = { 0, 0 };
    DWORD charsWritten;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOutput, &screenBufferInfo);
    FillConsoleOutputCharacter(hConsoleOutput, ' ', screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y, cursorPosition, &charsWritten);
    SetConsoleCursorPosition(hConsoleOutput, cursorPosition);
}

// 랜덤 좌표 생성
Point generateRandomPoint(int stageWidth, int stageHeight, const std::vector<Point>& snakeBody)
{
    Point point;
    srand(time(NULL));

    while (true) {
        point.x = 1 + rand() % (stageWidth - 2);
        point.y = 1 + rand() % (stageHeight - 2);

        bool collision = false;
        for (const auto& bodyPart : snakeBody) {
            if (point.x == bodyPart.x && point.y == bodyPart.y) {
                collision = true;
                break;
            }
        }

        if (!collision) {
            break;
        }
    }

    return point;
}

// game_state == 0일 때
void printTitleScreen()
{
    clearScreen();
    std::cout << "*********************************************" << std::endl;
    std::cout << "*                                           *" << std::endl;
    std::cout << "*                                           *" << std::endl;
    std::cout << "*                지렁이 게임                *" << std::endl;
    std::cout << "*             (Snake Bite Game)             *" << std::endl;
    std::cout << "*                                           *" << std::endl;
    std::cout << "*                                           *" << std::endl;
    std::cout << "* 1. 게임 시작                              *" << std::endl;
    std::cout << "* 2. 게임 설명                              *" << std::endl;
    std::cout << "* 3. 게임 랭킹 보기                         *" << std::endl;
    std::cout << "* 4. 게임 종료(ESC)                         *" << std::endl;
    std::cout << "*                                           *" << std::endl;
    std::cout << "*                                           *" << std::endl;
    std::cout << "*********************************************" << std::endl;
}

// game_state == 1일 때
void gotoxy(int x, int y)
{
    COORD cursorPosition = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void printGameScreen(int stageWidth, int stageHeight, const std::vector<Point>& snakeBody, const Point& food)
{
  


    // 사각형 그리기
    for (int j = 0; j < stageHeight; j++)
    {
        for (int i = 0; i < stageWidth; i++)
        {
            gotoxy(i, j);
            if (i == 0 || i == stageWidth - 1 || j == 0 || j == stageHeight - 1)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << " ";
            }
        }
    }


    // 뱀 출력하기
    for (size_t i = 0; i < snakeBody.size(); i++) {
        gotoxy(snakeBody[i].x, snakeBody[i].y);
        std::cout << (i == 0 ? "0" : "o");
    }


    // 음식 출력하기
    gotoxy(food.x, food.y);
    std::cout << "@";

}

// game_state == 2일 때
void printIntroductionScreen()
{
    clearScreen();
    std::cout << "*********************************************" << std::endl;
    std::cout << "시작 화면으로 돌아갈까요?(Y/N)" << std::endl;
    std::cout << "*********************************************" << std::endl;
}

int main()
{
    int gameState = 0;
    int gameIsRunning = 1;
    int n1, n2;
    bool game1 = false;
    bool game2 = false;
    int xPos1 = 0;
    int yPos1 = 0;
    int xPos2 = 0;
    int yPos2 = 0;
    int xPos3 = 0;
    int yPos3 = 0;
    int stageWidth = 0;
    int stageHeight = 0;

    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

    while (gameIsRunning)
    {
        char keyInput = '0';
        switch (gameState)
        {
        case 0:
            if (!game1)
            {
                printTitleScreen();
                while (keyInput != '1' && keyInput != '2' && keyInput != '4' && keyInput != 27)
                {
                    keyInput = _getch();
                }
                game1 = true;
            }
            if (keyInput == '1')
            {
                gameState = 1;
                clearScreen();
                std::cout << "가로 길이를 입력하세요: ";
                std::cin >> stageWidth;
                std::cout << "세로 길이를 입력하세요: ";
                std::cin >> stageHeight;
                xPos1 = stageWidth / 2;
                yPos1 = stageHeight / 2;
                xPos2 = xPos1 - 1;
                yPos2 = yPos1;
                xPos3 = xPos1 - 2;
                yPos3 = yPos1;
            }
            else if (keyInput == '2')
            {
                game1 = false;
                gameState = 2;
            }
            else if (keyInput == '4' || keyInput == 27)
            {
                gameIsRunning = 0;
            }
            break;
        case 1:
        {
            clearScreen();
            std::vector<Point> snakeBody;
            snakeBody.push_back({ xPos1, yPos1 });
            snakeBody.push_back({ xPos2, yPos2 });
            snakeBody.push_back({ xPos3, yPos3 });

            Point food = generateRandomPoint(stageWidth, stageHeight, snakeBody);

            printGameScreen(stageWidth, stageHeight, snakeBody, food);

            while (true)
            {
                keyInput = _getch();
                //clearScreen();

                for (const auto& bodyPart : snakeBody)
                {
                    gotoxy(bodyPart.x, bodyPart.y);
                    std::cout << " ";
                }
                gotoxy(food.x, food.y);
                std::cout << " ";

                // 뱀 이동하기
                switch (keyInput)
                {
                case 'a':
                    xPos1--;
                    xPos2--;
                    xPos3--;
                    break;
                case 'd':
                    xPos1++;
                    xPos2++;
                    xPos3++;
                    break;
                case 'w':
                    yPos1--;
                    yPos2--;
                    yPos3--;
                    break;
                case 's':
                    yPos1++;
                    yPos2++;
                    yPos3++;
                    break;
                default:
                    break;
                }

                // 뱀이 벽에 부딪히면 게임 종료
                if (xPos1 == 0 || xPos1 == stageWidth - 1 || yPos1 == 0 || yPos1 == stageHeight - 1)
                {
                    gameState = 2;
                    break;
                }

                // 뱀이 자기 자신과 충돌하면 게임 종료
                for (size_t i = 1; i < snakeBody.size(); i++)
                {
                    if (xPos1 == snakeBody[i].x && yPos1 == snakeBody[i].y)
                    {
                        gameState = 2;
                        break;
                    }
                }

                snakeBody.insert(snakeBody.begin(), { xPos1, yPos1 });

                // 뱀이 음식을 먹으면 꼬리가 자라고 새로운 음식 생성
                if (xPos1 == food.x && yPos1 == food.y)
                {
                    food = generateRandomPoint(stageWidth, stageHeight, snakeBody);
                }
                else
                {
                    snakeBody.pop_back();
                }

                printGameScreen(stageWidth, stageHeight, snakeBody, food);
            }
        }
        break;
        case 2:
            xPos1 = 0;
            yPos1 = 0;
            xPos2 = 0;
            yPos2 = 0;
            xPos3 = 0;
            yPos3 = 0;
            if (!game2)
            {
                printIntroductionScreen();
                keyInput = _getch();
                while (keyInput != 'y' && keyInput != 'n')
                {
                    keyInput = _getch();
                }
                game2 = true;
            }
            switch (keyInput)
            {
            case 'y':
                game1 = false;
                game2 = false;
                gameState = 0;
                break;
            case 'n':
                game1 = false;
                game2 = false;
                gameIsRunning = 0;
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
