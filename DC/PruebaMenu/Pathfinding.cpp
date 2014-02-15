#include "Pathfinding.h"

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
vector<Position> Pathfinding::AEstrella(){
	vector<Position> expandidos;
        //Recorremos el mapa y lo sacamos por pantalla y llenamos de -1 el array expandidos
        for (int i = 0; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                expandidos[i][j] = -1;
                //System.out.print(mundo[i][j]);
            }
            //System.out.println();
        }
        //contador de NodoPathfindings expandidos
        int cont = 0;
        //variable auxiliar
        int gprima;
        //Array para la Lista Interior
		vector<NodoPathfinding, NodoPathfinding> listaInterior;
        //Array para la lista Frontera
        vector<NodoPathfinding> listaFrontera;
        //Array para los hijos de cada NodoPathfinding.
        vector<NodoPathfinding> hijosM;
        //Inicializamos el primer NodoPathfinding (origen), con padre = null
        //NodoPathfinding n(0, -1, -1, null, origen, 1);
        //calculamos la h para el origen
		n.setH(calcularH(n));
        //calculamos la f para el origen
		n.setF(calcularF(n.g, n.h));
        //Y lo añadimos a la lista frontera
        listaFrontera.add(n);
        //Recorremos esta hasta que sea vacia
        while (!listaFrontera.isEmpty()) {
            //Buscamos el NodoPathfinding con menor F es decir, el mejor (camino más corto)
            n = new NodoPathfinding(menorF(listaFrontera));
            //NodoPathfinding encontrado lo ponemos en expandidos
            expandidos[n.x][n.y] = cont;
            //Aumentamos el contador de expandidos
            cont++;
            //Lo eliminamos de listaFrontera
            listaFrontera.remove(n);
            //Y lo añadimos a nuestra listaInterior como fijo
            listaInterior.add(n);
            //En el caso de que este NodoPathfinding sea estado solución
            if (n.getX() == destino && n.getY() == tamaño - 2) {
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
                if (!listaInterior.contains(hijosM.get(i))) {
                    //Obtemos el hijo i
                    NodoPathfinding m = new NodoPathfinding(hijosM.get(i));
                    //Calculamos su g, para ver si es mejor de lo que ya tenemos
                    gprima = n.g + calcularG(n, m);
                    //Si no está en lista frontera
                    if (!listaFrontera.contains(m)) {
                        //Lo añadimos
                        listaFrontera.add(m);
                    } else { //En caso contrario
                        //Obtenemos el NodoPathfinding que está en listafrontera
                        NodoPathfinding aux = new NodoPathfinding(listaFrontera.get(listaFrontera.indexOf(m)));
                        //Comprobamos que gprima es mejor que aux.g
                        if (aux.g > gprima) //si es g'(m) mejor m.g
                        {
                            //Si es mejor metemos sus características en el de la listafrontera
                            aux.padre = n;
                            aux.h = calcularH(m);
                            aux.g = calcularG(n, m);
                            aux.f = calcularF(m.g, m.h);
                        }
                    }
                }
            }
        }
        //Si no ha encontrado solución
        return -1;
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
    int Pathfinding::calcularG(NodoPathfinding n, NodoPathfinding m) {
        int g = 0;
        if (m.padre == null) {
            g = 1;
        } else {
            g = n.g + 1;
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
        int y = (tamaño - 2) - m.y;
		int x = destino - m.x;
        x = Math.abs(x);
        y = Math.abs(y);
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
    NodoPathfinding Pathfinding::menorF(vector<NodoPathfinding> listaFrontera) {
        //Cogemos el primer NodoPathfinding de la lista
        NodoPathfinding n = new NodoPathfinding(listaFrontera.get(0));
        //y lo tamamos como referencia
        int menor = n.f;
        //Recorremos la lista
        for (int i = 1; i < listaFrontera.size(); i++) {
            //Cogemos el siguiente
            NodoPathfinding nuevo = new NodoPathfinding(listaFrontera.get(i));
            //Comprobamos que es mejor o igual que el que tenemos
            //ya que así tenderemos a coger los que sean  insertado últimos
            if (menor >= nuevo.f) {
                //Si es mejor que el que tenemos nos lo quedamos 
                n = nuevo;
                menor = nuevo.f;
            }
        }
        //Devolvemos el NodoPathfinding con menor f
        return n;
    }