#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define infinito 32767
#define fst first
#define snd second

using namespace std;

struct nodo{
    int ID;
    vector<int> rutasDe;
};
struct ruta{
	int numero;		//numero esta entre [0, r)
    int peso;		//peso>0
    pair<nodo,nodo> nodosQueConecta;
};

void agregarRuta(ruta nuevaRuta, vector<nodo> nodos, vector<ruta>& rutasTotales, vector<ruta>& rutasPosibles, int f, int e1, int e2){
	cout<<"entro a agregarRuta"<<endl;
	rutasTotales.push_back(nuevaRuta);
    if (e1<=f){//fabrica-cliente
		cout<<"fabrica-cliente"<<endl;
		nodos[0].rutasDe.push_back(nuevaRuta.numero);
		cout<<"nodos[0]"<<endl;
		nodos[e2-f].rutasDe.push_back(nuevaRuta.numero);
		cout<<"nodos[e2-f]"<<endl;
		rutasPosibles.push_back(nuevaRuta);							//ya lo agrega en los 1ros posibles
		cout<<"agrega a rP"<<endl;
		nuevaRuta.nodosQueConecta = make_pair(nodos[0],nodos[e2-f]);
		cout<<"make pair"<<endl;
	}else{
	if (e2<=f){//cliente-fabrica
		cout<<"cliente-fabrica"<<endl;
		nodos[e1-f].rutasDe.push_back(nuevaRuta.numero);
		nodos[0].rutasDe.push_back(nuevaRuta.numero);		
		rutasPosibles.push_back(nuevaRuta);								//ya lo agrega en los 1ros posibles
		nuevaRuta.nodosQueConecta = make_pair(nodos[e1-f],nodos[0]);
	}else{//cliente-cliente
		cout<<"cliente-cliente"<<endl;
		nodos[e1-f].rutasDe.push_back(nuevaRuta.numero);
		nodos[e2-f].rutasDe.push_back(nuevaRuta.numero);
		nuevaRuta.nodosQueConecta = make_pair(nodos[e1-f],nodos[e2-f]);
	}
}
}

int minPeso(vector<ruta> rutasPosibles){//devuelve el numero de la ruta con menos peso
	int res;
	int pesoMin= infinito;
	for (int i=0; i<rutasPosibles.size(); ++i){
		if (rutasPosibles[i].peso<pesoMin){
			res=rutasPosibles[i].numero;
			pesoMin=rutasPosibles[i].peso;
		}
	}
	return res;
}

bool estaEnAGM(vector<nodo> nodosAGM, nodo ciudad){
	bool res=false;
	
	int i=0; 
	while (i<nodosAGM.size() && not res){
		if (ciudad.ID==nodosAGM[i].ID) res=true;
		++i;
	}
	return res;
}

void eliminarRutas(vector<ruta> rutasTotales, vector<nodo> nodosAGM, ruta rutaMinima, vector<ruta>& rutasPosibles, bool agreguePrimera){ //eliminar de rutasPosibles las asociadas a la ciudad nueva con las ciudades viejas
    if (agreguePrimera){
        for (int j=0; j<rutaMinima.nodosQueConecta.fst.rutasDe.size(); ++j){//recorre las rutas que salen de la nueva ciudad que agregamos
            if (estaEnAGM(nodosAGM, rutasTotales[rutaMinima.nodosQueConecta.fst.rutasDe[j]].nodosQueConecta.fst) && estaEnAGM(nodosAGM, rutasTotales[rutaMinima.nodosQueConecta.fst.rutasDe[j]].nodosQueConecta.snd)){
            //si la j-esima ruta que sale del nuevo nodo que agregamos une dos nodos que estan en el AGM
                //entonces borramos esa ruta de las rutasPosibles
                rutasPosibles.erase(rutasPosibles.begin()+j-1);
            }
        }
    }else{
        for (int j=0; j<rutaMinima.nodosQueConecta.snd.rutasDe.size(); ++j){
            if (estaEnAGM(nodosAGM, rutasTotales[rutaMinima.nodosQueConecta.snd.rutasDe[j]].nodosQueConecta.fst) && estaEnAGM(nodosAGM, rutasTotales[rutaMinima.nodosQueConecta.snd.rutasDe[j]].nodosQueConecta.snd)){
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
    fabricas.ID = 0;
    nodos.push_back(fabricas);				//el elemento 0 es el de las fabricas y el resto sigue la numeracion de c-f
    for (int i=f; i<f+c; ++i){										//para todos los clientes
        nodo nuevoNodo;													//crea un nuevo nodo
        nuevoNodo.ID=i;													//cuya ID es un numero entre f+1 y c+f
        nodos.push_back(nuevoNodo);										//lo agrego al cjto de nodos (que esta ordenado)
    }
    assert(nodos.size()==c+1);
    
    for (int i=0; i<r; ++i){//inserta las rutas en rutasTotales
		cin>>e1>>e2>>l;
        if (e1>f || e2>f){//si ambos son menores (ie, conecta solo fabricas) no nos interesa hacer una ruta entre dos nodos con fabricas
			ruta nuevaRuta;
			nuevaRuta.numero=i;                     //a cada ruta la identificamos con nro (0<=numero<r)
			nuevaRuta.peso=l;
            agregarRuta(nuevaRuta, nodos, rutasTotales, rutasPosibles, f, e1, e2);
        }
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
        eliminarRutas(rutasTotales, nodosAGM, rutasTotales[numRuta], rutasPosibles, agreguePrimera);
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
        cout<<" "<<rutasAGM[k].nodosQueConecta.fst.ID<<" "<<rutasAGM[k].nodosQueConecta.snd.ID;
    }
    cout<<endl;
cin>>f>>c>>r;
}

return 0;
}

