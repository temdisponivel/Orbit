#include "Game.h"

using namespace std;

//propriedades estaticas da classe
unsigned long long int Game::iFrameAtual;
unsigned int Game::iDeltaTime;
int Game::FPS;
int Game::COR_PADRAO;
Janela Game::janela;
DIFICULDADES Game::dificuldade;
int Game::teclas[45];
TELA Game::tela;
Save Game::save;
SDL_Event Game::eventos;
Timer Game::timerFPS;
Fase* Game::FaseAtual;
Fase* Game::faseMenu;
bool Game::bTrocaFase;
unsigned int Game::iIdObject;
NIVEIS  Game::ProximaFase;
int Game::ESTADO_JOGO;
int Game::iZoom;
Nave** Game::todasNaves;
Tiro** Game::todosTiros;
Inimigo** Game::todosInimigos;
int Game::iQuantNave;
int Game::iQuantInimigos;
int Game::iQuantTiros;
int Game::iTirosDisparados;
int Game::iInimigosMortos;
int Game::iQuantSelecaoNaves;
bool Game::bResultado;
int* Game::iSelecaoNaves;

Game::Game()
{
}

//inicia a SDL, tela, seta padroes de variais, cria array de gameobjects e le configuracoes e instancia a classe do nivel atual pego do arquivo de save
bool Game::IniciaJogo()
{
    ifstream* leitorConfig = new ifstream(arqConfig);

    //se nao conseguiu abrir o arquivo de configuração
    if (!*leitorConfig)
        return false;

    //pega informacoes do arquivo de configuracoes
    *leitorConfig >> janela.iLargura;
    *leitorConfig >> janela.iAltura;
    *leitorConfig >> janela.cTitulo;
    *leitorConfig >> janela.versao;
    *leitorConfig >> janela.bFullScreen;

    //inicia todos os sistemas da SDL e valida erro
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1 )
        return false;

    //inicia janela em full screen se for configurado pra isso, senao, inicia em janela normal
    if (janela.GetFullScreen())
        janela.SetSurface(SDL_SetVideoMode(janela.iLargura, janela.iAltura, 32, SDL_SWSURFACE | SDL_FULLSCREEN));
    else
        janela.SetSurface(SDL_SetVideoMode(janela.iLargura, janela.iAltura, 32, SDL_SWSURFACE));

    //se deu erro, retorna falso
    if (GetSurfacePrincipal() == NULL)
        return false;

    //seta titulo na janela
    SDL_WM_SetCaption(janela.GetTitulo(), NULL);

    //define padroes fixos do jogo
    COR_PADRAO = PRETO;
    FPS = 60;
    iDeltaTime = 0;
    iFrameAtual = 0;
    bTrocaFase = true;
    SetProximoNivel(NIVEL_MENU_PRINCIPAL);
    iIdObject = 0;
    ESTADO_JOGO = 1;
    iZoom = 1;
    todosInimigos = NULL;
    todosTiros = NULL;
    todosInimigos = NULL;
    iSelecaoNaves = NULL;

    //seta a instancia da fase menu
    Game::faseMenu = new FaseMenu();

    //reseta teclas
    for (register int i = 0; i < 20; i++)
        teclas[i] = 0;

    //libera memoria do leitor
    delete leitorConfig;

    CarregaObjetos();

    //se chegamos aqui, podemos retornar true
    return true;
}

//gameloop principal
void Game::GameLoop()
{
    //ENQUANTO O JOGO ESTIVER ATIVO
    while (ESTADO_JOGO)
    {
        //se precisamos trocar de nivel, efetua a troca antes de continuar o gameloop
        if (bTrocaFase)
        {
            GerenciaNiveis();
            continue;
        }

        //inicia o timer do jogo
        timerFPS.start();

        //pega enventos do usuario
        TrataEntradaUsuario();

        //só trata aqui eventos gerais do jogo, que nao tem relacao estrita com os níveis
        //-----------------------------------------------------------------------------//

        //se apertou X DA JANELA
        if (teclas[TECLA_SAIR])
            break;

        //-----------------------------------------------------------------------------//

        //se tiver na tela principal, chama gameloop da tela principal
        if (tela == TELA_PRINCIPAL)
        {
            //chama o gameloop do nivel atual
            FaseAtual->Atualiza(iDeltaTime, iFrameAtual);
        }
        //se estiver no menu, chama gameloop do menu
        else if (tela == TELA_PAUSA)
        {
            //chama o gameloop do menu de paulo
            faseMenu->Atualiza(iDeltaTime, iFrameAtual);
        }

        //LIMPA TELA ANTES DE ENTRAR NOS DESENHOS
        SDL_FillRect(GetSurfacePrincipal(), NULL, COR_PADRAO);

        //se tiver na tela principal, chama gameloop da tela principal
        if (tela == TELA_PRINCIPAL)
        {
            FaseAtual->Desenha();
        }
        //se estiver no menu, chama gameloop do menu
        else if (tela == TELA_PAUSA)
        {
            faseMenu->Desenha();
        }

        //ATUALIZA OS OBJETOS NA TELA
        if (SDL_Flip(GetSurfacePrincipal()) == -1)
            break;


        //TRATAMENTO DE FPS. PEGA O TEMPO (ms) DESDE QUE INICIOU O GAMELOOP E FAZ O TEMPO ATUAL (ms)
        // MENOS O TEMPO DESDE QUE COMECOU, OU SEJA, PEGA QUANTO TEMPO DEMOROU O GAMELOOP
        //DEPOIS PEGA QUANTOS FPS QUEREMOS, EM MILISEGUINDOS (60 FPS = 16 ms) E SUBTRAI O TEMPO DO GAMELOOP
        //ASSIM OBTEM O TEMPO QUE DEVEMOS ESPERAR ATE O PROXIMO GAMELOOP
        //-----------------------------------------------------------------------------//
        iDeltaTime = timerFPS.get_ticks();

        iFrameAtual = SDL_GetTicks(); //atualiza frame

        //se o cap esta ativo e se os tics do FPS dividido pelo tempo atual de jogo
        if (timerFPS.get_ticks() < (1000 / FPS))
        {
            iDeltaTime = (1000 / FPS) - timerFPS.get_ticks();

            //seta o delay como o gametime para que ele sempre seja o que queremos
            SDL_Delay(iDeltaTime);
        }
    }
}

