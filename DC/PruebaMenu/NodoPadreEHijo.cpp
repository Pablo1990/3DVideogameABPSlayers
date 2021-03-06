#include "NodoPadreEHijo.h"

NodoPadreEHijo::NodoPadreEHijo(NodoPathfinding n, NodoPadreEHijo* padre)
{
	this->nodo = n;
	if(padre!=NULL){
		this->padre = new NodoPadreEHijo(padre);
	}
	else{
		this->padre = NULL;
	}
}



NodoPadreEHijo::NodoPadreEHijo(){
	this->padre = NULL;
}

NodoPadreEHijo::~NodoPadreEHijo(void)
{
	this->nodo.~NodoPathfinding();
	delete this->padre;
	this->padre = NULL;
}

NodoPadreEHijo::NodoPadreEHijo(NodoPadreEHijo* p){
	this->nodo = p->getNodo();
	if(p->getPadre()!=NULL){
		this->padre = new NodoPadreEHijo(p->getPadre());
	}
	else{
		this->padre = NULL;
	}
}

NodoPadreEHijo::NodoPadreEHijo(const NodoPadreEHijo& p){
	nodo = p.nodo;
	if(p.padre!=NULL){
		padre = new NodoPadreEHijo(p.padre);
	}
	else{
		padre = NULL;
	}
}

NodoPadreEHijo* NodoPadreEHijo::getPadre(){
	return this->padre;
}

NodoPathfinding NodoPadreEHijo::getNodo(){
	return this->nodo;
}

void NodoPadreEHijo::setNodo(NodoPathfinding n){
	this->nodo = n;
}

void NodoPadreEHijo::setPadre(NodoPadreEHijo* padre){
	if(this->padre)
	{
		delete this->padre;
		this->padre = NULL;
	}

	if(padre!=NULL){
		this->padre = new NodoPadreEHijo(padre);
	}
	else{
		this->padre = NULL;
	}
}

NodoPadreEHijo NodoPadreEHijo::operator=(NodoPadreEHijo* p){
	NodoPadreEHijo nodoo;
	nodoo.setNodo(p->getNodo());
	if(this->padre)
	{
		delete this->padre;
		this->padre = NULL;
	}

	if(p->getPadre()!=NULL){
		nodoo.setPadre(p->getPadre());
	}
	else{
		nodoo.setPadre(NULL);
	}
	return nodoo;
}

NodoPadreEHijo NodoPadreEHijo::operator=(NodoPadreEHijo& p){
	NodoPadreEHijo nodoo;

	if(this->padre)
	{
		delete this->padre;
		this->padre = NULL;
	}

	nodoo.setNodo(p.getNodo());
	if(p.getPadre()!=NULL){
		nodoo.setPadre(p.getPadre());
	}
	else{
		nodoo.setPadre(NULL);
	}
	return nodoo;
}
