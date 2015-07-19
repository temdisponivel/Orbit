#include "Fase.h"
#include "Game.h"

Fase::Fase(NIVEIS nivelFase)
{
    nivel = nivelFase;
    fundo = NULL;
    menu = NULL;
}

//Funcao atualiza da fase, chamada pelas classes derivadas desta
void Fase::Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame)
{
    //percorre todos os gameobjects dessa fase e chama a funcao atualiza deles
    for (register int i = 0; i < ListaPrincipal.GetTamanho(); i++)
    {
        if (ListaPrincipal.listGame[i] != NULL)
            ListaPrincipal.listGame[i]->Atualiza(iDeltaTime, iFrame);
    }

    //chama funcao que valida colisao de todos os gameobjects
    Colisao();

    //gerencia lista de gameobjets
    ListaPrincipal.GerenciaGameObject();
}

//funcao desenha da classe, chamada pelas classes derivadas desta
void Fase::Desenha()
{
    //se a tela tem um fundo, desenha ele antes de qualquer outra coisa
    if (fundo != NULL)
       DesenhaFundo();

    //chama a funcao atualiza de todos os gameobjects dessa fase
    for (register int i = 0; i < ListaPrincipal.GetTamanho(); i++)
    {
        if (ListaPrincipal.listGame[i] != NULL)
            ListaPrincipal.listGame[i]->Desenha();
    }
}

//funcao que desenha o fundo por tiles
void Fase::DesenhaFundo()
{
    PontoCartesiano pc;

    //percorre a largura
    for (register int i = 0; i <= (Game::GetAlturaJanela() / fundo->GetAlturaImagem()); i++)
    {
        //percorre a altura
        for (register int j = 0; j <= (Game::GetLarguraJanela() / fundo->GetLarguraImagem()); j++)
        {
            //atualiza posicao cartesiana
            pc.SetY(fundo->GetAlturaImagem() * i);
            pc.SetX(fundo->GetLarguraImagem() * j);
            fundo->SetPontoCartesiano(pc);

            //desenha objeto na tela
            fundo->Desenha();
        }
    }
}

//Funcao que inclui um gameobject na lista de gameobjects da fase
void Fase::IncluirGameObject(GameObject* gameObjectIncluir)
{
    ListaPrincipal.IncluirGameObject(gameObjectIncluir);
}

//funcao que exclui o gameoject da lista de gameobjectsda dessa fase
void Fase::ExcluiGameObject(GameObject* gameObjectExcluir)
{
    ListaPrincipal.ExcluiGameObject(gameObjectExcluir);
}

//funcao que valida a colisao de todos os gameobjects com todos os outros
void Fase::Colisao()
{
    GameObject* objetoColisor;
    GameObject* objetoColidido;
    PontoCartesiano pcAColisor;
    PontoCartesiano pcBColisor;
    PontoCartesiano pcAColidido;
    PontoCartesiano pcBColidido;
    int iRaioColisor, iRadioColidido;
    bool bColidiu = false;

    //percorre a lista de gameobjets
    for (register long int i = 0; i < ListaPrincipal.GetTamanho(); i++)
    {
        //pega a referencia do gameobject atual
        objetoColisor = ListaPrincipal.listGame[i];

        //se for o fundo, pega o proximo
        if (objetoColisor->GetTipo() == OBJETO_FUNDO)
            continue;

        //pega os verices do gameobject para validar a colisao
        objetoColisor->GetVertices(&pcAColisor, &pcBColisor);

        //percorre todos os gameobjects a partir do atual
        for (register long int j = i+1; j < ListaPrincipal.GetTamanho(); j++)
        {
            //pega o gameobject a ser comparado e seus vertices
            objetoColidido = ListaPrincipal.listGame[j];

            //se for o fundo, pega o proximo
            if (objetoColidido->GetTipo() == OBJETO_FUNDO)
                continue;

            objetoColidido->GetVertices(&pcAColidido, &pcBColidido);

            //se os dois objetos colidiram
            bColidiu = ValidaColisao(pcAColisor, pcBColisor, pcAColidido, pcBColidido);

            if (bColidiu)
            {
                //seta os objetos como colidido
                objetoColidido->SetColidiu(true);
                objetoColisor->SetColidiu(true);

                //seta a referencia de um no outro
                objetoColidido->SetObjectColidiu(objetoColisor);
                objetoColisor->SetObjectColidiu(objetoColidido);
            }
        }
    }
}

//funcao que limpa todos os objetos da listprincipal
void Fase::LimpaGameObject()
{
    ListaPrincipal.LimpaGameObject();
}

//funcao que seta o fundo na tela
GameObject* Fase::SetFundo(char* caminhoImagemFundo)
{
    fundo = new GameObject(caminhoImagemFundo, OBJETO_FUNDO);
    IncluirGameObject(fundo);

    return fundo;
}

//FUNCAO QUE SETA QUAL NIVEL ESTA CLASSE REPRESENTA
void Fase::SetNivel(NIVEIS nivelFase)
{
    nivel = nivelFase;
}

//FUNCAO QUE RETORNA QUAL NIVEL ESTA CLASSE REPRESENTA
NIVEIS Fase::GetNivel()
{
    return nivel;
}

void Fase::ReiniciaNivel()
{
    ListaPrincipal.LimpaGameObject();
    IniciarNivel();
}

//funcao que valida a colisao a partir de dois veritices para cada objeto
//estes vertices devem ser o da diagonal principal do retangulo
bool Fase::ValidaColisao(PontoCartesiano VerticeAColisor, PontoCartesiano VerticeBColisor, PontoCartesiano VerticeAColidido, PontoCartesiano VerticeBColidido)
{
    if (VerticeAColisor.GetY() > VerticeBColidido.GetY())
        return false;

    if (VerticeBColisor.GetY() < VerticeAColidido.GetY())
        return false;

    if (VerticeAColisor.GetX() > VerticeBColidido.GetX())
        return false;

    if (VerticeBColisor.GetX() < VerticeAColidido.GetX())
        return false;

    return true;
}

//valida colisao pelos raios
bool Fase::ValidaColisao(int iRaioColisor, int iRaioColido)
{
    if (iRaioColisor < iRaioColido)
        return false;

    return true;
}

//FUNCAO QUE RETORNA A LISTA DE GAMEOBJECTS DA FASE
GameObject** Fase::GetListaGameObject(int* iQuantidade)
{
    *iQuantidade = ListaPrincipal.GetTamanho();
    return ListaPrincipal.GetLista();
}

//FUNCAO QUE CONTROLA OS TIMER DE TODOS OS GAMEOBJECTS DESTA FASE
bool Fase::ControleTimer(bool bPausar)
{
    for (register int i = 0; i < ListaPrincipal.GetTamanho(); i++)
        ListaPrincipal.GetLista()[i]->ControleTimers(bPausar);
}

//FUNCAO QUE RETORNA A LISTA DE GAMEOBJECT DA FASE
ListaGameObject& Fase::GetListaGameObject()
{
    return ListaPrincipal;
}

//FUNCAO PARA IMPLEMENTAR O FIM DA FASE, CASO NECESSARIO
void Fase::FimDoNivel()
{
}

Fase::~Fase()
{
    //LIMPA TODOS OS GAMEOBJECTS
    LimpaGameObject();
}
