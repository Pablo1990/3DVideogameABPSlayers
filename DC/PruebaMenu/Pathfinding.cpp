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

int*** Pathfinding::getMapa(){
	return this->mapa;
}

/* obstaculo[0] == finaldelmapa
 * obstaculo[1-N] == obstaculos con posiciones
 */
void Pathfinding::setMapa(vector<vector<Position>> obstaculos){
	int maxX = obstaculos[0][0].getX();
	int maxY = obstaculos[0][0].getY();
	int maxZ = obstaculos[0][0].getZ();
	finMapa.setX(maxX);
	finMapa.setY(maxY);
	finMapa.setZ(maxZ);
	
	mapa = new int **[maxX];
        //Recorremos el mapa y lo sacamos por pantalla y llenamos de -1 el array expandidos
	for (int i = 0; i < maxX; i++) {
		mapa[i] = new int *[maxZ];
		for (int j = 0; j < maxZ; j++) {
			mapa[i][j] = new int[2];
			mapa[i][j][0] = 0; //se puede
			mapa[i][j][1] = -1; //no puede
        }
    }

	for (int k = 1; k < obstaculos.size(); k++)
	{
		for(int x = obstaculos[k][0].getX(); x < obstaculos[k][1].getX(); x++){
			for(int z = obstaculos[k][0].getZ(); z < obstaculos[k][1].getZ(); z++){
				for(int y = obstaculos[k][0].getY(); y < obstaculos[k][1].getY(); y++){
					mapa[x][z][y] = -1;
					mapa[x][z][y+1] = 0;
				}
			}
		}
	}
}

/* functions */
/** Calcula el mejor camino en funcion de pIni y pFin
 * Return el vector de posiciones que sera el camino
 * Tambien se guarda en la clase ese vector.
 */
