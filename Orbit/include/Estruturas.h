#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include "Headers.h"
#include "Timer.h"

//ENUM DE TIPO DE NAVES
typedef enum
{
    NAVE_UM, NAVE_DOIS, NAVE_TRES, NAVE_QUATRO, NAVE_CINCO, NAVE_SEIS
}TIPO_NAVES;

//ENUM DO TIPO DE JOGADOR
typedef enum
{
    JOGADOR_NAVE, JOGADOR_ESCUDO
}JOGADOR;

//ENUM DE CONTROLE DA TELA ATUAL
typedef enum
{
    TELA_PRINCIPAL, TELA_PAUSA
}TELA;

//ENUM DE CONTROLE DAS DIFICULDADES DO JOGO
typedef enum
{
    DIFICULDADE_INICIAL, DIFICULDADE_NORMAL, DIFICULDADE_DIFICIL, DIFICULDADE_MUTANTE
}DIFICULDADES;

//ENUM DE CONTROLE DAS TECLAS DE ENTRADA DO USUARIO
typedef enum
{
    TECLA_SETA_DIREITA, TECLA_SETA_ESQUERDA, TECLA_A, TECLA_D, TECLA_SETA_CIMA,
    TECLA_SETA_BAIXO, TECLA_C, TECLA_ESC, TECLA_W, TECLA_T, TECLA_R, TECLA_S, TECLA_SAIR, TECLA_ENTER,
    TECLA_Q, TECLA_E, TECLA_Y, TECLA_U, TECLA_I, TECLA_O, TECLA_P, TECLA_F,
    TECLA_G, TECLA_H, TECLA_J, TECLA_K, TECLA_L, TECLA_Z, TECLA_X, TECLA_V, TECLA_B, TECLA_N, TECLA_M,
    TECLA_1, TECLA_2, TECLA_3, TECLA_4, TECLA_5, TECLA_6, TECLA_7, TECLA_8, TECLA_9, TECLA_0, TECLA_BACKSPACE
}TECLAS;

//ENUM DE CONTROLE DOS NIVEIS
typedef enum
{
    NIVEL_MENU_PAUSE, NIVEL_MENU_PRINCIPAL, NIVEL_JOGO_UM, NIVEL_MORTE, NIVEL_CONFIGURACOES
}NIVEIS;

//ENUM DE CONTROLE DOS TIROS
typedef enum
{
    TIRO_UM, TIRO_DOIS, TIRO_TRES, TIRO_QUATRO, TIRO_CINCO, TIRO_SEIS
}TIROS;

//ENUM DE CONTROLES DOS TIPOS DE INIMIGOS
typedef enum
{
    INIMIGO_UM, INIMIGO_DOIS, INIMIGO_TRES
}INIMIGOS;

//ENUM DE CONTROLE DOS TIPOS DE OBJETOS
typedef enum
{
    OBJETO_INIMIGO, OBJETO_NAVE, OBJETO_TIRO_AMIGO,
    OBJETO_TIRO_INIMIGO, OBJETO_PLANETA, OBJETO_FUNDO, OBJETO_BOSS
}TIPO_OBJETOS;

//ENUM DE CONTROLE DOS TIPOS DE OBJETOS
typedef enum
{
    ESTAGIO_NIVEL_UM, ESTAGIO_NIVEL_DOIS, ESTAGIOS_NIVEL_TRES, ESTAGIOS_NIVEL_QUATRO, ESTAGIOS_NIVEL_CINCO
}ESTAGIOS_NIVEL;

//ENUM DE CONTROLE DOS TIPOS DE OBJETOS
typedef enum
{
    BOSS_UM
}BOSS;

//ENUM DE ESPECIAIS
typedef enum
{
    ESPECIAL_UM, ESPECIAL_DOIS, ESPECIAL_TRES
}ESPECIAIS;

//ENUM DE COMPORTAMENTO DE INIMIGO
typedef enum
{
    COMPORTAMENTO_UM, COMPORTAMENTO_DOIS, COMPORTAMENTO_TRES
}COMPORTAMENTO_INIMIGO;

//estrutura que representa o ponto cartesiano para colocar objetos na tela
typedef struct PontoCartesiano
{
    int iX, iY;

    PontoCartesiano()
    {
        iX = iY = 0;
    }

    //---------- GET E SET ------------------//

    void SetX(int iNovoX)
    {
        iX = iNovoX;
    }

    int GetX()
    {
        return iX;
    }

    void SetY(int iNovoY)
    {
        iY = iNovoY;
    }

    int GetY()
    {
        return iY;
    }
}PontoCartesiano;