//funcao que trata todas as entradas do usuario
void Game::TrataEntradaUsuario()
{
    // TRATA OS EVENTOS
    while(SDL_PollEvent(&eventos)>0)
    {
        //se clicou no x da janela
        if (eventos.type == SDL_QUIT)
            teclas[TECLA_SAIR] = 1;

        //se apertou alguma tecla
        else if (eventos.type == SDL_KEYDOWN)
        {
            switch(eventos.key.keysym.sym){
                //TECLAS ESPECIAIS
                case SDLK_RIGHT: teclas[TECLA_SETA_DIREITA] = 1; break; // ->
                case SDLK_LEFT: teclas[TECLA_SETA_ESQUERDA] = 1; break; // <-
                case SDLK_UP: teclas[TECLA_SETA_CIMA] = 1; break; // ^
                case SDLK_DOWN: teclas[TECLA_SETA_BAIXO] = 1; break; // \/
                case SDLK_RETURN: teclas[TECLA_ENTER] = 1; break; // Frame Rate Cap
                case SDLK_ESCAPE: teclas[TECLA_ESC] = 1; break; //esc
                case SDLK_BACKSPACE: teclas[TECLA_BACKSPACE] = 1; break; //esc

                //ALFA NUMERICO
                case SDLK_a: teclas[TECLA_A] = 1; break; // A
                case SDLK_t: teclas[TECLA_T] = 1; break; // ANIMACOES PAUSA
                case SDLK_r: teclas[TECLA_R] = 1; break; //R
                case SDLK_s: teclas[TECLA_S] = 1; break; //R
                case SDLK_d: teclas[TECLA_D] = 1; break; // D
                case SDLK_w: teclas[TECLA_W] = 1; break; // W
                case SDLK_q: teclas[TECLA_Q] = 1; break; // A
                case SDLK_e: teclas[TECLA_E] = 1; break; // D
                case SDLK_y: teclas[TECLA_Y] = 1; break; // W
                case SDLK_u: teclas[TECLA_U] = 1; break; // A
                case SDLK_i: teclas[TECLA_I] = 1; break; // D
                case SDLK_o: teclas[TECLA_O] = 1; break; // W
                case SDLK_p: teclas[TECLA_P] = 1; break; // A
                case SDLK_f: teclas[TECLA_F] = 1; break; // D
                case SDLK_g: teclas[TECLA_G] = 1; break; // W
                case SDLK_h: teclas[TECLA_H] = 1; break; // A
                case SDLK_j: teclas[TECLA_J] = 1; break; // D
                case SDLK_k: teclas[TECLA_K] = 1; break; // W
                case SDLK_l: teclas[TECLA_L] = 1; break; // D
                case SDLK_z: teclas[TECLA_Z] = 1; break; // W
                case SDLK_x: teclas[TECLA_X] = 1; break; // A
                case SDLK_c: teclas[TECLA_C] = 1; break; // D
                case SDLK_v: teclas[TECLA_V] = 1; break; // W
                case SDLK_b: teclas[TECLA_B] = 1; break; // A
                case SDLK_n: teclas[TECLA_N] = 1; break; // D
                case SDLK_m: teclas[TECLA_M] = 1; break; // W
                case SDLK_1: teclas[TECLA_1] = 1; break; // W
                case SDLK_2: teclas[TECLA_2] = 1; break; // D
                case SDLK_3: teclas[TECLA_3] = 1; break; // W
                case SDLK_4: teclas[TECLA_4] = 1; break; // A
                case SDLK_5: teclas[TECLA_5] = 1; break; // D
                case SDLK_6: teclas[TECLA_6] = 1; break; // W
                case SDLK_7: teclas[TECLA_7] = 1; break; // A
                case SDLK_8: teclas[TECLA_8] = 1; break; // D
                case SDLK_9: teclas[TECLA_9] = 1; break; // W
                case SDLK_0: teclas[TECLA_0] = 1; break; // W
                default: break;
            }
        }
        //se soltou alguma teclas
        else if (eventos.type == SDL_KEYUP)
        {
            switch(eventos.key.keysym.sym)
            {
                //TECLAS ESPECIAIS
                case SDLK_RIGHT: teclas[TECLA_SETA_DIREITA] = 0; break; // ->
                case SDLK_LEFT: teclas[TECLA_SETA_ESQUERDA] = 0; break; // <-
                case SDLK_UP: teclas[TECLA_SETA_CIMA] = 0; break; // ^
                case SDLK_DOWN: teclas[TECLA_SETA_BAIXO] = 0; break; // \/
                case SDLK_RETURN: teclas[TECLA_ENTER] = 0; break; // Frame Rate Cap
                case SDLK_ESCAPE: teclas[TECLA_ESC] = 0; break; //esc
                case SDLK_BACKSPACE: teclas[TECLA_BACKSPACE] = 0; break; //esc

                //ALFA NUMERICO
                case SDLK_a: teclas[TECLA_A] = 0; break; // A
                case SDLK_t: teclas[TECLA_T] = 0; break; // ANIMACOES PAUSA
                case SDLK_r: teclas[TECLA_R] = 0; break; //R
                case SDLK_s: teclas[TECLA_S] = 0; break; //R
                case SDLK_d: teclas[TECLA_D] = 0; break; // D
                case SDLK_w: teclas[TECLA_W] = 0; break; // W
                case SDLK_q: teclas[TECLA_Q] = 0; break; // A
                case SDLK_e: teclas[TECLA_E] = 0; break; // D
                case SDLK_y: teclas[TECLA_Y] = 0; break; // W
                case SDLK_u: teclas[TECLA_U] = 0; break; // A
                case SDLK_i: teclas[TECLA_I] = 0; break; // D
                case SDLK_o: teclas[TECLA_O] = 0; break; // W
                case SDLK_p: teclas[TECLA_P] = 0; break; // A
                case SDLK_f: teclas[TECLA_F] = 0; break; // D
                case SDLK_g: teclas[TECLA_G] = 0; break; // W
                case SDLK_h: teclas[TECLA_H] = 0; break; // A
                case SDLK_j: teclas[TECLA_J] = 0; break; // D
                case SDLK_k: teclas[TECLA_K] = 0; break; // W
                case SDLK_l: teclas[TECLA_L] = 0; break; // D
                case SDLK_z: teclas[TECLA_Z] = 0; break; // W
                case SDLK_x: teclas[TECLA_X] = 0; break; // A
                case SDLK_c: teclas[TECLA_C] = 0; break; // D
                case SDLK_v: teclas[TECLA_V] = 0; break; // W
                case SDLK_b: teclas[TECLA_B] = 0; break; // A
                case SDLK_n: teclas[TECLA_N] = 0; break; // D
                case SDLK_m: teclas[TECLA_M] = 0; break; // W
                case SDLK_1: teclas[TECLA_1] = 0; break; // W
                case SDLK_2: teclas[TECLA_2] = 0; break; // D
                case SDLK_3: teclas[TECLA_3] = 0; break; // W
                case SDLK_4: teclas[TECLA_4] = 0; break; // A
                case SDLK_5: teclas[TECLA_5] = 0; break; // D
                case SDLK_6: teclas[TECLA_6] = 0; break; // W
                case SDLK_7: teclas[TECLA_7] = 0; break; // A
                case SDLK_8: teclas[TECLA_8] = 0; break; // D
                case SDLK_9: teclas[TECLA_9] = 0; break; // W
                case SDLK_0: teclas[TECLA_0] = 0; break; // W
                default: break;
            }
        }

    }
}

