#include "Tiro.h"

Tiro::Tiro(char* imagemTiro, TIROS tipo, TIPO_OBJETOS tipoObj) : GameObject(imagemTiro, tipoObj)
{
    //PROPRIEDADES PADRAO PRO TIRO
    vetor.SetSentido(1);
    vetor.SetMagnitude(10);
    tipoTiro = tipo;
    fDistanciaPercorrida = 0;
    iDano = 1;
}

//FUNCAO CHAMADA TODO GAMELOOP
void Tiro::Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame)
{
    //ATUALIZA ANIMACAO
    GameObject::Atualiza(iDeltaTime, iFrame);

    //se o tiro colidiu
    if (bColidiu)
    {
        //percorre todas as colisoes
        for (register int i = 0; i < iNumeroColisao; i++)
        {
            //SE COLIDIU COM UM TIRO AMIGO E É UM TIRO INIMIGO, SE SUICIDA
            if (gameObjectColidiu[i]->GetTipo() == OBJETO_TIRO_AMIGO && tipo == OBJETO_TIRO_INIMIGO)
            {
                Game::GetFaseAtual()->ExcluiGameObject(this);
                return;
            }
            //SE É UM TIRO AMIGO E COLIDIU COM UM TIRO INIMIGO, SE SUICIDA
            else if (gameObjectColidiu[i]->GetTipo() == OBJETO_TIRO_INIMIGO && tipo == OBJETO_TIRO_AMIGO)
            {
                Game::GetFaseAtual()->ExcluiGameObject(this);
                return;
            }
            //SE É UM TIRO INIMIGO E COLIDIU COM O PLANETA, CAUSA DANO NO PLANETA E SE SUICIDA
            else if (gameObjectColidiu[i]->GetTipo() == OBJETO_PLANETA && tipo == OBJETO_TIRO_INIMIGO)
            {
                if (gameObjectColidiu[i]->GetTomaDano())
                    gameObjectColidiu[i]->SetHP(gameObjectColidiu[i]->GetHP() - iDano);

                Game::GetFaseAtual()->ExcluiGameObject(this);
                return;
            }
            //SE É UM TIRO INIMIGO E COLIDIU COM A NAVE, DA DANO NA NAVE E SE SUICIDA
            else if (gameObjectColidiu[i]->GetTipo() == OBJETO_NAVE && tipo == OBJETO_TIRO_INIMIGO)
            {
                if (gameObjectColidiu[i]->GetTomaDano())
                    gameObjectColidiu[i]->SetHP(gameObjectColidiu[i]->GetHP() - iDano);

                Game::GetFaseAtual()->ExcluiGameObject(this);
                return;
            }
            //SE É UM TIRO AMIGO E COLIDIU COM UM INIMIGO, DA DANO NO INIMIGO E SE SUICIDA
            else if (gameObjectColidiu[i]->GetTipo() == OBJETO_INIMIGO && tipo == OBJETO_TIRO_AMIGO)
            {
                if (gameObjectColidiu[i]->GetTomaDano())
                    gameObjectColidiu[i]->SetHP(gameObjectColidiu[i]->GetHP() - iDano);

                Game::GetFaseAtual()->ExcluiGameObject(this);
                return;
            }
            //SE É UM TIRO AMIGO E COLIDIU COM O BOSS, DA DANO NO BOSS E SE SUICIDA
            else if (gameObjectColidiu[i]->GetTipo() == OBJETO_BOSS && tipo == OBJETO_TIRO_AMIGO)
            {
                if (gameObjectColidiu[i]->GetTomaDano())
                    gameObjectColidiu[i]->SetHP(gameObjectColidiu[i]->GetHP() - iDano);

                Game::GetFaseAtual()->ExcluiGameObject(this);
                return;
            }
        }

        //RESETA VARIAIVEIS DE CONTROLE
        iNumeroColisao = 0;
        bColidiu = false;
    }

    //SE O RAIO É MAIOR OU IGUAL A ORBITA 1 E O TIPO DO TIRO É TIRO UM, NAO ANDA MAIS NO RAIO, MAS NO ANGULO PRA FICAR EM ORBITA
    if (pp.GetRaio() >= Game::GetOrbitas()[1] && tipoTiro == TIRO_UM)
        pp.SetAngulo(pp.GetAngulo() + 0.03);
    //SE FOR UM TIRO DIFERENTE DO TIRO_UM, ANDA NO RAIO
    else
        //atualiza posicao do tiro
        pp.SetRaio(pp.GetRaio() + vetor.GetMagnitude() * vetor.GetSentido());

    //soma a distancia percorrida
    fDistanciaPercorrida += vetor.GetMagnitude();

    //pega o novo cartesiano
    pc = pp.GetCartesianoCorrespondente();

    //valida se saiu da janela, se saiu se SUICIDA
    if (pc.GetX() > Game::GetLarguraJanela() + 50 || pc.GetX() < -50 || pc.GetY() > Game::GetAlturaJanela() + 50 || pc.GetY() < -50)
    {
        Game::GetFaseAtual()->ExcluiGameObject(this);
        return;
    }
}

//funcao chamada em todo gameloop
void Tiro::Desenha()
{
    GameObject::Desenha();
}

//seta novo tipo de tiro
void Tiro::SetTipoTiro(TIROS tipo)
{
    tipoTiro = tipo;
}

//retorna o tipo do tiro
TIROS Tiro::GetTipoTiro()
{
    return tipoTiro;
}

Tiro::~Tiro()
{
    //dtor
}
