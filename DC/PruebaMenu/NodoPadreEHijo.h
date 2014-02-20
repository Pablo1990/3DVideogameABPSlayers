#pragma once

#include "NodoPathfinding.h"

class NodoPadreEHijo
{
private:
	NodoPathfinding nodo;
	NodoPadreEHijo *padre;
public:
	NodoPadreEHijo();
	NodoPadreEHijo(NodoPathfinding n, NodoPadreEHijo* padre);
	~NodoPadreEHijo(void);

	NodoPadreEHijo* getPadre() const;
	NodoPathfinding getNodo() const;

	void setNodo(NodoPathfinding n);
	void setPadre(NodoPadreEHijo* padre);

	NodoPadreEHijo operator=(NodoPadreEHijo* p);

};

