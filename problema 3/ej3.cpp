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
	int numero;
    int peso;
    pair<nodo,nodo> nodosQueConecta;
};

void agregarRuta(ruta& nuevaRuta, vector<ruta>& rutas, int f, int e1, int e2, int l, int nro){
    ruta nuevaRuta;
    nuevaRuta.numero=nro;
    nuevaRuta.peso=l;
    nuevaRuta.nodosQueConecta(make_pair(e1,e2));
    if (e1<=f){//fabrica-cliente
		nodos.[0].rutasDe.push_back(nuevaRuta);
		nodos.[e2-f].rutasDe.push_back(nuevaRuta);		
	}else{
	if (e2<=f){//cliente-fabrica
		nodos.[e1-f].rutasDe.push_back(nuevaRuta);
		nodos.[0].rutasDe.push_back(nuevaRuta);		
	}else{//cliente-cliente
		nodos.[e1-f].rutasDe.push_back(nuevaRuta);
		nodos.[e2-f].rutasDe.push_back(nuevaRuta);
	}
}

int minPeso(vector<ruta> rutas){
	//ruta res;
	int pesoMin= infinito;
	for (int i=0; i<rutas.size(); ++i){
		if (rutas[i].peso<pesoMin){
			res=i;
			pesoMin=rutas[i].peso;
		}
	}
	return res;
}

int main(){
	vector<nodo> nodos;//nodos totales: estan ORDENADOS de forma rara: [0]=fabricas; de 1 a c son los clientes
    vector<nodo> nodosAGM;//nodos que se van guardando en el AGM. Al final deberia tener los mismos elementos que el anterior
    vector<ruta> rutasTotales;//guarda el conjunto total de rutas
    vector<ruta> rutasAGM;//va guardando las rutas con menor peso que generan al AGM
    vector<ruta> rutasPosibles;//guarda las rutas que tienen un extremo en el AGM y el otro en Totales
    
    int f, c, r, e1, e2, l;
    cin>>f>>c>>r;
    nodo fabricas;
    nodos.push_back(fabricas);//el elemento 0 es el de las fabricas y el resto sigue la numeracion de c-f
    for (int i=f+1; i<=f+c; ++i){//para todos los clientes
        nodo nuevoNodo;			//crea un nuevo nodo
        nuevoNodo.ID=i;			//cuya ID es un numero entre f+1 y c+f
        nodos.push_back(nuevoNodo);//lo agrego al cjto de nodos (que esta ordenado por IDs)
    }
    assert(nodos.size()==c+1);
    
    cin>>e1>>e2>>l;
    int nro=0;//a cada ruta la identificamos con nro
    while(e1 != 0){
        if (e1>f || e2>f){//si ambos son menores no nos interesa hacer una ruta entre dos nodos con fabricas
			ruta nuevaRuta;
			++nro;
            agregarRuta(nuevaRuta, rutasTotales, f, e1, e2, l, nro);
        }
        cin>>e1>>e2>>l;
    }
    //assert(nro==f+c);
    
    for(int i=f+1; i<=f+c; ++i){//este es el ciclo posta del semiprim
        int numRuta = minPeso(rutasTotales);
        rutasAGM.push_back(rutasTotales[i]);
        ///vec.erase(vec.begin() + i); //elimina la posicion i, ie, el elemento i+1
        /*
         * tomar el e de minimo peso de rutas
         * agregar e a rutasAGM
         * agregar la nueva ciudad a nodosAGM?
         * eliminar de rutas las asociadas a la ciudad nueva con las ciudades viejas
         * 
         */
    }
    
    assert(nodos.size()==nodosAGM.size());
    assert();
    
	return 0;
}

