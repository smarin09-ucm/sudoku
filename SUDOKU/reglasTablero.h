#pragma once

#include <string>
#include <fstream>

#include "celda.h"
#include "tablero.h"

using namespace std;

struct tListaBLoq {
	int contBloq;
	tCelda array[MAX][MAX];
};

class tReglas {
private:
	tTablero tablero;
	int cont;
	tListaBLoq lista;
public:
	/* consultoras */
	int dame_dimension(); // devuelve la dimensión del tablero
	tCelda dame_celda(int f, int c); // devuelve la celda en la posición (f,c)
	bool terminado(); // true si y sólo si el Sudoku está resuelto
	bool bloqueo(); // true si el Sudoku tiene celdas bloqueadas
	int dame_num_celdas_bloqueadas(); // devuelve el número de celdas bloqueadas
	int dame_num_celdas_vacias(); // devuelve el número de celdas vacías
	tCelda dame_celda_bloqueada(int p, int& f, int& c); // devuelve en (f,c) la celda bloqueada en la posición p
	bool es_valor_posible(f, c, v); // true si y sólo si v se puede colocar en (f,c)
	int posibles_valores(f, c); // devuelve el número de posibles valores para (f,c)
	/* modificadoras */
	void pon_valor(f, c, v); // pone v en (f,c)
	void quita_valor(f, c); // pone la celda (f,c) a VACIA
	void reset(); // recupera el Sudoku original
	void autocompletar(); // rellena todas las celdas con un único valor posible
	/* inicializadora */
	void carga_sudoku(ifstream& arch); // carga un Sudoku original de un archivo
};