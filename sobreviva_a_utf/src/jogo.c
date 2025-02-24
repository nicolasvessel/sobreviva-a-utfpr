#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include "recursos.h"
#include "inicializacao.h"
#include "jogo.h"
#include "menu_principal.h"
#include "snake.h" // parte bruna
#include "logointro.h" // parte bruna
#include "save.h"

// inicializacao das funcoes
void introducao();
void mostrar_opcoes();
void verificar_portal();
void sala_calculo();
void biblioteca();
void sala_jana();
void ru();
void exame_especial();
void restart_jogo();

int opcao_jogo = 0;

int main() {
    srand(time (NULL)); // numeros aleatorios (tempo)
    initscr(); // inicializa a tela (curses)
    keypad(stdscr, TRUE); // teclado
    noecho();
    cbreak();
    curs_set(0); // cursor (false)

    // suporte a cores
    start_color();

    inic_menu(); // chama o menu (menu_principal.c)

    clear();
    refresh();
    endwin();
    return 0;
}

// reseta o jogo
void restart_jogo() {
    // reseta tudo
    nota = 0;
    sanidade = 15;
    sala_calculo_visitada = 0;
    biblioteca_visitada = 0;
    sala_jana_visitada = 0;
    ru_visitado = 0;
    portal_visitado = 0;
    notac = 0;
    notag = 0;
}

// selecao de opcao dentro do jogo
void jogo_opcao(int ch) {
    // switch com condicoes para as opcoes e para pular ja visitadas
    switch (ch) {
        case KEY_UP: // seta cima
            do {
                opcao_jogo--;
                if (opcao_jogo < 0) {
                    opcao_jogo = 5; // volta para a ultima opcao
                }
            } while ((opcao_jogo == 0 && sala_calculo_visitada) || (opcao_jogo == 1 && biblioteca_visitada) || (opcao_jogo == 2 && sala_jana_visitada) || (opcao_jogo == 3 && ru_visitado) || (opcao_jogo == 4 && portal_visitado));
            break;

        case KEY_DOWN: // seta baixo
            do {
                opcao_jogo++;
                if (opcao_jogo > 5) {
                    opcao_jogo = 0; // volta para a primeira opcao
                }
            } while ((opcao_jogo == 0 && sala_calculo_visitada) || (opcao_jogo == 1 && biblioteca_visitada) || (opcao_jogo == 2 && sala_jana_visitada) || (opcao_jogo == 3 && ru_visitado) || (opcao_jogo == 4 && portal_visitado));
            break;
    }
}

// verifica as opcoes ainda nao visitadas
void mostrar_opcoes() {
    init_pair(12, COLOR_YELLOW, COLOR_RED);

    attron(COLOR_PAIR(12));
    mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "PARA ONDE VOCE QUER IR?");
    attroff(COLOR_PAIR(12));

    for (int i = 0; i < 6; i++) {
        if (i == opcao_jogo) {
            attron(A_REVERSE); // destaca a opção selecionada (inversao de cores)
        }
    
        switch (i)
        {
        case 0:
        if (!sala_calculo_visitada)
        {
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2 , "Sala de calculo");
        }
        break;
    
        case 1:
        if (!biblioteca_visitada) {
        mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Biblioteca");
        }
        break;

        case 2:
        if (!sala_jana_visitada) {
            mvprintw((LINES / 2) + 1, (COLS - 40) / 2 , "Sala da Jana");
        }
        break;
    
        case 3:
        if (!ru_visitado) {
            mvprintw((LINES / 2) + 2, (COLS - 40) / 2 , "R.U");
        }
        break;

         case 4:
        if (!portal_visitado){
            mvprintw((LINES / 2) + 3, (COLS - 40) / 2 , "Portal do aluno");
        }
        break;
    
        case 5:
        mvprintw((LINES / 2) + 4, (COLS - 40) / 2 , "SALVAR (SAIR)");
        break;
    }
    attroff(A_REVERSE); // remove o destaque
    }
}

