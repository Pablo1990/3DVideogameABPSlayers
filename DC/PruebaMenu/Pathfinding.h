#pragma once

#include "Position.h"
#include "NodoPathfinding.h"

#include <vector>
#include <iostream>

using namespace std;

#include "NodoPadreEHijo.h"

class Pathfinding  {
private:
	Position pIni;
	Position pFin;
	vector<Position> camino;

public:
	/* Constructores y destructor */
	Pathfinding(Position, Position);
	~Pathfinding(void);

	/*getters y setters*/
	Position getPosInicio();
	void setPosInicio(Position);

	Position getPosFin();
	void setPosFin(Position);

	vector<Position> getCamino();
	void setCamino(vector<Position>);

	/* functions */
	/** Calcula el mejor camino en funcion de pIni y pFin
	 * Return el vector de posiciones que sera el camino
	 * Tambien se guarda en la clase ese vector.
	 */
	vector<Position> AEstrella(Position);

	/** Imprime el camino que se ha calculado
	* Sirve para el modo debug
	*/
	void imprimirCamino();

	int calcularF(int, int);

	int calcularG(NodoPadreEHijo, NodoPadreEHijo);

	int calcularH(NodoPathfinding);

	NodoPadreEHijo menorF(vector<NodoPadreEHijo>);

	vector<Position> reconstruirCamino(NodoPadreEHijo n, int ***expandidos);

	vector<NodoPadreEHijo> crearHijos(NodoPadreEHijo n);
};