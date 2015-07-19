#include "MenuPrincipal.h"

MenuPrincipal::MenuPrincipal() : Fase(NIVEL_MENU_PRINCIPAL)
{
    IniciarNivel();
}

void MenuPrincipal::IniciarNivel()
{
    //VARIAVEIS DE CONTROLE DA SELECAO DE NAVES
    iQuantSelecao = 0;
    iTotalSelecao = 4;
    listaNaves = NULL;
    bMostraNaves = false;
    iNavesSelecionadas = new int[iTotalSelecao];
    iTotalNavesJogo = Game::GetQuantidadeNaves();
    navesSelecionadas = new Nave*[iTotalSelecao];

    for (register int i = 0; i < iTotalSelecao; i++)
        navesSelecionadas[i] = NULL;

    //VARIAVEIS DE CONTROLE DO SCORE
    iIntervaloScore = 10;
    listaScore = NULL;
    scores = NULL;
    bMostraScore = false;

    //VARIAVEIS DE CONTROLE DE DIFICULDADE
    menuDificuldade = NULL;
    bMostraDificuldade = false;

    //VARIAVEIS DE CONTROLE DO MENU PRINCIPAL
    SDL_Rect modelo, posicao;

    //SETA MODELO PARA CADA OPÇÃO DO MENU
    modelo.h = 30;
    modelo.w = 130;

    //SETA POSICAO PARA O MENU
    posicao.x = Game::GetLarguraJanela() - 200;
    posicao.y = Game::GetAlturaJanela() / 1.5;

    //CRIA O MENU E GUARDA A REFERENCIA
    menu = new Menu(Game::GetSurfacePrincipal(), "JOGAR\nSCORES\nCONFIGURAÇÕES\nSAIR\n", PRETO, PRETO, BRANCO32, VERMELHO32, false, modelo, posicao);

    //SETA O FUNDO NA TELA
    Fase::SetFundo("imagens/fundo/fundo_menu_principal.png");
}

//FUNCAO CHAMADA A TODO GAMELOOP
void MenuPrincipal::Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame)
{
    Fase::Atualiza(iDeltaTime, iFrame);

    //SE É PRA MOSTRAR O SCORE E A LISTA DE SCORE É DIFERENTE DE NULO, CHAMA FUNCAO QUE TRATA ENTRADA DE USUARIO NO SCORE
    if (bMostraScore && scores != NULL)
    {
        TrataScore();
    }
    //SE É PRA MOSTRAR A LISTA DE DIFICULDADES E O MENU DE DIFICULDADES É DIFERENTE DE NULO, CHAMA FUNCAO QUE TRATA ENTRADA DE USUARIO NO MENU DE DIFICULDADE
    else if (bMostraDificuldade && menuDificuldade != NULL)
    {
        TrataDificuldade();
    }
    //SE É PRA MOSTRAR AS NAVES E A LISTA DE NAVES É DIFERENTE DE NULO, CHAMA FUNCAO QUE TRATA ENTRADA DE USUARIO NA LISTA DE NAVES
    else if (bMostraNaves && listaNaves != NULL)
    {
        TrataListaNaves();
    }
    //SE NAO É PRA MOSTRAR NADA, TRATA ENTRADA DE USUÁRIO PARAO MENU PRINCIPAL
    else
    {
        TrataMenuPrincipal();
    }
}

//FUNCAO CHAMADA A TODO GAMELOOP
void MenuPrincipal::Desenha()
{
    Fase::Desenha();

    //DESENHA O MENU PRINCIPAL NA TELA
    menu->Desenha();

    //SE É PREA MOSTRAR SCORE, DESENHA NA TELA
    if (scores != NULL && bMostraScore)
        scores->Desenha();
    //SE É PRA MOSTRAR DIFICULDADE, DESENHA NA TELA
    else if (menuDificuldade != NULL && bMostraDificuldade)
        menuDificuldade->Desenha();
    //SE É PRA MOSTRAR LISTA DE NAVES, DESENHA NA TELA
    else if (listaNaves != NULL & bMostraNaves)
    {
        listaNaves->Desenha();
        DesenhaHUD();
    }
}

