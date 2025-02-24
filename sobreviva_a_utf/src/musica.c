#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <curses.h>
#include <pthread.h> // thread pra tocar a musica mesmo fora do menu (antes travava no menu de musica)
#include "musica.h"
#include "menu_principal.h"

int musica_selecionada = 0; //selecao da musica
int musica_tocando = 0; // controle da musica)
const char *musica_atual = NULL; // variavel do arquivo de musica atual

// SDL (audio callback function)
void audio_callback(void *userdata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    SDL_AudioStream *audio_stream = (SDL_AudioStream *)userdata;
    int bytes = SDL_AudioStreamGet(audio_stream, stream, len);
    if (bytes < 0) {
        fprintf(stderr, "SDL_AudioStreamGet error: %s\n", SDL_GetError());
    }
}

// entrada do teclado (setas)
void selecionar_musica(int ch) {
    switch (ch) {
        case KEY_UP: // seta cima
            musica_selecionada--;
            if (musica_selecionada < 0) {
                musica_selecionada = 2; // volta para a ultima opcao
            }
            break;

        case KEY_DOWN: // seta baixo
            musica_selecionada++;
            if (musica_selecionada > 2) {
                musica_selecionada = 0; // volta para a primeira opcao
            }
            break;
    }
}

// thread para tocar a musica (evita travar o menu e mantem a musica tocando fora dele) E verificacao de erros do SDL (opcional)
void *tocar_musica_thread(void *arg) {
    const char *file_path = (const char *)arg;
    SDL_AudioSpec wav_spec;
    Uint8 *wav_buffer;
    Uint32 wav_length;

    if (SDL_LoadWAV(file_path, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        printf("Failed to load WAV file! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_AudioStream *audio_stream = SDL_NewAudioStream(wav_spec.format, wav_spec.channels, wav_spec.freq, wav_spec.format, wav_spec.channels, wav_spec.freq);
    if (audio_stream == NULL) {
        printf("Failed to create audio stream! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeWAV(wav_buffer);
        return NULL;
    }

    if (SDL_AudioStreamPut(audio_stream, wav_buffer, wav_length) < 0) {
        printf("Failed to put audio data into stream! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeWAV(wav_buffer);
        return NULL;
    }

    wav_spec.callback = audio_callback;
    wav_spec.userdata = audio_stream;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
    if (device_id == 0) {
        printf("Failed to open audio device! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeWAV(wav_buffer);
        return NULL;
    }

    SDL_PauseAudioDevice(device_id, 0);

    // continua tocando a musica indefinidamente
    while (musica_tocando) {
        if (SDL_AudioStreamAvailable(audio_stream) == 0) {
            if (SDL_AudioStreamPut(audio_stream, wav_buffer, wav_length) < 0) {
                printf("Failed to put audio data into stream! SDL_Error: %s\n", SDL_GetError());
                break;
            }
        }
        SDL_Delay(100); // espera a musica (anti crash)
    }

    SDL_CloseAudioDevice(device_id);
    SDL_FreeWAV(wav_buffer);
    SDL_FreeAudioStream(audio_stream);

    return NULL;
}

// toca musica
void tocar_musica(const char *file_path) {
    if (musica_tocando && musica_atual && strcmp(musica_atual, file_path) == 0) {
        // se a musica for a mesma nao faz nada
        return;
    }

    // para a musica atual
    musica_tocando = 0;
    SDL_Delay(100); // tempo para parar a musica (anti crash)

    // inicia a nova musica se for diferente da anterior (PATH = caminho)
    musica_atual = file_path;
    musica_tocando = 1;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, tocar_musica_thread, (void *)file_path); // cria a thread
    pthread_detach(thread_id); // desvincula a thread principal
}

// linka a musica selecionada
void opcoes_musica(WINDOW *opcoes_tela, int cor_borda) {
    wclear(opcoes_tela);
    box(opcoes_tela, 0, 0);

    const char *opcoes[] = {
        "Musica 1",
        "Musica 2",
        "Musica 3"
    };

    for (int i = 0; i < 3; i++) {
        if (i == musica_selecionada) {
            wattron(opcoes_tela, A_REVERSE);
        }
        mvwprintw(opcoes_tela, i + 1, 1, opcoes[i]);
        wattroff(opcoes_tela, A_REVERSE);
    }

    wrefresh(opcoes_tela);
}

// as opcaos de musica (menu_principal)
void musica_menu() {
    // ncurses
    initscr();
    keypad(stdscr, TRUE); // inicializa a funcao de teclado
    start_color();
    noecho();
    cbreak();
    curs_set(0); //false

    // define o tamanho e a posicao da borda
    int height = 6, width = 20;
    int start_y = (LINES - height) / 2; // centraliza verticalmente
    int start_x = (COLS - width) / 2;   // centraliza horizontalmente
    WINDOW *opcoes_tela = newwin(height, width, start_y, start_x);

    int estado_atual = 1;

    while (estado_atual) {
        opcoes_musica(opcoes_tela, 1);

        timeout(200); // 0,2 s
        int ch = getch();

        // verifica a tecla precionada (seta cima e baixo) (ERR = sem tecla) do #ncurses
        if (ch != ERR) {
            if (ch == '\n') {  // enter
                switch (musica_selecionada) {
                    case 0:
                        tocar_musica("C:assets/Serge-Quadrado-Alone-at-Night.wav");
                        break;
                    case 1:
                        tocar_musica("C:assets/Joseph-R.-Lilore-NOVEMBER.wav");
                        break;
                    case 2:
                        tocar_musica("C:assets/Serge-Quadrado-Slow-and-Sad-Dance.wav");
                        break;
                }
                estado_atual = 0; // depois de selecionado termina o menu
            } else if (ch == 27) { // ESC
                estado_atual = 0; // sai do menu
            } else {
                selecionar_musica(ch);
            }
        }
    }

    delwin(opcoes_tela);
    endwin();
    inic_menu();
}

// ativa a musica (SDL)
void ativar_musica() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    musica_menu();

   // SDL_Quit(); (finaliza a musica)
}
