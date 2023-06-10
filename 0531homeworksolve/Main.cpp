#include "stdafx.h"

void SetUp();
bool Update();
void Render();
void Food();

enum Direction
{
    UP = 72,
    LEFT = 75,
    RIGHT = 77,
    DOWN = 80
};

struct Pos
{
    int x;
    int y;
};

const int boardRowMax = 20;
const int boardColMax = 30;

const float delaySec = 0.2f;

std::list<Pos> snake;

Direction snakeDir;

Pos foodPos;
Stopwatch currentTime;

int main()
{
    system("pause");

    SetUp();

    while (Update())
    {
        Render();
    }

    return 0;
}

void SetUp()
{
    {
        HANDLE hConsole;
        CONSOLE_CURSOR_INFO ConsoleCursor;

        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        ConsoleCursor.bVisible = 0;
        ConsoleCursor.dwSize = 1;

        SetConsoleCursorInfo(hConsole, &ConsoleCursor);
    }

    Pos startPos = { boardColMax / 2, boardRowMax / 2 };
    snake.push_back(Pos{ startPos.x, startPos.y });
    snake.push_back(Pos{ startPos.x - 1, startPos.y - 1 });
    snake.push_back(Pos{ startPos.x - 2, startPos.y - 2 });

    snakeDir = RIGHT;

    Food();
}

bool Update()
{
    currentTime.Start();

    Pos next = snake.front();

    {
        while (currentTime.GetElapsedTimeSec() < delaySec)
        {
            if (_kbhit() == true)
            {
                int key = _getch();

                if (key == 224)
                {
                    key = _getch();

                    switch (key)
                    {
                    case UP: snakeDir = UP; break;
                    case DOWN: snakeDir = DOWN; break;
                    case LEFT: snakeDir = LEFT; break;
                    case RIGHT: snakeDir = RIGHT; break;
                    }
                }
            }
        }

        switch (snakeDir)
        {
        case UP: next.y--; break;
        case DOWN: next.y++; break;
        case LEFT: next.x--; break;
        case RIGHT: next.x++; break;
        }
    }

    {
        if (next.x <= 0 || next.x > boardColMax - 2) return false;
        if (next.y <= 0 || next.y > boardRowMax - 2) return false;

        //for (const auto& body : snake)
        //{
        //    if (next.x == body.x && next.y == body.y) return false;
        //}

        if (foodPos.x == next.x && foodPos.y == next.y)
        {
            snake.push_back(snake.back());
            Food();
        }
    }

    snake.push_front(next);
    snake.pop_back();

    return true;
}

void gotoxy(int x, int y)
{
    COORD pos = { x * 2, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Render()
{
    system("cls");

    for (int row = 0; row < boardRowMax; row++)
    {
        gotoxy(0, row);
        std::cout << "『";
        gotoxy(boardColMax - 1, row);
        std::cout << "『";
    }

    for (int col = 0; col < boardColMax; col++)
    {
        gotoxy(col, 0);
        std::cout << "『";
        gotoxy(col, boardRowMax - 1);
        std::cout << "『";
    }

    //iterator
    for (auto iter = snake.begin(); iter != snake.end(); iter++)
    {
        gotoxy(iter->x, iter->y);

        if (iter == snake.begin())
            std::cout << "【";
        else
            std::cout << "』";
    }

    {
        static int score = 0;
        score += 100;

        gotoxy(0, boardRowMax);
        std::cout << "SCORE : " << score << std::endl;
    }

    {
        gotoxy(foodPos.x, foodPos.y);
        std::cout << "−";
    }
}

void Food()
{
    foodPos.x = Math::Random(1, boardColMax - 2);
    foodPos.y = Math::Random(1, boardRowMax - 2);

    for (auto iter = snake.begin(); iter != snake.end(); iter++)
    {
        if (foodPos.x == iter->x && foodPos.y == iter->y)
        {
            iter = snake.begin();
            foodPos.x = Math::Random(1, boardColMax - 2);
            foodPos.y = Math::Random(1, boardRowMax - 2);
        }
    }
}


