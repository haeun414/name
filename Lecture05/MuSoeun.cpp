#include <iostream>
#include <vector>
#include <conio.h>

namespace MuSoeun {

    class Object {
    public:
        virtual void Render(char** screenBuffer) = 0;
    };

    class SnakeBody : public Object {
    private:
        int posX;
        int posY;

    public:
        SnakeBody(int x, int y) : posX(x), posY(y) {}

        void Render(char** screenBuffer) override {
            int center_x = posX / 2;
            int center_y = posY / 2;

            for (int j = 0; j < posY; j++) {
                for (int i = 0; i < posX; i++) {
                    if (i == 0 || i == posX - 1 || j == 0 || j == posY - 1) {
                        screenBuffer[j][i] = '*';
                    }
                    else if (i >= center_x - 1 && i <= center_x + 1 && j == center_y) {
                        screenBuffer[j][i] = 'X';
                    }
                    else if (i == center_x && j == center_y) {
                        screenBuffer[j][i] = 'O';
                    }
                    else {
                        screenBuffer[j][i] = ' ';
                    }
                }
            }
        }
    };

    class Scene {
    public:
        int width;
        int height;
        std::vector<Object*> objList;

        char** screenBuf;

        void InitScreenBuf() {
            screenBuf = new char* [height];
            for (int i = 0; i < height; i++) {
                screenBuf[i] = new char[width + 1];  // 문자열 끝에 null 문자를 추가하기 위해 +1
                for (int j = 0; j < width; j++) {
                    screenBuf[i][j] = ' ';  // 빈 칸으로 초기화
                }
                screenBuf[i][width] = '\0';  // 문자열의 끝에 null 문자 추가
            }
        }

        void ReleaseScreenBuf() {
            for (int i = 0; i < height; i++) {
                delete[] screenBuf[i];
            }
            delete[] screenBuf;
        }

        void DrawScreenBuf() {
            for (int i = 0; i < height; i++) {
                std::cout << screenBuf[i] << std::endl;
            }
        }
    };
}

class Component {
public:
    virtual void Start() {}
    virtual void Update() {}
};

int main() {
    int game_state = 0;
    int game_is_running = 1;
    int n1{}, n2{};
    int x_pos = 0;
    int y_pos = 0;

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

    while (game_is_running) {
        char key_input = '0';
        switch (game_state) {
        case 0:
            key_input = _getch();
            if (key_input == '1') {
                game_state = 1;  // 게임 시작 상태로 변경
            }
            else if (key_input == '4' || key_input == 27) {
                game_is_running = 0;  // 게임 종료 상태로 변경
            }
            break;
        case 1:
            printf("좌표를 입력하세요: \n");
            scanf_s("%d%d", &n1, &n2);
            x_pos = n1 - 1;
            y_pos = n2 - 1;
            {
                MuSoeun::Scene scene;
                scene.width = x_pos * 2 + 2;
                scene.height = y_pos + 2;
                scene.InitScreenBuf();

                MuSoeun::SnakeBody snakeBody1(x_pos, y_pos);

                scene.objList.push_back(&snakeBody1);

                for (const auto& obj : scene.objList) {
                    obj->Render(scene.screenBuf);
                }

                scene.DrawScreenBuf();
                scene.ReleaseScreenBuf();
            }
            game_state = 0;  // 게임 시작 후 게임 상태 초기화
            break;
        }
    }
    return 0;
}