//se clicou pra sair do jogo
void Game::Menu()
{
    //se ja estiver no menu principal, volta pra tela anterior
    if (tela == TELA_PAUSA)
    {
        //VOLTA TELA ATUAL PARA A TELA PRINCIPAL
        tela = TELA_PRINCIPAL;

        //DESPAUSA TODOS OS TIMERS DA FASE
        FaseAtual->ControleTimer(false);
    }
    //senao, seta tela para tela de menupause, para que abra o menu no gameloop dessa classe
    else
    {
        //DEFINE A TELA ATUAL PARA TELA PAUSA
        tela = TELA_PAUSA;

        //PAUSA TODOS OS TIMERS DA FASE
        FaseAtual->ControleTimer(true);
    }

    teclas[TECLA_ESC] = 0;
}

//seta o proximo nivel para trocar no proximo loop
void Game::SetProximoNivel(NIVEIS ProximoNivel)
{
    ProximaFase = ProximoNivel;
    bTrocaFase = true;
}

//FUNCAO QUE RETORNA AS TECLAS
int* Game::GetTeclas()
{
    return teclas;
}

//FUNCAO QUE RETORNA A SURFACE DA JANELA
SDL_Surface* Game::GetSurfacePrincipal()
{
    return janela.GetSurface();
}

//FUNCAO QUE RETORNA A ALTURA DA JANELA
int Game::GetAlturaJanela()
{
    return janela.iAltura;
}

//FUNCAO QUE RETORNA A LARGURA DA JANELA
int Game::GetLarguraJanela()
{
    return janela.iLargura;
}

//FUNCAO QUE RETORNA A ESTRUTRA JANELA DO JOGO CONTENDO TODOS AS INFORMACOES REFERENTE A JANELA
Janela& Game::GetJanela()
{
    return janela;
}

//FUNCAO QUE RETORNA A DIFICULDADE ATUAL
DIFICULDADES Game::GetDificuldade()
{
    return dificuldade;
}

//FUNCAO QUE RETORNA O SAVE ATUAL
Save& Game::GetSave()
{
    return save;
}

//FUNCAO QUE SETA UM NOVO SALVE
void Game::SetSave(Save novoSave)
{
    save = novoSave;
}

//FUNCAO QUE RETORNA A VERSAO DO JOGO
char* Game::GetVersao()
{
    return janela.GetVersao();
}

//FUNCAO QUE SETA A QUANTOS FPS O JOGO VAI RODAR
void Game::SetFPS(int NovoFPS)
{
    FPS = NovoFPS;
}

//FUNCAO QUE RETORNA QUANTOS FPS O JOGO TA RODANDO
int Game::GetFPS()
{
    return FPS;
}

//FUNCAO QUE SETA A COR PADRAO, QUE É ULTILIZADA PARA LIMPAR A JANELA
void Game::SetCorPadrao(int iCor)
{
    COR_PADRAO = iCor;
}

//FUNCAO QUE RETORNA A COR PADRAO
int Game::GetCorPadrao()
{
    return COR_PADRAO;
}

//FUNCAO QUE RETORNA A REFERENCIA DA FASE ATUAL DO JOGO
Fase* Game::GetFaseAtual()
{
    return FaseAtual;
}