//ESTRUTURA PARA CALCULAR O PONTO CARTESIANO DO OBJETO NA TELA, PARTINDO DE UM ANGULO E UM RAIO
typedef struct PontoPolar
{
    float fAngulo, fRaio;
    PontoCartesiano pcCentroDeMassa, pc;
    int iVoltas;

    PontoPolar()
    {
        fAngulo = fRaio = iVoltas = 0;
    }

    //funcao que seta um novo angulo para o ponto polar e corrige possiveis voltas
    void SetAngulo(float fNovoAngulo)
    {
        fAngulo = fNovoAngulo;

        //se passou de 360 volta para 0 para evitar angulos muito grande
        if (fAngulo >= 360)
        {
            fAngulo -= 360;
            iVoltas++;
        }
        //se ficou negativo, vai para 360 para evitar angulos negativos
        else if (fAngulo < -360)
        {
            fAngulo += 360;
            iVoltas++;
        }
    }

    //--------------- GET E SET -------------------------//
    float GetAngulo()
    {
       return fAngulo;
    }

    void SetRaio(float fNovoRaio)
    {
        fRaio = fNovoRaio;
    }

    float GetRaio()
    {
        return fRaio;
    }

    PontoCartesiano& GetCentroDeMassa()
    {
        return pcCentroDeMassa;
    }

    void SetCentroDeMassa(PontoCartesiano pc)
    {
        pcCentroDeMassa = pc;
    }

    int GetVoltas()
    {
        return iVoltas;
    }

    int SetVoltas(int voltas)
    {
        iVoltas = voltas;
    }

    //funcao que atualiza o ponto cartesiano
    void Atualiza()
    {
        GetCartesianoCorrespondente();
    }

    void SetPontoCartesiano(PontoCartesiano pontocartesiano)
    {
        pc = pontocartesiano;
    }

        //calcula qual é o ponto cartesiano que corresponde a este ponto polar e retorna em forma de estrutura
    //tambem já reajusta o centro do ponto polar no centro de massa
    PontoCartesiano& GetCartesianoCorrespondente()
    {
        pc.SetX(fRaio * cos(GetAnguloRadianos()));
        pc.SetY(fRaio * sin(GetAnguloRadianos()));
        pc.SetX(pc.GetX() + pcCentroDeMassa.GetX());
        pc.SetY(pc.GetY() + pcCentroDeMassa.GetY());

        return pc;
    }

    //transforma um angulo que estava em graus para radiano
    float GetAnguloRadianos()
    {
        return (PI/180)*fAngulo;
    }
}PontoPolar;

//estrutura vetor, tem este nome pq tem as propriedades de uma vetor fisico/matematico
typedef struct Vetor
{
    float fDirecao, fMagnitude, fMagnitudeAntiga;
    int iSentido;

    Vetor()
    {
        fDirecao = fMagnitude = fMagnitudeAntiga = 0;
        iSentido = 1;
    }

    //---------------- GET E SET -----------------------//

    void SetDirecao(float fNovaDirecao)
    {
        fDirecao = fNovaDirecao;
    }

    float GetDirecao()
    {
        return fDirecao;
    }

    void SetMagnitude(float fNovaMagnitude)
    {
        fMagnitudeAntiga = fMagnitude;

        if (fNovaMagnitude <= 0)
            fMagnitude = 0;
        else
            fMagnitude = fNovaMagnitude;
    }

    float GetMagnitude()
    {
        return fMagnitude;
    }

    float GetMagnitudeAntiga()
    {
        return fMagnitudeAntiga;
    }

    void SetSentido(int iNovoSentido)
    {
        iSentido = iNovoSentido;
    }

    int GetSentido()
    {
        return iSentido;
    }
}Vetor;

//estrutra que representa a janela do jogo, com largura, altura, titulo e se é fullscreen ou nao
//tambem contem a surface principal do jogo
typedef struct Janela
{
    SDL_Surface* surfaceJanela;
    char* cTitulo;
    int iAltura, iLargura, bFullScreen;
    char* versao;

    Janela()
    {
        surfaceJanela = NULL;
        cTitulo = new char[20];
        iAltura = iLargura = 0;
        bFullScreen = 0;
        versao = new char[50];
    }

    //--------------- GET E SET ------------------------//

    void SetSurface(SDL_Surface* surface)
    {
        surfaceJanela = surface;
    }

    SDL_Surface* GetSurface()
    {
        return surfaceJanela;
    }

    void SetTitulo(char* cNovoTitulo)
    {
        cTitulo = cNovoTitulo;
    }

    char* GetTitulo()
    {
        return cTitulo;
    }

    void SetAltura(int iNovaAltura)
    {
        iAltura = iNovaAltura;
    }

    int GetAltura()
    {
        return iAltura;
    }

    void SetLargura(int iNovaLargura)
    {
        iLargura = iNovaLargura;
    }

    int GetLargura()
    {
        return iLargura;
    }

    void SetFullScreen(bool bFull)
    {
        bFullScreen = bFull;
    }

    bool GetFullScreen()
    {
        return bFullScreen;
    }

    char* GetVersao()
    {
        return versao;
    }

    void SetVersao(char* novaVersao)
    {
        versao = novaVersao;
    }

    //FUNCAO QUE SALVA AS INFORMACOES NO ARQUIVO DE CONFIGURACAO
    void Salvar()
    {
        std::ofstream* escritor = new std::ofstream(arqConfig);

        //se nao conseguir abrir o arquivo
        if (!*escritor)
            return;

        //salva as informacoes no arquivo de leitura
        *escritor << iLargura << std::endl;
        *escritor << iAltura << std::endl;
        *escritor << cTitulo << std::endl;
        *escritor << versao << std::endl;
        *escritor << bFullScreen << std::endl;
    }

    ~Janela()
    {
        if (versao != NULL)
            delete versao;
    }
}Janela;

