#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"
#include "checkML.h"
using namespace std;

class tListaSudokus {
private:
	tReglas** sudokus;   // array dinamico de punteros a tReglas
	int numElems;
	int capacidad;

	void redimensionar();
	
public:
	tListaSudokus();
	tListaSudokus(const tListaSudokus& l);
	tListaSudokus& operator=(const tListaSudokus& l);
	~tListaSudokus();

	int dame_num_elems();
	const tReglas& dame_sudoku(int i);
	tReglas& operator[](int i);

	void insertar(tReglas sudoku);
	void eliminar(int pos); // elimina el elemento de la posición pos
	void mostrar_lista();

	void carga_lista_sudokus(const std::string& nombreArchivo);
	void carga_lista_partidas(const std::string& nombreArchivo);
	void guarda_lista_partidas(const std::string& nombreArchivo);

};