#include <iostream>
#include "Headers.h"
#include "Game.h"

using namespace std;

int main(int arg, char **args)
{
    //incia novo jogo
    Game *g = new Game();

    //se o jogo iniciou corretamente, vai pro gameloop principal
    if (g->IniciaJogo())
    {
        g->GameLoop();
    }
    //se deu erro, retorna -1
    else
    {
        return -1;
    }

    //libera memoria do jogo, onde ele deleta todos os recursos e encerra a SDL
    if (g != NULL)
        delete g;

    //se rodou corretamente retorna 1
    return 1;
}