//ESTRUTURA QUE CRIA E GERENCIA A ANIMACAO DE UM GAMEOBJECT
typedef struct Animacao
{
    SDL_Rect* clip;
    int iFrameAtual, iTotalFrame, iVelocidade, iFrameInicial, iFrameFinal; //controle de frames e subanimacao
    bool bPausa, bFrente;
    Timer timerAnimacao;

    Animacao()
    {
        clip = NULL;
        iFrameFinal = iFrameInicial = iFrameAtual = iTotalFrame = 0;
        bPausa = bFrente = true;
        timerAnimacao.start();
    }

    //CONTRUTOR ONDE JÁ SE CRIA UMA ANIMACAO
    Animacao(int iLarguraImagem, int iAlturaImagem, int iLarguraFrame, int iAlturaFrame, int iVelocidade) : Animacao()
    {
        ControiAnimacao(iLarguraImagem, iAlturaImagem, iLarguraFrame, iAlturaFrame, iVelocidade);
    }

    //FUNCAO QUE CORTA A IMAGEM EM QUADROS NA HORIZONTAL E VERTICAL, BASEADO NO TAMANHO DA IMAGEM E NO TAMANHO DO FRAME DESEJADO
    void ControiAnimacao(int iLarguraImagem, int iAlturaImagem, int iLarguraFrame, int iAlturaFrame, int velocidade)
    {
        //calcula a quantidade de quadros que teremos na horizontal e verticial
        int nLargura = iLarguraImagem / iLarguraFrame;
        int nAltura = iAlturaImagem / iAlturaFrame;

        if (nLargura == 0)
            nLargura++;
        if (nAltura == 0)
            nAltura++;

        //calcula o total de frames na animacao
        iTotalFrame = nLargura * nAltura;

        //cria os clips que cortam a imagem em tempo de execucao já com a quantidade necessaria
        clip = new SDL_Rect[iTotalFrame];

        //seta o framefinal como o ultimo frame valido
        iFrameFinal = iTotalFrame - 1;

        //seta nova velocidade
        SetVelocidade(velocidade);

        //percorre todos os frames que teremos na animacao
        for (register int w = 0, h = 0, i = 0; i < iTotalFrame; i++, w++)
        {
            //se for multiplo do total de frames da largura
            //temos que incrementar o contador de largura para que o clip
            //desca uma posicao na imagem
            if (i % nLargura == 0 && i!= 0)
            {
                h++;
                w = 0;
            }

            //seta tamanho e posicao do clip na imagem
            clip[i].x = w * iLarguraFrame;
            clip[i].y = h * iAlturaFrame;
            clip[i].w = iLarguraFrame;
            clip[i].h = iAlturaFrame;
        }

        bPausa = false;
    }

    //passa para o proximo frame
    //se atigir o ultimo frame, volta para o inicial
    //ultimo frame e frame inicial são referente a subanimacao
    void PassaFrame()
    {
        if (iFrameAtual + 1 <= iFrameFinal)
        {
            iFrameAtual++;
        }
        else
        {
            iFrameAtual = iFrameInicial;
        }
    }

    //volta frame, assim como o passa frame, entretante este volta
    //se atingir o primeiro frame da animacao, volta para o ultimo
    //primeiro e ultimo tambem referentes a subanimacao
    void VoltaFrame()
    {
        if (iFrameAtual - 1 >= iFrameInicial)
        {
            iFrameAtual--;
        }
        else
        {
            iFrameAtual = iFrameFinal;
        }
    }

    //seta um frame desejado e se quer pausar a animacao ou nao
    void SetFrameAtual(int iFrame, bool bPausar)
    {
        if (iFrame >= 0 && iFrame < iTotalFrame)
            iFrameAtual = iFrame;

        bPausa = bPausar;
    }

    //----------------------- GET E SET ---------------------//

    //pausa a animacao para que nao troque os frames
    void Pausar(bool bPausar)
    {
        bPausa = bPausar;
    }

    //retorna o indice do frame atual da animacao
    int GetFrameAtual()
    {
        return iFrameAtual;
    }

    //retorna o total de frames da animacao
    int GetTotalFrames()
    {
        return iTotalFrame;
    }

    //retorna o frame inicial da subanimacao
    int GetFrameInicial()
    {
        return iFrameInicial;
    }

    //retorna o frame final da subaniamacao
    int GetFrameFinal()
    {
        return iFrameFinal;
    }

    //retorna os indices do inicio e fim da ultima animacao por referencia
    void GetFramesAnimacao(int* iFrameInicio, int* iFrameFim)
    {
        *iFrameInicio = iFrameInicial;
        *iFrameFim = iFrameFinal;
    }

    //retorna o vetor de clips e o total de frames por referencia
    SDL_Rect* GetClp(int* nClip)
    {
        *nClip = iTotalFrame;
        return clip;
    }

    //retorna um clip contendo o modelo do frame
    //com tamanho
    //IMPORTANTE VALIDAR NULO NA CHAMADA DA FUNCAO
    SDL_Rect* GetModeloFrame()
    {
        if (clip != NULL)
            return &clip[0];
        else
            return NULL;
    }

    //seta uma nova velocidade para animacao
    void SetVelocidade(int iNovaVelocidade)
    {
        if (iNovaVelocidade == 0)
            iNovaVelocidade = -1;

        iVelocidade = iNovaVelocidade;
    }

    //retorna a velocidade atual da animacao
    int GetVelocidade()
    {
        return iVelocidade;
    }

    //seta uma subanimacao
    //a animacao vai rodar dentro do range destes parametros
    //QUANDO NAO HA SUBANIMACAO, A ANIMACAO PERCORRE TODOS OS FRAMES
    void SetSubAnimacao(int iFrameInicio, int iFrameFim)
    {
        if (iFrameInicio >= 0)
            iFrameInicial = iFrameInicio;

        if (iFrameFim < iTotalFrame)
            iFrameFinal = iFrameFim;
    }

    //seta o sentido da animacao, se ela vai percorrer os frames para frente ou para tras
    void SetSentidoAnimacao(bool bParaFrente)
    {
        bFrente = bParaFrente;
    }

    //retorna o clip atual da animacao
    SDL_Rect* GetClipAtual()
    {
        if (clip != NULL)
            return &clip[iFrameAtual];
        else
            return NULL;
    }

    //funcao que atualiza o frame da animacao segundo a velocidade
    void Atualiza()
    {
        //se a animacao estiver pausada, pausa o timer
        if (bPausa)
        {
            timerAnimacao.pause();
            return;
        }

        //despausa o timer, nao ha efeito caso ele nao esteja pausado
        timerAnimacao.unpause();

        //se nao for hora de trocar de frame, retorna
        if (timerAnimacao.get_ticks() < (1000 / iVelocidade))
            return;

        //passa o frame segundo o sentido desejado
        if (bFrente)
            PassaFrame();
        else
            VoltaFrame();

        //reseta timer
        timerAnimacao.start();
    }

    bool Pausada()
    {
        return bPausa;
    }

    void Resetar()
    {
        iFrameAtual = iFrameInicial;
    }

    SDL_Rect* GetClip()
    {
        return clip;
    }

    ~Animacao()
    {
        if (clip != NULL)
            delete clip;
    }
}Animacao;