void introducao() {
    clear();
    init_pair(7, COLOR_CYAN, COLOR_WHITE);

    mvprintw((LINES / 2) - 4, (COLS - 40) / 2 , "Voce estava descansando nos bancos da UTFPR");
    mvprintw((LINES / 2) - 3, (COLS - 40) / 2 , "Quando se da conta de que o fim do semestre esta chegando...");
    mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "O que seria bom, se voce fosse um bom aluno.");
    mvprintw((LINES / 2) - 1, (COLS - 40) / 2 , "Voce abre o portal do aluno, e percebe que para passar, voce precisa de:");

    attron(COLOR_PAIR(7));
    mvprintw((LINES / 2) + 1, (COLS - 40) / 2 , "Objetivos:");
    mvprintw((LINES / 2) + 2, (COLS - 40) / 2 , "Voce precisa de 5 pontos para passar!");
    mvprintw((LINES / 2) + 3, (COLS - 40) / 2 , "Ira conseguir ou precisara do temido exame especial?");
    mvprintw((LINES / 2) + 4, (COLS - 40) / 2 , "BOA SORTE, voce vai precisar!");
    attroff(COLOR_PAIR(7));

    refresh();
    esperar_enter();
}

void sala_calculo() {
    clear();
    atualizar_informacoes(); // status bar
    init_pair(9, COLOR_RED, COLOR_WHITE);
    init_pair(10, COLOR_GREEN, COLOR_WHITE);

    mvprintw((LINES / 2) - 3, (COLS - 40) / 2 , "Voce sobe para o segundo andar da UTF e entra na classe da professora Marcia.");
    mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "Deseja realizar a prova? (1 para Sim, 2 para Nao)");
    mvprintw((LINES / 2) - 1, (COLS - 40) / 2 , "Dica: Voce pode adquirir um bonus se for na biblioteca antes!");
    refresh();
    int escolha;
    timeout (-1); // desabilita timeout do scanw
    scanw("%d", &escolha);

    if (escolha == 1) {
        mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Voce iniciou o teste, RESOLVA:");
        int resposta;

        mvprintw((LINES / 2) + 1, (COLS - 40) / 2 , "1. 159 + 27? ");
        scanw("%d", &resposta);
        if (resposta == 186) {
            attron(COLOR_PAIR(10));
        mvprintw((LINES / 2) + 2, (COLS - 40) / 2 , "Correto! (+1 nota)");
            attroff(COLOR_PAIR(10));
            notac += 1;
        } else {
            attron(COLOR_PAIR(9));
        mvprintw((LINES / 2) + 2, (COLS - 40) / 2 , "Errado! (-5 sanidade)");
            attroff(COLOR_PAIR(9));
            sanidade -= 5;
        }

        mvprintw((LINES / 2) + 3, (COLS - 40) / 2 , "2. 25 x 7? ");
        scanw("%d", &resposta);
        if (resposta == 175) {
            attron(COLOR_PAIR(10));
            mvprintw((LINES / 2) + 4, (COLS - 40) / 2 , "Correto! (+1 nota)");
            attroff(COLOR_PAIR(10));
            notac += 1;
        } else {
            attron(COLOR_PAIR(9));
            mvprintw((LINES / 2) + 4, (COLS - 40) / 2 , "Errado! (-5 sanidade)");
            attroff(COLOR_PAIR(9));
            sanidade -= 5;
        }

        mvprintw((LINES / 2) + 5, (COLS - 40) / 2 , "3. Lim x^2 - 4 / x - 2 (x -> 2): ");
        scanw("%d", &resposta);
        if (resposta == 4) {
            attron(COLOR_PAIR(10));
            mvprintw((LINES / 2) + 6, (COLS - 40) / 2 , "Correto! (+1 nota)");
            attroff(COLOR_PAIR(10));
            notac += 1;
        } else {
            attron(COLOR_PAIR(9));
            mvprintw((LINES / 2) + 6, (COLS - 40) / 2 , "Errado! (-5 sanidade)");
            attroff(COLOR_PAIR(9));
            sanidade -= 5;
        }

        if (sanidade > 0) {
            attron(COLOR_PAIR(10));
            mvprintw((LINES / 2) + 8, (COLS - 40) / 2 , "Voce completou a licao e ganhou %d de nota!", notac);
            attroff(COLOR_PAIR(10));
        } else {
            attron(COLOR_PAIR(9));
            mvprintw((LINES / 2) + 8, (COLS - 40) / 2 , "Voce perdeu muita sanidade...");
            attroff(COLOR_PAIR(9));
        }

        // marca a sala como visitada
        sala_calculo_visitada = 1;
    } else {
        mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Voce decidiu nao fazer a prova.");
    }

    refresh();
    esperar_enter();
}

