#include "Pathfinding.h"
#include <iostream>
#include <math.h>

using namespace std;


Pathfinding::Pathfinding(Position pIni, Position pFin){
	this->pIni = Position(pIni);
	this->pFin = Position(pFin);
	this->camino.clear();
}

Pathfinding::~Pathfinding(){
	this->camino.~vector();
}

/*getters y setters*/
Position Pathfinding::getPosInicio(){
	return this->pIni;
}

void Pathfinding::setPosInicio(Position p){
	this->pIni.setX(p.getX());
	this->pIni.setY(p.getY());
	this->pIni.setZ(p.getZ());
}

Position Pathfinding::getPosFin(){
	return this->pFin;
}

void Pathfinding::setPosFin(Position p){
	this->pFin.setX(p.getX());
	this->pFin.setY(p.getY());
	this->pFin.setZ(p.getZ());
}

vector<Position> Pathfinding::getCamino(){
	return this->camino;
}

void Pathfinding::setCamino(vector<Position> c){
	this->camino.clear();
	this->camino = c;
}

/* functions */
/** Calcula el mejor camino en funcion de pIni y pFin
 * Return el vector de posiciones que sera el camino
 * Tambien se guarda en la clase ese vector.
 */
vector<Position> Pathfinding::AEstrella(Position mapa){
	const int x = mapa.getX();
	const int y = mapa.getY();
	const int z = mapa.getZ();
	int expandidos[x][y][z];
        //Recorremos el mapa y lo sacamos por pantalla y llenamos de -1 el array expandidos
	for (int i = 0; i < mapa.getX(); i++) {
		for (int j = 0; j < mapa.getZ(); j++) {
			expandidos[i][j][0] = -1;
                //System.out.print(mundo[i][j]);
            }
            //System.out.println();
        }
        //contador de NodoPathfindings expandidos
        int cont = 0;
        //variable auxiliar
        int gprima;
        //Array para la Lista Interior
		vector<NodoPadreEHijo> listaInterior;
        //Array para la lista Frontera
		vector<NodoPadreEHijo> listaFrontera;
        //Array para los hijos de cada NodoPathfinding.
		vector<NodoPadreEHijo> hijosM;
        //Inicializamos el primer NodoPathfinding (origen), con padre = null
        NodoPathfinding nActual(0, -1, -1, pIni);
		NodoPadreEHijo n(nActual, NULL);
        //calculamos la h para el origen
		n.getNodo().setH(calcularH(n.getNodo()));
        //calculamos la f para el origen
		n.getNodo().setF(calcularF(n.getNodo().getG(), n.getNodo().getH()));
        //Y lo añadimos a la lista frontera
		listaFrontera.push_back(n);
        //Recorremos esta hasta que sea vacia
		while (!listaFrontera.empty()) {
            //Buscamos el NodoPathfinding con menor F es decir, el mejor (camino más corto)
			NodoPadreEHijo n(menorF(listaFrontera));
            //NodoPathfinding encontrado lo ponemos en expandidos
			expandidos[n.getNodo().getPosition().getX()][n.getNodo().getPosition().getY()][n.getNodo().getPosition().getZ()] = cont;
            //Aumentamos el contador de expandidos
            cont++;
            //Lo eliminamos de listaFrontera
			listaFrontera.erase(n);
            //Y lo añadimos a nuestra listaInterior como fijo
			listaInterior.push_back(n);
            //En el caso de que este NodoPathfinding sea estado solución
			if (n.getNodo().getPosition().getX() == pFin.getX() && n.getNodo().getPosition().getY() == pFin.getY() && n.getNodo().getPosition().getZ() == pFin.getZ()) {
                //Sacamos por pantalla expandidos y el camino
                reconstruirCamino(n);
                //Y salimos con todo correcto
                return 0;
            }
            //Creamos los hijos posibles para el NodoPathfinding actual n.
            hijosM = crearHijos(n);
            //Recorremos la lista de hijos
            for (int i = 0; i < hijosM.size(); i++) {
                //Comprobamos que no esté en lista interior
				if (!(std::find(listaInterior.begin(), listaInterior.end(),hijosM[i]) != listaInterior.end())) {
                    //Obtemos el hijo i
					NodoPadreEHijo m(hijosM[i]);
                    //Calculamos su g, para ver si es mejor de lo que ya tenemos
					gprima = n.getNodo().getG() + calcularG(n, m);
                    //Si no está en lista frontera
					if(!(std::find(listaFrontera.begin(), listaFrontera.end(), m) != listaFrontera.end())) {
						/* v contains x */
                        //Lo añadimos
						listaFrontera.push_back(m);
                    } else { //En caso contrario
                        //Obtenemos el NodoPathfinding que está en listafrontera
						NodoPadreEHijo np(std::find(listaFrontera.begin(), listaFrontera.end(), m));
						NodoPathfinding aux = np.getNodo();
                        //Comprobamos que gprima es mejor que aux.g
						if (aux.getG() > gprima) //si es g'(m) mejor m.g
                        {
                            //Si es mejor metemos sus características en el de la listafrontera
							np.setPadre(new NodoPadreEHijo(n));
							aux.setH(calcularH(m.getNodo()));
                            aux.setG(calcularG(n, m));
							aux.setF(calcularF(m.getNodo().getG(), m.getNodo().getH()));
                        }
                    }
                }
            }
        }
		vector<Position> p;
        //Si no ha encontrado solución
		return p;
}

