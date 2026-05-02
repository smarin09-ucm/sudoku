#include "listaSudokus.h"
#include <iostream>
#include <fstream>
using namespace std;

const int CAPACIDAD_INICIAL = 4;

//GESTION DE MEMORIA
tListaSudokus::tListaSudokus()
{
    numElems = 0;
    capacidad = CAPACIDAD_INICIAL;
    sudokus = new tReglas * [capacidad];
    for (int i = 0; i < capacidad; i++)
    {
        sudokus[i] = nullptr;
    }
        
}

tListaSudokus::tListaSudokus(const tListaSudokus& l)
{
    numElems = l.numElems;
    capacidad = l.capacidad;
    sudokus = new tReglas * [capacidad];
    for (int i = 0; i < numElems; i++)
        sudokus[i] = new tReglas(*l.sudokus[i]);
    for (int i = numElems; i < capacidad; i++)
        sudokus[i] = nullptr;
}

tListaSudokus& tListaSudokus::operator=(const tListaSudokus& l)
{
    if (this != &l)
    {
        for (int i = 0; i < numElems; i++)
        {
            delete sudokus[i];
            sudokus[i] = nullptr;
        }
        delete[] sudokus;

        numElems = l.numElems;
        capacidad = l.capacidad;
        sudokus = new tReglas * [capacidad];
        for (int i = 0; i < numElems; i++)
            sudokus[i] = new tReglas(*l.sudokus[i]);
        for (int i = numElems; i < capacidad; i++)
            sudokus[i] = nullptr;
    }
    return *this;
}

tListaSudokus::~tListaSudokus()
{
    for (int i = 0; i < numElems; i++)
    {
        delete sudokus[i];
        sudokus[i] = nullptr;
    }
    delete[] sudokus;
    sudokus = nullptr;
}

void tListaSudokus::redimensionar()
{
    int nuevaCapacidad = capacidad * 2;
    tReglas** nuevos = new tReglas * [nuevaCapacidad];
    for (int i = 0; i < numElems; i++)
        nuevos[i] = sudokus[i];
    for (int i = numElems; i < nuevaCapacidad; i++)
        nuevos[i] = nullptr;
    delete[] sudokus;
    sudokus = nuevos;
    capacidad = nuevaCapacidad;
}

//CONSULTORES
int tListaSudokus::dame_num_elems()
{
    return numElems;
}

const tReglas& tListaSudokus::dame_sudoku(int i)
{
    return *sudokus[i];
}

tReglas& tListaSudokus::operator[](int i)
{
    return *sudokus[i];
}

//ORDENACION POR DIFICULTAD
void tListaSudokus::insertar( tReglas sudoku)
{
    if (numElems == capacidad) redimensionar();

    int pos = numElems;

    for (int i = 0; i < numElems; i++)
    {
        if (sudoku < *sudokus[i])
        {
            pos = i;
            break;
        }
    }

    for (int i = numElems; i > pos; i--)
        sudokus[i] = sudokus[i - 1];

    sudokus[pos] = new tReglas(sudoku);
    numElems++;
}

void tListaSudokus::eliminar(int pos)
{
    if (pos < 0 || pos >= numElems) return;
    delete sudokus[pos];
    // desplazar a la izquierda
    for (int i = pos; i < numElems - 1; i++)
        sudokus[i] = sudokus[i + 1];
    sudokus[numElems - 1] = nullptr;
    numElems--;
}

//MOSTRAR
void tListaSudokus::mostrar_lista()
{
    int dim = (numElems > 0) ? sudokus[0]->dame_dimension() : 9;
    for (int i = 0; i < numElems; i++)
    {
        cout << i + 1 << ": Sudoku con " << sudokus[i]->dame_num_celdas_vacias()
            << " casillas vacias\n";
        for (int n = 1; n <= dim; n++)
            cout << "   celdas con " << n << " valores posibles: "
            << sudokus[i]->dame_num_celdas_con_n_posibles(n) << "\n";
    }
}

//CARGA Y GUARDADO
//CARGA Y GUARDADO

void tListaSudokus::carga_lista_sudokus(const string& nombreArchivo)
{
    ifstream arch(nombreArchivo);

    if (!arch.is_open())
    {
        cout << "Error abriendo archivo: " << nombreArchivo << endl;
        return;
    }

    int n;
    arch >> n;

    for (int i = 0; i < n; i++)
    {
        string nombre;
        arch >> nombre;

        ifstream archSudoku(nombre);

        if (!archSudoku.is_open())
        {
            cout << "ERROR: no se pudo abrir el sudoku: " << nombre << endl;
        }
        else
        {
            tReglas r;
            r.carga_sudoku(archSudoku);
            insertar(r);
            archSudoku.close();
        }
    }

    arch.close();
}


void tListaSudokus::carga_lista_partidas(const string& nombreArchivo)
{
    ifstream arch(nombreArchivo);

    if (!arch.is_open())
    {
        cout << "Error abriendo archivo: " << nombreArchivo << endl;
        return;
    }

    int n;
    arch >> n;

    for (int i = 0; i < n; i++)
    {
        tReglas r;

        // Carga el sudoku original de la partida
        r.carga_sudoku(arch);

        // Lee las jugadas guardadas hasta encontrar -1
        int f;
        arch >> f;

        while (f != -1)
        {
            int c, v;
            arch >> c >> v;

            r.pon_valor(f, c, v);

            arch >> f;
        }

        insertar(r);
    }

    arch.close();
}


void tListaSudokus::guarda_lista_partidas(const string& nombreArchivo)
{
    ofstream arch(nombreArchivo);

    if (!arch.is_open())
    {
        cout << "Error abriendo archivo: " << nombreArchivo << endl;
        return;
    }

    arch << numElems << "\n";

    for (int i = 0; i < numElems; i++)
    {
        sudokus[i]->guarda_partida(arch);
    }

    arch.close();
}