//FUNCAO QUE TROCA DE NIVEIS CASO NECESSARIO
void Game::GerenciaNiveis()
{
    //deleta o nivel atual
    if (FaseAtual != NULL)
    {
        FaseAtual->FimDoNivel();
        delete FaseAtual;
    }

    //instancia o novo nivel segundo a ENUM de niveis
    switch (ProximaFase)
    {
        case NIVEL_MENU_PRINCIPAL:
            FaseAtual = new MenuPrincipal();
            break;
        case NIVEL_JOGO_UM:
            FaseAtual = new PrimeiraFase();
            break;
        case NIVEL_MORTE:
            FaseAtual = new FaseMorte();
            break;
        case NIVEL_CONFIGURACOES:
            FaseAtual = new Configuracoes();
            break;
        default:
            break;
    }

    bTrocaFase = false;
    iFrameAtual = 0;

    if (tela == TELA_PAUSA)
        tela = TELA_PRINCIPAL;
}

//funcao que gera um novo ID para cada objeto do jogo
unsigned long int Game::GetIdObject()
{
    return iIdObject++;
}

//FUNCAO QUE RETORNA O ZOOM DO JOGO
int Game::GetZoom()
{
    return iZoom;
}

//FUNCAO QUE SETA O ZOOM NO JOGO
void Game::SetZoom(int iNovoZoom)
{
    iZoom = iNovoZoom;
}

//FUNCAO QUE RETORNA UM PONTOCARTESIANO COM AS INFORMACOES DE CENTRO DA JANELA
PontoCartesiano& Game::GetCentroJanela()
{
    PontoCartesiano pc;

    pc.SetX(GetLarguraJanela() / 2);
    pc.SetY(GetAlturaJanela() / 2);

    return pc;
}

//FUNCAO QUE RETORNA UM PONTOCARTESIANO COM AS COORDENADAS X;Y QUE REPRESENTAM O CENTRO DA JANELA PARA O GAMEOBJECT QUE VEIO POR PARAMETRO
PontoCartesiano& Game::CentralizaObjetoTela(GameObject* gameObjectCentralizar)
{
    PontoCartesiano pc;
    pc = GetCentroJanela();

    pc.SetX(pc.GetX() - gameObjectCentralizar->GetLarguraImagem() / 2);
    pc.SetY(pc.GetY() - gameObjectCentralizar->GetAlturaImagem() / 2);

    return pc;
}

//FUNCAO QUE SETA O FIM DA PARTIDA E SETA UM RESULTADO, SE GANHOU OU PERDEU
void Game::FimDaPartida(bool resultado)
{
    bResultado = resultado;
    save.SetNivel(FaseAtual->GetNivel());
    SetProximoNivel(NIVEL_MORTE);
}

//FUNCAO QUE CONTINUA O JOGO A PARTIR DA FASE DO SAVE ATUAL
void Game::ContinuaJogo()
{
    SetProximoNivel(save.GetNivel());
}

//FUNCAO QUE SALVA O JOGO
void Game::SalvaJogo()
{
    save.Salvar();
}

//FUNCAO QUE FECHA O JOGO
void Game::FecharJogo()
{
    ESTADO_JOGO = 0;
}

//FUNCAO QUE RETORNA TODOS OS SCORES LIDOS DO ARQUIVO
Save* Game::GetScores(int* iQuantidadeRetorno)
{
    Save* s;
    Save* lista;
    Save* temp;
    int iMaximoLista = 50;
    int nivel, dificuldade, pontuacao, i = 0;
    char c[1]{'\0'};
    int bResult;
    *iQuantidadeRetorno = 0;

    ifstream* leitor = new ifstream();

    //CRIA A LISTA DE SAVES
    lista = new Save[iMaximoLista];

    //ABRE O ARQUIVO
    leitor->open(arqSave, leitor->in);

    //ANDA COM O LEITOR
    leitor->get();

    //SE CHEGOU NO FIM DO ARQUIVO, NAO TEM NADA, ENTAO RETORNA NULO
    if (leitor->eof())
        return NULL;
    //SE ARQUIVO TEM ALGO, "DESPEGA" O QUE TINHA PEGO ANTES
    else
        leitor->unget();

    //ENQUANTO O ARQUIVO TER INFORMACOES
    for (i = 0; leitor->good(); i++)
    {
        s = new Save();
        //LE AS INFORMACOES DO ARQUIVO
        *leitor >> s->cIniciaisJogador;
        *leitor >> pontuacao;
        *leitor >> dificuldade;
        *leitor >> nivel;
        *leitor >> bResult;

        //SETA PROPRIEDADES NO SAVE
        s->SetPontuacao(pontuacao);
        s->SetDificuldade(dificuldade);
        s->SetNivel(nivel);
        s->SetResultado(bResult);

        //SEATINGIU O MAXIMO DA LISTA
        if (i == iMaximoLista)
        {
            //DOBRA A LISTA E PASSA AS INFORMACOES PARA A LISTA ANTIFA
            temp = new Save[(iMaximoLista *= 2)];

            for (register int j = 0; j < i; j++)
                temp[j] = lista[j];

            lista = temp;
        }

        //SETA A NOVA REFERENCIA NA LISTA
        lista[i] = *s;
    }

    //QUANTIDADE DERETORNO É SEMPRE A QUANTIDADE DE LEITURA MENOS UM PORQUE O \N DA ULTIMA LINHA NAO CONTA COMO SAVE
    *iQuantidadeRetorno = --i;

    //DIMINIU A LISTA DE RETORNO PARA A QUANTIDADE REAL
    temp = new Save[*iQuantidadeRetorno];

    //PASSA AS REFERENCIAS PARA A LISTA ANTIGA
    for (register int i = 0; i < *iQuantidadeRetorno; i++)
        temp[i] = lista[i];

    //TROCA AS REFERENCIAS
    lista = temp;

    //FECHA ARQUIVO E DELETA LEITOR DA MEMÓRIA
    leitor->close();
    delete leitor;

    //RETORNA LISTA COM OS SCORES
    return lista;
}