//FUNCAO QUE DESENHA O HUD NA TELA
void MenuPrincipal::DesenhaHUD()
{
    Nave* nave;

    //RECTS DE CONTROLE DE MODELO E POSICAO DA LISTA DE NAVES NA TELA
    SDL_Rect modelo{0, 0, 0, 0}, posicao{30, Game::GetAlturaJanela() - 100, 0, 0};

    //PERCORRE TODAS AS NAVES SELECIONADAS
    for (register int j = 0; j < iQuantSelecao; j++)
    {
        //SE FOR O ESCUDO SEMPRE DEIXA A ESQUERDA DA LISTA
        if (navesSelecionadas[j]->GetJogador() == JOGADOR_ESCUDO)
            posicao.x = 30;
        //SE NAO FOR O ESCUDO AJUSTA A POSICAO À DIREITA DA NAVE ANERIOR
        else
            posicao.x += navesSelecionadas[j]->GetImagem()->w + 20;

        //AJUSTA POSICAO NA TELA E COLOCA IMAGEM DA NAVE SELECIONADA
        modelo.w = navesSelecionadas[j]->GetImagem()->w;
        modelo.h = navesSelecionadas[j]->GetImagem()->h;

        //retira o fundo antes de desenhar
        Game::RetiraFundo(navesSelecionadas[j]->GetImagem(), 0x0);

        SDL_BlitSurface(navesSelecionadas[j]->GetImagem(), &modelo, Game::GetSurfacePrincipal(), &posicao);
    }
}

//FUNCAO QUE TRATA ENTRADA DO USUARIO NO MENU PRINCIPAL
void MenuPrincipal::TrataMenuPrincipal()
{
    int iOpc;

    //ATUALIZA O MENU COM A ENTRADA DO USUARIO E PEGA A OPCAO SELECIONADA
    iOpc = menu->Atualiza(Game::GetTeclas());

    //SE SELECIONOU A OPCAO 0, MOSTRA O MENU DE DIFICULDADE
    if (iOpc == 0)
    {
        //SE ESTAVA MOSTRANDO TIRA, SE NAO ESTAVA MOSTRANDO MOSTRA
        bMostraDificuldade = !bMostraDificuldade;

        //SE O MENU FOR NULO E É PRA MOSTRAR O MENU, CRIA NOVO MENU DE DIFICULDADE
        if (menuDificuldade == NULL && bMostraDificuldade)
        {
            CriaMenuDificuldade();
        }
    }
    //SE SELECIONOU A OPCAO 1, MOSTRA LISTA DE SCORES
    else if (iOpc == 1)
    {
        //SE ESTAVA MOSTRANDO ESCONDE, SENAO MOSTRA
        bMostraScore = !bMostraScore;

        //SE É PRA MOSTRAR E A LISTA ESTA NULA, CRIA NOVA LISTA
        if (bMostraScore && scores == NULL)
        {
            CriaListaScore();
        }
    }
    //SE SELECIONOU A OPCAO 2, MOSTRA A TELA DE CONFIGURACOES
    else if (iOpc == 2)
    {
        Game::SetProximoNivel(NIVEL_CONFIGURACOES);
    }
    //SE APERTOU A OPCAO 3, FECHA O JOGO
    else if (iOpc  == 3)
    {
        Game::FecharJogo();
    }
}

