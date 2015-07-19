#ifndef PRIMEIRAFASE_H
#define PRIMEIRAFASE_H
#include "Headers.h"
#include "Fase.h"
#include "Game.h"
#include "Nave.h"
#include "Inimigo.h"
#include "Planeta.h"
#include "GameObject.h"

class Inimigo;
class Nave;
class Planeta;

class PrimeiraFase : public Fase
{
    public:
        PrimeiraFase();
        void IniciarNivel();
        void Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame);
        void Desenha();
        void ExcluiGameObject(GameObject* gameObjectExcluir);
        virtual ~PrimeiraFase();
    private:
        void CriaPlaneta();
        void CriarBoss();
        void DesenhaFundo();
        void DesenhaHUD();
        Planeta* planeta;
        int* iSelecaoNave;
        int iQuantNavesSelecionadas;
        int iQuantNavesJogo;
        int iControleNave;
};

#endif // PRIMEIRAFASE_H
