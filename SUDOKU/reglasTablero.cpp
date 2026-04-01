#include "reglasTablero.h"

using namespace std;

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
    return (cont == dim * dim) && !bloqueo();
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

tCelda tReglas::dame_celda_bloqueada(int p, int& f, int& c) const {
    if (p >= 0 && p < lista.contBloq) {
        f = lista.lista[p].f;
        c = lista.lista[p].c;
    }
    return tablero.dame_celda(f, c);
}

bool tReglas::es_valor_posible(int f, int c, int v)
{
    int dim = tablero.dame_dim();

    if (f < 0 || f >= dim || c < 0 || c >= dim)
        return false;

    tCelda celda = tablero.dame_celda(f, c);

    // Verificar si la celda ya tiene un numero
    if (!celda.es_vacia()) {
        return false;
    }

    // Verificar fila
    for (int j = 0; j < dim; j++) {
        if (tablero.dame_celda(f, j).dame_valor() == v) {
            return false;
        }
    }

    // Verificar columna
    for (int i = 0; i < dim; i++) {
        if (tablero.dame_celda(i, c).dame_valor() == v) {
            return false;
        }
    }

    // Verificar regiones
    int n = 1;
    while (n * n < dim) {
        n++;
    }
    int inicioF = (f / n) * n;
    int inicioC = (c / n) * n;

    for (int i = inicioF; i < inicioF + n; i++) {
        for (int j = inicioC; j < inicioC + n; j++) {
            if (tablero.dame_celda(i, j).dame_valor() == v) {
                return false;
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
    if (!celda.es_vacia()) {
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
    if (es_valor_posible(f, c, v)) {
        tCelda celda = tablero.dame_celda(f, c);
        celda.set_valor(v);
        celda.set_ocupada();
        tablero.set_celda(f, c, celda);
        cont++;
        actualiza_bloqueos();
    }
}

void tReglas::quita_valor(int f, int c)
{
    tCelda celda = tablero.dame_celda(f, c);

    if (celda.es_ocupada() && !celda.es_original()) {
        celda.set_vacia();
        celda.set_valor(0);
        tablero.set_celda(f, c, celda);
        cont--;
        actualiza_bloqueos();
    }
}

void tReglas::actualiza_bloqueos() {
    lista.contBloq = 0;
    int dim = tablero.dame_dim();
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (tablero.dame_celda(i, j).es_vacia() && posibles_valores(i, j) == 0) {
                lista.lista[lista.contBloq].f = i;
                lista.lista[lista.contBloq].c = j;
                lista.contBloq++;
            }
        }
    }
}

void tReglas::reset()
{
   tablero = tableroOriginal;
   cont = 0;
   int dim = tablero.dame_dim();

   for (int i = 0; i < dim; i++) {
       for (int j = 0; j < dim; j++) {
           if (tablero.dame_celda(i, j).es_original()) {
               cont++;
           }
       }
   }
   actualiza_bloqueos();
}

void tReglas::autocompletar()
{
    int dim = tablero.dame_dim();
    bool cambios = true;

    while (cambios) {
        cambios = false;

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                // Solo procesar celdas vacías
                if (tablero.dame_celda(i, j).es_vacia()) {
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
    tablero = tTablero(d);
    cont = 0;
    tCelda celda;
  

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int valor;
            archivo >> valor;

            if (valor == 0) {
                celda.set_valor(0);
                celda.set_vacia();
            }
            else {
                celda.set_valor(valor);
                celda.set_original();
                cont++; 
            }

            tablero.set_celda(i, j, celda);
        }
    }

    // Guardar una copia del tablero original
    tableroOriginal = tablero;
    actualiza_bloqueos();
}