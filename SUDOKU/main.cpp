#include <iostream>
#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"


using namespace std;
int main() {

	tReglas reglas;
	ifstream archivo("C:/Users/sarma/Downloads/sudoku_1");
	reglas.carga_sudoku(archivo);
	int opcion = 0;
	cout << "1.- poner número" << '\n';
	cout << "2.- quitar número" << '\n';
	cout << "3.- reset" << '\n';
	cout << "4.- posibles valores de celda vacia" << '\n';
	cout << "5.- autocompletar celdas con valor único" << '\n';
	cout << "6.- salir" << '\n';
	cout << "Elige una opción: " << '\n';
	int f = 0;
	int c = 0;
	switch (opcion) {
	case 1:
		cout << "Fila y columna entre 1...9: ";
		cin >> f >> c;
		cout << "Valor: ";
		int valor;
		cin >> valor;
		cout << "Sudoku bloqueado....Las bloqueadas son: ";
		reglas.pon_valor(f, c, valor);
		//tenemos que imprimir las celdas bloqueadas

	case 2:
		cout << "Fila y columna entre 1...9: ";
		cin >> f >> c;
		reglas.quita_valor(f, c);

	case 3:
		reglas.reset();

	case 4:
		//imprimir los posibles valores

	case 5:
		reglas.autocompletar();

	case 6:
		return 0;
	}

	return 0;
}