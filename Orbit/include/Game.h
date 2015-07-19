#ifndef GAME_H
#define GAME_H
#include "Estruturas.h"
#include "Fase.h"
#include "PrimeiraFase.h"
#include "FaseMenu.h"
#include "MenuPrincipal.h"
#include "FaseMorte.h"
#include "Configuracoes.h"
#include "Nave.h"
#include "Tiro.h"
#include "Inimigo.h"

class Boss;

class Game
{
    public:
        Game();
        bool IniciaJogo();
        void GameLoop();
        static void SetProximoNivel(NIVEIS ProximoNivel);
        static int* GetTeclas();
        static void CarregaObjetos();
        static SDL_Surface* GetSurfacePrincipal();
        static int GetAlturaJanela();
        static int GetLarguraJanela();
        static Janela& GetJanela();
        static DIFICULDADES GetDificuldade();
        static void SetDificuldade(DIFICULDADES novaDificuldade);
        static Save& GetSave();
        static void SetSave(Save novoSave);
        static char* GetVersao();
        static void SetFPS(int NovoFPS);
        static int GetFPS();
        static void SetCorPadrao(int iCor);
        static int GetCorPadrao();
        static Fase* GetFaseAtual();
        static unsigned long int GetIdObject();
        static void FimDaPartida(bool bPerder);
        static void Ganhar();
        static int GetZoom();
        static void SetZoom(int iNovoZoom);
        static PontoCartesiano& GetCentroJanela();
        static PontoCartesiano& CentralizaObjetoTela(GameObject* gameObjectCentralizar);
        static void ContinuaJogo();
        static void SalvaJogo();
        static void FecharJogo();
        static void Menu();
        static Save* GetScores(int* iQuantidadeRetorno);
        static Nave* GetNaves(TIPO_NAVES tipoNave);
        static Tiro* GetTiros(TIROS tipoTiro);
        static Inimigo* GetInimigos(INIMIGOS tipoInimigo);
        static void RotacionaGameObject(GameObject* gameObjectRotacionar);
        static bool DentroJanela(GameObject* gameobject);
        static int GetInimigosMortos();
        static int GetTirosDisparados();
        static void SetInimigosMortos(int iInimigos);
        static void SetTirosDisparados(int iTirosDisparados);
        static void IncrementaInimigoMorto(int iValorSoma);
        static void IncrementaTirosDisparados(int iValorSoma);
        static void LimpaDadosPartida();
        static bool GetResultadoPartida();
        static int* GetOrbitas();
        static int* GetSelecaoNaves(int* iQuantNaves);
        static void SetSelecaoNaves(int* iNaves, int iQuantSelecao);
        static int GetQuantidadeNaves();
        static int GetQuantidadeInimigos();
        static int GetQuantidadeTiros();
        static Nave* GetEscudo();
        static void SetResolucao(int altura, int largura);
        static void SetFullScreen(bool fullScreen);
        static void RetiraFundo(SDL_Surface* imagem, int iCorRetirar);
        virtual ~Game();
    private:
        static void TrataEntradaUsuario();
        static void GerenciaNiveis();
        static void LeNaves();
        static void LeTiros();
        static void LeInimigos();
        static void LeBoss();
        static unsigned long long int iFrameAtual;
        static unsigned int iDeltaTime;
        static int FPS;
        static int COR_PADRAO;
        static Janela janela;
        static DIFICULDADES dificuldade;
        static int teclas[45];
        static TELA tela;
        static Save save;
        static SDL_Event eventos;
        static Timer timerFPS;
        static Fase* FaseAtual;
        static Fase* faseMenu;
        static NIVEIS ProximaFase;
        static bool bTrocaFase;
        static unsigned int iIdObject;
        static int iZoom;
        static int ESTADO_JOGO;
        static Nave** todasNaves;
        static Tiro** todosTiros;
        static Inimigo** todosInimigos;
        static int iQuantNave, iQuantInimigos, iQuantTiros, iQuantSelecaoNaves;
        static int iTirosDisparados;
        static int iInimigosMortos;
        static bool bResultado;
        static int* iSelecaoNaves;
};

#endif
