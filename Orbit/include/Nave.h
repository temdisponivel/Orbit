#ifndef NAVE_H
#define NAVE_H
#include "Headers.h"
#include "GameObject.h"
#include "Estruturas.h"
#include "Timer.h"
#include "Game.h"
#include "Tiro.h"

class Tiro;

class Nave : public GameObject
{
    public:
        Nave(char *caminhoImagem, TIPO_NAVES tipo, JOGADOR jog, Controle controles);
        void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        void Desenha();
        void Atirar();
        void SetIntervaloTiro(int iNovoIntervalo);
        void SetIntervaloEspecial(int iNovoIntervalo);
        void SetTempoEspecial(int iNovoTempo);
        void GerenciaEspecial();
        char *GetNome();
        void SetNome(char *nome);
        TIPO_NAVES GetNave();
        void SetNave(TIPO_NAVES tipo);
        TIROS GetTipoTiro();
        void SetTipoTiro(TIROS tipo);
        Controle& GetControles();
        int GetIntervaloTiro();
        int GetIntervaloEspecial();
        int GetTempoEspecial();
        void SetJogador(JOGADOR j);
        JOGADOR GetJogador();
        void SetEspecial(ESPECIAIS esp);
        ESPECIAIS GetEspecial();
        bool ControleTimers(bool bPausar);
        int GetQuantTiro();
        void SetQuantTiro(int quant);
        virtual ~Nave();
    private:
        void Especial(bool bAtiva);
        TIPO_NAVES tipoNave;
        Controle controle;
        int iIntervaloTiro;
        int iIntervaloEspecial;
        int iTempoEspecial;
        int iQuantTiro;
        bool bEspecialAtivo;
        char *nomeNave;
        Timer timerIntervaloEspecial;
        Timer timerEspecial;
        Timer timerTiro;
        TIROS tipoTiro;
        JOGADOR jogador;
        Nave* naveEspecial;
        ESPECIAIS especial;
    };

#endif // NAVE_H
