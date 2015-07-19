#include "Boss.h"

Boss::Boss(char* caminhoImagem, BOSS b) : GameObject(caminhoImagem, OBJETO_BOSS)
{
    //SETA RAIO PARA FORA DA JANELA PARA QUE ELE NAO APARECE NO INICIO DO JOGO
    GetPontoPolar().SetRaio(Game::GetLarguraJanela() + GetLarguraImagem());

    //VELOCIDADE DE MOVIMENTO
    vetor.SetMagnitude(0.3);

    //ESTAGIO ATUAL COMO NIVEL UM PARA QUE INICIE AS ONDAS DE INIMIGO DO INICIO
    estagioAtual = ESTAGIO_NIVEL_UM;

    //JA CRIA COM WAVE ATIVA
    bWaveAtiva = true;

    //INTERVALO ENTRE A CRIACAO DOS INIMIGOS, MENOR DE ACORDO COM A DIFICULDADE
    iIntervaloInimigos = 1000 / (Game::GetDificuldade() + 1);

    //QUANTIDADE DE ONDAS POR INIMIGO, MAIOR SEGUNDO A DIFICULDADE
    iQuantInimigosOnda = 2 * (Game::GetDificuldade() + 1);

    //INTERVALO ENTRE AS ONDAS, MENOR SEGUNDO A DIFICULDADE
    iIntervaloProximaOnda = 20000 / (Game::GetDificuldade() + 1);

    //QUANTIDADE DE INIMIGOS ATÉ AGORA
    iQuantInimigosAtual = 0;

    //PEGA A QUANTIDADE DE INIMIGOS
    iInimigosJogo = Game::GetQuantidadeInimigos();

    //SETA O TIPO DO BOSS
    tipoBoss = b;

    //PEGA A QUANTIDADE DE INIMIGOS MORTOS DESDE A ULTIMA ONDE
    iInimigosMortosAteUltimaOnda = 0;

    //INICIA OS TIMERS DE CONTROLE DE WAVE E INIMIGOS
    timerInimigo.start();
    timerWave.start();

    //SETA HP PADRAO COMO 100
    iHP = 100;

    //SETA MOVIMENTACAO COMO FALSA PARA QUE ELE NAO ANDA ATÉ QUE ACABE A PRIMEIRA WAVE
    bMovimentar = false;
}

//FUNCAO ATUALIZA, CHAMADA A TODO GAMELOOP
void Boss::Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame)
{
    //ATUALIZA A PARTE GAMEOBJECT DO OBJETO
    GameObject::Atualiza(iDeltaTime, iFrame);

    //SE O HP CHEGOU EM ZERO, ENTAO JOGADOR GANHOU
    if (iHP <= 0)
        Game::FimDaPartida(true);

    //SE EXISTE UMA WAVE ATIVA E JÁ PASSOU O INTERVALO DE INIMIGOS, CRIA UM NOVO INIMIGO
    if (timerInimigo.get_ticks() >= iIntervaloInimigos && bWaveAtiva)
    {
        GerenciaOnda();
        timerInimigo.start();
    }

    //SE A WAVE NAO ESTA ATIVA
    if (!bWaveAtiva)
    {
        //MOVIMENTA BOSS POIS ELE NAO ESTA SOLTANDO INIMIGOS
        bMovimentar = true;

        //SE A QUANTIDADE DE INIMIGOS MORTOS ATE O MOMENTO MENOS A QUANTIDADE DE INIMIGOS MORTOS ATÉ O COMECO DA ULTIMA WAVE É MAIOR OU IGUAL A QUANTIDADE DE INIMIGOS DA ULTIMA WAVE
        //OU SEJA, SE O JOGADOR JÁ MATOU TODOS OS INIMIGOS DA ULTIMA WAVE OU O TEMPO ENTRE WAVES JÁ PASSOU, CRIA NOVA WAVE E RESETA TIMERS
        if (Game::GetInimigosMortos() - iInimigosMortosAteUltimaOnda >= iQuantInimigosOnda / 2 || timerWave.get_ticks() > iIntervaloProximaOnda)
        {
            bWaveAtiva = true;
            timerWave.start();
            timerInimigo.start();
        }
    }
    //SE A WAVE ESTA ATIVA, NAO MOVIMENTO BOSS PQ ELE ESTA SOLTANDO INIMIGOS
    else
    {
        bMovimentar = false;
    }

    //INIMIGO SÓ TOMA DANO SE ELE ESTIVER ANDANDO
    SetTomaDano(bMovimentar);

    //SE FOR PRA ANDAR, ATUALIZA O ANGULO DO INIMIGO, ENQUANTO ELE FOR MAIOR QUE A ORBITA MAIS PROXIMA DO PLANETA
    if (bMovimentar)
    {
        //atualiza o angulo com a proxima posicao
        pp.SetAngulo(vetor.GetMagnitude() + pp.GetAngulo());

        if (pp.GetRaio() > Game::GetOrbitas()[0])
            pp.SetRaio(pp.GetRaio() - vetor.GetMagnitude() / 2);
    }

    //pega o proximo ponto cartesiano
    pc = pp.GetCartesianoCorrespondente();
}