//FUNCAO QUE TRATA ENTRADA DO USUARIO QUANDO ESTA MOSTRANDO O ESCORE
void MenuPrincipal::TrataScore()
{
    //SE APERTOU PARA BAIXO, SOBE A POSICAO DA LISTA DE SCORE
    if (Game::GetTeclas()[TECLA_SETA_BAIXO])
    {
        AjustaPosicaoScore(-1);
    }

    //SE APERTOU PARA CIMA, DESCE A LISTA DE SCORE
    if (Game::GetTeclas()[TECLA_SETA_CIMA])
    {
        AjustaPosicaoScore(1);
    }

    //AJUSTE AUTOMATICO DA POSICAO DA LISTA
    AjustaPosicaoScore(0);

    //SE APERTOU ESC, FECHA A LISTA E VOLTA PARA MENU PRINCIPAL
    if (Game::GetTeclas()[TECLA_ESC])
    {
        //ESCONDEMENU
        bMostraScore = false;
        Game::GetTeclas()[TECLA_ESC] = 0;

        //DELETA A LISTA E SETA REFERENCIA NULA
        delete scores;
        scores = NULL;

        //DELETA A REFERENCIA DO SCORE E SETA COMO NULA
        delete listaScore;
        listaScore = NULL;

        //RETORNA DA FUNCAO
        return;
    }
}

//TRATA ENTRADA DE USUARIO NA TELA DE DIFICULDADE
void MenuPrincipal::TrataDificuldade()
{
    bool bSelecionou = false;
    int iOpc;

    //ATUALIZA MENU COM A ENTRADA DO USUARIO E PEGA RETORNO
    iOpc = menuDificuldade->Atualiza(Game::GetTeclas());

    //SE RETORNO FOI DIFERENTE DE -1, ENTAO USUÁRIO SELECIONOU UMA DIFICULDADE
    if (iOpc != -1)
    {
        //SE DIFICULDADE COMO A OPCAO SELECIONADA PELO USUARIO
        Game::SetDificuldade((DIFICULDADES) iOpc);
        bSelecionou = true;
    }

    //SE SELECIONOU, CRIA A LISTA DE NAVES PARA SELECAO
    if (bSelecionou)
    {
        bMostraNaves = true;

        if (listaNaves == NULL && bMostraNaves)
            CriaListaNaves();

        Game::GetTeclas()[TECLA_ESC] = 1;
    }

    //SE APERTOU ESC, FECHA O MENU DE DIFICULDADE
    if (Game::GetTeclas()[TECLA_ESC])
    {
        //ESCONDE MENU
        bMostraDificuldade = false;
        Game::GetTeclas()[TECLA_ESC] = 0;

        //DELETA REFERENCIA DO MENU E SETA PONTEIRO NULO
        delete menuDificuldade;
        menuDificuldade = NULL;

        return;
    }
}

//TRATA ENTRADA DE USUÁRIO NA LISTA DE NAVES
void MenuPrincipal::TrataListaNaves()
{
    int iOpc, iNave;

    //ATUALIZA LISTA COM ENTRADA DE USUARIO
    iOpc = listaNaves->Atualiza(Game::GetTeclas());

    //SE A OPCAO SELECIONADA É DIFERENTE DE -1, ENTAO USUÁRIO SELECIONOU UMA NOVA NAVE
    if (iOpc != -1)
    {
        //LISTA DE SELECAO RECEBE A NAVE SELECIONADA
        iNavesSelecionadas[iQuantSelecao++] = iOpc;

        //REFERENCIA DAS NAVES RECEBE UM OBJETO NAVE
        navesSelecionadas[iQuantSelecao - 1] = Game::GetNaves((TIPO_NAVES) iOpc);
    }

    //se usuário apertou esc
    if (Game::GetTeclas()[TECLA_ESC])
    {
        //RESETA SELECAO DE NAVE
        for (register int i = 0; i < iTotalSelecao; i++)
            iNavesSelecionadas[i] = -1;

        //SETA SELECAO COMO 0
        iQuantSelecao = 0;

        //DELETA LISTA E SETA REFERENCIA NULA
        delete listaNaves;
        listaNaves = NULL;

        return;
    }

    //SE ATINGIMOS O MAXIMO DA SELECAO DE NAVES. VAI PRA ELA DE JOGO
    if (iQuantSelecao == iTotalSelecao)
    {
        //SETA A SELECAO DE NAVES E TROCA PARA TELA DE JOGO
        Game::SetSelecaoNaves(iNavesSelecionadas, iQuantSelecao);
        Game::SetProximoNivel(NIVEL_JOGO_UM);
        return;
    }
}

