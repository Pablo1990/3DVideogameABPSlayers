#include "Pathfinding.h"


Pathfinding::Pathfinding(Position pIni, Position pFin){
	this->pIni = Position(pIni);
	this->pFin = Position(pFin);
}

/**
     * Calcula el A*
     * @return 0, en caso de que todo funcione correcto, -1 en otro caso
     */
    public int AEstrella() {
        //mundo tamaño máximo 20
        //Recorremos el mapa y lo sacamos por pantalla y llenamos de -1 el array expandidos
        for (int i = 0; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                expandidos[i][j] = -1;
                //System.out.print(mundo[i][j]);
            }
            //System.out.println();
        }
        //contador de nodos expandidos
        int cont = 0;
        //variable auxiliar
        int gprima;
        //Array para la Lista Interior
        ArrayList<Nodo> listaInterior = new ArrayList<Nodo>();
        //Array para la lista Frontera
        ArrayList<Nodo> listaFrontera = new ArrayList<Nodo>();
        //Array para los hijos de cada nodo.
        ArrayList<Nodo> hijosM = new ArrayList<Nodo>();
        //Inicializamos el primer nodo (origen), con padre = null
        Nodo n = new Nodo(0, -1, -1, null, origen, 1);
        //calculamos la h para el origen
        n.h = calcularH(n);
        //calculamos la f para el origen
        n.f = calcularF(n.g, n.h);
        //Y lo añadimos a la lista frontera
        listaFrontera.add(n);
        //Recorremos esta hasta que sea vacia
        while (!listaFrontera.isEmpty()) {
            //Buscamos el nodo con menor F es decir, el mejor (camino más corto)
            n = new Nodo(menorF(listaFrontera));
            //Nodo encontrado lo ponemos en expandidos
            expandidos[n.x][n.y] = cont;
            //Aumentamos el contador de expandidos
            cont++;
            //Lo eliminamos de listaFrontera
            listaFrontera.remove(n);
            //Y lo añadimos a nuestra listaInterior como fijo
            listaInterior.add(n);
            //En el caso de que este nodo sea estado solución
            if (n.getX() == destino && n.getY() == tamaño - 2) {
                //Sacamos por pantalla expandidos y el camino
                reconstruirCamino(n);
                //Y salimos con todo correcto
                return 0;
            }
            //Creamos los hijos posibles para el nodo actual n.
            hijosM = crearHijos(n);
            //Recorremos la lista de hijos
            for (int i = 0; i < hijosM.size(); i++) {
                //Comprobamos que no esté en lista interior
                if (!listaInterior.contains(hijosM.get(i))) {
                    //Obtemos el hijo i
                    Nodo m = new Nodo(hijosM.get(i));
                    //Calculamos su g, para ver si es mejor de lo que ya tenemos
                    gprima = n.g + calcularG(n, m);
                    //Si no está en lista frontera
                    if (!listaFrontera.contains(m)) {
                        //Lo añadimos
                        listaFrontera.add(m);
                    } else { //En caso contrario
                        //Obtenemos el nodo que está en listafrontera
                        Nodo aux = new Nodo(listaFrontera.get(listaFrontera.indexOf(m)));
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
 
    /**
     * Suma de g y h. Coste de camino mínimo desde el nodo
     * inicial, pasando por el actual haste un estado solución.
     * @param g
     * @param h
     * @return la suma de g y h
     */
    public int calcularF(int g, int h) {
        return g + h;
    }
 
    /**
     * Calcula coste del camino de coste mínimo desde el 
     * nodo inicial s al nodo n.
     * @param n
     * @param m
     * @return
     */
    public int calcularG(Nodo n, Nodo m) {
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
     * los caminos desde el nodo n a cualquier
     * estado solución tj
     * @param m
     * @return
     */
    public int calcularH(Nodo m) {
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
    public Nodo menorF(ArrayList<Nodo> listaFrontera) {
        //Cogemos el primer nodo de la lista
        Nodo n = new Nodo(listaFrontera.get(0));
        //y lo tamamos como referencia
        int menor = n.f;
        //Recorremos la lista
        for (int i = 1; i < listaFrontera.size(); i++) {
            //Cogemos el siguiente
            Nodo nuevo = new Nodo(listaFrontera.get(i));
            //Comprobamos que es mejor o igual que el que tenemos
            //ya que así tenderemos a coger los que sean  insertado últimos
            if (menor >= nuevo.f) {
                //Si es mejor que el que tenemos nos lo quedamos 
                n = nuevo;
                menor = nuevo.f;
            }
        }
        //Devolvemos el nodo con menor f
        return n;
    }