/** Imprime el camino que se ha calculado
* Sirve para el modo debug
*/
void Pathfinding::imprimirCamino(){
	vector<Position>::iterator it;
	cout<<"Camino encontrado: "<<endl;
	for(it = camino.begin(); it!=camino.end(); it++)
	{
		cout<<(*it)<<endl;
	}
}
 
    /**
     * Suma de g y h. Coste de camino mínimo desde el NodoPathfinding
     * inicial, pasando por el actual haste un estado solución.
     * @param g
     * @param h
     * @return la suma de g y h
     */
    int Pathfinding::calcularF(int g, int h) {
        return g + h;
    }
 
    /**
     * Calcula coste del camino de coste mínimo desde el 
     * NodoPathfinding inicial s al NodoPathfinding n.
     * @param n
     * @param m
     * @return
     */
	int Pathfinding::calcularG(NodoPadreEHijo n, NodoPadreEHijo m) {
        int g = 0;
		if (m.getPadre() == NULL) {
            g = 1;
        } else {
			g = n.getNodo().getG() + 1;
        }
 
 
        return g;
    }
 
    /**
     * Coste del camino de coste mínimo de todos
     * los caminos desde el NodoPathfinding n a cualquier
     * estado solución tj
     * @param m
     * @return
     */
	int Pathfinding::calcularH(NodoPathfinding m) {
        int h = 0;
        //Distancía Euclídea
        //h = (int) Math.sqrt(Math.pow(((tamaño - 2) - m.y), 2) + Math.pow(destino - m.x, 2));
        //Distancía Manhattan
		int y = pFin.getY() - m.getPosition().getY();
		int x = pFin.getX() - m.getPosition().getX();
        x = abs(x);
        y = abs(y);
        h = x + y;
 
        //Algoritmo voraz creado por mí
       /* int x = m.getX();
        int y = m.getY();
        boolean arriba = true; //si es false es que hay pared arriba
        boolean abajo = true; //si es false es que hay pared abajo
        while(x != destino || y != tamaño-2)
        {
        if(mundo[x][y+1]==0)
        {
        y++;
        abajo= true;
        arriba = true;
        }
        else if(arriba && mundo[x-1][y]==0)
        {
        x--;
        }
        else if(abajo && mundo[x+1][y]==0)
        {
        arriba = false;
        x++;
        }
        else
        {
        y--;
        }
        h++;
        }*/
 
        return h;
    }
 
    /**
     * Recorremos la list frontera y buscamos el que tenga menor f
     * @param listaFrontera
     * @return
     */
	NodoPadreEHijo menorF(vector<NodoPadreEHijo> listaFrontera) {
        //Cogemos el primer NodoPathfinding de la lista
		NodoPadreEHijo nPadreEHijo(listaFrontera[0]);
		NodoPathfinding n = nPadreEHijo.getNodo();
        //y lo tamamos como referencia
		int menor = n.getF();
        //Recorremos la lista
        for (int i = 1; i < listaFrontera.size(); i++) {
            //Cogemos el siguiente
			NodoPadreEHijo nuevoPadreEHijo(listaFrontera[i]);
			NodoPathfinding nuevo = nuevoPadreEHijo.getNodo();
            //Comprobamos que es mejor o igual que el que tenemos
            //ya que así tenderemos a coger los que sean  insertado últimos
			if (menor >= nuevo.getF()) {
                //Si es mejor que el que tenemos nos lo quedamos 
                n = nuevo;
				menor = nuevo.getF();
				nPadreEHijo.setNodo(n);
				nPadreEHijo.setPadre(new NodoPadreEHijo(nuevoPadreEHijo));
            }
        }
        //Devolvemos el NodoPathfinding con menor f
		return nPadreEHijo;
    }

	ArrayList<Nodo> crearHijos(Nodo n) {
        //Array de hijos
        ArrayList<Nodo> hijosM = new ArrayList<Nodo>();
        Nodo hijo;
        //comprobamos todas las posibilidades, como máximo puede tener 4 hijos.
        //Derecha, comprobamos que esté vacío y que no nos salgamos del mapa
        if (n.y + 1 < tamaño && mundo[n.x][n.y + 1] == 0) {
            hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
            hijo.g = calcularG(n, hijo);
            hijo.h = calcularH(hijo);
            hijo.f = calcularF(hijo.g, hijo.h);
            hijo.padre = n;
            hijosM.add(hijo);
        }
        //arriba, comprobamos que esté vacío y que no nos salgamos del mapa
        if (n.x - 1 >= 0 && mundo[n.x - 1][n.y] == 0) {
            hijo = new Nodo(0, 0, 0, n, n.x - 1, n.y);
            hijo.g = calcularG(n, hijo);
            hijo.h = calcularH(hijo);
            hijo.f = calcularF(hijo.g, hijo.h);
            hijo.padre = n;
            hijosM.add(hijo);
        }
        //Abajo, comprobamos que esté vacío y que no nos salgamos del mapa
        if (n.x + 1 < tamaño && mundo[n.x + 1][n.y] == 0) {
            hijo = new Nodo(0, 0, 0, n, n.x + 1, n.y);
            hijo.g = calcularG(n, hijo);
            hijo.h = calcularH(hijo);
            hijo.f = calcularF(hijo.g, hijo.h);
            hijo.padre = n;
            hijosM.add(hijo);
        }
        //Izquierda, comprobamos que esté vacío y que no nos salgamos del mapa
        if (n.y - 1 >= 0 && mundo[n.x][n.y - 1] == 0) {
            hijo = new Nodo(0, 0, 0, n, n.x, n.y - 1);
            hijo.g = calcularG(n, hijo);
            hijo.h = calcularH(hijo);
            hijo.f = calcularF(hijo.g, hijo.h);
            hijo.padre = n;
            hijosM.add(hijo);
        }
 
        return hijosM;
    }

	/**
     * Sacamos por pantalla el camino al que hemos llegado y los nodos por los
     * que hemos pasado
     * @param n nodo solución
     */
	vector<Position> reconstruirCamino(NodoPadreEHijo n) {
		char camino[100][100][100];
		NodoPadreEHijo m(n);
        //Nodo solución
		camino[m.getNodo().getPosition().getX()][m.getNodo().getPosition().getY()][m.getNodo().getPosition().getZ()] = 'X';
        System.out.println("Camino: ");
        //Mientras no lleguemos al hijo origen
		while (!(m.getPadre() == NodoPathfinding())) {
            //Cogemos el padre y lo que convertimos en el actual
            m = m.);
            //Mentemos en el array las coordenadas 
            camino[m.x][m.y] = 'X';
        }
        //Recorremos el array y mostramos por pantalla la solución al problema
        for (int i = 0; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                if (camino[i][j] == 'X') {
                    System.out.print('X');
                } else {
                    System.out.print('.');
                }
            }
            System.out.println();
        }
        //Recorremos expandidos y mostramos los nodos.
        System.out.println("Expandidos:");
        for (int i = 0; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                if (expandidos[i][j] >= 0 && expandidos[i][j] < 10) {
                    System.out.print(" ");
                }
                System.out.print(expandidos[i][j] + " ");
            }
            System.out.println();
        }
 
    }
     