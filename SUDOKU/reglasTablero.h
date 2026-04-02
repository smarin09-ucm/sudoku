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
    void actualiza_valores_pon(int f, int c, int v);
    void actualiza_valores_quita(int f, int c, int v);
public:
    tReglas();
    int dame_dimension();
    tCelda dame_celda(int f, int c);
    bool terminado();
    bool bloqueo();
    int dame_num_celdas_bloqueadas();
    int dame_num_celdas_vacias();
    tCelda dame_celda_bloqueada(int p, int& f, int& c);
    bool es_valor_posible(int f, int c, int v);
    int posibles_valores(int f, int c);
    void pon_valor(int f, int c, int v);
    void quita_valor(int f, int c);
    void reset();
    void autocompletar();
    void carga_sudoku(std::ifstream& arch); 
    //usa el struct tValores
    void dame_valores_posibles(int f, int c, int lista[], int& n);

};