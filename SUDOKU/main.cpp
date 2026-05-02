#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <string>
#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"
#include "colors.h"
#include "checkML.h"
#include "ListaSudokus.h"
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

//RESOLVER SUDOKU RECURSIVAMENTE
bool resolver_sudoku(tReglas& sudoku, int fila, int columna)
{
    int dim = sudoku.dame_dimension();

    // avanzar a la siguiente celda vacia
    while (fila < dim && !sudoku.dame_celda(fila, columna).es_vacia())
    {
        columna++;
        if (columna == dim) { columna = 0; fila++; }
    }

    if (fila == dim) return true;  // todas rellenas

    for (int v = 1; v <= dim; v++)
    {
        if (sudoku.es_valor_posible(fila, columna, v))
        {
            sudoku.pon_valor(fila, columna, v);
            int sigFila = fila, sigCol = columna + 1;
            if (sigCol == dim) { sigCol = 0; sigFila++; }
            if (resolver_sudoku(sudoku, sigFila, sigCol)) return true;
            sudoku.quita_valor(fila, columna);
        }
    }
    return false;
}

//JUGAR A UN SUDOKU
void jugar(tReglas& juego)
{
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
        cout << "6.- resolver sudoku\n";
        cout << "7.- salir\n";
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
                juego.pon_valor(f - 1, c - 1, v);
            else
                cout << RED << "No se puede poner ese valor" << RESET << "\n";
            mostrar_bloqueos(juego);
            break;
        case 2:
            cout << "Fila y columna entre 1..." << juego.dame_dimension() << ": ";
            cin >> f >> c;
            if (juego.dame_celda(f - 1, c - 1).es_vacia() || juego.dame_celda(f - 1, c - 1).es_original())
                cout << RED << "No se puede quitar ese valor" << RESET << "\n";
            else
                juego.quita_valor(f - 1, c - 1);
            break;
        case 3:
            juego.reset();
            break;
        case 4:
        {
            cout << "Fila y columna entre 1..." << juego.dame_dimension() << ": ";
            cin >> f >> c;
            if (!juego.dame_celda(f - 1, c - 1).es_vacia())
                cout << RED << "La celda no esta vacia" << RESET << "\n";
            else
            {
                int dim = juego.dame_dimension();
                cout << "Los valores posibles para la celda son: { ";
                for (int i = 1; i <= dim; i++)
                    if (juego.es_valor_posible(f - 1, c - 1, i))
                        cout << i << " ";
                cout << "}\n";
            }
            break;
        }
        case 5:
            juego.autocompletar();
            mostrar_bloqueos(juego);
            break;
        case 6:
            if (resolver_sudoku(juego, 0, 0))
                cout << GREEN << "Sudoku resuelto!\n" << RESET;
            else
                cout << RED << "No tiene solucion.\n" << RESET;
            mostrar_tablero(juego);
            salir = true;
            break;
        case 7:
            salir = true;
            break;
        default:
            cout << "Opcion no valida\n";
        }
    }
    if (juego.terminado())
    {
        cout << "\n" << GREEN << "Sudoku completado!" << RESET << "\n";
        mostrar_tablero(juego);
    }
}

//MAIN V2
int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Cargar lista de partidas y lista de sudokus originales
    tListaSudokus partidas;
    tListaSudokus originales;
    originales.carga_lista_sudokus("lista_sudokus.txt");
    partidas.carga_lista_partidas("lista_partidas.txt");
    
    

    bool app_salir = false;

    while (!app_salir)
    {
        char modo;
        cout << "\nPartida nueva (N), continuar partida (C) o abandonar la aplicacion (A)? ";
        cin >> modo;
        modo = toupper(modo);

        if (modo == 'A')
        {
            app_salir = true;
        }
        else if (modo == 'C' && partidas.dame_num_elems() == 0)
        {
            cout << "No hay partidas empezadas. Se mostraran los sudokus disponibles.\n";
            modo = 'N';
        }

        if (!app_salir)
        {
            tListaSudokus* lista = (modo == 'C') ? &partidas : &originales;
            cout << lista->dame_num_elems();
            if (lista->dame_num_elems() == 0)
            {
                cout << "No hay sudokus disponibles.\n";
                continue;
            }

            // Mostrar lista y pedir eleccion
            lista->mostrar_lista();
            cout << "Elige un sudoku: ";
            int eleccion;
            cin >> eleccion;
            eleccion--;  // pasar a indice 0

            if (eleccion < 0 || eleccion >= lista->dame_num_elems())
            {
                cout << "Opcion no valida.\n";
                continue;
            }

            // Ver o jugar
            cout << "1.- Visualizar sudoku\n2.- Jugar al sudoku\nElige: ";
            int op;
            cin >> op;

            if (op == 1){         
                mostrar_tablero((*lista)[eleccion]);
            }
            else if (op == 2)
            {
                // Hacer copia para jugar
                tReglas juego = (*lista)[eleccion];
                bool era_partida = (modo == 'C');

                jugar(juego);

                if (juego.terminado())
                {
                    // Si era partida empezada, borrarla
                    if (era_partida)
                        partidas.eliminar(eleccion);
                    // Si era nueva, no se hace nada
                }
                else
                {
                    // Se abandono con salir
                    if (era_partida)
                    {
                        // Eliminar la antigua y reinsertar actualizada
                        partidas.eliminar(eleccion);
                        partidas.insertar(juego);
                    }
                    else
                    {
                        // Era nueva: guardar en partidas
                        partidas.insertar(juego);
                    }
                }
            }
        }
    }

    // Guardar lista de partidas al salir
    partidas.guarda_lista_partidas("lista_partidas.txt");

    cout << "Hasta luego!\n";
    return 0;
}

