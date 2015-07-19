#include "FaseMenu.h"

FaseMenu::FaseMenu() : Fase(NIVEL_MENU_PAUSE)
{
    IniciarNivel();
}

//FUNCAO DESTINADA A INICIALIZACAO DA FASE
void FaseMenu::IniciarNivel()
{
    //PROPRIEDADES PARA CRIA O MENU DA PRIMEIRA TELA
    SDL_Rect modelo, posicao;

    //AJUSTA PROPRIEDADES DE CADA OPCAO DO MENU
    modelo.h = 30;
    modelo.w = 130;

    //AJUSTA POSICAO DO MENU NA TELA
    posicao.x = Game::GetLarguraJanela() /2;
    posicao.y = Game::GetAlturaJanela() / 2;

    //CRIA O MENU E QUANDA A REFERENCIA PARA MANIPULACAO
    menu = new Menu(Game::GetSurfacePrincipal(), "CONTINUAR\nMENU PRINCIPAL\nSAIR\n", PRETO, PRETO, BRANCO32, VERMELHO32, false, modelo, posicao);
}

//FUNCAO CHAMA A TODO GAMELOOP
void FaseMenu::Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame)
{
    int iOpc;

    //ATUALIZA A POSICAO SELECIONADA DO MENU E PEGA RETORNO
    iOpc = menu->Atualiza(Game::GetTeclas());

    //SE CLICOU NA OPCAO ZERO DO MENU OU APERTOU ESC, SAIMOS DO MENU
    if (iOpc == 0 || Game::GetTeclas()[TECLA_ESC])
    {
        menu->SetOpcaoSelecionada(0);
        Game::Menu();
    }

    //SE SELECIONOU A PRIMEIRA OPCAO DO MENU, VAI PRA TELA DO MENU PRINCIPAL
    if (iOpc == 1)
        Game::SetProximoNivel(NIVEL_MENU_PRINCIPAL);
    //SE APERTOU A SEGUNDO OPCAO, FECHA JOGO
    else if (iOpc == 2)
        Game::FecharJogo();
}

//FUNCAO CHAMADA A TODO GAMELOOP
void FaseMenu::Desenha()
{
    menu->Desenha();
}

FaseMenu::~FaseMenu()
{
    //dtor
}
