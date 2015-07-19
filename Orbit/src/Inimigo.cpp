#include "Inimigo.h"

Inimigo::Inimigo(char *caminhoImagem, INIMIGOS inimigo) : GameObject(caminhoImagem, OBJETO_INIMIGO)
{
    //seta propriedades padrao para inimigo
    pc = pp.GetCartesianoCorrespondente();
    vetor.SetMagnitude(0.3);
    tipoInimigo = inimigo;
    iIntervaloTiro = 1000;
    timerTiro.start();
    iHP = 3;
    comportamento = COMPORTAMENTO_UM;
}

//FUNCAO CHAMADA A TODO GAMELOOP
void Inimigo::Atualiza(unsigned int DeltaTime, unsigned long long int iFrame)
{
    //SE O HP CHEGOU A ZERO
    if (iHP <= 0)
    {
        //SOMA PONTUACAO NO SCORE DO JOGADOR E INCREMENTA INIMIGOS MORTOS
        Game::GetSave().SomaPontuacao(iPontoPorMorte);
        Game::IncrementaInimigoMorto(1);

        //COMETE SUICIDIO
        Game::GetFaseAtual()->ExcluiGameObject(this);
        return;
    }

    //SE O COMPORTAMENTO FOR COMPORTAMENTO UM E O RAIO FOR MAIOR QUE A PRIMEIRA ORBITA, DIMINIU O RAIO CONFORME A METADE DA MAGNITUDE
    if (comportamento == COMPORTAMENTO_UM && pp.GetRaio() > Game::GetOrbitas()[0])
    {
        pp.SetRaio(pp.GetRaio() - (vetor.GetMagnitude() / 2));
    }
    //SE FOR O COMPORTAMENTO DOIS E JÁ DEU UMA VOLTA NO SEU CENTRO DE MASSA, TROCA O SENTIDO (HORARIO E ANTIHORARIO)
    else if (comportamento == COMPORTAMENTO_DOIS && pp.GetVoltas() >= 1)
    {
        vetor.SetSentido(vetor.GetSentido() * -1);
        pp.SetVoltas(0);
    }
    //SE O COMPORTAMENTO É O COMPORTAMENTO TRES E JÁ DEU UMA VOLTA, TROCA O SENTIDO E, SE AINDA NÃO ATINGIU A PRIMEIRA ORBITA, CHEGA MAIS PERTO DO PLANETA
    else if (comportamento == COMPORTAMENTO_TRES && pp.GetVoltas() >= 1)
    {
        if (pp.GetRaio() > Game::GetOrbitas()[0])
            pp.SetRaio(pp.GetRaio() - 50);

        vetor.SetSentido(vetor.GetSentido() * -1);
        pp.SetVoltas(0);
    }

    //se estiver na hora, atira
    if (timerTiro.get_ticks() > iIntervaloTiro && Game::DentroJanela(this))
        Atirar();

    //atualiza o angulo com a proxima posicao
    pp.SetAngulo((vetor.GetMagnitude() * vetor.GetSentido()) + pp.GetAngulo());

    //pega o proximo ponto cartesiano
    pc = pp.GetCartesianoCorrespondente();
}

//FUNCAO CHAMADA A TODO GAMELOOP
void Inimigo::Desenha()
{
    //rotaciona imagem
    Game::RotacionaGameObject(this);

    //desenha imagem na tela
    GameObject::Desenha();

    //libera surface temporaria
    SDL_FreeSurface(imagem);

    //reseta surface para a surface backup
    imagem = imagemAuxilia;
}

//funcao para gerar um novo tiro
void Inimigo::Atirar()
{
    //cria novo tiro
    Tiro* tiro = Game::GetTiros(tipoTiro);

    //seta propriedades
    tiro->SetTipo(OBJETO_TIRO_INIMIGO);
    tiro->SetPontoPolar(GetPontoPolar());
    tiro->GetVetor().SetSentido(-1);

    //ADICIONA O NOVO TIRO NA LISTA DE GAMEOBJECT
    Game::GetFaseAtual()->IncluirGameObject(tiro);

    //RESETA TIMER DO TIRO
    timerTiro.start();
}

//SETA UM NOVO INTERVALO DE TIRO
void Inimigo::SetIntervaloTiro(int iNovoIntervalo)
{
    iIntervaloTiro = iNovoIntervalo;
}

//RETORNA O TIPO DE INIMIGO
INIMIGOS Inimigo::GetTipoInimigo()
{
    return tipoInimigo;
}

//RETORNA A QUANTIDADE DE PONTOS POR MORTE DESTE INIMIGO
int Inimigo::GetPontosPorMorte()
{
    return iPontoPorMorte;
}

//SETA UMA NOVA QUANTIDADE DE PONTOS POR MORTE
void Inimigo::SetPontosPorMorte(int iPontos)
{
    iPontoPorMorte = iPontos;
}

//RETORNA O TIPO DE TIRO DESTE INIMIGO
TIROS Inimigo::GetTipoTiro()
{
    return tipoTiro;
}

//SETA UM NOVO TIPO DE TIRO PARA ESTE INIMIGO
void Inimigo::SetTipoTiro(TIROS tiro)
{
    tipoTiro = tiro;
}

//RETORNA O INTERVALO DE TIRO
int Inimigo::GetIntervaloTiro()
{
    return iIntervaloTiro;
}

//PAUSA OU DESPAUSA OS TIMERS DESTE INIMIGO
bool Inimigo::ControleTimers(bool bPausar)
{
    if (bPausar)
        timerTiro.pause();
    else
        timerTiro.unpause();
}

//RETORNA O COMPORTAMENTO DESTE INIMIGO
COMPORTAMENTO_INIMIGO Inimigo::GetComportamento()
{
    return comportamento;
}

//SETA UM NOVO COMPORTAMENTO PARA O INIMIGO
void Inimigo::SetComportamento(COMPORTAMENTO_INIMIGO novoComportamento)
{
    comportamento = novoComportamento;
}

Inimigo::~Inimigo()
{
    //dtor
}
