#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"

salvarjogo jogo;

// save game
void salvar_jogo(salvarjogo *save) {
    FILE *arquivo = fopen("save.dat", "wb");
    if (!arquivo) {
        return;
    }

    fwrite(save, sizeof(salvarjogo), 1, arquivo);
    fclose(arquivo);
}

// load game
int carregar_jogo(salvarjogo *save) {
    int n = 0;
    FILE *arquivo = fopen("save.dat", "rb");
    if (!arquivo) {
        return 0;
    }

    salvarjogo *tmp = malloc(sizeof(salvarjogo));
    n = fread(tmp, sizeof(salvarjogo), 1, arquivo);
    if (n != 1) {
        free(tmp);
        fclose(arquivo);
        return 0;
    }
    memcpy(save, tmp, sizeof(salvarjogo));
    free(tmp);
    fclose(arquivo);

    // save na struct global (save.h)
    jogo = *save;

    return 1;
}


