#ifndef FASEMORTE_H
#define FASEMORTE_H
#include "Headers.h"
#include "Fase.h"
#include "Game.h"


class FaseMorte : public Fase
{
    public:
        FaseMorte();
        void Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame);
        void Desenha();
        virtual ~FaseMorte();
    private:
        void IniciarNivel();
        void TrataMenu();
        void TrataSave();
        void CriaTextBox();
        bool bMostraIniciais;
        Timer timerMensagem;
        int iIntervaloMensagem;
        bool bSalvou;
        TextBox* textSave;
        char* resultadoSave;
};

#endif // FASEMORTE_H