//FUNCAO QUE CRIA A LISTA DE SCORES
void MenuPrincipal::CriaListaScore()
{
    //PEGA REFERENCIA DA LISTA DE SAVES QUE REPRESENTA OS SCORES
    listaScore = Game::GetScores(&iQuantScore);

    //CRIA O ARRAY DE CARACTER PARA O TEXTO
    textoScores = new char[70 * iQuantScore];

    for (register int i = 0; i < 70 * iQuantScore; i++)
        textoScores[i] = '\0';

    //CRIA UM ARRAY TEMPORARIO PARA MANIPULAR NUMEROS
    char* textoTemp = new char[10];

    for (register int i = 0; i < 10; i++)
        textoScores[i] = '\0';

    int j = 0;

    //VARIAVEIS DE CONTROLE DE POSICAO DA LISTA
    posicaoListaScore.x = 20;
    posicaoListaScore.y = 20;
    tamanhoListaScore.h = 30;
    tamanhoListaScore.w = 550;
    posicaoListaScore.h = tamanhoListaScore.h * iQuantScore;

    //PERCORRE A LISTA DE SAVES
    for (register int i = iQuantScore - 1; i >= 0 && listaScore != NULL; i--)
    {
        //NOME DO JOGADOR
        textoScores[j++] = 'N';
        textoScores[j++] = 'O';
        textoScores[j++] = 'M';
        textoScores[j++] = 'E';
        textoScores[j++] = ':';
        textoScores[j++] = ' ';
        textoScores[j++] = listaScore[i].GetInicias()[0];
        textoScores[j++] = listaScore[i].GetInicias()[1];
        textoScores[j++] = listaScore[i].GetInicias()[2];

        //PONTOS DO JOGADOR
        textoScores[j++] = ' ';
        textoScores[j++] = '|';
        textoScores[j++] = ' ';
        textoScores[j++] = 'P';
        textoScores[j++] = 'O';
        textoScores[j++] = 'N';
        textoScores[j++] = 'T';
        textoScores[j++] = 'O';
        textoScores[j++] = 'S';
        textoScores[j++] = ':';
        textoScores[j++] = ' ';
        sprintf(textoTemp, "%i", listaScore[i].GetPontuacao());

        //PEGA O NUMERO QUE ESTA NO ARRAY DE CHAR
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoScores[j++] = textoTemp[k];

        //DIFICULDADE DO JOGO
        textoScores[j++] = ' ';
        textoScores[j++] = '|';
        textoScores[j++] = ' ';
        textoScores[j++] = 'D';
        textoScores[j++] = 'I';
        textoScores[j++] = 'F';
        textoScores[j++] = 'I';
        textoScores[j++] = 'C';
        textoScores[j++] = 'U';
        textoScores[j++] = 'L';
        textoScores[j++] = 'D';
        textoScores[j++] = 'A';
        textoScores[j++] = 'D';
        textoScores[j++] = 'E';
        textoScores[j++] = ':';
        textoScores[j++] = ' ';
        sprintf(textoTemp, "%i", listaScore[i].GetDificuldade());

        //PEGA O NUMERO QUE ESTA NO ARRAY DE CHAR
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoScores[j++] = textoTemp[k];

        //NIVEL QUE USUARIO PAROU
        textoScores[j++] = ' ';
        textoScores[j++] = '|';
        textoScores[j++] = ' ';
        textoScores[j++] = 'N';
        textoScores[j++] = 'I';
        textoScores[j++] = 'V';
        textoScores[j++] = 'E';
        textoScores[j++] = 'L';
        textoScores[j++] = ':';
        textoScores[j++] = ' ';
        sprintf(textoTemp, "%i", listaScore[i].GetNivel());

        //PEGA O NUMERO QUE ESTA NO ARRAY DE CHAR
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoScores[j++] = textoTemp[k];

        //RESULTADO DO JOGO, SE GANHOU OU PERDEU
        textoScores[j++] = ' ';
        textoScores[j++] = '|';
        textoScores[j++] = ' ';
        textoScores[j++] = 'R';
        textoScores[j++] = 'E';
        textoScores[j++] = 'S';
        textoScores[j++] = 'U';
        textoScores[j++] = 'L';
        textoScores[j++] = 'T';
        textoScores[j++] = 'A';
        textoScores[j++] = 'D';
        textoScores[j++] = 'O';
        textoScores[j++] = ':';
        textoScores[j++] = ' ';
        sprintf(textoTemp, "%i", listaScore[i].GetResultado());

        //PEGA O NUMERO QUE ESTA NO ARRAY DE CHAR
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoScores[j++] = textoTemp[k];

        textoScores[j++] = '\n';
    }

    //SE NAO TEM NENHUM SCORE, ENTAO MOSTRA MENSAGEM DE QUE NAO ENCONTROU SCORE SALVO
    if (iQuantScore == NULL)
    {
        textoScores[j++] = 'N';
        textoScores[j++] = 'A';
        textoScores[j++] = 'O';
        textoScores[j++] = ' ';
        textoScores[j++] = 'H';
        textoScores[j++] = 'A';
        textoScores[j++] = ' ';
        textoScores[j++] = 'S';
        textoScores[j++] = 'C';
        textoScores[j++] = 'O';
        textoScores[j++] = 'R';
        textoScores[j++] = 'E';
        textoScores[j++] = 'S';
        textoScores[j++] = ' ';
        textoScores[j++] = 'S';
        textoScores[j++] = 'A';
        textoScores[j++] = 'L';
        textoScores[j++] = 'V';
        textoScores[j++] = 'O';
        textoScores[j++] = 'S';
        textoScores[j++] = '\n';
    }

    //CRIA MENU QUE REPRESENTA A LISTA DE SCORE
    scores = new Menu(Game::GetSurfacePrincipal(), textoScores, QUASE_PRETO, QUASE_PRETO, BRANCO32, BRANCO32, false, tamanhoListaScore, posicaoListaScore);

    //STARTA TIMER QUE VAI MOVER A LISTA CASO NECESSÁRIO
    timerScore.start();
}

