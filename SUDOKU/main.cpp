#include <iostream>
#include <fstream>
#include <string>
#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"


using namespace std;

void mostrar_tablero(tReglas& juego) {
	int dim = juego.dame_dimension();
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			int v = juego.dame_celda(i, j).dame_valor();
			if (v == 0) cout << ". ";
			else cout << v << " ";
		}
		cout << '\n';
	}
}
int main() {
	tReglas juego;
	ifstream archivo;
	string nombre;

	cout << "Introduce el nombre del archivo" << '\n';
	cin >> nombre;

	archivo.open(nombre);

	if (!archivo.is_open()) {
		cout << "Error al abrir el archivo" << '\n';
		return 0;
	}

	juego.carga_sudoku(archivo);

	int opcion = 0;
	bool salir = false;

	while (!salir && !juego.terminado()) {
		cout << "SUDOKU" << '\n';
		mostrar_tablero(juego);

		cout << "1. Poner numero\n";
		cout << "2. Quitar numero\n";
		cout << "3. Reiniciar\n";
		cout << "4. Posibles valores\n";
		cout << "5. Autocompletar\n";
		cout << "6. Salir\n";
		cout << "Opcion: ";
		cin >> opcion;

		int f, c, v;

		switch (opcion) {
		case 1:
			cout << "Introduce fila, columna y valor:";
			cin >> f >> c >> v;
			if (juego.es_valor_posible(f-1, c-1, v)) {
				juego.pon_valor(f-1, c-1, v);
			}
			else cout << "No se puede poner ese valor";
			break;

		case 2:
			cout << "Introduce fila y columna:";
			cin >> f >> c;
			juego.quita_valor(f-1, c-1);
			break;

		case 3:
			juego.reset();
			break;

		case 4:
			cout << "Introduce fila y columna:";
			cin >> f >> c;
			cout << "Posibles valores son:" << juego.posibles_valores(f-1, c-1) << '\n';
			break;

		case 5:
			juego.autocompletar();
			break;

		case 6:
			salir = true;
			break;

		default:
			cout << "Opcion no valida" << '\n';
		}
	}

	if (juego.terminado()) {
		cout << "Sudoku competado" << '\n';
		mostrar_tablero(juego);
	}

	return 0;
}