#include "Configuracoes.h"

Configuracoes::Configuracoes() : Fase(NIVEL_CONFIGURACOES)
{
    IniciarNivel();
}

void Configuracoes::IniciarNivel()
{
    menuPrincipal = NULL;

    //VARIAIVEIS DE CONTROLE DE POSICAO E MODELO DPARA CADA OPC DO MENU
    SDL_Rect modelo, posicao;

    modelo.h = 30;
    modelo.w = 100;

    posicao.x = 100;
    posicao.y = Game::GetAlturaJanela() / 2;

    //CRIA MENU COM AS OPCOES DE CONFIGURAÇÃO
    menuPrincipal = new Menu(Game::GetSurfacePrincipal(), "RESOLUCAO\nFULL SCREEN\nVOLTAR\n", PRETO, PRETO, BRANCO32, VERMELHO32, false, modelo, posicao);

    //seta valores padroes
    menuFullScreen = NULL;
    menuResolusoes = NULL;
    bMostraFullScreen = bMostraResolusoes = false;
}

void Configuracoes::Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame)
{
    if (bMostraFullScreen && menuFullScreen != NULL)
    {
        TrataFullScreen();
    }
    else if (bMostraResolusoes && menuResolusoes != NULL)
    {
        TrataResolusoes();
    }
    else
    {
        TrataMenu();
    }
}

void Configuracoes::Desenha()
{
    menuPrincipal->Desenha();

    if (bMostraFullScreen && menuFullScreen != NULL)
    {
        menuFullScreen->Desenha();
    }
    else if (bMostraResolusoes && menuResolusoes != NULL)
    {
        menuResolusoes->Desenha();
    }
}

//FUNCAO QUE CRIA O MENU DE RESOLUCOES
void Configuracoes::CriaMenuResolusoes()
{
    //rects de controle de modelo e posicao do menu
    SDL_Rect modelo, posicao;

    modelo.x = 0;
    modelo.y = 0;

    modelo.w = 100;
    modelo.h = 30;

    posicao.x = menuPrincipal->GetRectPosicao().x + 100;
    posicao.y = menuPrincipal->GetRectPosicao().y + 10;

    //CRIA MENU DE RESOLUCOES
    menuResolusoes = new Menu(Game::GetSurfacePrincipal(), "1920 x 1080\n1600 x 900\n1280 x 800\n1440 x 900\n1024 X 768\n800 X 600\n", PRETO, PRETO, BRANCO32, VERMELHO32, true, modelo, posicao);
}

//FUNCAO QUE CRIA O MENU DE FULL SCREEN
void Configuracoes::CriaMenuFullScreen()
{
    //rects de controle de modelo e posicao do menu
    SDL_Rect modelo, posicao;

    modelo.x = 0;
    modelo.y = 0;

    modelo.w = 50;
    modelo.h = 30;

    posicao.x = menuPrincipal->GetRectPosicao().x + 100;
    posicao.y = menuPrincipal->GetRectPosicao().y + 40;

    //CRIA MENU DE RESOLUCOES
    menuFullScreen = new Menu(Game::GetSurfacePrincipal(), "SIM\nNAO\n", PRETO, PRETO, BRANCO32, VERMELHO32, true, modelo, posicao);
}

void Configuracoes::TrataResolusoes()
{
    int iOpc;

    //ATUALIZA O MENU COM A ENTRADA DE USUARIO E PEGA RETORNO
    iOpc = menuResolusoes->Atualiza(Game::GetTeclas());

    //SE APERTOU NA PRIMEIRA OPC
    if (iOpc == 0)
    {
        Game::SetResolucao(1080, 1920);
        Game::GetTeclas()[TECLA_ESC] = 1;
    }
    //SE APERTOU NA SEGUNDA OPC
    else if (iOpc == 1)
    {
        Game::SetResolucao(900, 1600);
        Game::GetTeclas()[TECLA_ESC] = 1;
    }
    //SE APERTOU NA TERCEIRA OPC
    else if (iOpc == 2)
    {
        Game::SetResolucao(800, 1280);
        Game::GetTeclas()[TECLA_ESC] = 1;
    }
    //SE APERTOU NA QUARTA OPC
    else if (iOpc == 3)
    {
        Game::SetResolucao(900, 1440);
        Game::GetTeclas()[TECLA_ESC] = 1;
    }
    //SE APERTOU NA QUINTA OPC
    else if (iOpc == 4)
    {
        Game::SetResolucao(768, 1024);
        Game::GetTeclas()[TECLA_ESC] = 1;
    }
    //SE APERTOU NA SEXTA OPC
    else if (iOpc == 5)
    {
        Game::SetResolucao(600, 800);
        Game::GetTeclas()[TECLA_ESC] = 1;
    }

    //se apertou esc, reseta variaveis de controle
    if (Game::GetTeclas()[TECLA_ESC])
    {
        bMostraResolusoes = false;
        delete menuResolusoes;
        Game::GetTeclas()[TECLA_ESC] = 0;
        menuResolusoes = NULL;
    }
}

void Configuracoes::TrataFullScreen()
{
    int iOpc;

    //ATUALIZA O MENU COM A ENTRADA DE USUARIO E PEGA RETORNO
    iOpc = menuFullScreen->Atualiza(Game::GetTeclas());

    //SE APERTOU NA PRIMEIRA OPC
    if (iOpc == 0)
    {
        Game::SetFullScreen(true);
    }
    //SE APERTOU NA SEGUNDA OPC
    else if (iOpc == 1)
    {
        Game::SetFullScreen(false);
    }

    //se apertou esc, reseta variaveis de controle
    if (Game::GetTeclas()[TECLA_ESC])
    {
        bMostraFullScreen = false;
        Game::GetTeclas()[TECLA_ESC] = 0;
        delete menuFullScreen;
        menuFullScreen = NULL;
    }
}

void Configuracoes::TrataMenu()
{
    int iOpc;

    //ATUALIZA O MENU COM A ENTRADA DE USUARIO E PEGA RETORNO
    iOpc = menuPrincipal->Atualiza(Game::GetTeclas());

    //SE APERTOU NA PRIMEIRA OPC
    if (iOpc == 0)
    {
        //SE ESTA MOSTRANDO, SOME, SENAO MOSTRA
        bMostraResolusoes = !bMostraResolusoes;

        //SE NAO TIVER CRIADO O MENU AINDA, CRIA
        if (bMostraResolusoes && menuResolusoes == NULL)
        {
            CriaMenuResolusoes();
        }
    }
    //SE APERTOU NA SEGUNDA OPC
    else if (iOpc == 1)
    {
        //SE ESTA MOSTRANDO, SOME, SENAO MOSTRA
        bMostraFullScreen = !bMostraFullScreen;

        //SE NAO TIVER CRIADO O MENU AINDA, CRIA
        if (bMostraFullScreen && menuFullScreen == NULL)
        {
            CriaMenuFullScreen();
        }
    }
    //SE APERTOU NA TERCEIRA OPC
    else if (iOpc == 2)
    {
        Game::SetProximoNivel(NIVEL_MENU_PRINCIPAL);
        return;
    }
}

Configuracoes::~Configuracoes()
{
    if (menuPrincipal != NULL)
        delete menuPrincipal;
}
