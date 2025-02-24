#include <curses.h>
#include <unistd.h> // timeout
#include <stdlib.h>
#include "jogo.h"
#include "menu_principal.h"
#include "musica.h"
#include "save.h"

int opcao_selecionada = 0;
extern void jogar();
extern void jogarsave();

// exibe o menu de opcoes
void opcoes_menu(WINDOW *opcoes_tela, int cor_borda) {
    wclear(opcoes_tela);

    // cria a borda
    wattron(opcoes_tela, COLOR_PAIR(cor_borda));
    box(opcoes_tela, 0, 0);
    wattroff(opcoes_tela, COLOR_PAIR(cor_borda));

    // opcoes de selecao do menu
    for (int i = 0; i < 5; i++) {
        if (i == opcao_selecionada) {
            wattron(opcoes_tela, A_REVERSE); // destaca a opção selecionada (inversao de cores)
        }

        // exibe cada opção do menu
        switch (i) {
            case 0:
                mvwprintw(opcoes_tela, 1, 1, "Novo Jogo");
                break;
            case 1:
                mvwprintw(opcoes_tela, 2, 1, "Carregar Jogo");
                break;
            case 2:
                mvwprintw(opcoes_tela, 3, 1, "Musica");
                break;
            case 3:
                mvwprintw(opcoes_tela, 4, 1, "Creditos");
                break;
            case 4:
                mvwprintw(opcoes_tela, 5, 1, "Sair");
                break;
        }

        wattroff(opcoes_tela, A_REVERSE); // remove o destaque
    }

    wrefresh(opcoes_tela);
}

// entrada do teclado (setas)
void selecionar_opcao(int ch) {
    switch (ch) {
        case KEY_UP: // seta cima
            opcao_selecionada--;
            if (opcao_selecionada < 0) {
                opcao_selecionada = 4; // volta para a ultima opcao
            }
            break;

        case KEY_DOWN: // seta baixo
            opcao_selecionada++;
            if (opcao_selecionada > 4) {
                opcao_selecionada = 0; // volta para a primeira opcao
            }
            break;
    }
}

// menu pricipal
void inic_menu() {
    // ncurses
    initscr();
    keypad(stdscr, TRUE); // inicializa a funcao de teclado
    start_color();
    noecho();
    cbreak();
    curs_set(0); //false

    // cores para a borda
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);

    // define o tamanho e a posicao da borda
    int height = 7, width = 20;
    int start_y = (LINES - height) / 2; // centraliza verticalmente
    int start_x = (COLS - width) / 2;   // centraliza horizontalmente
    WINDOW *opcoes_tela = newwin(height, width, start_y, start_x);

    nodelay(opcoes_tela, TRUE); // loop continua mesmo sem tecla ser apertada

    int cor_atual = 1;
    int tempo_mudanca_cor = 0;
    int estado_atual = 1;

    while (estado_atual) {
        opcoes_menu(opcoes_tela, cor_atual);

        timeout(200); // 0,2 s
        int ch = getch();

        // verifica a tecla precionada (seta cima e baixo) (ERR = sem tecla) do #ncurses
        if (ch != ERR) {
            if (ch == '\n') {  // enter
                if (opcao_selecionada == 0) {  // iniciar opcao 1 (jogo)
                    estado_atual = 0;  // sai do loop
                    delwin(opcoes_tela);
                    endwin();
                    flushinp();
                    jogar();  // chama a funcao do jogo (jogo.c)
                    return;  // sai do loop

                } else if (opcao_selecionada == 1) {  // carregar jogo
                    if (opcao_selecionada == 1) {
                        estado_atual = 0;  // sai do loop
                        delwin(opcoes_tela);
                        endwin();
                        flushinp();
                        jogarsave();  // chama a funcao do jogo (jogo.c)
                        return;  // sai do loop
                    }

                } else if (opcao_selecionada == 2) {
                    erase(); //alternativa ao clear
                    ativar_musica();
                    refresh();

                } else if (opcao_selecionada == 3) {
                    int height = 7, width = 70; // tamanho
                    int start_y = (LINES - height) / 2; // centraliza verticalmente
                    int start_x = (COLS - width) / 2;   // centraliza horizontalmente
                    WINDOW *creditos_win = newwin(height, width, start_y, start_x);

                    wclear(creditos_win);
                    box(creditos_win, 0, 0);

                    mvwprintw(creditos_win, 1, 1, "Desenvolvido por: Nicolas Vessel, Bruna Ferraz, Nicoli Otta"); // imprime o texto 
                    mvwprintw(creditos_win, 3, 1, "Musicas por: Joseph R. Lilore & Serge Quadrado"); // imprime o texto

                    wrefresh(creditos_win); // atualiza

                    int ch;
                    while ((ch = wgetch(creditos_win)) != 27) { // 27 is the ASCII (ESC)
                    }

                    delwin(creditos_win); // deleta a janela
                    clear();
                    refresh();
                } else if (opcao_selecionada == 4) {  // Sair
                    delwin(opcoes_tela);
                    endwin();  
                    exit(0);
                }
            } else {
                selecionar_opcao(ch);
            }
        }

        // incrementa a cor
        tempo_mudanca_cor++;
        if (tempo_mudanca_cor >= 5) {
            cor_atual = (cor_atual % 6) + 1; // muda para a próxima cor
            tempo_mudanca_cor = 0;
        } 
    }

    // limpa a tela
    delwin(opcoes_tela);
    endwin();
    return;
}