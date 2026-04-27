
#include <iostream>
#include <fstream>
#include <string>
#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"
#include "colors.h"
using namespace std;


void mostrar_tablero(tReglas& juego) 
{
    int dim = juego.dame_dimension();
    int n = 1;
    while (n * n < dim) n++;

    //columnas
    cout << "    ";
    for (int j = 0; j < dim; j++) 
    {
        if (j > 0 && j % n == 0) cout << "  ";
        cout << j + 1 << " ";
    }
    cout << "\n";

    for (int i = 0; i < dim; i++) 
    {
        if (i > 0 && i % n == 0) 
        {
            cout << "    ";
            for (int j = 0; j < dim; j++) 
            {
                if (j > 0 && j % n == 0) cout << "--";
                cout << "--";
            }
            cout << "\n";
        }
        cout << " " << i + 1 << " |";
        for (int j = 0; j < dim; j++)
        {
            if (j > 0 && j % n == 0) cout << "|";
            tCelda cel = juego.dame_celda(i, j);
            int v = cel.dame_valor();
            if (v == 0) 
            {
                cout << GRAY << " ." << RESET;
            }
            else if (cel.es_original()) 
            {
                cout << ORANGE << " " << v << RESET;
            }
            else
            {
                cout << " " << v;
            }
        }
        cout << "|\n";
    }
}

void mostrar_bloqueos(tReglas& juego)
{
    if (juego.bloqueo()) 
    {
        int f, c;
        cout << RED << "Sudoku bloqueado.....Las casillas bloqueadas son: ";
        for (int p = 0; p < juego.dame_num_celdas_bloqueadas(); p++)
        {
            juego.dame_celda_bloqueada(p, f, c);
            cout << "(" << f + 1 << "," << c + 1 << ") ";
        }
        cout << RESET << "\n";
    }
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    tReglas juego;
    ifstream archivo;
    string nombre;
    cout << "Introduce el nombre del archivo: ";
    cin >> nombre;
    archivo.open(nombre);
    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo\n";
        return 0;
    }
    juego.carga_sudoku(archivo);
    archivo.close();

    int opcion = 0;
    bool salir = false;

    while (!salir && !juego.terminado()) 
    {
        cout << "\nSUDOKU\n";
        mostrar_tablero(juego);
        mostrar_bloqueos(juego);
        cout << "\n1.- poner numero\n";
        cout << "2.- quitar numero\n";
        cout << "3.- reset\n";
        cout << "4.- posibles valores de una celda vacia\n";
        cout << "5.- autocompletar celdas con valor unico\n";
        cout << "6.- salir\n";
        cout << "Elige una opcion: ";
        cin >> opcion;

        int f, c, v;
        switch (opcion) 
        {
        case 1:
            cout << "Fila y columna entre 1..." << juego.dame_dimension() << ": ";
            cin >> f >> c;
            cout << "Valor: ";
            cin >> v;
            if (juego.es_valor_posible(f - 1, c - 1, v))
            {
                juego.pon_valor(f - 1, c - 1, v);
            }
            else 
            {
                cout << RED << "No se puede poner ese valor" << RESET << "\n";
            }
            mostrar_bloqueos(juego);
            break;
        case 2:
            cout << "Fila y columna entre 1..." << juego.dame_dimension() << ": ";
            cin >> f >> c;
            if (juego.dame_celda(f - 1, c - 1).es_vacia() || juego.dame_celda(f - 1, c - 1).es_original()) {
                cout << RED << "No se puede quitar ese valor" << RESET << "\n";
            }
            else {
                juego.quita_valor(f - 1, c - 1);
            }
            break;
        case 3:
            juego.reset();
            break;
        case 4:
            {
            cout << "Fila y columna entre 1..." << juego.dame_dimension() << ": ";
            cin >> f >> c;
            if (!juego.dame_celda(f - 1, c - 1).es_vacia()) {
                cout << RED << "La celda no esta vacia" << RESET << "\n";
            }
            else 
            {
                int vals[MAX_DIM];
                int cont = 0;
                int n = juego.posibles_valores(f,c);
                cout << "Los valores posibles para la celda son: { ";
                for (int i = 0; i < 10 && cont < n; i++) {
                    if (juego.es_valor_posible(f-1, c-1, i)) {
                        cout << i << " ";
                        cont++;
                    }
                }
                cout << "}\n";
            }
            break;
        }
        case 5:
            juego.autocompletar();
            mostrar_bloqueos(juego);
            break;
        case 6:
            salir = true;
            break;
        default:
            cout << "Opcion no valida\n";
        }
    }

    if (juego.terminado())
    {
        cout << "\n¡Sudoku completado!\n";
        mostrar_tablero(juego);
    }
    return 0;
}