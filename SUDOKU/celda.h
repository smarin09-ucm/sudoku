#pragma once

enum tEstado { ORIGINAL, OCUPADA, VACIA };

class tCelda {
   private:
	   int valor;
	   tEstado estado;
   public:
	tCelda();
	/* métodos de consulta */
	bool es_vacia(); // devuelve true si y sólo si la celda es VACIA
	bool es_original(); // devuelve true si y sólo si la celda es ORIGINAL
	bool es_ocupada(); // devuelve true si y sólo si la celda es OCUPADA
	int dame_valor(); // devuelve el valor de la celda
	/* métodos modificadores*/
	void set_valor(int v); // pone el valor v en la celda
	void set_ocupada(); // pone el estado de la celda a OCUPADA
	void set_original(); // pone el estado de la celda a ORIGINAL
	void set_vacia(); // pone el estado de la celda a VACIA
};

