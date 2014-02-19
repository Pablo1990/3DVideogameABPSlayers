#pragma once

#include "NodoPathfinding.h"

class NodoPadreEHijo
{
private:
	NodoPathfinding nodo;
	NodoPadreEHijo *padre;
public:
	NodoPadreEHijo();
	NodoPadreEHijo(NodoPathfinding n, NodoPathfinding* padre);
	~NodoPadreEHijo(void);

	NodoPathfinding* getPadre() const;
	NodoPathfinding getNodo() const;

	void setNodo(NodoPathfinding n);
	void setPadre(NodoPadreEHijo* padre);

};

