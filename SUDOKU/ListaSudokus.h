#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"
using namespace std;

class tListaSudokus {
private:
	tListaSudokus listaSudokus();
	
public:
	int dame_num_elems();
	const tReglas& dame_sudoku(int i);
	void insertar(const tReglas& sudoku);
	void eliminar(int pos); // elimina el elemento de la posición pos
	void mostrar_lista();

};