vector<Position> Pathfinding::AEstrella(float pasos){ //250 por default

	int minX = pIni.getX()-250 <= 0 ? 0 : pIni.getX()-250;
	int minZ = pIni.getZ()-250 <= 0 ? 0 : pIni.getZ()-250;

	int maxX = pIni.getX()+250 > finMapa.getX() ? finMapa.getX() : pIni.getX()+250;
	int maxZ = pIni.getZ()+250 > finMapa.getZ() ? finMapa.getZ() : pIni.getZ()+250;
	
	int*** expandidos = new int **[maxX];
        //Recorremos el mapa y lo sacamos por pantalla y llenamos de -1 el array expandidos
	for (int i = minX; i < maxX; i++) {
		expandidos[i] = new int *[maxZ];
		for (int j = minZ; j < maxZ; j++) {
			expandidos[i][j] = new int[1];
			expandidos[i][j][0] = -1;
        }
    }
	//HABRA QUE RELLENAR DE -10 O ALGO ASI PARA QUE SE SEPA QUE HAY OBSTACULOS O SIMPLEMENTE QUE SON INALCANZABLES
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
			int x = n.getNodo().getPosition().getX();
			int z = n.getNodo().getPosition().getZ();
			int y = n.getNodo().getPosition().getY();
			expandidos[x][z][y] = cont;
            //Aumentamos el contador de expandidos
            cont++;
            //Lo eliminamos de listaFrontera
			for(std::vector<NodoPadreEHijo>::iterator it = listaFrontera.begin(); it != listaFrontera.end(); ++it) {
				if(it->getNodo() == n.getNodo())
				{
					listaFrontera.erase(it);
					break;
				}
			}
            //Y lo añadimos a nuestra listaInterior como fijo
			listaInterior.push_back(n);
            //En el caso de que este NodoPathfinding sea estado solución
			if (n.getNodo().getG() >= pasos || ( n.getNodo().getPosition().getX() == pFin.getX() && n.getNodo().getPosition().getY() == pFin.getY() && n.getNodo().getPosition().getZ() == pFin.getZ())) {
                //Sacamos por pantalla expandidos y el camino
                return reconstruirCamino(n, expandidos);
            }
            //Creamos los hijos posibles para el NodoPathfinding actual n.
            hijosM = crearHijos(n,minX, minZ, maxX, maxZ);
            //Recorremos la lista de hijos
            for (int i = 0; i < hijosM.size(); i++) {
                //Comprobamos que no esté en lista interior
				bool encon = false;
				for(std::vector<NodoPadreEHijo>::iterator it = listaInterior.begin(); it != listaInterior.end(); ++it) {
					if(it->getNodo() == hijosM[i].getNodo())
					{
						encon = true;
						break;
					}
				}
				if (!encon) {
                    //Obtemos el hijo i
					NodoPadreEHijo m(hijosM[i]);
                    //Calculamos su g, para ver si es mejor de lo que ya tenemos
					gprima = n.getNodo().getG() + calcularG(n, m);
                    //Si no está en lista frontera
					bool encon = false;
					for(std::vector<NodoPadreEHijo>::iterator it = listaFrontera.begin(); it != listaFrontera.end(); ++it) {
						if(it->getNodo() == m.getNodo())
						{
							encon = true;
							break;
						}
					}
					if(!encon) {
						/* v contains x */
                        //Lo añadimos
						listaFrontera.push_back(m);
                    } else { //En caso contrario
						
						NodoPadreEHijo np;
                        //Obtenemos el NodoPathfinding que está en listafrontera
						for(std::vector<NodoPadreEHijo>::iterator it = listaFrontera.begin(); it != listaFrontera.end(); ++it) {
							if(it->getNodo() == n.getNodo())
							{
								np.setNodo(it->getNodo());
								np.setPadre(it->getPadre());
								break;
							}
						}
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
	NodoPadreEHijo Pathfinding::menorF(vector<NodoPadreEHijo> listaFrontera) {
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

	vector<NodoPadreEHijo> Pathfinding::crearHijos(NodoPadreEHijo n, int minX, int minZ, int maxX, int maxZ) {
        //Array de hijos
        vector<NodoPadreEHijo> hijosM;
		NodoPadreEHijo hijo;
        //comprobamos todas las posibilidades, como máximo puede tener 4 hijos.
        //Derecha, comprobamos que esté vacío y que no nos salgamos del mapa
		int x = n.getNodo().getPosition().getX();
		int z = n.getNodo().getPosition().getZ();
		int y = n.getNodo().getPosition().getY();
		if (mapa[x+1][z][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x+1,y,z));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
        //arriba, comprobamos que esté vacío y que no nos salgamos del mapa
        if (mapa[x+1][z+1][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x+1,y,z+1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x+1][z-1][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x+1,y,z-1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
        //Abajo, comprobamos que esté vacío y que no nos salgamos del mapa
        if (mapa[x-1][z][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x-1,y,z));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
        //Izquierda, comprobamos que esté vacío y que no nos salgamos del mapa
        if (mapa[x-1][z+1][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x-1,y,z+1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x-1][z-1][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x-1,y,z-1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x][z+1][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x,y,z+1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x][z-1][y] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x,y,z-1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		/** Saltos **/
		if (mapa[x+1][z][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x+1,y+1,z));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
        //arriba, comprobamos que esté vacío y que no nos salgamos del mapa
        if (mapa[x+1][z+1][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x+1,y+1,z+1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x+1][z-1][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x+1,y+1,z-1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
        //Abajo, comprobamos que esté vacío y que no nos salgamos del mapa
        if (mapa[x-1][z][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x-1,y+1,z));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
        //Izquierda, comprobamos que esté vacío y que no nos salgamos del mapa
        if (mapa[x-1][z+1][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x-1,y+1,z+1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x-1][z-1][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x-1,y+1,z-1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x][z+1][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x,y+1,z+1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
		if (mapa[x][z-1][y+1] == 0) {
			NodoPathfinding nodoHijo(0,0,0, Position(x,y+1,z-1));
			hijo.setNodo(nodoHijo);
			hijo.setPadre(new NodoPadreEHijo(n));
			//PUEDE PETAAAAAAAAARRRRRR
            //hijo = new Nodo(0, 0, 0, n, n.x, n.y + 1);
			hijo.getNodo().setG(calcularG(n,hijo));
            //hijo.g = calcularG(n, hijo);
			hijo.getNodo().setH(calcularH(hijo.getNodo()));
            //hijo.h = calcularH(hijo);
			hijo.getNodo().setF(calcularF(hijo.getNodo().getG(), hijo.getNodo().getH()));
            //hijo.f = calcularF(hijo.g, hijo.h);
            //hijo.padre = n;
			hijosM.push_back(hijo);
        }
 
        return hijosM;
    }

	/**
     * Sacamos por pantalla el camino al que hemos llegado y los nodos por los
     * que hemos pasado
     * @param n nodo solución
     */
	vector<Position> Pathfinding::reconstruirCamino(NodoPadreEHijo n, int ***expandidos) {
		vector<Position> camino;
		NodoPadreEHijo m(n);
        //Nodo solución
        //Mientras no lleguemos al hijo origen
		while (!(m.getPadre() == NULL)) {
			camino.push_back(m.getNodo().getPosition());
            //Cogemos el padre y lo que convertimos en el actual
			m = m.getPadre();
        }
		return camino;
    }
     