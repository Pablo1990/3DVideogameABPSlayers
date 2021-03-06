#include "NodoPathfinding.h"

NodoPathfinding::NodoPathfinding(void)
{
}

NodoPathfinding::~NodoPathfinding(void)
{
	this->pos.~Position();
}

/**
 * Constructor del NodoPathfiding
 * @param g 
 * @param f 
 * @param h 
 * @param padre
 * @param x
 * @param y
*/
    NodoPathfinding::NodoPathfinding(int g, int f, int h, Position p) {
        this->g = g;
        this->f = f;
        this->h = h;
		this->pos = p;
    }
 
    /**
     * Constructor de copia
     * @param n
     */
    NodoPathfinding::NodoPathfinding(NodoPathfinding& n) {
        this->g = n.g;
        this->f = n.f;
        this->h = n.h;
		this->pos = n.pos;
    }
 
    /**
     * @return int f, devuelve la f
     */
    int NodoPathfinding::getF() {
        return f;
    }
 
    /**
     * 
     * @return inf h, devuelve la h
     */
    int NodoPathfinding::getH() {
        return h;
    }
 
    /**
     * Devuelve la g
     * @return int g
     */
    int NodoPathfinding::getG() {
        return g;
    }
 
    /**
     * Modifica la g
     * @param g
     */
    void NodoPathfinding::setG(int g) {
        this->g = g;
    }
 
    /**
     * Modifica la f
     * @param f
     */
    void NodoPathfinding::setF(int f) {
        this->f = f;
    }
 
    /**
     * Modifica la h
     * @param h
     */
    void NodoPathfinding::setH(int h) {
        this->h = h;
    }

	Position NodoPathfinding::getPosition(){
		return this->pos;
	}
 
    /**
     * Comprueba si dos nodos son iguales
     * mirando las x e y
     * @param obj con el que compara
     * @return si son iguales
     */
	bool NodoPathfinding::operator==(NodoPathfinding& p) {
		return p.getPosition() == this->getPosition();
    }