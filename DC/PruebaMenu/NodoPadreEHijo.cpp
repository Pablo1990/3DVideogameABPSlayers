#include "NodoPadreEHijo.h"



NodoPadreEHijo::NodoPadreEHijo(NodoPathfinding n, NodoPadreEHijo* padre)
{
	this->nodo = n;
	if(padre!=NULL){
		this->padre = new NodoPadreEHijo(padre->getNodo(), padre->getPadre());
	}
	else{
		this->padre = null;
	}
}

NodoPadreEHijo::~NodoPadreEHijo(void)
{
	this->nodo.~NodoPathfinding();
	this->padre.~~NodoPadreEHijo();
}

NodoPathfinding* getPadre() const{
	return this->padre;
}

NodoPathfinding getNodo() const{
	return this->nodo;
}

void setNodo(NodoPathfinding n){
	this->nodo = n;
}

void setPadre(NodoPadreEHijo* padre){
	this->padre = padre;
}