void biblioteca() {
    clear();
    atualizar_informacoes(); // status bar
    init_pair(9, COLOR_RED, COLOR_WHITE);
    init_pair(10, COLOR_GREEN, COLOR_WHITE);

    mvprintw((LINES / 2) - 3, (COLS - 40) / 2 , "Voce entra na biblioteca e percebe diversos estudantes focados.");
    mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "Voce sabe que estudar pode te ajudar... ou te deixar mais confuso.");
    mvprintw((LINES / 2) - 1, (COLS - 40) / 2 , "Deseja estudar? (1 para Sim, 2 para Nao)");
    refresh();

    int escolha;
    timeout (-1); // desabilita timeout do scanw
    scanw("%d", &escolha);

    if (escolha == 1) {
        int dado = rolar_dado(20);
        if (dado > 10) {
            mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Durante seus estudos, voce resolve o ultimo exercicio da lista que era:");
            mvprintw((LINES / 2) + 1, (COLS - 40) / 2 , "Lim x^2 - 4/ x - 2 (x ->2)");
            attron(COLOR_PAIR(10));
            mvprintw((LINES / 2) + 2, (COLS - 40) / 2 , "Voce lembra que a resposta e 4 (+5 sanidade)");
            attroff(COLOR_PAIR(10));
            sanidade += 5;
        } else {
            mvprintw((LINES / 2) + 0, (COLS - 40) / 2 , "Durante seus estudos, voce resolve o ultimo exercicio da lista que era:");
            mvprintw((LINES / 2) + 1, (COLS - 40) / 2 , "Lim x^2 - 4/ x - 2 (x ->2)");
            attron(COLOR_PAIR(9));
            mvprintw((LINES / 2) + 2, (COLS - 40) / 2 , "Voce fica muito cansando mas lembra que a resposta e 4 (-5 sanidade)");
            attroff(COLOR_PAIR(9));
            sanidade -= 5;
        }
    } else {
        mvprintw((LINES / 2) + 3, (COLS - 40) / 2 , "Voce decidiu nao estudar e saiu da biblioteca.");
    }

    // marca a biblioteca como visitada
    biblioteca_visitada = 1;

    refresh();
    esperar_enter();
}

void sala_jana() {
    clear();
    atualizar_informacoes(); // status bar
    init_pair(9, COLOR_RED, COLOR_WHITE);
    init_pair(10, COLOR_GREEN, COLOR_WHITE);
    init_pair(11, COLOR_YELLOW, COLOR_WHITE);

    mvprintw((LINES / 2) - 3, (COLS - 40) / 2 , "Voce entra na sala da Jana, com um pouco de receio...");
    mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "Deseja implorar por pontos extras? (1 para Sim, 2 para Nao)");
    refresh();
    int escolha;
    timeout (-1); // desabilita timeout do scanw
    scanw("%d", &escolha);

    if (escolha == 1) {
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2 , "Voce tenta argumentar com a Jana...");
        int dado = rolar_dado(20);
        if (dado <= 6) {
            attron(COLOR_PAIR(9));
            mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Jana: Seus estudos sao sua responsabilidade! (-5 sanidade)");
            attroff(COLOR_PAIR(9));
            sanidade -= 5;
        } else if (dado <= 15) {
            attron(COLOR_PAIR(11));
            mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Jana: Vou te ajudar com 1 ponto. (-2 sanidade, +1 nota)");
            attroff(COLOR_PAIR(11));
            sanidade -= 2;
            notag += 1;
        } else {
            attron(COLOR_PAIR(10));
            mvprintw((LINES / 2) - 0, (COLS - 40) / 2 , "Jana: Voce se esforcou... Vou te dar 2 pontos. (+2 nota)");
            attroff(COLOR_PAIR(10));
            notag += 2;
        }

        // marca a sala da Jana como visitada
        sala_jana_visitada = 1;
    } else {
        mvprintw((LINES / 2) + 1, (COLS - 40) / 2 , "Voce decidiu nao pedir ajuda e saiu da sala.");
    }

    refresh();
    esperar_enter();
}

void ru() {
    clear();
    atualizar_informacoes(); // status bar
    init_pair(9, COLOR_RED, COLOR_WHITE);
    init_pair(10, COLOR_GREEN, COLOR_WHITE);
    init_pair(11, COLOR_YELLOW, COLOR_WHITE);

    mvprintw((LINES / 2) - 3, (COLS - 40) / 2 , "Voce decide passar no R.U para fazer um lanche.");
    int dado = rolar_dado(20);

    if (dado <= 5) {
        attron(COLOR_PAIR(9));
        mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "Voce esqueceu sua carteira e comeu um sanduiche abandonado...");
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2 , "Voce morreu de intoxicacao alimentar)");
        attroff(COLOR_PAIR(9));
        sanidade = 0;
    } else if (dado <= 10) {
        attron(COLOR_PAIR(11));
        mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "Voce comeu algo com lactose e se sentiu mal. (-2 sanidade)");
        attroff(COLOR_PAIR(11));
        sanidade -= 2;
    } else {
        attron(COLOR_PAIR(10));
        mvprintw((LINES / 2) - 2, (COLS - 40) / 2 , "Voce teve uma otima refeicao: pao de queijo e suco de laranja! (+5 sanidade)");
        attroff(COLOR_PAIR(10));
        sanidade += 5;
    }

    // marca o ru como visitado
    ru_visitado = 1;

    refresh();
    esperar_enter();
}

