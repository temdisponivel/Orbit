#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(char *caminhoImagem, TIPO_OBJETOS tipoobjeto)
{
    imagem = imagemAuxilia = NULL;

    iHP = iDano = 0;

    //PEGA SURFACE DA JANELA
    surfacePai = Game::GetSurfacePrincipal();

    //PEGA NOVO ID E SETA O TIPO DESTE OBJETO
    ID = Game::GetIdObject();
    tipo = tipoobjeto;

    //CARREGA A IMAGEM E SETA O CAMINHO NA PROPRIEDADE
    if (CarregaImagem(caminhoImagem))
        cCaminhoImagem = caminhoImagem;
    else
        cCaminhoImagem = NULL;

    //INICIA PROPRIEDADES REFERENTES A COLISAO
    bColidiu = false;
    iNumeroColisao = 0;
    iTotalColisao = 25;
    gameObjectColidiu = new GameObject*[iTotalColisao];

    //CRIA UMA COPIA DA SURFACE PARA ROTACAO
    imagemAuxilia = imagem;

    //SETA A COR DE FUNDO COMO PRETO
    COR_FUNDO_IMAGEM = 0x0;

    //SE NAO FOR UM FUNDO, CENTRALIZA OBJETO NA TELA
    if (tipoobjeto != OBJETO_FUNDO)
        pp.SetCentroDeMassa(Game::CentralizaObjetoTela(this));

    //SETA TOMA DANO COMO TRUE PARA PODER TOMAR DANO INICIALMENTE
    bTomaDano = true;
}

void GameObject::Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame)
{
    //chama funcao que vai atualizar a animacao
    animacao.Atualiza();
}

void GameObject::Desenha()
{
    //retangulo para colocar imagem na tela
    SDL_Rect* offset = new SDL_Rect;

    //seta os pontos cartesianos
    offset->x = pc.GetX();
    offset->y = pc.GetY();

    //seta a cor de fundo como transparente
    SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, COR_FUNDO_IMAGEM);

    //coloca a surface sobre a tela
    SDL_BlitSurface(imagem, animacao.GetClipAtual(), surfacePai, offset);

    //libera memória
    delete offset;
}

//FUNCAO QUE RETORNA O PONTO POLAR
PontoPolar& GameObject::GetPontoPolar()
{
    return pp;
}

//FUNCAO QUE SETA UM NOVO PONTOPOLAR
void GameObject::SetPontoPolar(PontoPolar pontoPolar)
{
    pp = pontoPolar;
}

//FUNCAO QUE RETORNA O PONTOCARTESIANO
PontoCartesiano& GameObject::GetPontoCartesiano()
{
    return pc;
}

//FUNCAO QUE SETA UM NOVO PONTOCARTESIANO
void GameObject::SetPontoCartesiano(PontoCartesiano pontoCartesiano)
{
    pc = pontoCartesiano;
}

//FUNCAO QUE CARREGA A IMAGEM
bool GameObject::CarregaImagem(char* caminhoImagem)
{
    SDL_Surface *surfaceTemp = NULL;

    //carrega a imagem passada como parametro
    surfaceTemp = IMG_Load(caminhoImagem);

    //se a imagem nao foi carregada corretamente, retorna false para simbolizar erro;
    if(surfaceTemp == NULL)
        return false;

    //OTIMIZA A IMAGEM PARAO FORMATO DA TELA
    imagem = SDL_DisplayFormat(surfaceTemp);

    //LIBERA SURFACETEMPORARIA
    SDL_FreeSurface(surfaceTemp);

     //se imagem nao carregou corretamente, retorna falso para simbolizar erro
    if (imagem == NULL)
        return false;

    //seta a cor de fundo como transparente
    SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, COR_FUNDO_IMAGEM);

    //se chegou nessa linha, podemos retornar true porque a imagem foi carregada corretamente
    return true;
}

//FUNCAO QUE RETORNA O VETOR DO OBJETO
Vetor& GameObject::GetVetor()
{
    return vetor;
}

//FUNCAO QUE SETA UM NOVO VETOR NO OBJETO
void GameObject::SetVetor(Vetor vet)
{
    vetor = vet;
}