//ESTRUTURA QUE REPRESENTA UM CONTROLE DE USUARIO COM 5 DISPOSITIVOS FISICOS
typedef struct Controle
{
    TECLAS controle_um, controle_dois, controle_tres, controle_quatro, controle_cinco;
}Controle;

//estrutura que representa um save no jogo, contem nome, pontuacao e tela que o jogador parou
typedef struct Save
{
    char* cIniciaisJogador;
    int iPontuacao;
    DIFICULDADES dificuldade;
    NIVEIS nivelAtual;
    bool bResultado;

    Save()
    {
        cIniciaisJogador = new char[3]{'A', 'A', 'A'};
        iPontuacao = 0;
        nivelAtual = NIVEL_JOGO_UM;
        dificuldade = DIFICULDADE_NORMAL;
        bResultado = false;
    }

    //---------------------- GET E SET -------------------------------//

    int GetPontuacao()
    {
        return iPontuacao;
    }

    void SetPontuacao(int iNovaPontuacao)
    {
        iPontuacao = iNovaPontuacao;
    }

    void SomaPontuacao(int iSomaPontuacao)
    {
        iPontuacao += iSomaPontuacao;
    }

    NIVEIS GetNivel()
    {
        return nivelAtual;
    }

    void SetNivel(NIVEIS nivel)
    {
        nivelAtual = nivel;
    }

    void SetNivel(int nivel)
    {
        nivelAtual = (NIVEIS) nivel;
    }

    DIFICULDADES GetDificuldade()
    {
        return dificuldade;
    }

    void SetDificuldade(DIFICULDADES dif)
    {
        dificuldade = dif;
    }

    void SetDificuldade(int dif)
    {
        dificuldade = (DIFICULDADES) dif;
    }

    char* GetInicias()
    {
        return cIniciaisJogador;
    }

    void SetInicias(char* iniciais)
    {
        cIniciaisJogador = iniciais;
    }

    void SetResultado(bool resultado)
    {
        bResultado = resultado;
    }

    bool GetResultado()
    {
        return bResultado;
    }

    //FUNCAO QUE SALVA O JOGO. GRAVA NO ARQUIVO TODAS AS PROPRIEDADES DESTE SAVE
    void Salvar()
    {
        cIniciaisJogador[3] = '\0';
        std::ofstream leitor;
        leitor.open(arqSave, leitor.app);
        leitor << cIniciaisJogador << std::endl;
        leitor << iPontuacao << std::endl;
        leitor << nivelAtual << std::endl;
        leitor << dificuldade << std::endl;
        leitor << bResultado << std::endl;
    }
}Save;