//FUNCAO DESENHA, CHAMA A TODO GAMELOOP
void Boss::Desenha()
{
    GameObject::Desenha();
}

//FUNCAO QUE GERENCIA AS ONDAS DE INIMIGOS
void Boss::GerenciaOnda()
{
    //SE CHEGAMOS AO FIM DOS INIMIGOS, RESETA A VARIAVEL DE CONTROLE PARA COMECAR A CRIAR INIMIGOS DO INICIO
    if (iInimigosJogo - 1 < 0)
        iInimigosJogo = Game::GetQuantidadeInimigos();

    //PEGA REFERENCIA DO NOVO INIMIGO, SETA O PONTO POLAR DO BOSS NELE
    inimigo = Game::GetInimigos((INIMIGOS)--iInimigosJogo);
    inimigo->SetPontoPolar(GetPontoPolar());
    inimigo->GetPontoPolar().SetRaio(Game::GetOrbitas()[2] + 50);
    inimigo->SetCentroDeMassa(Game::CentralizaObjetoTela(inimigo));

    //INCLUI UM NOVO INIMIGO
    Game::GetFaseAtual()->IncluirGameObject(inimigo);

    //ATUALIZA O CONTADOR DE INIMIGOS CRIADOS
    iQuantInimigosAtual++;

    //SE CHEGAMOS NA ULTIMA WAVE
    if (estagioAtual == ESTAGIOS_NIVEL_CINCO)
    {
        //SE AINGIMOS A QUANTIDADE MAXIMA DE INIMIGOS DESTA WAVE
        if (iQuantInimigosAtual == iQuantInimigosOnda)
        {
            //RESETA AS VARIAIVEIS DE CONTROLE PARA QUE RECOMECE A ULTIMA WAVE ATÉ Q O BOSS SEJA MORTO
            iQuantInimigosAtual = 0;
            timerInimigo.start();
            timerWave.start();
            iInimigosMortosAteUltimaOnda = Game::GetInimigosMortos();
            bWaveAtiva = false;
        }
    }
    //SE NAO CHEGAMOS NA ULTIMA WAVE, MAS CHEGAMOS NA QUANTIDADE MAXIMA DE INIMIGOS DA WAVE ATUAL
    else if (iQuantInimigosAtual == iQuantInimigosOnda)
    {
        //DOBRA A QUANTIDADE DE INIMIGOS POR WAVE E VAI PARA A PROXIMA WAVE
        iQuantInimigosOnda *= 2;
        iQuantInimigosAtual = 0;
        estagioAtual = (ESTAGIOS_NIVEL) ((int)estagioAtual + 1);
        timerInimigo.start();
        timerWave.start();
        iInimigosMortosAteUltimaOnda = Game::GetInimigosMortos();
        bWaveAtiva = false;
    }
}

//FUNCAO DE CONTROLE DOS TIMERS DO BOSS
bool Boss::ControleTimers(bool bPausar)
{
    //SE É PRA PAUSAR OS TIMER DO BOSS, PAUSA TODOS
    if (bPausar)
    {
        timerInimigo.pause();
        timerWave.pause();
    }
    //SE FOR PRA DESPAUSAR, DESPAUSA TODOS OS TIMERS
    else
    {
        timerInimigo.unpause();
        timerWave.unpause();
    }
}

Boss::~Boss()
{
}
