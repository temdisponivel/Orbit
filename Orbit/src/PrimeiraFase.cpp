#include "PrimeiraFase.h"
#include "Boss.h"

PrimeiraFase::PrimeiraFase() : Fase(NIVEL_JOGO_UM)
{
    IniciarNivel();
}

//funcao que inicia o nivel
void PrimeiraFase::IniciarNivel()
{
    //SETA O FUNDO NA FASE
    Fase::SetFundo("imagens/fundo/fundo.png");

    //CRIA O PLANETA
    CriaPlaneta();

    //CONTROLE DE NAVE É 1, PQ A PRIMEIRA JÁ É O ESCUDO, PORTANTE, A PROXIMA É A 1
    iControleNave = 1;

    //PEGA AS NAVES SELECIONADAS PELO USUÁRIO
    iSelecaoNave = Game::GetSelecaoNaves(&iQuantNavesSelecionadas);

    //INCLUI O ESCUDO E A PRIMEIRA NAVE SELECIONADA NA LISTA DE GAMEOBJECT
    IncluirGameObject(Game::GetNaves((TIPO_NAVES)iSelecaoNave[0]));
    IncluirGameObject(Game::GetNaves((TIPO_NAVES)iSelecaoNave[iControleNave]));

    //CRIA O BOSS NA TELA
    CriarBoss();
}

//funcao de artualizar da fase, onde chamamos a funcao atualiza da classe pai, que por sua vez, chama a funcao atualiza de cada gameobject
void PrimeiraFase::Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame)
{
    //se apertou esc, VAI PRO MENU
    if (Game::GetTeclas()[TECLA_ESC])
        Game::Menu();

    //ATUALIZA TODOS OS GAMEOBJECTS DA LISTA
    Fase::Atualiza(iDeltaTime, iFrame);
}

//funcao desenha da classe, chama a desenha da pai que, por sua vez, chama a desenha de cada gameobject
//depois chama a funcao que desenha o HUD na janela
void PrimeiraFase::Desenha()
{
    Fase::Desenha();
    DesenhaHUD();
}

//funcao que cria o planeta
void PrimeiraFase::CriaPlaneta()
{
    planeta = new Planeta("imagens/planeta/planeta.png");
    planeta->SetCentroDeMassa(Game::CentralizaObjetoTela(planeta));
    IncluirGameObject(planeta);
}

//funcao que desenha o tile do fundo
void PrimeiraFase::DesenhaFundo()
{
    PontoCartesiano pc;

    //PREENCHE O FUNDO POR TILE NA HORIZONTAL E VERTICAL SEGUNDO LARGURA E ALTURA DA IMAGEM E JANELA
    for (register int i = 0; i <= (Game::GetAlturaJanela() / fundo->GetAlturaImagem()); i++)
    {
        for (register int j = 0; j <= (Game::GetLarguraJanela() / fundo->GetLarguraImagem()); j++)
        {
            pc.SetY(fundo->GetAlturaImagem() * i);
            pc.SetX(fundo->GetLarguraImagem() * j);
            fundo->SetPontoCartesiano(pc);
            fundo->Desenha();
        }
    }
}

//desenha o HUD com as infomacoes de jogo na tela
void PrimeiraFase::DesenhaHUD()
{
    //CRIA RECT QUE REPRESENTA A BARRA DE HP DO PLANETA
    SDL_Rect* vida = new SDL_Rect;
    char* texto;

    //ALTURA, LARGURA, X E Y  DA BARRA DE HP
    vida->h = 25;
    vida->w = planeta->GetHP() * 2;
    vida->x = 20;
    vida->y = 20;

    //DESENHA BARRA DE HP NA TELA
    SDL_FillRect(Game::GetSurfacePrincipal(), vida, VERMELHO32);

    //INSTANCIA VETOR DE CARACTER PARA MANIPULAR INTEIROS
    texto = new char[10];

    //PEGA O TEXTO QUE REPRESENTA O HP DO PLANETA
    sprintf(texto, "%i", planeta->GetHP());

    //DESENHA AO LADO DA BARRA
    stringColor(Game::GetSurfacePrincipal(), vida->x + vida->w + 10, 30, texto, VERMELHO32);

    //PEGA A PONTUACAO DO JOGADOR
    sprintf(texto, "%i", Game::GetSave().GetPontuacao());

    //DESENHA À DIREITA DA JANELA
    stringColor(Game::GetSurfacePrincipal(), Game::GetLarguraJanela() - 50, 30, texto, VERMELHO32);

    //LIBERA MEMÓRIA
    delete vida;
    delete texto;
}

//FUNCAO QUE CRIA O BOSS
void PrimeiraFase::CriarBoss()
{
    Boss* b = new Boss("imagens/boss/boss_um.png", BOSS_UM);
    b->SetCentroDeMassa(Game::CentralizaObjetoTela(b));
    IncluirGameObject(b);
}

//FUNCAO SOBREESCRITA QUE EXCLUI UM GAMEOBJECT DA LISTA
void PrimeiraFase::ExcluiGameObject(GameObject* gameObjectExcluir)
{
    Nave* novaNave;

    //VERIFICA SE O OBJETO A SER EXCLUÍDO É UMA NAVE
    if (gameObjectExcluir->GetTipo() == OBJETO_NAVE)
    {
        //SE CHEGAMOS AO FIM DAS NAVES, PERDE O JOGO
        if (iControleNave + 1 >= iQuantNavesSelecionadas)
        {
            Game::FimDaPartida(false);
            return;
        }
        //SENAO, TROCA DE NAVE E ADICIONA A NOVA NAVE NA LISTA
        else
        {
            novaNave = Game::GetNaves((TIPO_NAVES)iSelecaoNave[++iControleNave]);
            novaNave->SetPontoPolar(gameObjectExcluir->GetPontoPolar());
            IncluirGameObject(novaNave);
        }
    }

    //EXCLUI OBJETO DA LISTA
    Fase::ExcluiGameObject(gameObjectExcluir);
}

PrimeiraFase::~PrimeiraFase()
{
}