//ESTRUTURA UTILIZA PARA MANUPULACAO DE MENUS
typedef struct Menu
{
    int iPosicaoAtual;
    int iTotalPosicao;
    SDL_Rect rectModelo;
    SDL_Rect rectDesenho;
    SDL_Rect posicaoTela;
    int iCorFundo;
    int iCorFundoSelecionado;
    int iCorLetra;
    int iCorSelecionada;
    int iDistancia;
    SDL_Surface* surfacePai;
    char* cOpcoes;
    bool bHorizontal;

    Menu(SDL_Surface* surface, char* cOpcoesSeparadasPorQuebraLinha, int corFundo, int corFundoSelecionado,
         int corLetra, int corLetraSelecionada, bool horizontal, SDL_Rect modelo, SDL_Rect posicao)
    {
        iTotalPosicao = 0;
        iPosicaoAtual = 0;
        cOpcoes = cOpcoesSeparadasPorQuebraLinha;
        iCorLetra = corLetra;
        iCorFundo = corFundo;
        iCorSelecionada = corLetraSelecionada;
        rectModelo = modelo;
        posicaoTela = posicao;
        surfacePai = surface;
        iDistancia = 10;
        bHorizontal = horizontal;
        iCorFundoSelecionado = corFundoSelecionado;

        //PEGA O TOTAL DE OPCOES
        for (int i = 0; cOpcoes[i] != '\0'; i++)
            if (cOpcoes[i] == '\n')
                iTotalPosicao++;
    }

    //----------------- GET E SET --------------------------//

    void SetOpcaoSelecionada(int posicao)
    {
        if (posicao > 0 && posicao < iTotalPosicao)
            iPosicaoAtual = posicao;
    }

    int GetOpcaoSelecionada()
    {
        return iPosicaoAtual;
    }

    void SetCorOpcaoSelecionada(int corPosicao)
    {
        iCorSelecionada = corPosicao;
    }

    int GetCorOpcaoSelecionada()
    {
        return iCorSelecionada;
    }

    void SetCorFundo(int corFundo)
    {
        iCorFundo = corFundo;
    }

    int GetCorFundo()
    {
        return iCorFundo;
    }

    void SetCorOpcao(int corPosicao)
    {
        iCorLetra = corPosicao;
    }

    int GetCorOpcao()
    {
        return iCorLetra;
    }

    void SetDistancia(int distancia)
    {
        iDistancia = distancia;
    }

    int GetDistancia()
    {
        return iDistancia;
    }

    void SetSentido(bool bhorizontal)
    {
        bHorizontal = bhorizontal;
    }

    bool GetSentido()
    {
        return bHorizontal;
    }

    void SomaOpcao(int posicaoSoma)
    {
        if (iPosicaoAtual + posicaoSoma >= 0 && iPosicaoAtual + posicaoSoma < iTotalPosicao)
            iPosicaoAtual += posicaoSoma;
        else if (iPosicaoAtual + posicaoSoma < 0)
            iPosicaoAtual = iTotalPosicao - 1;
        else if (iPosicaoAtual + posicaoSoma >= iTotalPosicao)
            iPosicaoAtual = 0;
    }

    void SetOpcoes(char* cNovasOpcoes)
    {
        cOpcoes = cNovasOpcoes;
    }

    char* GetOpcoes()
    {
        return cOpcoes;
    }

    void SetSurfacePai(SDL_Surface* surface)
    {
        surfacePai = surface;
    }

    SDL_Surface* GetSurfacePai()
    {
        return surfacePai;
    }

    void SetRectModelo(SDL_Rect rect)
    {
        rectModelo = rect;
    }


    SDL_Rect& GetRectModelo()
    {
        return rectModelo;
    }

    void SetRectPosicao(SDL_Rect rect)
    {
        posicaoTela = rect;
    }


    SDL_Rect& GetRectPosicao()
    {
        return posicaoTela;
    }

    //FUNCAO ATUALIZA RECEBE UM ARRAY DE INTEIRO COM A ENTRADA DE USUARIO
    int Atualiza(int* teclas)
    {
        //SE APERTOU PRA CIMA OU ESQUERDA. SOBE UMA OPCAO
        if (teclas[TECLA_SETA_CIMA] || teclas[TECLA_SETA_ESQUERDA])
        {
            SomaOpcao(-1);
            teclas[TECLA_SETA_CIMA] ? teclas[TECLA_SETA_CIMA] = 0 : teclas[TECLA_SETA_CIMA] = teclas[TECLA_SETA_CIMA];
            teclas[TECLA_SETA_ESQUERDA] ? teclas[TECLA_SETA_ESQUERDA] = 0 : teclas[TECLA_SETA_ESQUERDA] = teclas[TECLA_SETA_ESQUERDA];
        }

        //SE APERTOU PRA BAIXO OU DIREITA, DESCE UMA OPCAO
        if (teclas[TECLA_SETA_BAIXO] || teclas[TECLA_SETA_DIREITA])
        {
            SomaOpcao(1);

            teclas[TECLA_SETA_BAIXO] ? teclas[TECLA_SETA_BAIXO] = 0 : teclas[TECLA_SETA_BAIXO] = teclas[TECLA_SETA_BAIXO];
            teclas[TECLA_SETA_DIREITA] ? teclas[TECLA_SETA_DIREITA] = 0 : teclas[TECLA_SETA_DIREITA] = teclas[TECLA_SETA_DIREITA];
        }

        //SE APERTOU ENTER, RETORNA A OPCAO SELECIONADA
        if (teclas[TECLA_ENTER])
        {
            teclas[TECLA_ENTER] = 0;
            return iPosicaoAtual;
        }

        //SENAO APERTOU ENTER, RETORNA -1
        return -1;
    }

    //FUNCAO QUE DESENHA O MENU NA TELA
    void Desenha()
    {
        char opcao[150];
        int j, indice;

        //PERCORRE TODAS AS OPCOES
        for (register int i = j = 0; i < iTotalPosicao; i++)
        {
            //SE É NA VERTICAL
            if (!bHorizontal)
            {
                //SOMA O Y COM A POSICAO DA TELA MAIS A ALTURA VEZES O INDICE DA POSICAO ATUAL MAIS A DISTANCIA
                rectModelo.y = posicaoTela.y + (rectModelo.h * i) + iDistancia;
                rectModelo.x = posicaoTela.x;
            }
            //SE É NA HORIZONTAL
            else
            {
                //SOMA O X COM A POSICAO DA TELA MAIS A LARGURA VEZES O INDICE DA POSICAO ATUAL MAIS A DISTANCIA
                rectModelo.y = posicaoTela.y;
                rectModelo.x = posicaoTela.x + iDistancia + (rectModelo.w * i);
            }

            //PERCORRE AS OPCOES ATÉ O '\N' E PEGA O TEXTO DA POSICAO ATUAL
            for (indice = 0; cOpcoes[j] != '\n' && cOpcoes[j] != '\0'; indice++, j++)
                opcao[indice] = cOpcoes[j];

            //SETA O RESTANTO DO ARRAY COM \0
            for (register int aux = indice; aux < 150; aux++)
                opcao[aux] = '\0';

            //INCREMENTA O J PARA Q O '\N' DA OPCAO TAMBEM ENTRE NA CONTAGEM
            j++;

            //COPIA O RECT DE MODELO
            rectDesenho = rectModelo;

            //SE A POSICAO ATUAL É A OPCAO SELECIONADA
            if (i == iPosicaoAtual)
            {
                //DESENHA O TEXTO E O FUNDO COM AS CORES SELECIONADAS
                if (iCorFundoSelecionado != 0)
                    SDL_FillRect(surfacePai, &rectDesenho, iCorFundoSelecionado);

                stringColor(surfacePai, rectModelo.x + 10, rectModelo.y + rectModelo.h / 2, opcao, iCorSelecionada);
            }
            //SE É UMA OPCAO COMUM
            else
            {
                //DESENHA O TEXTO E O FUNDO COM A COR NORMAL
                if (iCorFundo != 0)
                    SDL_FillRect(surfacePai, &rectDesenho, iCorFundo);

                stringColor(surfacePai, rectModelo.x + 10, rectModelo.y + rectModelo.h / 2, opcao, iCorLetra);
            }
        }
    }
}Menu;