void exame_especial() {
    clear();

    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // exame intro
    init_pair(2, COLOR_WHITE, COLOR_MAGENTA); // dado
    init_pair(3, COLOR_BLACK, COLOR_GREEN);  // passar
    init_pair(4, COLOR_BLACK, COLOR_RED);  // reprovar

    attron(COLOR_PAIR(1));
    mvprintw((LINES / 2) - 4, (COLS - 40) / 2, "Voce escolheu a sorte");
    attroff(COLOR_PAIR(1));

    mvprintw((LINES / 2) - 3, (COLS - 40) / 2, "Pressione Enter para rolar o dado");
    refresh();
    while (getch() != '\n');

    int dado = rolar_dado(20);

    attron(COLOR_PAIR(2));
    mvprintw((LINES / 2) - 2, (COLS - 40) / 2, "VOCE ROLOU: %d", dado);
    attroff(COLOR_PAIR(2));

    if (dado > 14) {
        attron(COLOR_PAIR(3));
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2, "VOCE PASSOU, PARABENS!");
        attroff(COLOR_PAIR(3));
        mvprintw((LINES / 2), (COLS - 40) / 2, "Pressione ENTER para continuar");
        refresh();
        while (getch() != '\n');
        flushinp();
        endwin();
        inic_menu();
    } else {
        attron(COLOR_PAIR(4));
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2, "VOCE REPROVOU, TENTE NOVAMENTE NO PROXIMO SEMESTRE!");
        attroff(COLOR_PAIR(4));
        mvprintw((LINES / 2), (COLS - 40) / 2, "Pressione ENTER para continuar");
        refresh();
        while (getch() != '\n');
        flushinp();
        endwin();
        inic_menu();
    }
}

void verificar_portal() {
    atualizar_informacoes(); // status bar

    init_pair(11, COLOR_YELLOW, COLOR_WHITE);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_WHITE, COLOR_RED);
    init_pair(10, COLOR_GREEN, COLOR_WHITE);

    clear();
    mvprintw((LINES / 2) - 3, (COLS - 40) / 2, "Voce decide verificar o portal do aluno para ver sua nota.");

    if (sala_calculo_visitada == 0 || sala_jana_visitada == 0) {
        attron(COLOR_PAIR(11));
        mvprintw((LINES / 2) - 2, (COLS - 40) / 2, "Voce nao fez todas as provas, voce nao pode verificar sua nota ainda.");
        attroff(COLOR_PAIR(11));
        refresh();
        esperar_enter();
        return;
    } else if (nota == 5) {
        attron(COLOR_PAIR(10));
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2, "VOCE PASSOU DE SEMESTRE, ESSA FOI POR POUCO!");
        attroff(COLOR_PAIR(10));
        refresh();
        esperar_enter();
        flushinp();
        endwin();
        inic_menu();
    } else {
        mvprintw((LINES / 2) - 1, (COLS - 40) / 2, "Voce nao passou de semestre...");
        mvprintw((LINES / 2), (COLS - 40) / 2, "Voce prefere tentar a sorte ou enfrentar o desafio final?");

        int escolha = 0;
        while (1) {
            mvprintw((LINES / 2) + 1, (COLS - 40) / 2, "TENTAR A SORTE");
            mvprintw((LINES / 2) + 2, (COLS - 40) / 2, "EXAME ESPECIAL");

            // inversao de cores (destaque)
            if (escolha == 0) {
                attron(COLOR_PAIR(5));
                mvprintw((LINES / 2) + 1, (COLS - 40) / 2, "TENTAR A SORTE");
                attroff(COLOR_PAIR(5));
            } else {
                attron(COLOR_PAIR(6));
                mvprintw((LINES / 2) + 2, (COLS - 40) / 2, "EXAME ESPECIAL");
                attroff(COLOR_PAIR(6));
            }

            refresh();

            int ch = getch();
            if (ch == KEY_UP || ch == KEY_DOWN) {
                escolha = !escolha;
            } else if (ch == '\n') {
                if (escolha == 0) {
                    exame_especial();
                } else {
                    endwin();
                    startfinalboss();
                }
                break;
            }
        }
    }
    refresh();
    esperar_enter();
}

