COMO JOGAR:
Para selecionar as opções dentro das salas, pressione o número e ENTER, demais opções com setas e ENTER. 


REQUISITOS:

MINGW x64
TESTANDO APENAS NO WINDOWS 10


COMPILANDO:
exemplo pelo terminal:

cd C:\Users\nicol\Documents\Códigos\projetos\sobreviva_a_utf\

gcc -o jogo src/jogo.c src/inicializacao.c src/recursos.c src/menu_principal.c src/musica.c src/snake.c src/logointro.c src/save.c -I include -I PDCurses-3.9/wincon -I SDL2-2.30.11/include -L PDCurses-3.9/wincon -L SDL2-2.30.11/lib/x64 -l:pdcurses.a -lSDL2 -mconsole


CRÉDITOS
"logointro.c" e "snake.c" foram feitos pela BRUNA (colaboradora), parcialmente alterada por mim, o restante dos códigos foi feito inteiramente por mim.
