#ifndef PLANETA_H
#define PLANETA_H
#include "Headers.h"
#include "GameObject.h"
#include "Game.h"

class Planeta : public GameObject
{
    public:
        Planeta(char *caminhoImagem);
        void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        void Desenha();
        void SetHP(int novoHP);
        void SetTomaDano(bool bDano);
        bool ControleTimers(bool bPausar);
        virtual ~Planeta();
    private:
        int iIntervaloRegeneracao;
        int iIntervaloCadaReg;
        int iRegeneracao;
        bool bEscudoTotal;
        Timer timerUltimoDano;
        Timer timerRegeneracao;
};

#endif // PLANETA_H