//FUNCAO QUE CARREGA AS NAVES, INIMIGOS E TIROS
void Game::CarregaObjetos()
{
    LeNaves();
    LeInimigos();
    LeTiros();
}

//FUNCAO QUE LE AS NAVES DO JOGO
void Game::LeNaves()
{
    Nave* nave;

    //SETA CONTROLES PRE DEFINIDOS
    Controle controleNave{TECLA_SETA_DIREITA, TECLA_SETA_ESQUERDA, TECLA_SETA_CIMA, TECLA_SETA_BAIXO};
    Controle controleEscudo{TECLA_D, TECLA_A, TECLA_W, TECLA_S};
    Controle controle;

    //VARIAVEIS QUE REPRESENTAM AS PROPRIEDADES DA CLASSE NAVE
    int iIntervaloTiro, iIntervaloEspecial, iTempoEspecial, iHP, iAlturaFrame, iLarguraFrame, iVelocidadeAnimacao, iJogador, iEspecial, iQuantTirosDisparo;
    float fVelocidade;
    int tipoTiro;
    int tipo;
    int tipoNave;
    char* caminhoImagem, *nome;

    //INSTANCIA OS CHAR ARRAYS PARA NOME E CAMINHO DA IMAGEM
    caminhoImagem = new char[50]{'\0'};
    nome = new char[50]{'\0'};

    //ABRE ARQUIVO
    ifstream* leitor = new ifstream(arqNaves);

    //PEGA A QUANTIDADE DE NAVES
    *leitor >> iQuantNave;

    //CRIA O ARRAY DE REFERENCIAS COM A QUANTIDADE DENAVES
    todasNaves = new Nave*[iQuantNave];

    //PERCORRE TODO O ARQUIVO
    for (register int i = 0; i < iQuantNave; i++)
    {
        //PEGA AS INFORMACOES DAS NAVES
        *leitor >> nome;
        *leitor >> caminhoImagem;
        *leitor >> iJogador;
        *leitor >> tipoNave;
        *leitor >> iIntervaloTiro;
        *leitor >> iIntervaloEspecial;
        *leitor >> iTempoEspecial;
        *leitor >> fVelocidade;
        *leitor >> iHP;
        *leitor >> iAlturaFrame;
        *leitor >> iLarguraFrame;
        *leitor >> iVelocidadeAnimacao;
        *leitor >> tipoTiro;
        *leitor >> iEspecial;
        *leitor >> iQuantTirosDisparo;

        //SE O JOGADOR FOR UMA NAVE, SETA CONTROLE DE NAVE
        if (iJogador == 0)
            controle = controleNave;
        //SE O JOGADOR FOR UM ESCUDO, SETA CNTROLE DE ESCUDO
        else
            controle = controleEscudo;

        //CRIA OBJETO COM AS INFORMACOES DO ARQUIVO
        nave = new Nave(caminhoImagem, (TIPO_NAVES) tipoNave, (JOGADOR) iJogador, controle);

        //SETA AS PROPRIEDADES NO OBJETO
        nave->SetIntervaloTiro(iIntervaloTiro);
        nave->SetIntervaloEspecial(iIntervaloEspecial);
        nave->SetTempoEspecial(iTempoEspecial);
        nave->GetVetor().SetMagnitude(fVelocidade);
        nave->SetHP(iHP);
        nave->SetNome(nome);
        nave->SetTipoTiro((TIROS) tipoTiro);
        nave->SetEspecial((ESPECIAIS)iEspecial);
        nave->SetCentroDeMassa(CentralizaObjetoTela(nave));

        //SE A ALTURA DO FRAME E LARGURA FOREM DIFERENTES DE 0, CRIA A ANIMACAO
        if (iLarguraFrame != 0 && iAlturaFrame != 0)
        {
            SDL_Rect r;
            r.h = iAlturaFrame;
            r.w = iLarguraFrame;
            nave->ControiAnimacao(&r, iVelocidadeAnimacao);
        }

        //SETA O CENTRO DE MASSA COMO O CENTRO DA TELA
        nave->SetCentroDeMassa(CentralizaObjetoTela(nave));

        //ADICIONA NAS REFENCIAS
        todasNaves[i] = nave;

        //CRIA NOVAS REFERENCIAS PARA CAMINHO DA IMAGEM E NOME
        caminhoImagem = new char[50]{'\0'};
        nome = new char[50]{'\0'};
    }

    //libera memória
    delete leitor;
}

