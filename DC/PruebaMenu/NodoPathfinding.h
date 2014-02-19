#pragma once

#include "Position.h"

class NodoPathfinding
{
private:
	/* Propiedad G: Coste del camino de coste mínimo desde el 
     * nodo inicial s al nodo n.
     */
    int g;
    /* Propiedad f:Suma de g y h. Coste de camino mínimo desde el nodo
     * inicial, pasando por el actual haste un estado solución.
     */
    int f;
    /* Propiedad h: Coste del camino de coste mínimo de todos
     * los caminos desde el nodo n a cualquier
     * estado solución tj
     */
    int h;

	Position pos;
public:
	NodoPathfinding(void);
	~NodoPathfinding(void);
 
    /**
     * Constructor del NodoPathfinding
     * @param g 
     * @param f 
     * @param h
     * @param x
     * @param y
     */
    NodoPathfinding(int g, int f, int h, Position);
 
    /**
     * Constructor de copia
     * @param n
     */
    NodoPathfinding(NodoPathfinding& n);
 
    /**
     * @return int f, devuelve la f
     */
     int getF();
 
    /**
     * 
     * @return inf h, devuelve la h
     */
     int getH();
 
    /**
     * Devuelve la g
     * @return int g
     */
     int getG();
 
    /**
     * Modifica la g
     * @param g
     */
     void setG(int g);
 
    /**
     * Modifica la f
     * @param f
     */
     void setF(int f);
 
    /**
     * Modifica la h
     * @param h
     */
     void setH(int h);

	 void setPosition(Position);

	 Position getPosition();
 
    /**
     * Comprueba si dos nodos son iguales
     * mirando las x e y
     * @param obj con el que compara
     * @return si son iguales
     */
	 bool operator==(NodoPathfinding&);


};

