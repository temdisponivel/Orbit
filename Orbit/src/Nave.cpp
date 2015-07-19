#include "Nave.h"

Nave::Nave(char *caminhoImagem, TIPO_NAVES tipo, JOGADOR jog, Controle controles) : GameObject(caminhoImagem, OBJETO_NAVE)
{
    //SETA PROPRIEDADES PADRAO DA NAVE
    pp.SetAngulo(0);
    pp.SetRaio(150);
    vetor.SetMagnitude(1);
    iIntervaloTiro = 50;
    tipoNave = tipo;
    iIntervaloEspecial = 5000;
    bEspecialAtivo = false;
    iTempoEspecial = 3000;
    timerIntervaloEspecial.start();
    timerTiro.start();
    controle = controles;
    jogador = jog;
    naveEspecial = NULL;
    especial = ESPECIAL_UM;
    iQuantTiro = 1;

    //SE FOR UM ESCUDO, SETA O ANGULO OPOSTO A NAVE
    if (jogador == JOGADOR_ESCUDO)
    {
        pp.SetAngulo(180);
        pp.SetRaio(100);
    }

    iHP = 1;
}

//FUNCAO CHAMADA A TODO GAMELOOP
void Nave::Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame)
{
    //ATUALIZA ANIMACAO
    GameObject::Atualiza(iDeltaTime, iFrame);

    //SE O HP CHEGOU A ZERO ENÃO É O ESCUDO
    if (iHP <= 0 && jogador != JOGADOR_ESCUDO)
    {
        //SUICIDA
        Game::GetFaseAtual()->ExcluiGameObject(this);
        return;
    }

    //se apertou a tecla de controle que move em sentido horario
    if (Game::GetTeclas()[controle.controle_um] == 1 )
    {
        pp.SetAngulo(pp.GetAngulo() + vetor.GetMagnitude());
    }
    //se apertou o controle para mover sentido antihorario
    if (Game::GetTeclas()[controle.controle_dois] == 1 )
    {
        pp.SetAngulo(pp.GetAngulo() - vetor.GetMagnitude());
    }

    //se apertou a tecla para atirar e já passou o tempo do intervalo de tiro
    if (Game::GetTeclas()[controle.controle_tres] && timerTiro.get_ticks() > iIntervaloTiro)
        Atirar();

    //se apertou especial ou esta com especial ativo
    if (Game::GetTeclas()[controle.controle_quatro] || bEspecialAtivo)
        GerenciaEspecial();

    //atualiza posicao na tela
    pc = pp.GetCartesianoCorrespondente();
}

//FUNCAO CHAMADA A TODO GAMELOOP
void Nave::Desenha()
{
    Game::RotacionaGameObject(this);

    //desenha imagem na tela
    GameObject::Desenha();

    //libera surface temporaria
    SDL_FreeSurface(imagem);

    //reseta surface com o backup
    imagem = imagemAuxilia;
}

//FUNCAO QUE CRIA UM NOVO TIRO
void Nave::Atirar()
{
    //atira a quantidade de vezes padrao da nave
    for (register int i = 0; i < iQuantTiro; i++)
    {
        Tiro* tiro = Game::GetTiros(tipoTiro);

        if (tiro == NULL)
            continue;

        //seta propriedades
        tiro->SetTipo(OBJETO_TIRO_AMIGO);
        tiro->GetPontoPolar().SetRaio(pp.GetRaio() - GetAlturaImagem() / 2);
        tiro->GetPontoPolar().SetAngulo(pp.GetAngulo());

        //ADICIONA O NOVO TIRO NA LISTA DE GAMEOBJECT
        Game::GetFaseAtual()->IncluirGameObject(tiro);
    }

    //STARTA TIMER TIRO PARA CONTAR INTERVALO DE TIRO
    timerTiro.start();
}

//SETA NOVO INTERVALO DE TIRO
void Nave::SetIntervaloTiro(int iNovoIntervalo)
{
    iIntervaloTiro = iNovoIntervalo;
}

//SETA NOVO INTERVALO ESPECIAL
void Nave::SetIntervaloEspecial(int iNovoIntervalo)
{
    iIntervaloEspecial = iNovoIntervalo;
}

