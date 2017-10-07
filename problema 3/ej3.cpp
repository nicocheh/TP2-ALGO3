#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define infinito 32767

using namespace std;

struct nodo{
    int ID;
    vector<ruta> rutasDe;
};
struct ruta{
	int numero;		//numero esta entre [0, r)
    int peso;		//peso>0
    pair<nodo,nodo> nodosQueConecta;
};

void agregarRuta(ruta nuevaRuta, vector<ruta>& rutasTotales, vector<ruta>& rutasPosibles, int f, int e1, int e2){
	rutasTotales.push_back(nuevaRuta);
    if (e1<=f){//fabrica-cliente
		nodos.[0].rutasDe.push_back(nuevaRuta);
		nodos.[e2-f].rutasDe.push_back(nuevaRuta);
		rutasPosibles.push_back(nuevaRuta);								//ya lo agrega en los 1ros posibles
	}else{
	if (e2<=f){//cliente-fabrica
		nodos.[e1-f].rutasDe.push_back(nuevaRuta);
		nodos.[0].rutasDe.push_back(nuevaRuta);		
		rutasPosibles.push_back(nuevaRuta);								//ya lo agrega en los 1ros posibles
	}else{//cliente-cliente
		nodos.[e1-f].rutasDe.push_back(nuevaRuta);
		nodos.[e2-f].rutasDe.push_back(nuevaRuta);
	}
}

int minPeso(vector<ruta> rutasPosibles){//devuelve el numero de la ruta con menos peso
	int res;
	int pesoMin= infinito;
	for (int i=0; i<rutas.size(); ++i){
		if (rutas[i].peso<pesoMin){
			res=rutas[i].numero;
			pesoMin=rutas[i].peso;
		}
	}
	return res;
}

bool estaEnAGM(vector<nodo> nodosAGM, nodo ciudad){
    return find(nodosAGM.begin(), nodosAGM.end(), ciudad) != nodosAGM.end();
}

void eliminarRutas(ruta rutaMinima, vector<ruta>& rutasPosibles, bool agreguePrimera){ //eliminar de rutasPosibles las asociadas a la ciudad nueva con las ciudades viejas
    if (agreguePrimera){
        for (int j=0; j<rutaMinima.nodosQueConecta.fst.rutasDe.size(); ++j){//recorre las rutas que salen de la nueva ciudad que agregamos
            if (estaEnAGM(nodosAGM, rutaMinima.nodosQueConecta.fst.rutasDe[j].nodosQueConecta.fst) && estaEnAGM(nodosAGM, rutaMinima.nodosQueConecta.fst.rutasDe[j].nodosQueConecta.snd)){
            //si la j-esima ruta que sale del nuevo nodo que agregamos une dos nodos que estan en el AGM
                //entonces borramos esa ruta de las rutasPosibles
                rutasPosibles.erase(rutasPosibles.begin()+j-1);
            }
        }
    }else{
        for (int j=0; j<rutaMinima.nodosQueConecta.snd.rutasDe.size(); ++j){
            if (estaEnAGM(nodosAGM, rutaMinima.nodosQueConecta.snd.rutasDe[j].nodosQueConecta.fst) && estaEnAGM(nodosAGM, rutaMinima.nodosQueConecta.snd.rutasDe[j].nodosQueConecta.snd)){
            //si la j-esima ruta que sale del nuevo nodo que agregamos une dos nodos que estan en el AGM
                //entonces borramos esa ruta de las rutasPosibles
                rutasPosibles.erase(rutasPosibles.begin()+j-1);
            }
        }        
    }
}










int main(){
int f, c, r;
cin>>f>>c>>r;
while(f != 0){
	vector<nodo> nodos;//nodos totales: estan ORDENADOS de forma rara: [0]=fabricas; de 1 a c son los clientes
    //ORDENADO por ID-f de nodo
    vector<nodo> nodosAGM;//nodos que se van guardando en el AGM. Al final deberia tener los mismos elementos que el anterior
    //MODIFICABLE
    vector<ruta> rutasTotales;//guarda el conjunto total de rutas
    //ORDENADO por numero de ruta
    vector<ruta> rutasAGM;//va guardando las rutas con menor peso que generan al AGM
    //MODIFICABLE
    vector<ruta> rutasPosibles;//guarda las rutas que tienen un extremo en el AGM y el otro en Totales
    //VARIABLE todo el tiempo
    
    int e1, e2, l;
    int costoTotal=0;

    nodo fabricas;
    nodos.push_back(fabricas);				//el elemento 0 es el de las fabricas y el resto sigue la numeracion de c-f
    for (int i=f+1; i<=f+c; ++i){										//para todos los clientes
        nodo nuevoNodo;													//crea un nuevo nodo
        nuevoNodo.ID=i;													//cuya ID es un numero entre f+1 y c+f
        nodos.push_back(nuevoNodo);										//lo agrego al cjto de nodos (que esta ordenado)
    }
    assert(nodos.size()==c+1);
    
    cin>>e1>>e2>>l;
    for (int i=0; i<r; ++i){//inserta las rutas en rutasTotales
        if (e1>f || e2>f){//si ambos son menores (ie, conecta solo fabricas) no nos interesa hacer una ruta entre dos nodos con fabricas
			ruta nuevaRuta;
			nuevaRuta.numero=i;                     //a cada ruta la identificamos con nro (0<=numero<r)
			nuevaRuta.peso=l;
			nuevaRuta.nodosQueConecta(make_pair(e1,e2));
            agregarRuta(nuevaRuta, rutasTotales, rutasPosibles, f, e1, e2);
        }
        cin>>e1>>e2>>l;
    }
    
    for(int i=f+1; i<=f+c; ++i){//este es el ciclo posta del casi prim
        int numRuta = minPeso(rutasTotales);
        rutasAGM.push_back(rutasTotales[numRuta]);
        costoTotal+=rutasTotales[numRuta].peso;
        
        bool agreguePrimera;
        if (estaEnAGM(nodosAGM, rutasTotales[numRuta].nodosQueConecta.fst)){
        //si la primera ciudad de la ruta es la que esta en el agm
            //entonces que agregue la segunda ciudad al AGM
            nodosAGM.push_back(rutasTotales[numRuta].nodosQueConecta.snd);
            agreguePrimera=false;
        }else{//si la segunda ciudad de la ruta es la que esta en el agm
            //entonces que agregue la primera ciudad al AGM
            nodosAGM.push_back(rutasTotales[numRuta].nodosQueConecta.fst);
            agreguePrimera=true;
        }
        eliminarRutas(rutasTotales[numRuta], rutasPosibles, agreguePrimera);
        /*
         * tomar el e de minimo peso de rutas
         * agregar e a rutasAGM
         * agregar la nueva ciudad a nodosAGM?
         * eliminar de rutasPosibles las asociadas a la ciudad nueva con las ciudades viejas
         * 
         */
    }
    
    assert(nodos.size()==nodosAGM.size());

    cout<<costoTotal<<" "<<rutasAGM.size();
    for (int k=0; k<rutasAGM.size(); ++k){
        cout<<" "<<rutasAGM[k].nodosQueConecta.fst<<" "<<rutasAGM[k].nodosQueConecta.snd;
    }
    cout<<endl;
cin>>f>>c>>r;
}

return 0;
}

