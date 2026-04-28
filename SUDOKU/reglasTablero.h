#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "celda.h"
#include "tablero.h"
using namespace std;

struct tPos {
    int f;
    int c;
};

struct tListaBloq {
    int contBloq;
    int capacidad;
    tPos* lista;   // array dinamico de tPos( a lo mejor mal???)
};

struct tValor {
    bool posible;
    int celdas_que_afectan;
};

struct tValores {
    int nFilas;
    int nColumnas;
    tValor valores[MAX_DIM][MAX_DIM][MAX_DIM];
};

class tReglas {
private:
    tTablero tablero;
    tTablero tableroOriginal;
    int cont;
    tListaBloq lista;
    tValores valores_celda;

    void actualiza_bloqueos();
    void inicializa_valores();

    //actualiza los valores una vez escogidos
    void actualiza_valores(int f, int c, int v, bool poniendo);
    void actualiza_una_celda(int i, int j, int idx, bool poniendo);

    //nuevos métodos privados
    void inicializa_lista_bloq();
    void libera_lista_bloq();
    void copia_lista_bloq(const tListaBloq& origen);
    //

public:
    tReglas();

    //nuevo
    tReglas(const tReglas& r);
    tReglas& operator=(const tReglas& r);
    ~tReglas();
    //

    const int dame_dimension();
    tCelda dame_celda(int f, int c);
    const bool terminado();
    const bool bloqueo();
    const int dame_num_celdas_bloqueadas();
    const int dame_num_celdas_vacias();
    const tCelda dame_celda_bloqueada(int p, int& f, int& c);
    const bool es_valor_posible(int f, int c, int v);
    const int posibles_valores(int f, int c);
    void pon_valor(int f, int c, int v);
    void quita_valor(int f, int c);
    void reset();
    void autocompletar();
    void carga_sudoku(ifstream& arch); 

    //nuevo
    int dame_num_celdas_con_n_posibles(int n);
    void guarda_partida(ofstream& arch);
    //
};

bool operator<(tReglas& a, tReglas& b);
bool operator==(tReglas& a, tReglas& b);