//FUNCAO QUE GERENCIA ESPECIAL
void Nave::GerenciaEspecial()
{
    //se passou o intervalo ate atingir o especial
    if (timerIntervaloEspecial.get_ticks() > iIntervaloEspecial)
    {
        //seta especial ativo, ativa especial e inicia timer de especial
        bEspecialAtivo = true;
        Especial(true);

        if (!timerEspecial.is_started())
            timerEspecial.start();
    }

    //se o especial nao esta ativo retorna
    if (!bEspecialAtivo)
        return;

    //se ja atingiu o tempo de especial e especial esta ativo, desativa
    if (timerEspecial.get_ticks() > iTempoEspecial && bEspecialAtivo)
    {
        //desativa especial, chama funcao que para especial e reinicia timer de intervalo especial
        bEspecialAtivo = false;
        Especial(false);
        timerIntervaloEspecial.start();
        timerEspecial.stop();
        return;
    }
}

//FUNCAO QUE RETORNA O NOME DA NAVE
char* Nave::GetNome()
{
    return nomeNave;
}

//FUNCAO QUE SETA O NOME DA NAVE
void Nave::SetNome(char* nome)
{
    nomeNave = nome;
}

//FUNCAO QUE RETORNA O TIPO DA NAVE
TIPO_NAVES Nave::GetNave()
{
    return tipoNave;
}

//FUNCAO QUE SETA O TIPO DA NAVE
void Nave::SetNave(TIPO_NAVES tipo)
{
    tipoNave = tipo;
}

//FUNCAO QUE RETORNA O TIPO DO TIRO
TIROS Nave::GetTipoTiro()
{
    return tipoTiro;
}

//FUNCAO QUE SETA O TIPO DO TIRO
void Nave::SetTipoTiro(TIROS tipo)
{
    tipoTiro = tipo;
}

//FUNCAO QUE SETA O TEMPO ESPECIAL
void Nave::SetTempoEspecial(int iNovoTempo)
{
    iTempoEspecial = iNovoTempo;
}

//FUNCAO QUE RETORNA OS CONTROLES DA NAVE
Controle& Nave::GetControles()
{
    return controle;
}

//FUNCAO QUE SETA O INTERVALO DO TIRO
int Nave::GetIntervaloTiro()
{
    return iIntervaloTiro;
}

//FUNCAO QUE RETORNA O INTERVALO DE ESPECIAL
int Nave::GetIntervaloEspecial()
{
    return iIntervaloEspecial;
}

//FUNCAO QUE RETORNA O TEMPO DE ESPECIAL
int Nave::GetTempoEspecial()
{
    return iTempoEspecial;
}

//FUNCAO QUE SETA O TIPO DO JOGADOR
void Nave::SetJogador(JOGADOR j)
{
    jogador = j;
}

//FUNCAO QUE RETORNA O TIPO DE JOGADOR
JOGADOR Nave::GetJogador()
{
    return jogador;
}

//*FUNCAO QUE SETA UM ESPECIAL
void Nave::SetEspecial(ESPECIAIS esp)
{
    especial = esp;
}

//FUNCAO QUE RETORNA O TIPO DE ESPECIAL
ESPECIAIS Nave::GetEspecial()
{
    return especial;
}

