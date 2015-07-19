#include "Planeta.h"

Planeta::Planeta(char *caminhoImagem) : GameObject(caminhoImagem, OBJETO_PLANETA)
{
    //SETA PROPRIEDADES PADRAO PRO PLANETA
    iHP = 100;
    bEscudoTotal = false;

    //SETA TAMANHO DO FRAME DA ANIMACAO
    SDL_Rect r;
    r.h = 216;
    r.w = 213;

    //CONTROI A ANIMACAO E PAUSA
    ControiAnimacao(&r, 1);
    animacao.Pausar(true);

    //SETA O CENTRO DE MASSA COMO O CENTRO DA TELA
    pp.SetCentroDeMassa(Game::CentralizaObjetoTela(this));

    //SETAO TEMPO DE REGENERACAO E O INTERVALO COM QUE SE REGENETA E O VALOR QUE REGENERA A CADA ITERACAO
    iIntervaloRegeneracao = 5000;
    iIntervaloCadaReg = 200;
    iRegeneracao = 1;
}

//FUNCAO CHAMADA EM TODO GAMELOOP
void Planeta::Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame)
{
    //ATUALIZA ANIMACAO
    GameObject::Atualiza(iDeltaTime, iFrame);


    //SE JÁ PASSOU O INTERVALO DE REGENERACAO DESDE A ULTIMA
    if (timerUltimoDano.get_ticks() >= iIntervaloRegeneracao)
    {
        //SE VAI REGENERAR PRA UM VALOR MENOR QUE 100 E JÁ PASSOU O TEMPO ENTRE CADA ITERACAO
        if (iHP + iRegeneracao <= 100 && timerRegeneracao.get_ticks() >= iIntervaloCadaReg)
        {
            //REGENERA O HP
            iHP += iRegeneracao;
            timerRegeneracao.start();
        }

        //SE O HP FOR IGUAL A 100, PARA TODOS OS TIMER
        if (iHP == 100)
        {
            timerRegeneracao.stop();
            timerUltimoDano.stop();
        }
    }

    //PEGA PROXIMO CARTESIANO
    pc = pp.GetCartesianoCorrespondente();

    //SE O HP CHEGOU A ZERO, PERDE A PARTIDA
    if (iHP <= 0)
        Game::FimDaPartida(false);
}

//FUNCAO CHAMADA A TODO GAMELOOP
void Planeta::Desenha()
{
    GameObject::Desenha();
}

//FUNCAO SOBREESCRITA DE TOMA DANO, TROCA A IMAGEM DA ANIMACAO
void Planeta::SetTomaDano(bool bDano)
{
    bTomaDano = bDano;

    animacao.SetFrameAtual(!bDano, true);
}

//FUNCAO SOBRESCRITA PARA QUE, SE O NOVO HP FOR MENOR QUE O ANTERIOR, RESETA TIMER DE REGENERACAO PQ TOMOU DANO
void Planeta::SetHP(int novoHP)
{
    if (novoHP < iHP)
    {
        timerUltimoDano.start();
        timerRegeneracao.start();
    }

    iHP = novoHP;
}

//FUNCAO DE CONTROLE DE TIMER QUE PAUSA OU DISPAUSA
bool Planeta::ControleTimers(bool bPausar)
{
    if (bPausar)
    {
        timerRegeneracao.pause();
        timerUltimoDano.pause();
    }
    else
    {
        timerRegeneracao.unpause();
        timerUltimoDano.unpause();
    }
}

Planeta::~Planeta()
{
    //dtor
}