//ESTRUTRA QUE REPRESENTA UM TEXTO BOX PARA ENTRADA DE TEXTO DE USUARIO
typedef struct TextBox
{
    int* teclas;
    int iTotalPosicao, iPosicaoAtual, iDistancia, iCorLetra, iCorFundo, iCapacidade;
    char* texto;
    char* desenha;
    SDL_Rect posicaoTela, rectModelo;
    SDL_Surface* surfacePai;
    Timer timerPonteiro;

    TextBox(SDL_Surface* surface, int capacidade, int corFundo, int corLetra, SDL_Rect modelo, SDL_Rect posicao)
    {
        iCapacidade = capacidade;
        iTotalPosicao = 0;
        iPosicaoAtual = 0;
        iCorLetra = corLetra;
        iCorFundo = corFundo;
        posicaoTela = posicao;
        rectModelo = modelo;
        surfacePai = surface;
        iDistancia = 10;
        texto = new char[capacidade];
        iTotalPosicao = capacidade;

        //SETA O CHARARRAY INTEIRO COMO '\0'
        for (register int i = 0; i < iTotalPosicao; i++)
            texto[i] = '\0';
    }

    //-------------------- GET E SET --------------------------//

    void SetCorFundo(int corFundo)
    {
        iCorFundo = corFundo;
    }

    int GetCorFundo()
    {
        return iCorFundo;
    }

    void SetCorLetra(int corPosicao)
    {
        iCorLetra = corPosicao;
    }

    int GetCorLetra()
    {
        return iCorLetra;
    }

    void SetDistancia(int distancia)
    {
        iDistancia = distancia;
    }

    int GetDistancia()
    {
        return iDistancia;
    }

    void SetSurfacePai(SDL_Surface* surface)
    {
        surfacePai = surface;
    }

    SDL_Surface* GetSurfacePai()
    {
        return surfacePai;
    }

    void SetRectPosicao(SDL_Rect rect)
    {
        posicaoTela = rect;
    }

    SDL_Rect& GetRectPosicao()
    {
        return posicaoTela;
    }

        void SetRectModelo(SDL_Rect rect)
    {
        rectModelo = rect;
    }


    SDL_Rect& GetRectModelo()
    {
        return rectModelo;
    }

    //FUNCAO QUE ATUALIZA O TEXT BOX COM A ENTRADA DO USUARIO
    char* Atualiza(int* teclas)
    {
        //SE APERTOU BACKSPACE, RETIRA UMA LETA E ATUALIZA VARIAVEL DE CONTROLE
        if (teclas[TECLA_BACKSPACE] == 1)
        {
            teclas[TECLA_BACKSPACE] = 0;

            if (iPosicaoAtual - 1 >= 0)
                texto[--iPosicaoAtual] = '\0';
        }

        //SE AINDA NÃO COMPLETOU O TEXTBOX
        if (iPosicaoAtual + 1 <= iTotalPosicao)
        {
            //------------------ TRATA TODO O TECLADO ALFA NUMERICO --------------------------- //
            if (teclas[TECLA_A] == 1)
            {
                teclas[TECLA_A] = 0;
                texto[iPosicaoAtual++] = 'A';
            }
            else if (teclas[TECLA_T] == 1)
            {
                teclas[TECLA_T] = 0;
                texto[iPosicaoAtual++] = 'T';
            }
            else if (teclas[TECLA_R] == 1)
            {
                teclas[TECLA_R] = 0;
                texto[iPosicaoAtual++] = 'R';
            }
            else if (teclas[TECLA_S] == 1)
            {
                teclas[TECLA_S] = 0;
                texto[iPosicaoAtual++] = 'S';
            }
            else if (teclas[TECLA_D] == 1)
            {
                teclas[TECLA_D] = 0;
                texto[iPosicaoAtual++] = 'D';
            }
            else if (teclas[TECLA_W] == 1)
            {
                teclas[TECLA_W] = 0;
                texto[iPosicaoAtual++] = 'W';
            }
            else if (teclas[TECLA_Q] == 1)
            {
                teclas[TECLA_Q] = 0;
                texto[iPosicaoAtual++] = 'Q';
            }
            else if (teclas[TECLA_E] == 1)
            {
                teclas[TECLA_E] = 0;
                texto[iPosicaoAtual++] = 'E';
            }
            else if (teclas[TECLA_Y] == 1)
            {
                teclas[TECLA_Y] = 0;
                texto[iPosicaoAtual++] = 'Y';
            }
            else if (teclas[TECLA_U] == 1)
            {
                teclas[TECLA_U] = 0;
                texto[iPosicaoAtual++] = 'U';
            }
            else if (teclas[TECLA_I] == 1)
            {
                teclas[TECLA_I] = 0;
                texto[iPosicaoAtual++] = 'I';
            }
            else if (teclas[TECLA_O] == 1)
            {
                teclas[TECLA_O] = 0;
                texto[iPosicaoAtual++] = 'O';
            }
            else if (teclas[TECLA_P] == 1)
            {
                teclas[TECLA_P] = 0;
                texto[iPosicaoAtual++] = 'P';
            }
            else if (teclas[TECLA_F] == 1)
            {
                teclas[TECLA_F] = 0;
                texto[iPosicaoAtual++] = 'F';
            }
            else if (teclas[TECLA_G] == 1)
            {
                teclas[TECLA_G] = 0;
                texto[iPosicaoAtual++] = 'G';
            }
            else if (teclas[TECLA_H] == 1)
            {
                teclas[TECLA_H] = 0;
                texto[iPosicaoAtual++] = 'H';
            }
            else if (teclas[TECLA_J] == 1)
            {
                teclas[TECLA_J] = 0;
                texto[iPosicaoAtual++] = 'J';
            }
            else if (teclas[TECLA_K] == 1)
            {
                teclas[TECLA_K] = 0;
                texto[iPosicaoAtual++] = 'K';
            }
            else if (teclas[TECLA_L] == 1)
            {
                teclas[TECLA_L] = 0;
                texto[iPosicaoAtual++] = 'L';
            }
            else if (teclas[TECLA_Z] == 1)
            {
                teclas[TECLA_Z] = 0;
                texto[iPosicaoAtual++] = 'Z';
            }
            else if (teclas[TECLA_X] == 1)
            {
                teclas[TECLA_X] = 0;
                texto[iPosicaoAtual++] = 'X';
            }
            else if (teclas[TECLA_C] == 1)
            {
                teclas[TECLA_C] = 0;
                texto[iPosicaoAtual++] = 'C';
            }
            else if (teclas[TECLA_V] == 1)
            {
                teclas[TECLA_V] = 0;
                texto[iPosicaoAtual++] = 'V';
            }
            else if (teclas[TECLA_B] == 1)
            {
                teclas[TECLA_B] = 0;
                texto[iPosicaoAtual++] = 'B';
            }
            else if (teclas[TECLA_N] == 1)
            {
                teclas[TECLA_N] = 0;
                texto[iPosicaoAtual++] = 'N';
            }
            else if (teclas[TECLA_M] == 1)
            {
                teclas[TECLA_M] = 0;
                texto[iPosicaoAtual++] = 'M';
            }
            else if (teclas[TECLA_1] == 1)
            {
                teclas[TECLA_1] = 0;
                texto[iPosicaoAtual++] = '1';
            }
            else if (teclas[TECLA_2] == 1)
            {
                teclas[TECLA_2] = 0;
                texto[iPosicaoAtual++] = '2';
            }
            else if (teclas[TECLA_3] == 1)
            {
                teclas[TECLA_3] = 0;
                texto[iPosicaoAtual++] = '3';
            }
            else if (teclas[TECLA_4] == 1)
            {
                teclas[TECLA_4] = 0;
                texto[iPosicaoAtual++] = '4';
            }
            else if (teclas[TECLA_5] == 1)
            {
                teclas[TECLA_5] = 0;
                texto[iPosicaoAtual++] = '5';
            }
            else if (teclas[TECLA_6] == 1)
            {
                teclas[TECLA_6] = 0;
                texto[iPosicaoAtual++] = '6';
            }
            else if (teclas[TECLA_7] == 1)
            {
                teclas[TECLA_7] = 0;
                texto[iPosicaoAtual++] = '7';
            }
            else if (teclas[TECLA_8] == 1)
            {
                teclas[TECLA_8] = 0;
                texto[iPosicaoAtual++] = '8';
            }
            else if (teclas[TECLA_9] == 1)
            {
                teclas[TECLA_9] = 0;
                texto[iPosicaoAtual++] = '9';
            }
            else if (teclas[TECLA_0] == 1)
            {
                teclas[TECLA_0] = 0;
                texto[iPosicaoAtual++] = '0';
            }
        }

        //SE APERTOU ENTER E JÁ COMPLETOU O TEXTBOX, RETORNA O TEXTO
        if (teclas[TECLA_ENTER])
        {
            teclas[TECLA_ENTER] = 0;

            if (iPosicaoAtual == iTotalPosicao)
                return texto;
        }

        //SENAO APERTOU ENTER OU N COMPLETOU O TEXTBOX, RETORNA NULO
        return NULL;
    }

    //FUNCAO QUE DESENHA O TEXTBOX NA TELA
    void Desenha()
    {
        //PERCORRE TODO O TEXTBOX
        for (register int i = 0; i < iTotalPosicao; i++)
        {
            //ATUALIZA O X DE CADA OPCAO COM O W DO MODELO MAIS A DISTANCIA VEZES O INDICE ATUAL MAIS A POSICAO NA TELA
            rectModelo.y = posicaoTela.y;
            rectModelo.x = posicaoTela.x + ((rectModelo.w + iDistancia) * i);

            //DESENHA O FUNDO NA TELA
            if (iCorFundo != 0)
                SDL_FillRect(surfacePai, &rectModelo, iCorFundo);

            //SE A LETRA ATUAL É DIFERENTE DE '\0'
            if (texto[i] != '\0')
            {
                //DESENHA TEXTO NA TELA
                desenha = new char[1]{texto[i]};
                desenha[1] = '\0';

                stringColor(surfacePai, rectModelo.x + rectModelo.w / 3, rectModelo.y + rectModelo.h / 2, desenha, iCorLetra);
                delete desenha;
            }
        }
    }
}TextBox;