//FUNCAO QUE CONTROLA A POSICAO DA LISTA DE SCORE NA TELA
void MenuPrincipal::AjustaPosicaoScore(int somaY)
{
    //SE A ALTURA DA LISTA É MENOR QUE A ALTURA DA JANELA, NAO PRECISAMOS FAZER NADA
    if (scores->GetRectPosicao().h < Game::GetAlturaJanela())
        return;

    //SE A SOMA É DIFERENTE DE ZERO, COMA O VALOR AO Y DA LISTA DE SCORE
    if (somaY != 0)
    {
        scores->GetRectPosicao().y += somaY;
        timerScore.start();
        return;
    }

    //SE A SOMA É IGUAL A ZERO, ENTAO CALCULA PROXMA POSICAO E DESPAUSA TIMER PARA PEGAR O TEMPO PASSADO DESDE A ULTIMA INTERACAO
    timerScore.unpause();

    //SE JÁ AINDA NÃO É HORA DE ATUALIZAR A POSICAO
    if (timerScore.get_ticks() < iIntervaloScore)
        return;

    //Y DA LISTA RECEBE -1 PARA QUE SUBA NA TELA
    scores->GetRectPosicao().y -= 1;

    //SE O Y -E MAIOR QUE A ALTURA DE JANELA, OU SEJA, SE A LISTA ESTA PARA BAIXO DA TELA, RESETA A POSICAO
    if (scores->GetRectPosicao().y > Game::GetAlturaJanela())
        scores->GetRectPosicao().y = 20;

    //SE O FIM DA LISTA CHEGOU NO MEIO DA TELA, RESETA POSICAO
    if (scores->GetRectPosicao().y + scores->GetRectPosicao().h < Game::GetAlturaJanela() / 2)
        scores->GetRectPosicao().y = 20;

    //STARTA TIMER NOVAMENTE PARA PROXIMA INTERACAO
    timerScore.start();
}

