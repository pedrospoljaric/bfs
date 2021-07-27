#include <stdio.h>
#include <stdlib.h>
#define MAX 500

#define BRANCO 1
#define CINZA 2
#define PRETO 3
#define INFINITO 999999

typedef struct Vertice
{
    int id;
    char tipo;
    int nArestas;
    int dist;
    int cor;
    struct Vertice *prox;
}Vertice;

typedef struct Grafo
{
    int M[MAX][MAX];
    Vertice *V;
    int nArestas;
    int nVertices;
}Grafo;

typedef struct Fila
{
    Vertice *inicio, *fim;
}Fila;

Vertice novoVert(int id, char tipo, int i, int j);
void InserirAresta(Grafo *grafo, Vertice u, Vertice v);
void BFS(Grafo *g, int vInicial);

void Enfileirar(Fila *f, Vertice v);
Vertice Desenfileirar(Fila *f);
int FilaVazia(Fila *f);

int main()
{
    Grafo grafo;
    int N=1, M=1;
    int i, j;

    scanf("%d %d", &N, &M);
    while (N != 0 || M != 0)
    {
        if (N == 0 && M == 0) return 0;
        int nVert = N*M;
        grafo.V = (Vertice*) malloc(nVert * sizeof(Vertice));
        grafo.nArestas = 0;
        grafo.nVertices = 0;

        char labirinto[N][M];
        char t;

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < M; j++)
            {
                scanf(" %c", &t);
                labirinto[i][j] = t;
            }
        }

        int vert = 0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < M; j++)
            {
                grafo.V[vert] = novoVert(vert, labirinto[i][j], i, j);
                vert++;
            }
        }
        grafo.nVertices = vert;

        vert = 0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < M; j++)
            {
                if (grafo.V[vert].tipo != '#')
                {
                    if (j != 0 && grafo.V[vert-1].tipo != '#')
                    {
                        InserirAresta(&grafo, grafo.V[vert], grafo.V[vert-1]);
                        grafo.nArestas++;
                    }
                    if (j != M-1 && grafo.V[vert+1].tipo != '#')
                    {
                        InserirAresta(&grafo, grafo.V[vert], grafo.V[vert+1]);
                        grafo.nArestas++;
                    }
                    if (i != 0 && grafo.V[vert-M].tipo != '#')
                    {
                        InserirAresta(&grafo, grafo.V[vert], grafo.V[vert-M]);
                        grafo.nArestas++;
                    }
                    if (i != N-1 && grafo.V[vert+M].tipo != '#')
                    {
                        InserirAresta(&grafo, grafo.V[vert], grafo.V[vert+M]);
                        grafo.nArestas++;
                    }
                }
                vert++;
            }
        }

        int vInicial = 0;
        while (grafo.V[vInicial].tipo != '.') vInicial++;

        BFS(&grafo, vInicial);

        int indiceDistMaior = vInicial;
        for (i = 0; i < grafo.nVertices; i++)
        {
            if (grafo.V[i].dist != INFINITO && grafo.V[i].dist > grafo.V[indiceDistMaior].dist)
                indiceDistMaior = i;
        }

        BFS(&grafo, indiceDistMaior);

        for (i = 0; i < grafo.nVertices; i++)
        {
            if (grafo.V[i].dist != INFINITO && grafo.V[i].dist > grafo.V[indiceDistMaior].dist)
                indiceDistMaior = i;
        }

        printf("%d\n", grafo.V[indiceDistMaior].dist);

        scanf("%d %d", &N, &M);
    }

    return 0;
}

void BFS(Grafo *g, int vInicial)
{
    int i;

    for (i = 0; i < g->nVertices; i++)
    {
        g->V[i].cor = BRANCO;
        g->V[i].dist = INFINITO;
    }

    Fila f;
    f.inicio = NULL;
    f.fim = NULL;
    g->V[vInicial].cor = CINZA;
    g->V[vInicial].dist = 0;
    Enfileirar(&f, g->V[vInicial]);

    while(!FilaVazia(&f))
    {
        Vertice v = Desenfileirar(&f);
        v.cor = PRETO;

        for(i = 0; i < g->nVertices; i++)
        {
            if(g->M[v.id][i] == 1 && g->V[i].cor == BRANCO && g->V[i].tipo != '#')
            {
                g->V[i].cor = CINZA;
                g->V[i].dist = v.dist+1;
                Enfileirar(&f, g->V[i]);
            }
        }
    }
}

void Enfileirar(Fila *f, Vertice v)
{
    Vertice *aux = (Vertice*) malloc(sizeof(Vertice));
    *aux = v;

    aux->prox = NULL;

    if (f->inicio == NULL)
        f->inicio = aux;

    if (f->fim != NULL)
        f->fim->prox = aux;

    f->fim = aux;
}

int FilaVazia(Fila *f)
{
    if (f->inicio == NULL) return 1;
    return 0;
}

Vertice Desenfileirar(Fila *f)
{
    Vertice v = *f->inicio;

    Vertice *aux = f->inicio->prox;

    //free(f->inicio);
    f->inicio = aux;

    return v;
}

Vertice novoVert(int id, char tipo, int i, int j)
{
    Vertice v;
    v.id = id;
    v.tipo = tipo;
    v.nArestas = 0;

    return v;
}

void InserirAresta(Grafo *grafo, Vertice u, Vertice v)
{
    if (grafo->M[u.id][v.id] != 1)
    {
        grafo->M[u.id][v.id] = 1;
        u.nArestas++;
        v.nArestas++;
    }
}