//FUNCAO QUE SETAA SURFACE PAI (SURFACE UTILIZADA PARA DESENHAR A IMAGEM)
void GameObject::SetSurfacePai(SDL_Surface* surface)
{
    surfacePai = surface;
}

//seta um novo array de clips com a quantidade
void GameObject::SetAnimacao(Animacao animac)
{
    animacao = animac;
}

//FUNCAO QUE RETORNA UMA REFEERENCIA PARA ANIMACAO
Animacao& GameObject::GetAnimacao()
{
    return animacao;
}

//FUNCAO QUE CONTROI A ANIMACAO PARA O GAMEOBJECT
void GameObject::ControiAnimacao(const SDL_Rect* padraoClips, int iVelocidade)
{
    if (padraoClips != NULL)
        animacao.ControiAnimacao(GetLarguraImagem(), GetAlturaImagem(), padraoClips->w, padraoClips->h, iVelocidade);
}

//FUCNAO QUE SETA UM FRAME NA ANIMACAO E PAUSA OU NAO PAUSA A ANIMACAO
void GameObject::SetFrame(int frame, bool bPause)
{
    animacao.SetFrameAtual(frame, bPause);
}

//FUNCAO QUE SETA UMA SUBANIMACAO NO GAMEOBJECT
void GameObject::SubAnimacao(int frameInicial, int framefinal)
{
    animacao.SetSubAnimacao(frameInicial, framefinal);
}

//FUNCAO QUE RETORNA A IMAGEM DO GAMEOBJECT
SDL_Surface* GameObject::GetImagem()
{
    return imagem;
}

//FUNCAO QUE SETA UMA NOVA IMAGEM PARA O GAMEOBJECT
void GameObject::SetImagem(SDL_Surface* novaimagem)
{
    imagem = novaimagem;
}

//FUNCAO QUE RETORNA A IMAGEMAUXILIXAR DO GAMEOBJECT
SDL_Surface* GameObject::GetImagemAuxiliar()
{
    return imagemAuxilia;
}

//FUNCAO QUE SETA UMA IMAGEM AUXILIAR PARA O GAMEOBJECT
void GameObject::SetImagemAuxiliar(SDL_Surface* novaimagem)
{
    imagemAuxilia = novaimagem;
}

//FUNCAO QUE RETORNA A SURFACE PAI DO OBJETO (SURFACE ONDE A IMAGEM É DESENHADA)
SDL_Surface* GameObject::GetSurfacePai()
{
    return surfacePai;
}

//FUNCAO QUE RETORNA A ALTURA DA IMAGEM (OU DO FRAME DA ANIMACAO CASO HOUVER) ou 0 CASO A IMAGEM NAO FOR VALIDA
int GameObject::GetAlturaImagem()
{
    if (animacao.GetClip() != NULL)
        return animacao.GetModeloFrame()->h;

    if (imagem != NULL)
        return imagem->h;
    else
        0;
}

//FUNCAO QUE RETORNA A LARGURA DA IMAGEM (OU DO FRAME DA ANIMACAO CASO HOUVER) OU 0 CASO A IMAGEM NAO FOR VALIDA
int GameObject::GetLarguraImagem()
{
    if (animacao.GetClipAtual() != NULL)
        return animacao.GetModeloFrame()->w;

    if (imagem != NULL)
        return imagem->w;
    else
        0;
}

//faz a conversao do ponto polar para o ponto cartesiano
void GameObject::PolarParaCartesiano()
{
    pc = pp.GetCartesianoCorrespondente();
}

//converte o angulo atual que esta em graus para radianos
double GameObject::GetAngRadianos()
{
    return pp.GetAnguloRadianos();
}

//FUNCAO QUE RETORNA O ID DO GAMEOBJECT
unsigned int GameObject::GetId()
{
    return ID;
}

