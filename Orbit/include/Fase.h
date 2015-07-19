#ifndef FASE_H
#define FASE_H
#include "Headers.h"
#include "GameObject.h"

//estrutura de controle da lista de gameobjects
typedef struct ListaGameObject
{
    GameObject** listGame;
    int iTotalGameObject;
    int iMaximo;

    ListaGameObject()
    {
        iTotalGameObject = 0;
        iMaximo = 100;
        listGame = new GameObject*[iMaximo];

        for (register int i= 0; i < iMaximo; i++)
            listGame[i] = NULL;
    }

    //Funcao que inclui um gameobject na lista de gameobjects da fase
    void IncluirGameObject(GameObject* gameObjectIncluir)
    {
        listGame[iTotalGameObject++] = gameObjectIncluir;
    }

    //funcao que exclui o gameoject da lista de gameobjectsda dessa fase
    void ExcluiGameObject(GameObject* gameObjectExcluir)
    {
        //BUSCA BINÁRIA PARA ENCONTRAR O ID DO OBJETO QUE QUEREMOS EXCLUIR
        int i = 0;
        int n = iTotalGameObject;
        int m = (i + n)/2;
        unsigned long int iId = gameObjectExcluir->GetId();
        bool bDeletado = false;

        while (i < n)
        {
            m = (i + n)/2;

            //se achamos o ID, excluimos ele da memoria
            if (iId == listGame[m]->GetId())
            {
                delete listGame[m];
                listGame[m] = NULL;
                bDeletado = true;
                break;
            }
            //se o ID esta a esquerda do meio do vetor
            //o maximo se torna o meio menos um
            //para que possa quebrar o vetor ao meio e pesquisar somente na submetade
            else if (iId < listGame[m]->GetId())
            {
                n = m;
            }
            //se o ID esta a direita do meio
            //o inferior recebe o meio mais um para que ele possa buscar somente na metade
            else
            {
                i = m;
            }
        }

        //se encontrou e deletou o gameobject da memória
        if (bDeletado)
        {
            //percorre todas as posicoes a partir do indice excluido
            for (; m < iTotalGameObject - 1; m++)
            {
                //pega o da direita e coloca no atual
                listGame[m] = listGame[m+1];
            }

            //decrementa o contador de objetos
            listGame[iTotalGameObject--] = NULL;
        }
    }

    void GerenciaGameObject()
    {
        GameObject** listTemp = NULL;

        //se faltar menos que cinco gameobjects para atingir maximo, cria vetor temporario com o dobro do vetor atual
        if (iMaximo - iTotalGameObject < 25)
        {
            //cria o array com o dobro do tamanho do atual
            listTemp = new GameObject*[(iMaximo*=2)];
        }
        else if (iMaximo - iTotalGameObject > 100)
        {
            //cria o array com o dobro do tamanho do atualw
            listTemp = new GameObject*[(iMaximo-=50)];
        }

        if (listTemp != NULL)
        {
            //percorre o vetor atual transferindo todos os dados dele para o vetor temporario
            for (register int i = 0; i < iTotalGameObject; i++)
            {
                listTemp[i] = listGame[i];
                listGame[i] = NULL;
            }

            delete listGame;

            //aposta o vetor atual para o vetor temporario
            listGame = listTemp;
        }

        for (register int i = iTotalGameObject; i < iMaximo; i++)
            listGame[i] = NULL;
    }

    int GetTamanho()
    {
        return iTotalGameObject;
    }

    int GetMaximo()
    {
        return iMaximo;
    }

    GameObject** GetLista()
    {
        return listGame;
    }

    //funcao que limpa todos os objetos da listprincipal
    void LimpaGameObject()
    {
        //percorre a lista
        for (register int i = 0; i < GetTamanho(); i++)
        {
            //deleta com seguranca
            if (listGame[i] != NULL)
            {
                delete listGame[i];
                listGame[i] = NULL;
            }
        }

        //deleta o array de gameobject
        iTotalGameObject = 0;
        iMaximo = 100;
    }

    ~ListaGameObject()
    {
        LimpaGameObject();
        delete listGame;
    }
}ListaGameObject;

class Fase
{
    public:
        Fase(NIVEIS nivelFase);
        virtual void Atualiza(const unsigned int iDeltaTime, const unsigned long long int iFrame);
        virtual void Desenha();
        virtual void FimDoNivel();
        virtual void Colisao();
        virtual void ReiniciaNivel();
        void IncluirGameObject(GameObject* gameObjectIncluir);
        virtual void ExcluiGameObject(GameObject* gameObjectExcluir);
        GameObject** GetListaGameObject(int* iQuantidade);
        void SetNivel(NIVEIS nivelFase);
        NIVEIS GetNivel();
        bool ValidaColisao(PontoCartesiano VerticeAObjetoUm, PontoCartesiano VerticeBObjetoUm, PontoCartesiano VerticeAObjetoDois, PontoCartesiano VerticeBObjetoDois);
        bool ValidaColisao(int iRaioColisor, int iRaioColido);
        virtual bool ControleTimer(bool bPausar);
        ListaGameObject& GetListaGameObject();
        virtual ~Fase();
    protected:
        GameObject* SetFundo(char* caminhoImagemFundo);
        void LimpaGameObject();
        virtual void DesenhaFundo();
        virtual void IniciarNivel() = 0;
        GameObject* fundo;
        ListaGameObject ListaPrincipal;
        NIVEIS nivel;
        Menu* menu;
};

#endif // FASE_H