//FUNCAO QUE LE OS TIROS DO ARQUIVO
void Game::LeTiros()
{
    Tiro* tiro;

    //PROPRIEDADES QUE REPRESENTAM AS INFORMACOES DO TIRO
    int iDano, iAlturaFrame, iLarguraFrame, iVelocidadeAnimacao;
    float fVelocidade;
    int tipoTiro;
    int tipo;
    char* caminhoImagem;

    //INSTANCIA ARRAY PARA RECEBER CAMINHO DA IMAGEM
    caminhoImagem = new char[50]{'\0'};

    //CRIA LEITOR PARA ARQUIVO
    ifstream* leitor = new ifstream(arqTiros);

    //PEGA QUANTIDADE DE TIROS
    *leitor >> iQuantTiros;

    //CRIA O ARRAY DE REFERENCAS COM A QUANTIDADE CORRETA
    todosTiros = new Tiro*[iQuantTiros];

    //PERCORRE O ARQUIVO
    for (register int i = 0; i < iQuantTiros; i++)
    {
        //PEGA AS INFORMACOES DO TIRO
        *leitor >> caminhoImagem;
        *leitor >> tipoTiro;
        *leitor >> iDano;
        *leitor >> fVelocidade;
        *leitor >> iAlturaFrame;
        *leitor >> iLarguraFrame;
        *leitor >> iVelocidadeAnimacao;

        //INSTANCIA O OBJETO E SETA AS PROPRIEDADES
        tiro = new Tiro(caminhoImagem, (TIROS) tipoTiro, (TIPO_OBJETOS) OBJETO_TIRO_INIMIGO);
        tiro->SetDano(iDano);
        tiro->GetVetor().SetMagnitude(fVelocidade);

        //SE A LARGURA E ALTURA DO FRAME FOR DIFERENTE DE ZERO, CRIA ANIMACAO
        if (iLarguraFrame != 0 && iAlturaFrame != 0)
        {
            SDL_Rect r;
            r.h = iAlturaFrame;
            r.w = iLarguraFrame;
            tiro->ControiAnimacao(&r, iVelocidadeAnimacao);
        }

        //ADICIONA TIRO NAS REFERENCIAS
        todosTiros[i] = tiro;

        //CRIA NOVA REFERENCIA PARA A PROXIMA IMAGEM
        caminhoImagem = new char[50]{'\0'};
    }

    //LIBERA MEMÓRIA
    delete leitor;
}

//FUNCAO QUE LE OS INIMIGOS DO ARQUIVO
void Game::LeInimigos()
{
    Inimigo* inimigo;

    //PROPRIEDADES DO INIGIMIGO
    int iHP, iAlturaFrame, iLarguraFrame, iVelocidadeAnimacao, iPontoPorMorte, iIntervaloTiro, iComportamento;
    float fVelocidade;
    int tipoTiro;
    int tipoInimigo;
    char* caminhoImagem;

    //REFERENCIA PARA O CAMINHO DA IMAGEM
    caminhoImagem = new char[50]{'\0'};

    //ABRE ARQUIVO
    ifstream* leitor = new ifstream(arqInimigos);

    //PEGA A QUANTIDADE
    *leitor >> iQuantInimigos;

    //CRIA ARRAY DE REFERENCIA COM A QUANTIDADE CORRETA
    todosInimigos = new Inimigo*[iQuantInimigos];

    //PERCORRE O ARQUIVO DE INIMIGOS
    for (register int i = 0; i < iQuantInimigos; i++)
    {
        //LE AS PROPRIEDADES DO INIMIGO
        *leitor >> caminhoImagem;
        *leitor >> tipoInimigo;
        *leitor >> fVelocidade;
        *leitor >> iIntervaloTiro;
        *leitor >> iHP;
        *leitor >> iAlturaFrame;
        *leitor >> iLarguraFrame;
        *leitor >> iVelocidadeAnimacao;
        *leitor >> tipoTiro;
        *leitor >> iComportamento;
        *leitor >> iPontoPorMorte;

        //CRIA OBJETO INIMIGO
        inimigo = new Inimigo(caminhoImagem, (INIMIGOS) tipoInimigo);

        //SETA AS PROPRIEDADES RELEVANTES DO TIRO
        inimigo->SetIntervaloTiro(iIntervaloTiro);
        inimigo->GetVetor().SetMagnitude(fVelocidade);
        inimigo->SetHP(iHP);
        inimigo->SetTipoTiro((TIROS) tipoTiro);
        inimigo->SetPontosPorMorte(iPontoPorMorte);
        inimigo->SetComportamento((COMPORTAMENTO_INIMIGO) iComportamento);

        //SE A ALTURA E LARGURA FOR DIFERENTE DE 0, CRIA ANIMACAO
        if (iLarguraFrame != 0 && iAlturaFrame != 0)
        {
            SDL_Rect r;
            r.h = iAlturaFrame;
            r.w = iLarguraFrame;
            inimigo->ControiAnimacao(&r, iVelocidadeAnimacao);
        }

        //SETA CENTRO DE MASSA COMO CENTRO DA JANELA
        inimigo->SetCentroDeMassa(CentralizaObjetoTela(inimigo));

        //ADICIONA NA REFERENCIA DE INIMIGOS
        todosInimigos[i] = inimigo;

        //CRIA NOVA REFERENCIA PARA CAMINHO DA IMAGEM
        caminhoImagem = new char[50]{'\0'};
    }

    //LIBERA MEMÓRIA
    delete leitor;
}

//FUNCAO QUE RETORNA 3 ÓRBITAS
int* Game::GetOrbitas()
{
    int orbitas[3];

    orbitas[0] = (GetLarguraJanela() / 6) * 1;
    orbitas[1] = (GetLarguraJanela() / 6) * 2;
    orbitas[2] = (GetLarguraJanela() / 6) * 3;

    return orbitas;
}