void jogar() {
    // introducao
    animacao();

    introducao();
    restart_jogo();

    // main loop
    while (sanidade > 0) {
        clear();
        atualizar_informacoes(); // atualiza os status no topo da tela
        mostrar_opcoes();

        // verifica a tecla precionada (seta cima e baixo) (ERR = sem tecla) do #ncurses
        int ch = getch();
        if (ch != ERR) {
            if (ch == '\n') {  // enter
                if (!sala_calculo_visitada && opcao_jogo == 0) {  // iniciar opcao 1 
                    clear();
                    flushinp();
                    sala_calculo();
                } else if (!biblioteca_visitada && opcao_jogo == 1) {
                    clear();
                    flushinp();
                    biblioteca();            
                } else if (!sala_jana_visitada && opcao_jogo == 2) {
                    clear();
                    flushinp();
                    sala_jana();
                } else if (!ru_visitado && opcao_jogo == 3) {
                    clear();
                    flushinp();
                    ru();
                } else if (opcao_jogo == 4) { 
                    clear();
                    flushinp();
                    verificar_portal();
                } else if (opcao_jogo == 5) {
                    clear();
                    flushinp();
                    salvar_jogo(&jogo); // linka a funcao para salvar jogo direto ao save jogo.
                    refresh();
                    break;
                }
            } else {
                jogo_opcao(ch);
            }
        }

        if (sanidade <= 0) {
            clear();
            atualizar_informacoes(); // status bar
            init_pair(6, COLOR_WHITE, COLOR_RED);     // morrer
            attron(COLOR_PAIR(6));
            mvprintw((LINES / 2) - 1, (COLS - 40) / 2, "VOCE TRANCOU O CURSO, DE CERTA FORMA...");
            attroff(COLOR_PAIR(6));
            refresh();
            esperar_enter();
            flushinp();
            break; // Exit the loop
        }

        nota = notac + notag;
    }

    clear();
    refresh();
    endwin();
    inic_menu();
}

void jogarsave() {

    // carrega o jogo
    salvarjogo save;
    int carregar = carregar_jogo(&save);
    // detecta se ja tem algum jogo salvo
    if (!carregar) {
        clear();
        mvprintw((LINES / 2), (COLS - 20) / 2, "NUNHUM JOGO SALVO");
        refresh();
        esperar_enter();
        flushinp();
        endwin();
        inic_menu();
        return;
    }

    // introducao
    animacao();
    introducao();

    // main loop
    while (sanidade > 0) {
        clear();
        atualizar_informacoes(); // atualiza os status no topo da tela
        mostrar_opcoes();

        // verifica a tecla precionada (seta cima e baixo) (ERR = sem tecla) do #ncurses
        int ch = getch();
        if (ch != ERR) {
            if (ch == '\n') {  // enter
                if (!sala_calculo_visitada && opcao_jogo == 0) {  // iniciar opcao 1 
                    clear();
                    flushinp();
                    sala_calculo();
                } else if (!biblioteca_visitada && opcao_jogo == 1) {
                    clear();
                    flushinp();
                    biblioteca();            
                } else if (!sala_jana_visitada && opcao_jogo == 2) {
                    clear();
                    flushinp();
                    sala_jana();
                } else if (!ru_visitado && opcao_jogo == 3) {
                    clear();
                    flushinp();
                    ru();
                } else if (opcao_jogo == 4) { 
                    clear();
                    flushinp();
                    verificar_portal();
                } else if (opcao_jogo == 5) {
                    clear();
                    flushinp();
                    salvar_jogo(&jogo); // linka a funcao para salvar jogo direto ao save jogo.
                    refresh();
                    break;
                }
            } else {
                jogo_opcao(ch);
            }
        }
        
        if (sanidade <= 0) {
            clear();
            atualizar_informacoes(); // status bar
            init_pair(6, COLOR_WHITE, COLOR_RED);     // morrer
            attron(COLOR_PAIR(6));
            mvprintw((LINES / 2) - 1, (COLS - 40) / 2, "VOCE TRANCOU O CURSO, DE CERTA FORMA...");
            attroff(COLOR_PAIR(6));
            refresh();
            esperar_enter();
            flushinp();
            endwin();
            inic_menu();
        }

        nota = notac + notag;
    }

    clear();
    refresh();
    endwin();
    inic_menu();
}