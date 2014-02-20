#include "NodoPadreEHijo.h"

NodoPadreEHijo::NodoPadreEHijo(NodoPathfinding n, NodoPadreEHijo* padre)
{
	this->nodo = n;
	if(padre!=NULL){
		this->padre = new NodoPadreEHijo(padre->getNodo(), padre->getPadre());
	}
	else{
		this->padre = NULL;
	}
}

NodoPadreEHijo::~NodoPadreEHijo(void)
{
	this->nodo.~NodoPathfinding();
	delete this->padre;
	this->padre = NULL;
}

NodoPadreEHijo* NodoPadreEHijo::getPadre() const{
	return this->padre;
}

NodoPathfinding NodoPadreEHijo::getNodo() const{
	return this->nodo;
}

void NodoPadreEHijo::setNodo(NodoPathfinding n){
	this->nodo = n;
}

void NodoPadreEHijo::setPadre(NodoPadreEHijo* padre){
	if(padre!=NULL){
		this->padre = new NodoPadreEHijo(padre->getNodo(), padre->getPadre());
	}
	else{
		this->padre = NULL;
	}
}

NodoPadreEHijo NodoPadreEHijo::operator=(NodoPadreEHijo* p){
	this->nodo = p->getNodo();
	if(padre!=NULL){
		this->padre = new NodoPadreEHijo(padre->getNodo(), padre->getPadre());
	}
	else{
		this->padre = NULL;
	}
}
