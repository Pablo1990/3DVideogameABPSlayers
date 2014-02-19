#pragma once

#include "NodoPathfinding.h"

class NodoPadreEHijo
{
private:
	NodoPathfinding nodo;
	NodoPathfinding padre;
public:
	NodoPadreEHijo();
	NodoPadreEHijo(NodoPathfinding n, NodoPathfinding padre);
	~NodoPadreEHijo(void);

	NodoPathfinding getPadre() const;
	NodoPathfinding getNodo() const;

	void setNodo(NodoPathfinding n);
	void setPadre(NodoPathfinding padre);

};

