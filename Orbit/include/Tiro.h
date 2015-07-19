#ifndef TIRO_H
#define TIRO_H
#include "Headers.h"
#include "GameObject.h"
#include "Game.h"

class Tiro : public GameObject
{
    public:
        Tiro(char* imagemTiro, TIROS tipo, TIPO_OBJETOS tipoObj);
        void Atualiza(unsigned int iDeltaTime, unsigned long long int iFrame);
        void Desenha();
        void SetTipoTiro(TIROS tipo);
        TIROS GetTipoTiro();
        virtual ~Tiro();
    private:
        TIROS tipoTiro;
        float fDistanciaPercorrida;
};

#endif // TIRO_H
