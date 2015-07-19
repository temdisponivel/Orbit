#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Headers.h"
#include "Estruturas.h"

class GameObject
{
    public:
        GameObject(char *caminhoImagem, TIPO_OBJETOS tipo);
        PontoPolar& GetPontoPolar();
        void SetPontoPolar(PontoPolar pontoPolar);
        PontoCartesiano& GetPontoCartesiano();
        void SetPontoCartesiano(PontoCartesiano pontoCartesiano);
        Vetor& GetVetor();
        void SetVetor(Vetor vet);
        bool CarregaImagem(char *caminhoImagem);
        virtual void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        virtual void Desenha();
        void SetAnimacao(Animacao animac);
        void ControiAnimacao(const SDL_Rect* padraoClips, int iVelocidade);
        Animacao& GetAnimacao();
        void SetFrame(int frame, bool bPause);
        void SubAnimacao(int frameInicial, int framefinal);
        void PausaAnimacao(bool pausa);
        void SetSurfacePai(SDL_Surface *surfacePai);
        SDL_Surface* GetImagem();
        void SetImagem(SDL_Surface* novaimagem);
        SDL_Surface* GetImagemAuxiliar();
        void SetImagemAuxiliar(SDL_Surface* novaimagem);
        SDL_Surface* GetSurfacePai();
        void SetDificuldade(DIFICULDADES dif);
        int GetAlturaImagem();
        int GetLarguraImagem();
        unsigned int GetId();
        void SetObjectColidiu(GameObject* objectColidiu);
        void SetColidiu(bool colidiu);
        GameObject** GetObjectColidiu();
        bool GetColidiu();
        int GetDano();
        void SetDano(int iNovoDano);
        int GerNColisao();
        int GetHP();
        virtual void SetHP(int novoHP);
        TIPO_OBJETOS GetTipo();
        void SetTipo(TIPO_OBJETOS tipoObj);
        void GetVertices(PontoCartesiano* pcA, PontoCartesiano* pcB);
        char* GetCaminhoImagem();
        void SetCentroDeMassa(PontoCartesiano pc);
        PontoCartesiano& GetCentroDeMassa();
        bool GetTomaDano();
        virtual bool ControleTimers(bool bPausar);
        virtual void SetTomaDano(bool bDano);
        virtual ~GameObject();
    protected:
        void PolarParaCartesiano();
        double GetAngRadianos();
        PontoCartesiano pc;
        PontoPolar pp;
        Vetor vetor;
        SDL_Surface *imagem;
        SDL_Surface *surfacePai;
        SDL_Surface *imagemAuxilia;
        Animacao animacao;
        unsigned int ID;
        Uint32 COR_FUNDO_IMAGEM;
        int iHP;
        int iDano;
        bool bColidiu;
        GameObject** gameObjectColidiu;
        int iNumeroColisao;
        int iTotalColisao;
        TIPO_OBJETOS tipo;
        char* cCaminhoImagem;
        bool bTomaDano;
};

#endif
