#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H
#include "Fase.h"
#include "Game.h"
#include "Estruturas.h"

class Configuracoes : public Fase
{
    public:
        Configuracoes();
        void Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame);
        void Desenha();
        virtual ~Configuracoes();
    private:
        void IniciarNivel();
        void CriaMenuResolusoes();
        void CriaMenuFullScreen();
        void TrataResolusoes();
        void TrataFullScreen();
        void TrataMenu();
        Menu* menuPrincipal;
        Menu* menuResolusoes;
        Menu* menuFullScreen;
        bool bMostraResolusoes;
        bool bMostraFullScreen;
};

#endif // CONFIGURACOES_H
