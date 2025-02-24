#include "recursos.h"
#include <stdlib.h>
#include <inicializacao.h>
#include <curses.h>

int rolar_dado(int max) {
    return rand() % max + 1;
}

void esperar_enter() {
    attron(COLOR_PAIR(2)); // fundo com texto preto
    mvprintw((LINES / 2) + 10, (COLS - 40) / 2 , "Pressione Enter para continuar...");
    attroff(COLOR_PAIR(2)); // desliga as cores
    refresh();
    while (getch() != '\n');
}

void atualizar_informacoes() {
    start_color();
    init_pair(8, COLOR_BLACK, COLOR_WHITE); // fundo branco com texto preto
    attron(COLOR_PAIR(8)); // fundo com texto preto
    mvprintw((LINES / 2) - 4, (COLS - 40) / 2 , "Sanidade: %d | Nota: %d", sanidade, nota);
    attroff(COLOR_PAIR(8)); // desliga as cores
    refresh();
}