//FUNCAO QUE SETA UMA REFERENCIA DE UM OBJETO QUE COLIDIU COM ESTE
void GameObject::SetObjectColidiu(GameObject* objectColidiu)
{
    //se ainda nao atingiu o total de objetos colididos
    if (iNumeroColisao < iTotalColisao)
    {
        //valida se o objeto ainda nao foi incluido
        for (register int i = 0; i < iNumeroColisao; i++)
        {
            if (gameObjectColidiu[iNumeroColisao] == objectColidiu)
                return;
        }

        //se o objeto ainda nao foi incluido, inclui
        gameObjectColidiu[iNumeroColisao++] = objectColidiu;
    }
}

//FUNCAO QUE SETA SE O OBJETO COLIDIU OU NAO
void GameObject::SetColidiu(bool colidiu)
{
    bColidiu = colidiu;
}

//FUNCAO QUE RETORNA A LISTA DE OBJECTS COM OS QUAIS ESTE COLIDIU
GameObject** GameObject::GetObjectColidiu()
{
    return gameObjectColidiu;
}

//FUNCAO QUE RETORNA SE ESTE OBJETO COLIDIU
bool GameObject::GetColidiu()
{
    return bColidiu;
}

//FUNCAO QUE RETORNA A QUANTIDADE DE COLISOES DESTE OBJETO
int GameObject::GerNColisao()
{
    return iNumeroColisao;
}

//FUNCAO QUE RETORNA O TIPO DESTE OBJETO
TIPO_OBJETOS GameObject::GetTipo()
{
    return tipo;
}

//FUNCAO QUE SETA O TIPO DO OBJETO
void GameObject::SetTipo(TIPO_OBJETOS tipoObj)
{
    tipo = tipoObj;

        //SE FOR UM TIRO AMIGO, INCREMENTA NO CONTADOR DE TIROS DISPARADOS PELO JOGADOR
    if (tipoObj == OBJETO_TIRO_AMIGO)
        Game::IncrementaTirosDisparados(1);
}

//FUNCAO QUE RETORNA O DANO DESTE OBJETO
int GameObject::GetDano()
{
    return iDano;
}

//FUNCAO QUE SETA O DANO QUE ESTE OBJETO CAUSA
void GameObject::SetDano(int iNovoDano)
{
    iDano = iNovoDano;
}

//FUNCAO QUE RETORNA O HP DESTE OBJETO
int GameObject::GetHP()
{
    return iHP;
}

//FUNCAO QUE SETA O HP NESTE OBJETO
void GameObject::SetHP(int novoHP)
{
    iHP = novoHP;
}

//retorna, por referencia, os vertices da diagonal principal do objeto
void GameObject::GetVertices(PontoCartesiano* pcA, PontoCartesiano* pcB)
{
    pcA->SetX(pc.GetX());
    pcA->SetY(pc.GetY());
    pcB->SetX(pc.GetX() + GetLarguraImagem());
    pcB->SetY(pc.GetY() + GetAlturaImagem());
}

//FUNCAO QUE RETORNA O CAMINHO DA IMAGEM DESTE OBJETO
char* GameObject::GetCaminhoImagem()
{
    return cCaminhoImagem;
}

//FUNCAO QUE SETA O CENTRO DE MASSA DESTE OBJETO (AO REDOR DO QUAL ELE GIRA E DO QUAL O RAIO SE INICIA)
void GameObject::SetCentroDeMassa(PontoCartesiano pc)
{
    pp.SetCentroDeMassa(pc);
}

//FUNCAO QUE RETORNA O CENTRO DE MASSA DESTE OBJETO
PontoCartesiano& GameObject::GetCentroDeMassa()
{
    return pp.GetCentroDeMassa();
}

//FUNCAO QUE RETORNA SE ESTE OBJETO TOMA DANO OU NAO
bool GameObject::GetTomaDano()
{
    return bTomaDano;
}

//FUNCAO QUE SETA SE ESTE OBJETO TOMA DANO OU NAO
void GameObject::SetTomaDano(bool bDano)
{
    bTomaDano = bDano;
}

//FUNCAO DE CONTROLE DO TIMER PARA SER IMPLEMENTADA NOS FILHOS
bool GameObject::ControleTimers(bool bPausar)
{
}

GameObject::~GameObject()
{
}
