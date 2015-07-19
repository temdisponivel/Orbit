#ifndef INIMIGO_H
#define INIMIGO_H
#include "Headers.h"
#include "GameObject.h"
#include "Estruturas.h"
#include "Tiro.h"
#include "Game.h"

class Inimigo : public GameObject
{
    public:
        Inimigo(char *caminhoImagem, INIMIGOS TIPO_INIMIGO);
        void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        void Desenha();
        void Atirar();
        void SetIntervaloTiro(int iNovoIntervalo);
        INIMIGOS GetTipoInimigo();
        int GetPontosPorMorte();
        void SetPontosPorMorte(int iPontos);
        TIROS GetTipoTiro();
        void SetTipoTiro(TIROS tiro);
        int GetIntervaloTiro();
        bool ControleTimers(bool bPausar);
        COMPORTAMENTO_INIMIGO GetComportamento();
        void SetComportamento(COMPORTAMENTO_INIMIGO novoComportamento);
        virtual ~Inimigo();
    private:
        Timer timerTiro;
        int iIntervaloTiro;
        int iPontoPorMorte;
        COMPORTAMENTO_INIMIGO comportamento;
        INIMIGOS tipoInimigo;
        TIROS tipoTiro;
};

#endif // INIMIGO_H
