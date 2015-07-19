#ifndef FASEMENU_H
#define FASEMENU_H
#include "Headers.h"
#include "Fase.h"
#include "Game.h"

class FaseMenu : public Fase
{
    public:
        FaseMenu();
        void IniciarNivel();
        void Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame);
        void Desenha();
        virtual ~FaseMenu();
    private:
};

#endif // FASEMENU_H