//FUNCAO QUE RETORNA UMA NOVA REFERENCIA SEGUNDO O TIPO DESEJADO
Nave* Game::GetNaves(TIPO_NAVES tipoNave)
{
    Nave* nave = NULL;

    //PERCORRE TODAS AS NAVES
    for (register int i = 0; i < iQuantNave; i++)
    {
        //SE A NAVE ATUAL É DIFERENTE DA DESEJADA, PASSA PARA PROXIMA
        if (todasNaves[i]->GetNave() != tipoNave)
            continue;

        //SE A NAVE ATUAL É IGUAL A NAVE DESEJADA, CRIA O OBJETO E SETA AS PROPRIEDADES RELEVANTES
        nave = new Nave(todasNaves[i]->GetCaminhoImagem(), todasNaves[i]->GetNave(), todasNaves[i]->GetJogador(), todasNaves[i]->GetControles());
        nave->SetPontoPolar(todasNaves[i]->GetPontoPolar());
        nave->SetPontoCartesiano(todasNaves[i]->GetPontoCartesiano());
        nave->SetVetor(todasNaves[i]->GetVetor());
        nave->SetIntervaloTiro(todasNaves[i]->GetIntervaloTiro());
        nave->SetIntervaloEspecial(todasNaves[i]->GetIntervaloEspecial());
        nave->SetTempoEspecial(todasNaves[i]->GetTempoEspecial());
        nave->SetNome(todasNaves[i]->GetNome());
        nave->SetTipoTiro(todasNaves[i]->GetTipoTiro());
        nave->SetEspecial(todasNaves[i]->GetEspecial());
        nave->SetCentroDeMassa(Game::CentralizaObjetoTela(nave));
        nave->ControiAnimacao(todasNaves[i]->GetAnimacao().GetClipAtual(), todasNaves[i]->GetAnimacao().GetVelocidade());

        //QUEBRA O FOR PORQUE JA ACHAMOS O QUE QUERIAMOS
        break;
    }

    //RETORNA A REFERENCIA DA NOVA NAVE
    return nave;
}

//FUNCAO QUE RETORNA O TIRO SEGUNDO O TIPO DESEJADO
Tiro* Game::GetTiros(TIROS tipoTiro)
{
    Tiro* tiro = NULL;

    //PERCORRE TODOS OS TIROS DISPONIVEIS
    for (register int i = 0; i < iQuantTiros; i++)
    {
        //SE O TIRO ATUAL FOR DIFERENTE DO DESEJADO, PASSAPARA PROXIMO
        if (todosTiros[i]->GetTipoTiro() != tipoTiro)
            continue;

        //SE O TIRO ATUAL É O DESEJADO, CRIA NOVA REFERENCA E SETA PROPRIEDADES REFERENTES A ELE
        tiro = new Tiro(todosTiros[i]->GetCaminhoImagem(), todosTiros[i]->GetTipoTiro(), todosTiros[i]->GetTipo());
        tiro->SetPontoPolar(todosTiros[i]->GetPontoPolar());
        tiro->SetPontoCartesiano(todosTiros[i]->GetPontoCartesiano());
        tiro->SetVetor(todosTiros[i]->GetVetor());
        tiro->SetDano(todosTiros[i]->GetDano());
        tiro->SetCentroDeMassa(Game::CentralizaObjetoTela(tiro));
        tiro->ControiAnimacao(todosTiros[i]->GetAnimacao().GetClipAtual(), todosTiros[i]->GetAnimacao().GetVelocidade());

        //QUEBRA FOR PORQUE JÁ ENCONTRAMOS O QUE QUERIAMOS
        break;
    }

    //RETORNA UMA NOVA REFERENCIA DO TIRO
    return tiro;
}

//FUNCAO QUE PEGA O INIMIGO SEGUNDO O TIPO DESEJADO
Inimigo* Game::GetInimigos(INIMIGOS tipoInimigo)
{
    Inimigo* inimigo = NULL;

    //PERCORRE TODOS OS INIMIGOS DESEJADOS
    for (register int i = 0; i < iQuantInimigos; i++)
    {
        //SE O INIMIGO ATUAL FOR DIFRERENTE DO DESEJADO, PULA PARA PROXIMO
        if (todosInimigos[i]->GetTipoInimigo() != tipoInimigo)
            continue;

        //SE INIMIGO ATUAL FOR O QUE QUEREMOS, CRIA NOVA REFERNCIA E SETA PROPRIEDADES RELEVANTES
        inimigo = new Inimigo(todosInimigos[i]->GetCaminhoImagem(), todosInimigos[i]->GetTipoInimigo());
        inimigo->SetVetor(todosInimigos[i]->GetVetor());
        inimigo->SetPontoPolar(todosInimigos[i]->GetPontoPolar());
        inimigo->SetIntervaloTiro(todosInimigos[i]->GetIntervaloTiro());
        inimigo->SetComportamento(todosInimigos[i]->GetComportamento());
        inimigo->SetHP(todosInimigos[i]->GetHP());
        inimigo->SetTipoTiro(todosInimigos[i]->GetTipoTiro());
        inimigo->SetPontosPorMorte(todosInimigos[i]->GetPontosPorMorte());
        inimigo->SetCentroDeMassa(Game::CentralizaObjetoTela(inimigo));
        inimigo->ControiAnimacao(todosInimigos[i]->GetAnimacao().GetClipAtual(), todosInimigos[i]->GetAnimacao().GetVelocidade());

        //QUEBRA FOR PORQUE JÁ ENCONTRAMOS O QUE QUERIAMOS
        break;
    }

    //RETORNA NOVA REFERENCIA
    return inimigo;
}

//FUNCAO QUE ROTACIONA UM GAMEOBJECT SEGUNDO SEY ANGULO
void Game::RotacionaGameObject(GameObject* gameObjectRotacionar)
{
    //rotaciona a imagem
    gameObjectRotacionar->SetImagem(rotozoomSurface(gameObjectRotacionar->GetImagemAuxiliar(), -gameObjectRotacionar->GetPontoPolar().GetAngulo(), GetZoom(), SMOOTHING_OFF));
}

//FUNCAO QUE RETORNA TRUE OU FALSE PARA SE O GAMEOBJECT ESTIVER DENTRO DA JANELA OU NAO
bool Game::DentroJanela(GameObject* gameobject)
{
    if (gameobject->GetPontoCartesiano().GetX() > GetLarguraJanela() || gameobject->GetPontoCartesiano().GetX() < 0)
        return false;

    if (gameobject->GetPontoCartesiano().GetY() > GetAlturaJanela() || gameobject->GetPontoCartesiano().GetY() < 0)
        return false;

    return true;
}

