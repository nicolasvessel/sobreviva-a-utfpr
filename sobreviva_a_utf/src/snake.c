#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "jogo.h"

#define WIDTH 30
#define HEIGHT 20
#define WIN_SCORE 200

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int dir;

HANDLE hConsole;
int offsetX, offsetY;

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void GetConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Calculando deslocamento para centralizar
    offsetX = (consoleWidth - WIDTH - 2) / 2;
    offsetY = (consoleHeight - HEIGHT - 4) / 2;
}

void GotoXY(int x, int y) {
    COORD coord;
    coord.X = x + offsetX;
    coord.Y = y + offsetY;
    SetConsoleCursorPosition(hConsole, coord);
}

void Setup() {
    gameOver = 0;
    dir = 2;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
}

void Draw() {
    int textWidth = 45;  // Comprimento do enunciado
    int padding = (WIDTH - textWidth) / 2;
    GotoXY(padding, 0);

    printf("\033[1;34m=====================================================\n");
    GotoXY(padding, 1);
    printf("\033[1;31mVOCE PRECISA DE 200 PONTOS PARA PASSAR DE SEMESTRE!\033[1;34m\n");

    // Desenhando moldura superior
    GotoXY(0, 2);
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        GotoXY(0, i + 3);
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");

            if (i == y && j == x)
                printf("\033[1;33mO\033[1;32m");
            else if (i == fruitY && j == fruitX)
                printf("\033[1;31m@\033[1;32m");
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("\033[1;36mo\033[1;32m");
                        print = 1;
                        break;
                    }
                }
                if (!print) printf(" ");
            }

            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    // moldura inferior
    GotoXY(0, HEIGHT + 3);
    for (int i = 0; i < WIDTH + 2; i++) printf("#");

    GotoXY(0, HEIGHT + 5);
    printf("\033[1;37mPontuacao: %d\n\033[0m", score);
}

void Input() {
    if (GetAsyncKeyState(VK_LEFT) & 0x8000 && dir != 2) dir = 1;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && dir != 1) dir = 2;
    if (GetAsyncKeyState(VK_UP) & 0x8000 && dir != 4) dir = 3;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000 && dir != 3) dir = 4;
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) gameOver = 1;
}

void Logic() {
    int prevX = tailX[0], prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case 1: x--; break;
        case 2: x++; break;
        case 3: y--; break;
        case 4: y++; break;
    }

    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = 1;
            return;
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }

    if (score >= WIN_SCORE) gameOver = 2;
}

void Historia() {

    system("cls");

    printf("\033[1;37mVoce sobe lances de escadas, ate chegar no terceiro andar do seu bloco...\n");
    Sleep(2000);
    printf("A sala mais temida estava a sua frente.\n");
    Sleep(2000);
    printf("Era hora de enfrentar ele...\n");
    Sleep(2000);
    printf("O professor de programacao.\n");
    Sleep(2000);
    printf("Voce precisa de pontos para passar, e ele sabe disso.\n");
    Sleep(3000);
    printf("\n\033[1;31m\"Ora ora ora, se nao e voce, meu caro aluno...\"\033[0m\n");
    Sleep(3000);
    printf("\033[1;31m\"Sei o porque esta aqui, precisa de nota para passar.\"\033[0m\n");
    Sleep(3000);
    printf("\033[1;31m\"Mas antes tera que me vencer... No meu jogo favorito! HAHAHAHAHA!\"\033[0m\n");
    Sleep(4000);

    //  LIMPA TUDO ANTES DO JOGO INICIAR
    system("cls");
}

void startfinalboss() {
    srand(time(0));

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
    GetConsoleSize();

    Historia();

    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(80);
    }

    // Limpa a tela antes de exibir a mensagem final
    system("cls");

    // Pega o tamanho do console novamente para garantir que está atualizado
    GetConsoleSize();

    // Centraliza a mensagem final
    GotoXY(WIDTH / 2 - 15, HEIGHT / 2);

    if (gameOver == 2) {
        printf("\033[1;32mParabens! Voce passou de semestre !!!\033[0m\n");
    } else {
        printf("\033[1;31mFIM DE JOGO! Voce trancou o curso!\033[0m\n");
    }

    Sleep(4000);
    
    inic_menu();
}