//FUNCAO QUE CRIA O MENU DE DIFICULDADE
void MenuPrincipal::CriaMenuDificuldade()
{
    //RECTS DE CONTROLE DE POSICAO E MODELO DO MENU
    SDL_Rect modelo, posicao;
    modelo.h = 30;
    modelo.w = 100;
    posicao.x = Game::GetLarguraJanela() / 2 - modelo.w * 4;
    posicao.y = Game::GetAlturaJanela() / 2;

    //CRIA MENU DE DIFICULDADE COM AS DIFICULDADE PADROES NA VERTICAL
    menuDificuldade = new Menu(Game::GetSurfacePrincipal(), "NOVATO\nCAPITAO\nGENERAL\n", PRETO, PRETO, BRANCO32, VERMELHO32, true, modelo, posicao);
}

//FUNCAO QUE CRIA A LISTA DE NAVES PARA SELECAO
void MenuPrincipal::CriaListaNaves()
{
    Nave* naveAtual;
    char* textoTemp;
    SDL_Surface** imagens;
    SDL_Rect modeloDesenho, modeloTexto, posicao;

    //PEGA TOTAL DE NAVES DO JOGO
    iTotalNavesJogo = Game::GetQuantidadeNaves();

    //CRIA ARRAY CARACTER PARA TEXTO DE CADA NAVE
    textoNave = new char*[iTotalNavesJogo];

    //CRIA ARRAY CARACTER PARA MANIPULAR NUMEROS
    textoTemp = new char[50]{'\0'};

    //CRIA ARRAY DE REFERENCIA PARA IMAGENS DAS NAVES
    imagens = new SDL_Surface*[iTotalNavesJogo];

    //PERCORRE TODAS AS NAVES DO JOGO
    for (register int i, j = i = 0; i < iTotalNavesJogo; i++, j = 0)
    {
        //INSTANCIA NOVO TEXTO PARA ESTA NAVE
        textoNave[i] = new char[200]{'\0'};

        for (register int k = 0; k < 200; k++)
            textoNave[i][k] = '\0';

        //PEGA A NAVE ATUAL
        naveAtual = Game::GetNaves((TIPO_NAVES)i);

        //PEGA A IMAGEM DA NAVE ATUAL
        imagens[i] = naveAtual->GetImagem();

        //PEGA O NOME DA NAVE
        textoNave[i][j++] = 'N';
        textoNave[i][j++] = 'O';
        textoNave[i][j++] = 'M';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = ':';
        textoNave[i][j++] = ' ';

        //pega o nome da nave
        for (int k = 0; naveAtual->GetNome()[k] != '\0'; k++)
            textoNave[i][j++] = naveAtual->GetNome()[k];

        //VELOCIDADE DA NAVE
        textoNave[i][j++] = '\n';
        textoNave[i][j++] = 'V';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'L';
        textoNave[i][j++] = 'O';
        textoNave[i][j++] = 'C';
        textoNave[i][j++] = 'I';
        textoNave[i][j++] = 'D';
        textoNave[i][j++] = 'A';
        textoNave[i][j++] = 'D';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = ':';
        textoNave[i][j++] = ' ';
        sprintf(textoTemp, "%.2f", naveAtual->GetVetor().GetMagnitude() * 3);

        //PEGA O NUMERO DA VELOCIDADE
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoNave[i][j++] = textoTemp[k];

        //INTERVALO DE TIRO
        textoNave[i][j++] = '\n';
        textoNave[i][j++] = 'I';
        textoNave[i][j++] = 'N';
        textoNave[i][j++] = 'T';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'R';
        textoNave[i][j++] = 'V';
        textoNave[i][j++] = 'A';
        textoNave[i][j++] = 'L';
        textoNave[i][j++] = 'O';
        textoNave[i][j++] = ' ';
        textoNave[i][j++] = 'T';
        textoNave[i][j++] = 'I';
        textoNave[i][j++] = 'R';
        textoNave[i][j++] = 'O';
        textoNave[i][j++] = ':';
        textoNave[i][j++] = ' ';
        sprintf(textoTemp, "%i", naveAtual->GetIntervaloTiro());

        //PEGA O NUMERO DO INTERVALO
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoNave[i][j++] = textoTemp[k];

        //INTERVALO ESPECIAL
        textoNave[i][j++] = '\n';
        textoNave[i][j++] = 'I';
        textoNave[i][j++] = 'N';
        textoNave[i][j++] = 'T';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'R';
        textoNave[i][j++] = 'V';
        textoNave[i][j++] = 'A';
        textoNave[i][j++] = 'L';
        textoNave[i][j++] = ' ';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'S';
        textoNave[i][j++] = 'P';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'C';
        textoNave[i][j++] = 'I';
        textoNave[i][j++] = 'A';
        textoNave[i][j++] = 'L';
        textoNave[i][j++] = ':';
        textoNave[i][j++] = ' ';
        sprintf(textoTemp, "%i", naveAtual->GetIntervaloEspecial());

        //PEGA O NUMERO DO INTERVALO
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoNave[i][j++] = textoTemp[k];

        //TEMPO DE ESPECIAL
        textoNave[i][j++] = '\n';
        textoNave[i][j++] = 'T';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'M';
        textoNave[i][j++] = 'P';
        textoNave[i][j++] = 'O';
        textoNave[i][j++] = ' ';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'S';
        textoNave[i][j++] = 'P';
        textoNave[i][j++] = 'E';
        textoNave[i][j++] = 'C';
        textoNave[i][j++] = 'I';
        textoNave[i][j++] = 'A';
        textoNave[i][j++] = 'L';
        textoNave[i][j++] = ':';
        textoNave[i][j++] = ' ';
        sprintf(textoTemp, "%i", naveAtual->GetTempoEspecial());

        //PEGA TEMPO DE ESPECIAL
        for (int k = 0; textoTemp[k] != '\0'; k++)
            textoNave[i][j++] = textoTemp[k];

        textoNave[i][j++] = '\n';

        //DELETA REFERENCIA DA NAVE ATUAL
        delete naveAtual;
        naveAtual = NULL;
    }

    //AJUSTA RECTS DE POSICAO NA TELA E MODELO DE CADA LISTA
    modeloTexto.h = 30;
    modeloTexto.w = 200;

    modeloDesenho.x = 0;
    modeloDesenho.y = 0;

    modeloDesenho.h = imagens[0]->h + 10;
    modeloDesenho.w = imagens[0]->w + 10;

    posicao.x = Game::GetLarguraJanela() / 3;
    posicao.y = Game::GetAlturaJanela() / 3;

    //CRIA NOVA LISTA DE NAVES
    listaNaves = new ListaNaves(Game::GetSurfacePrincipal(), imagens, iTotalNavesJogo, textoNave, PRETO, BRANCO32, modeloDesenho, modeloTexto, posicao);

    //A PRIMEIRA NAVE DA SELEÇÃO SEMPRE VAI SER O ESCUDO
    navesSelecionadas[iQuantSelecao] = Game::GetEscudo();
    iNavesSelecionadas[iQuantSelecao++] = navesSelecionadas[iQuantSelecao]->GetNave();
}

MenuPrincipal::~MenuPrincipal()
{
    if (scores != NULL)
        delete scores;

    if (menuDificuldade != NULL)
        delete menuDificuldade;

    if (listaNaves != NULL)
        delete listaNaves;

    if (listaScore != NULL)
        delete listaScore;

    for (register int i = 0; i < iTotalSelecao && navesSelecionadas != NULL; i++)
        if (navesSelecionadas[i] != NULL)
            delete navesSelecionadas[i];

    if (navesSelecionadas != NULL)
        delete navesSelecionadas;
}