//FUNCAO QUE RETORNA A QUANTIDADE DE INIMIGOS MORTOS
int Game::GetInimigosMortos()
{
    return iInimigosMortos;
}

//FUNCAO QUE RETORNA A QUANTIDADE DE TIROS DISPARADOS PELO JOGADOR
int Game::GetTirosDisparados()
{
    return iTirosDisparados;
}

//FUNCAO QUE SETA QUANTIDADE DE INIMIGOS MORTOS
void Game::SetInimigosMortos(int iInimigos)
{
    iInimigosMortos = iInimigos;
}

//FUNCAO QUE SETA A QUANTIDADE DE TIROS DISPARADOS
void Game::SetTirosDisparados(int iTiros)
{
    iTirosDisparados = iTirosDisparados;
}

//FUNCAO QUE SOMA UM VALOR A QUANTIDADE DE INIMIGOS MORTOS
void Game::IncrementaInimigoMorto(int iValorSoma)
{
    iInimigosMortos += iValorSoma;
}

//FUNCAO QUE SOMA UM VALOR A QUANTIDADE DE TIROS DISPARADOS PELO JOGADOR
void Game::IncrementaTirosDisparados(int iValorSoma)
{
    iTirosDisparados += iValorSoma;
}

//FUNCAO QUE LIMPA OS DADOS DA PARTIDA ATUAL
void Game::LimpaDadosPartida()
{
    iTirosDisparados = 0;
    iInimigosMortos = 0;
    GetSave().SetPontuacao(0);
}

//FUNCAO QUE RETORNA O RESULTADO DA PARTIDA ATUAL
bool Game::GetResultadoPartida()
{
    return bResultado;
}

//FUNCAO QUE SETA UMA NOVA DIFICULDADE AO JOGO
void Game::SetDificuldade(DIFICULDADES novaDificuldade)
{
    dificuldade = novaDificuldade;
    save.SetDificuldade(dificuldade);
}

//FUNCAO RETORNA A SELECAO DE NAVES DO PERSONAGEM E A QUANTIDADE
int* Game::GetSelecaoNaves(int* iQuantNaves)
{
    *iQuantNaves = iQuantSelecaoNaves;
    return iSelecaoNaves;
}

//FUNCAO QUE SETA A SELECAO DE NAVES DO PERSONAGEM
void Game::SetSelecaoNaves(int* iNaves, int iQuantSelecao)
{
    iQuantSelecaoNaves = iQuantSelecao;
    iSelecaoNaves = iNaves;
}

//FUNCAO QUE RETORNA A QUANTIDADE DE NAVES DISPONIVEIS
int Game::GetQuantidadeNaves()
{
    return iQuantNave;
}

//FUNCAO QUE RETORNA A QUANTIDADE DE INIMIGOS DISPONIVEIS
int Game::GetQuantidadeInimigos()
{
    return iQuantInimigos;
}

//FUNCAO QUE RETORNA A QUANTIDADE DETIROS DISPONIVEIS
int Game::GetQuantidadeTiros()
{
    return iQuantTiros;
}

//FUNCAO QUE RETORNA UM NOVO ESCUDO
Nave* Game::GetEscudo()
{
    for (register int i = 0; i < iQuantNave; i++)
    {
        if (todasNaves[i]->GetJogador() == JOGADOR_ESCUDO)
        {
            return GetNaves((TIPO_NAVES) i);
        }
    }

    return NULL;
}

//FUNCAO QUE CARREGA UMA NOVA RESOLUSAO PRO JOGO
void Game::SetResolucao(int altura, int largura)
{
    janela.SetAltura(altura);
    janela.SetLargura(largura);

    if (janela.GetFullScreen())
        SDL_SetVideoMode(janela.iLargura, janela.iAltura, 32, SDL_SWSURFACE | SDL_FULLSCREEN);
    else
        SDL_SetVideoMode(janela.iLargura, janela.iAltura, 32, SDL_SWSURFACE);

    janela.Salvar();
}

//FUNCAO QUE SETA SE ESTA EM FULLSCREEN OU NAO
void Game::SetFullScreen(bool fullScreen)
{
    janela.SetFullScreen(fullScreen);

    if (janela.GetFullScreen())
        SDL_SetVideoMode(janela.iLargura, janela.iAltura, 32, SDL_SWSURFACE | SDL_FULLSCREEN);
    else
        SDL_SetVideoMode(janela.iLargura, janela.iAltura, 32, SDL_SWSURFACE);

    janela.Salvar();
}

//RETIRA O FUNDO DE UMA SURFACE IMAGEM
void Game::RetiraFundo(SDL_Surface* imagem, int iCorRetirar)
{
    //seta a cor de fundo como transparente
    SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, iCorRetirar);
}

//destrutor que libera os arrays, ponteirs e libera recursos da SDL
Game::~Game()
{
    if (FaseAtual != NULL)
        delete FaseAtual;

    if (faseMenu != NULL)
        delete faseMenu;

    if (todosInimigos != NULL)
    {
        for (register int i = 0; i < iQuantInimigos; i++)
            delete todosInimigos[i];

        delete todosInimigos;
    }

    if (todosTiros != NULL)
    {
        for (register int i = 0; i < iQuantTiros; i++)
            delete todosTiros[i];

        delete todosTiros;
    }

    if (todasNaves != NULL)
    {
        for (register int i = 0; i < iQuantNave; i++)
            delete todasNaves[i];

        delete todasNaves;
    }

    if (iSelecaoNaves != NULL)
        delete iSelecaoNaves;

    //libera recursos da SDL
    SDL_Quit();
}