//FUNCAO QUE ATIVA OU DESATIVA O ESPECIAL
void Nave::Especial(bool bAtiva)
{
    GameObject** listaGameObject = NULL;
    int iQuantGameObject = 0;

    //SE FOR PRA TIVAR O ESPECIAL
    if (bAtiva)
    {
        //SE FOR O ESPECAL UM E A NAVEESPECIAL FOR NULA
        if (especial == ESPECIAL_UM && naveEspecial == NULL)
        {
            //CRIA NOVA NAVE QUE ESPELHADA DA NAVE ATUAL
            naveEspecial = new Nave(GetCaminhoImagem(), GetNave(), GetJogador(), GetControles());
            naveEspecial->SetTipoTiro(GetTipoTiro());
            naveEspecial->GetVetor().SetMagnitude(GetVetor().GetMagnitude());
            naveEspecial->GetPontoPolar().SetAngulo(GetPontoPolar().GetAngulo() - 180);
            naveEspecial->SetAnimacao(GetAnimacao());
            naveEspecial->SetTomaDano(false);

            //INCLUI NA LISTA DE GAMEOBJECT
            Game::GetFaseAtual()->IncluirGameObject(naveEspecial);
        }
        //SE É O ESPECIAL DOIS
        else if (especial == ESPECIAL_DOIS)
        {
            //PEGA TODOS OS GAMEOBJECTS DA FASE
            listaGameObject = Game::GetFaseAtual()->GetListaGameObject(&iQuantGameObject);

            //PERCORRE OS GAMEOBJECTS
            for (register int i = 0; i < iQuantGameObject; i++)
            {
                //SE O GAMEOBJECT ATUAL FOR UM INIMIGO
                if (listaGameObject[i]->GetTipo() == OBJETO_INIMIGO)
                {
                    //SETA MAGNITUADE COMO ELA MESMO MENOS 1, PARA DEIXAR MAIS LENTO OU PARA O INIMIGO
                    listaGameObject[i]->GetVetor().SetMagnitude(listaGameObject[i]->GetVetor().GetMagnitude() - 1);
                }
            }
        }
        //SE O ESPECIAL FOR O TRES
        else if (especial == ESPECIAL_TRES)
        {
            //PEGA A LISTA DE GAMEOBJECT DAFASE
            listaGameObject = Game::GetFaseAtual()->GetListaGameObject(&iQuantGameObject);

            //PERCORRE A LISTA
            for (register int i = 0; i < iQuantGameObject; i++)
            {
                //SE O OBJETO FOR UM PLANETA, SETA TOMA DANO COMO FALSE, PARA QUE PROTEJA O PLANETA
                if (listaGameObject[i]->GetTipo() == OBJETO_PLANETA)
                {
                    listaGameObject[i]->SetTomaDano(false);
                    break;
                }
            }
        }
    }
    //SE FOR PRA DESATIVAR O ESPECIAL
    else
    {
        //SE FORPRADESATIVAR O ESPECIAL UM E A NAVEESPECIAL FOR DIFERENTE DE NULA
        if (especial == ESPECIAL_UM && naveEspecial != NULL)
        {
            //EXCLUI A NAVE ESPECIAL
            Game::GetFaseAtual()->GetListaGameObject().ExcluiGameObject(naveEspecial);

            //SETA PONTEIRO COMO NULO
            naveEspecial = NULL;
        }
        //SE FOR PRA DESATIVAR ESPECIAL DOIS
        else if (especial == ESPECIAL_DOIS)
        {
            //PEGA A LISTA DE OBJETOS
            listaGameObject = Game::GetFaseAtual()->GetListaGameObject(&iQuantGameObject);

            //PERCORRE A LISTA DE OBJETOS
            for (register int i = 0; i < iQuantGameObject; i++)
            {
                //SE O OBJETO FOR UM INIMIGO
                if (listaGameObject[i]->GetTipo() == OBJETO_INIMIGO)
                {
                    //VOLTA A MAGNITUDE DELE PARA A ANTERIOR
                    listaGameObject[i]->GetVetor().SetMagnitude(listaGameObject[i]->GetVetor().GetMagnitudeAntiga());
                }
            }
        }
        //SE FOR O ESPECIAL TRES
        else if (especial == ESPECIAL_TRES)
        {
            //PEGA A LISTA DE GAMEOBJECT
            listaGameObject = Game::GetFaseAtual()->GetListaGameObject(&iQuantGameObject);

            //PERCORRE A LISTA
            for (register int i = 0; i < iQuantGameObject; i++)
            {
                //SE FOR O PLANETA
                if (listaGameObject[i]->GetTipo() == OBJETO_PLANETA)
                {
                    //SETA TOMA DANO COMO TRUE NOVAMENTE
                    listaGameObject[i]->SetTomaDano(true);
                    break;
                }
            }
        }
    }
}

//FUNCAO QUE PAUSA OU DISPAUSA O TIMER DO TIRO
bool Nave::ControleTimers(bool bPausar)
{
    if (bPausar)
    {
        timerEspecial.pause();
        timerTiro.pause();
    }
    else
    {
        timerEspecial.unpause();
        timerTiro.unpause();
    }
}

//FUNCAO QUE RETORNA A QUANTIDADE DE TIROS POR DISPARO
int Nave::GetQuantTiro()
{
    return iQuantTiro;
}

//FUNCAO QUE SETA A QUANTIDADE DE TIROS POR DISPARO
void Nave::SetQuantTiro(int quant)
{
    iQuantTiro = quant;
}

Nave::~Nave()
{
    if (naveEspecial != NULL)
        Game::GetFaseAtual()->ExcluiGameObject(naveEspecial);
}
