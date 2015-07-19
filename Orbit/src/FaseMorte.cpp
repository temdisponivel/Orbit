#include "FaseMorte.h"

FaseMorte::FaseMorte() : Fase(NIVEL_MORTE)
{
    IniciarNivel();
}

//FUNCAO PARA INICIALIZACAO DO NIVEL
void FaseMorte::IniciarNivel()
{
    //SETA PROPRIEDADES INICIAIS
    bSalvou = false;
    iIntervaloMensagem = 2000;
    bMostraIniciais = false;
    textSave = NULL;
    resultadoSave = NULL;

    //CRIA RECT DE CONTROLE DO MENU
    SDL_Rect modelo, posicao;

    //SETA PROPRIEDADES DO MODELO DE CADA OPCAO DO MENU
    modelo.h = 50;
    modelo.w = 150;

    //SETA PROPRIEDADE DA POSICAO DO MENU NA TELA
    posicao.x = Game::GetLarguraJanela() - 200;
    posicao.y = Game::GetAlturaJanela() / 2;

    //CRIA O MENU E GUARD REFERENCIA PRA MANIPULACAO POSTERIOR
    menu = new Menu(Game::GetSurfacePrincipal(), "JOGAR NOVAMENTE\nSALVAR\nMENU PRINCIPAL\nSAIR\n", PRETO, PRETO, BRANCO32, VERMELHO32, false, modelo, posicao);

    //soma pontuacaos segundo tiros e inimigos mortos
    Game::GetSave().SomaPontuacao(Game::GetTirosDisparados() * Game::GetInimigosMortos());
}

//FUNCAO CHAMADA A TODO GAMELOOP
void FaseMorte::Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame)
{
    //SE O TEXTSAVE FOR DIFERENTE DE NULO, ENTAO TRATA O SAVE
    if (textSave != NULL && bMostraIniciais)
        TrataSave();
    //SENAO, TRATA MENU
    else
        TrataMenu();
}

//FUNCAO CHAMADA A TODO GAMELOOP
void FaseMorte::Desenha()
{
    //SE É PRA MOSTRAR INICIAR E O TEXTSAVE É DIFERENTE DE NULO, DESENHA O TEXTBOX NA TELA
    if (textSave != NULL && bMostraIniciais)
        textSave->Desenha();

    //DESENHA O MENU NA TELA
    menu->Desenha();

    //DESENHA MENSAGENS COM A PONTUACAO E RESULTADO DA PARTIDA
    char* texto = new char[50];

    //MOSTRA RESULTADO
    if (Game::GetResultadoPartida())
        stringColor(Game::GetSurfacePrincipal(), (Game::GetLarguraJanela() / 2)  - 50, Game::GetAlturaJanela() / 3, "GANHOU", VERMELHO32);
    else
        stringColor(Game::GetSurfacePrincipal(), (Game::GetLarguraJanela() / 2)  - 50, Game::GetAlturaJanela() / 3, "PERDEU", VERMELHO32);

    //MOSTRA QUANTIDADE DE TIROS DISPARADOS E INIMIGOS MORTOS
    sprintf(texto, "TIROS DISPARADOS %i", Game::GetTirosDisparados());
    stringColor(Game::GetSurfacePrincipal(), (Game::GetLarguraJanela() / 2)  - 50, (Game::GetAlturaJanela() / 3) + 50, texto, VERMELHO32);
    sprintf(texto, "INIMIGOS MORTOS %i",Game::GetInimigosMortos());
    stringColor(Game::GetSurfacePrincipal(), (Game::GetLarguraJanela() / 2)  - 50, (Game::GetAlturaJanela() / 3) + 100, texto, VERMELHO32);

    //MOSTRA PONTUACAO
    sprintf(texto, "PONTUACAO:%i", Game::GetSave().GetPontuacao());
    stringColor(Game::GetSurfacePrincipal(), (Game::GetLarguraJanela() / 2)  - 50, (Game::GetAlturaJanela() / 3) + 150, texto, VERMELHO32);

    //SE USUARIO SALVOU UM JOGO
    if (bSalvou)
    {
        //SE AINDA NAO PASSOU O TEMPO DA MENSAGEM NA TELA, DESENHA MENSAGEM DE SALVO COM SUCESSO
        if (timerMensagem.get_ticks() < iIntervaloMensagem)
            stringColor(Game::GetSurfacePrincipal(), (Game::GetLarguraJanela() / 2)  - 50, Game::GetAlturaJanela() / 3 + 50, "SALVO COM SUCESSO", VERMELHO32);
    }

    delete texto;
}

