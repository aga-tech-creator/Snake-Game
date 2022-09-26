#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int loose, move, width, high;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
typedef struct snake
{
    COORD *head;
    int tamanho;
} SNAKE;

void gotoxy(short x, short y);
void DrawBoard();
void DrawSnake(SNAKE *snk);
void DrawFood(COORD *food);
SNAKE *initSnake();
COORD *initFood();
void input();
void UpdatePositionSnk(SNAKE *snk);
void ClearScreen();
void hideCursor();
void GetFood();
void GenerateFood();

int main()
{
    hideCursor();
    loose = 0;
    width = 40;
    high = 20;
    SNAKE *snk = initSnake();
    COORD *food = initFood();
    system("cls");
    move = RIGHT;

    while (!loose)
    {
        input();
        UpdatePositionSnk(snk);
        GetFood(snk, food);
        DrawBoard();
        DrawSnake(snk);
        DrawFood(food);
        Sleep(15);
        ClearScreen(snk, food);
    }
    gotoxy(40, 20);
    return 0;
}

void gotoxy(short x, short y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void ClearScreen(SNAKE *snk, COORD *food)
{
    // Clear the snake.
    for (register int i = 0; i < snk->tamanho; i++)
    {
        gotoxy(snk->head[i].X, snk->head[i].Y);
        printf(" ");
    }

    // Clear the food.
    gotoxy(food->X, food->Y);
    printf(" ");
}

void input()
{
    if (kbhit())
    {
        switch (getch())
        {
        case 'w':
            if (move != DOWN)
                move = UP;
            break;
        case 's':
            if (move != UP)
                move = DOWN;
            break;
        case 'a':
            if (move != RIGHT)
                move = LEFT;
            break;
        case 'd':
            if (move != LEFT)
                move = RIGHT;
            break;
        }
    }
}

void DrawBoard()
{
    for (register int i = 0; i < high - 1; i++)
    {
        for (register int j = 0; j < width; j += width - 1)
        {
            gotoxy(j, i);
            printf("#");
        }
    }

    for (register int i = 0; i < high; i += high - 1)
    {
        for (register int j = 0; j < width; j++)
        {
            gotoxy(j, i);
            printf("#");
        }
    }
}

void DrawSnake(SNAKE *snk)
{
    for (register int i = 0; i < snk->tamanho; i++)
    {
        gotoxy(snk->head[i].X, snk->head[i].Y);
        printf("0");
    }
}

void DrawFood(COORD *food)
{
    gotoxy(food->X, food->Y);
    printf("*");
}

SNAKE *initSnake()
{
    SNAKE *auxSnk = malloc(sizeof(SNAKE));
    auxSnk->tamanho = 1;
    auxSnk->head = malloc(50 * sizeof(COORD));
    auxSnk->head[0].X = 12;
    auxSnk->head[0].Y = 8;
    return auxSnk;
}

COORD *initFood()
{
    COORD *auxF = malloc(sizeof(COORD));
    auxF->X = 10;
    auxF->Y = 10;
    return auxF;
}

void GenerateFood(COORD *food)
{
    srand(time(NULL));
    food->X = (rand() % (width - 2)) + 1;
    srand(time(NULL));
    food->Y = (rand() % (high - 2)) + 1;
}

void GetFood(SNAKE *snk, COORD *food)
{
    if ((snk->head[0].X == food->X) && (snk->head[0].Y == food->Y))
    {
        snk->head[snk->tamanho] = snk->head[snk->tamanho - 1];
        snk->tamanho++;

        GenerateFood(food);
    }
    else
        return;
}

void UpdatePositionSnk(SNAKE *snk)
{
    COORD ancestral = snk->head[0];
    COORD new;

    switch (move)
    {
    case UP:
        if ((snk->head->Y - 1) < 1)
            snk->head->Y = high - 2;
        else
            snk->head->Y--;
        break;
    case DOWN:
        if ((snk->head->Y + 1) > high - 2)
            snk->head->Y = 1;
        else
            snk->head->Y++;
        break;
    case LEFT:
        if ((snk->head->X - 1) < 1)
            snk->head->X = width - 2;
        else
            snk->head->X--;
        break;
    case RIGHT:
        if ((snk->head->X + 1) > width - 2)
            snk->head->X = 1;
        else
            snk->head->X++;
        break;
    }

    for (register int i = 1; i < snk->tamanho; i++)
    {
        // When the snake meet yourself.
        if ((snk->head[0].X == snk->head[i].X) && (snk->head[0].Y == snk->head[i].Y))
        {
            loose = 1;
            return;
        }

        new = snk->head[i];
        snk->head[i] = ancestral;
        ancestral = new;
    }
}