//FUNCAO QUE REPRESENTA A LISTA DE NAVES, É BASICAMENTE UM MULTIPLO MENU COM MULTIPLAS IMAGENS
typedef struct ListaNaves
{
    int iPosicaoAtual;
    int iTotalPosicao;
    SDL_Rect rectModelo;
    SDL_Rect rectDesenho;
    SDL_Rect posicaoTela;
    SDL_Rect rectFundo;
    int iCorFundo;
    int iCorLetra;
    SDL_Surface* surfacePai;
    SDL_Surface** surfaceImagens;
    Menu** descricoes;

    ListaNaves(SDL_Surface* surface, SDL_Surface** imagens, int iQuantOpc, char** cOpcoesSeparadasPorQuebraLinha,
        int corFundo, int corLetra, SDL_Rect modeloImagem, SDL_Rect modeloTexto, SDL_Rect posicao)
    {
        iTotalPosicao = 0;
        iPosicaoAtual = 0;
        iCorLetra = corLetra;
        iCorFundo = corFundo;
        rectModelo = modeloImagem;
        posicaoTela = posicao;
        surfacePai = surface;
        iTotalPosicao = iQuantOpc;
        posicao.y += imagens[0]->h;

        surfaceImagens = new SDL_Surface*[iTotalPosicao];
        descricoes = new Menu*[iTotalPosicao];

        //PRA CADA OPCAO, CRIA UM NOVO MENU PASSANDO O TEXTO DA DESCRICAO DA NAVE E AS PROPRIEDADES QUE VIERAM NO CONTRUTOR, TAMBÉM PEGA AS IMAGENS E PREENCHE O ARRAY DE REFERENCIA
        for (register int i = 0; i < iTotalPosicao; i++)
        {
            surfaceImagens[i] = imagens[i];
            descricoes[i] = new Menu(surface, cOpcoesSeparadasPorQuebraLinha[i], iCorFundo, iCorFundo, iCorLetra, iCorLetra, false, modeloTexto, posicao);
        }

        posicao.y -= imagens[0]->h;
    }

    //-------------------- GET E SET --------------------------//

    void SetOpcaoSelecionada(int posicao)
    {
        if (posicao > 0 && posicao < iTotalPosicao)
            iPosicaoAtual = posicao;
    }

    int GetOpcaoSelecionada()
    {
        return iPosicaoAtual;
    }

    void SetCorFundo(int corFundo)
    {
        iCorFundo = corFundo;
    }

    int GetCorFundo()
    {
        return iCorFundo;
    }

    void SetCorOpcao(int corPosicao)
    {
        iCorLetra = corPosicao;
    }

    int GetCorOpcao()
    {
        return iCorLetra;
    }

    void SomaOpcao(int posicaoSoma)
    {
        if (iPosicaoAtual + posicaoSoma >= 0 && iPosicaoAtual + posicaoSoma < iTotalPosicao - 1)
            iPosicaoAtual += posicaoSoma;
        else if (iPosicaoAtual + posicaoSoma < 0)
            iPosicaoAtual = iTotalPosicao - 2;
        else if (iPosicaoAtual + posicaoSoma >= iTotalPosicao - 1)
            iPosicaoAtual = 0;
    }

    void SetSurfacePai(SDL_Surface* surface)
    {
        surfacePai = surface;
    }

    SDL_Surface* GetSurfacePai()
    {
        return surfacePai;
    }

    void SetRectModelo(SDL_Rect rect)
    {
        rectModelo = rect;
    }


    SDL_Rect GetRectModelo()
    {
        return rectModelo;
    }

    void SetRectPosicao(SDL_Rect rect)
    {
        posicaoTela = rect;
    }


    SDL_Rect& GetRectPosicao()
    {
        return posicaoTela;
    }

    SDL_Surface** GetImagens(int* iQuantImagens)
    {
        *iQuantImagens = iTotalPosicao;
        return surfaceImagens;
    }

    void SetImagens(SDL_Surface** imagens, int iQuantImagens)
    {
        iTotalPosicao = iQuantImagens;
        surfaceImagens = imagens;
    }

    //FUNCAO QUE ATUALIZA A LISTA COM A ENTRADA DO USUARIO
    int Atualiza(int* teclas)
    {
        //SE APERTOU PARA CIMA OU PARA ESQUERDA, DECREMENTA UMA POSICAO
        if (teclas[TECLA_SETA_CIMA] || teclas[TECLA_SETA_ESQUERDA])
        {
            SomaOpcao(-1);
            teclas[TECLA_SETA_CIMA] ? teclas[TECLA_SETA_CIMA] = 0 : teclas[TECLA_SETA_CIMA] = teclas[TECLA_SETA_CIMA];
            teclas[TECLA_SETA_ESQUERDA] ? teclas[TECLA_SETA_ESQUERDA] = 0 : teclas[TECLA_SETA_ESQUERDA] = teclas[TECLA_SETA_ESQUERDA];
        }

        //SE APERTOU PARA BAIXO OU DIREITA, INCREMENTA UMA POSICAO
        if (teclas[TECLA_SETA_BAIXO] || teclas[TECLA_SETA_DIREITA])
        {
            SomaOpcao(1);

            teclas[TECLA_SETA_BAIXO] ? teclas[TECLA_SETA_BAIXO] = 0 : teclas[TECLA_SETA_BAIXO] = teclas[TECLA_SETA_BAIXO];
            teclas[TECLA_SETA_DIREITA] ? teclas[TECLA_SETA_DIREITA] = 0 : teclas[TECLA_SETA_DIREITA] = teclas[TECLA_SETA_DIREITA];
        }

        //SE APERTOU ENTER, RETORNA A POSICAO ATUAL
        if (teclas[TECLA_ENTER])
        {
            teclas[TECLA_ENTER] = 0;
            return iPosicaoAtual;
        }

        //SE NAO APERTOU ENTER, RETORNA -1
        return -1;
    }

    //FUNCAO QUE DESENHA A POSICAO ATUAL DA LISTA NA TELA
    void Desenha()
    {
        //PERCORRETODAS AS POSICOES
        for (register int i = 0; i < iTotalPosicao - 1; i++)
        {
            //SE A POSICAO ATUAL É A SELECIONADA, DESENHA
            if (iPosicaoAtual == i)
            {
                //ATUALIZA A POSICAO DA IMAGEM NO CENTRO DA LISTA
                posicaoTela.x = (descricoes[i]->GetRectPosicao().x + descricoes[i]->GetRectModelo().w / 2) - surfaceImagens[i]->w / 2;

                //DESENHA A IMAGEM NA TELA
                SDL_BlitSurface(surfaceImagens[i], &rectModelo, surfacePai, &posicaoTela);

                //DESENHA A DESCRICAO NA TELA (MENU COM AS OPCOES QUE, NA VDD, SAO A DESCRICAO DA NAVE)
                descricoes[i]->Desenha();
            }
        }
    }
}ListaNaves;

#endif
