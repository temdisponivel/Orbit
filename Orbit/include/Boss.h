#ifndef BOSS_H
#define BOSS_H
#include "GameObject.h"
#include "Game.h"
#include "Headers.h"
#include "Estruturas.h"
#include "Inimigo.h"

class Boss : public GameObject
{
    public:
        Boss(char* caminhoImagem, BOSS b);
        void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        void Desenha();
        bool ControleTimers(bool bPausar);
        virtual ~Boss();
    private:
        void GerenciaOnda();
        void SomaMorteInimigo();
        int iIntervaloInimigos;
        int iQuantInimigosOnda;
        int iQuantInimigosAtual;
        ESTAGIOS_NIVEL estagioAtual;
        int iInimigosJogo;
        Timer timerInimigo;
        Timer timerWave;
        bool bWaveAtiva;
        BOSS tipoBoss;
        Inimigo* inimigo;
        int iIntervaloProximaOnda;
        int iInimigosMortosAteUltimaOnda;
        bool bMovimentar;
};

#endif // BOSS_H
