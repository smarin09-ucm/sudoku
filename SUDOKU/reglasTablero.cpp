#include <string>
#include <fstream>
#include <iostream>

#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"


using namespace std;


tTablero tableroOriginal;

tReglas::tReglas() {
    cont = 0;
    lista.contBloq = 0;
}

int tReglas::dame_dimension()
{
    return tablero.dame_dim();
}

tCelda tReglas::dame_celda(int f, int c)
{
    return tablero.dame_celda(f, c);
}

//te dice si has terminado el sudoku o no
bool tReglas::terminado() {
    int dim = tablero.dame_dim();
    return (cont == dim * dim);
}

//funcion en proceso
bool tReglas::bloqueo() {
    return lista.contBloq > 0;
}

int tReglas::dame_num_celdas_bloqueadas() {
    return lista.contBloq;
}

int tReglas::dame_num_celdas_vacias() {
    int dim = tablero.dame_dim();
    return dim * dim - cont;
}

//POSICION P?? SOLO UN VALOR???
/*tCelda tReglas::dame_celda_bloqueada(int p, int& f, int& c) {
    tCelda celda;
    if (p >= 0 && p < lista.contBloq) {
        f = lista.array[p].f;
        c = lista.array[p].c;
        celda = tablero.dame_celda(f, c);
    }
    return celda;
}*/

bool tReglas::es_valor_posible(int f, int c, int v)
{
    int dim = tablero.dame_dim();

    // Verificar si la celda está bloqueada
    tCelda celda = tablero.dame_celda(f, c);
    if (celda.es_ocupada()) {
        return false;
    }

    // Verificar fila
    for (int j = 0; j < dim; j++) {
        if (j != c && celda.dame_valor() == v) {
            return false;
        }
    }

    // Verificar columna
    for (int i = 0; i < dim; i++) {
        if (i != f &&celda.dame_valor() == v) {
            return false;
        }
    }

    // Verificar región 3x3 (para Sudoku estándar de 9x9)
    if (dim == 9) {
        int inicioFila = (f / 3) * 3;
        int inicioCol = (c / 3) * 3;

        for (int i = inicioFila; i < inicioFila + 3; i++) {
            for (int j = inicioCol; j < inicioCol + 3; j++) {
                if (i != f && j != c && celda.dame_valor() == v) {
                    return false;
                }
            }
        }
    }

    return true;
}

int tReglas::posibles_valores(int f, int c)
{
    int dim = tablero.dame_dim();
    int contador = 0;

    // Si la celda está bloqueada o no está vacía, no tiene valores posibles
    tCelda celda = tablero.dame_celda(f, c);
    if (celda.es_ocupada() || !celda.es_vacia()) {
        return 0;
    }

    // Probar todos los valores posibles del 1 al 9 (para Sudoku estándar)
    for (int v = 1; v <= dim; v++) {
        if (es_valor_posible(f, c, v)) {
            contador++;
        }
    }

    return contador;
}

void tReglas::pon_valor(int f, int c, int v)
{
    tCelda celda = tablero.dame_celda(f, c);

    if (!celda.es_ocupada() && v >= 1 && v <= tablero.dame_dim()) {
        tablero.set_valor(f, c, v);
    }
}

void tReglas::quita_valor(int f, int c)
{
    tCelda celda;
    if (celda.es_ocupada()) {
        celda.set_vacia();
        celda.set_valor = "";
        tablero.set_celda(f, c, celda);
    }
}

void tReglas::reset()
{
   tablero = tableroOriginal;
}

void tReglas::autocompletar()
{
    int dim = tablero.dame_dim();
    bool cambios = true;

    while (cambios) {
        cambios = false;

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                // Solo procesar celdas vacías y no bloqueadas
                tCelda celda = tablero.dame_celda(i, j);
                if (celda.es_vacia() && !celda.es_ocupada()) {
                    int numPosibles = 0;
                    int valorUnico = -1;

                    // Encontrar el único valor posible
                    for (int v = 1; v <= dim; v++) {
                        if (es_valor_posible(i, j, v)) {
                            numPosibles++;
                            valorUnico = v;
                        }
                    }

                    // Si solo hay un valor posible, colocarlo
                    if (numPosibles == 1) {
                        pon_valor(i, j, valorUnico);
                        cambios = true;
                    }
                }
            }
        }
    }
}

void tReglas::carga_sudoku(ifstream& archivo)
{
    int dim;
    archivo >> dim;

    if (archivo.is_open()) { // 2. Verificar apertura
        // Cargar los valores del archivo
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int valor;
                archivo >> valor;
                tCelda celda;
                if (valor >= 1 && valor <= dim) {
                    celda.set_valor(valor);
                    celda.set_ocupada();
                    tablero.set_celda(i, j, celda);
                }
                else {
                    celda.set_vacia();
                }
            }
        }
        archivo.close(); // 4. Cerrar archivo
    }
    else {
        cout << "Error al abrir el archivo";
    }

    // Guardar una copia del tablero original
    tableroOriginal = tablero;
}