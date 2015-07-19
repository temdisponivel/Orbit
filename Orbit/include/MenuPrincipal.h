#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include "Headers.h"
#include "Game.h"
#include "Timer.h"
#include "Nave.h"

class MenuPrincipal : public Fase
{
    public:
        MenuPrincipal();
        void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        void Desenha();
        void IniciarNivel();
        virtual ~MenuPrincipal();
    private:
        void TrataMenuPrincipal();
        void TrataScore();
        void TrataDificuldade();
        void TrataListaNaves();
        void CriaListaScore();
        void CriaMenuDificuldade();
        void CriaListaNaves();
        void AjustaPosicaoScore(int somaY);
        void DesenhaHUD();
        Menu* scores;
        Menu* menuDificuldade;
        ListaNaves* listaNaves;
        Save* listaScore;
        bool bMostraScore;
        bool bMostraDificuldade;
        bool bMostraNaves;
        int iQuantScore;
        int iIntervaloScore;
        int iQuantSelecao;
        int iTotalSelecao;
        char* textoScores;
        char** textoNave;
        int iTotalNavesJogo;
        int* iNavesSelecionadas;
        Nave** navesSelecionadas;
        SDL_Rect posicaoListaScore;
        SDL_Rect tamanhoListaScore;
        Timer timerScore;
};

#endif // MENUPRINCIPAL_H
