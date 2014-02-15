#include "NodoPathfinding.h"

NodoPathfinding::NodoPathfinding(void)
{
	padre = nullptr;
}

NodoPathfinding::~NodoPathfinding(void)
{
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
    NodoPathfinding::NodoPathfinding(int g, int f, int h, int x, int y) {
        this->g = g;
        this->f = f;
        this->h = h;
        this->x = x;
        this->y = y;
    }
 
    /**
     * Constructor de copia
     * @param n
     */
    NodoPathfinding::NodoPathfinding(NodoPathfinding& n) {
        this->g = n.g;
        this->x = n.x;
        this->y = n.y;
        this->f = n.f;
        this->h = n.h;
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
     * Devuelve la fila
     * @return
     */
    int NodoPathfinding::getX() {
        return x;
    }
 
    /**
     * Devuelve la columna
     * @return
     */
    int NodoPathfinding::getY() {
        return y;
    }
 
    /**
     * Modifica la g
     * @param g
     */
    void NodoPathfinding::setG(int g) {
        this->g = g;
    }
 
    /**
     * Modifica la fila, por una nueva
     * @param x
     */
    void NodoPathfinding::setX(int x) {
        this->x = x;
    }
 
    /**
     * Cambia la columna, por  una nueva
     * @param y
     */
    void NodoPathfinding::setY(int y) {
        this->y = y;
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
 
    /**
     * Comprueba si dos nodos son iguales
     * mirando las x e y
     * @param obj con el que compara
     * @return si son iguales
     */
	bool NodoPathfinding::operator==(NodoPathfinding& p) {
		return p.getPosition() == this->getPosition();
    }