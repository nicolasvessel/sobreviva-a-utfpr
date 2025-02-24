#ifndef SAVE_H
#define SAVE_H

// estrutura para salvar o jogo
typedef struct {
    int sanidade;
    int sala_calculo_visitada;
    int biblioteca_visitada;
    int sala_jana_visitada;
    int ru_visitado;
    int nota;
    int notac;
    int notag;
    int portal_visitado;
} salvarjogo;

void salvar_jogo();
int carregar_jogo();

extern salvarjogo jogo;

#endif