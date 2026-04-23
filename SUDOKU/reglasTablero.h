#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "celda.h"
#include "tablero.h"

struct tPos {
    int f;
    int c;
};

struct tListaBLoq {
    int contBloq;
    tPos lista[MAX_DIM * MAX_DIM];
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
    tListaBLoq lista;
    void actualiza_bloqueos();
    tValores valores_celda;
    void inicializa_valores();

    //actualiza los valores una vez escogidos
    void actualiza_valores(int f, int c, int v, bool poniendo);
public:
    tReglas();
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
    void carga_sudoku(std::ifstream& arch); 
};