//FUNCAO QUE CRIA A TEXTBOX PARA ENTRADA DO NOME DO SAVE
void FaseMorte::CriaTextBox()
{
    SDL_Rect modelo, posicao;

    //SETA PROPRIEDADES DE TAMANHO E POSICAO NA TELA
    posicao.x = Game::GetLarguraJanela() / 2;
    posicao.y = Game::GetAlturaJanela() / 2;
    modelo.h = 20;
    modelo.w = 20;

    textSave = new TextBox(Game::GetSurfacePrincipal(), 3, BRANCO, VERMELHO32, modelo, posicao);
}

//FUNCAO QUE TRATA ENTRADA DO USUARIO SE ESTIVER NO MENU
void FaseMorte::TrataMenu()
{
    int iOpc;

    //ATUALIZA O MENU COM AS ENTRADAS DE USUARIO E PEGA RETORNO
    iOpc = menu->Atualiza(Game::GetTeclas());

    //SE APERTOU NO ZERO, RESETA PROPRIEDADES DA PARTIDA E JOGA NOVAMENTO
    if (iOpc == 0)
    {
        Game::LimpaDadosPartida();
        Game::ContinuaJogo();
    }
    //SE APERTOU NA OPCAO 1
    else if (iOpc == 1)
    {
        //SE ESTIVER MOSTRANDO TIRA E SENAO, COLOCA NA TELA AS INICIAIS
        bMostraIniciais = !bMostraIniciais;

        //SE FOR PRA MOSTRAR AS INICIAIS
        if (bMostraIniciais)
            if (textSave == NULL) //SE O TEXTBOX FOR NULO, CRIA UM NOVO
                CriaTextBox();
    }
    //SE SELECIONOU A OPCAO 2, LIMPA OS DADOS DA PARTIDA E VOLTA PARA O MENU PRINCIPAL
    else if (iOpc == 2)
    {
        Game::LimpaDadosPartida();
        Game::SetProximoNivel(NIVEL_MENU_PRINCIPAL);
    }
    //SE SELECIONOU 3, FECHA O JOGO
    else if (iOpc == 3)
    {
        Game::FecharJogo();
    }
}

//FUNCAO QUE TRATA ENTRADA DE USUARIO QUANDO ESTA NA TELA DE INICIAIS PARA SALVAR
void FaseMorte::TrataSave()
{
    //TRATA ENTRADA DE USUARIO, PEGA O RETORNO E=, SE FOR DIFIFERENTE DE NULO, SALVA O JOGO COM O RETORNO (QUE, SE FOR DIFERENTE DE NULO, É AS INICIAIS DO JOGADOR)
    if ((resultadoSave = textSave->Atualiza(Game::GetTeclas())) != NULL)
    {
        //SETA INICIAIS E SALVA O JOGO
        Game::GetSave().SetInicias(resultadoSave);
        Game::GetSave().Salvar();

        //INCIA TIMER E SETA PROPRIEDADE PRA MOSTRAR MENSAGEM DE SALVO
        timerMensagem.start();
        bSalvou = true;
        Game::GetTeclas()[TECLA_ESC] = 1;
    }

    //SE USUARIO APERTOU ESC, FECHA O TEXTBOX E VOLTA PRO MENU
    if (Game::GetTeclas()[TECLA_ESC])
    {
        //DELETA REFERENCIA, SETA COMO NULO E RESETA PROPRIEDADES DE CONTROLE
        delete textSave;
        textSave = NULL;
        bMostraIniciais = false;
        Game::GetTeclas()[TECLA_ESC] = 0;
        return;
    }
}

FaseMorte::~FaseMorte()
{
    if (resultadoSave != NULL)
        delete resultadoSave;

    if (textSave != NULL)
        delete textSave;
}
