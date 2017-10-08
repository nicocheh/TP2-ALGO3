#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <tuple>
#include <cassert>
#include <queue>

#define infinito 32767
#define fst first
#define snd second


using namespace std;


void actualizoVecinos(int proximo, int cantServidores, vector<vector<int>> & matrizAdy, vector<pair<int,bool>> & adyacentesAlAGM) {

	for (int i=1; i<=cantServidores; ++i) {
		
		//solo tiene sentido actualizar los vecinos, ie que la matriz de adyacencia no tenga -1
		if (matrizAdy[proximo][i] != -1) {

			int distanciaNueva = matrizAdy[proximo][i];

			if (adyacentesAlAGM[i].first == -1) {
				adyacentesAlAGM[i].first = proximo;
			}

			int distanciaVieja = matrizAdy[i][adyacentesAlAGM[i].first];

			//actualizo la distancia del vecino este.
			if (distanciaVieja == -1) {
				adyacentesAlAGM[i].first = proximo;
			} else {
				if (distanciaNueva < distanciaVieja) {
					adyacentesAlAGM[i].first = proximo;
				}
			}
		}
	}
	return;
}



int agarraElMasCercano(vector<pair<int,bool>> & adyacentesAlAGM, vector<vector<int>> & matrizAdy, int f) {

	int nuevoNodo = -1;
	// el nuevo nodo es el que voy a agregar al AGM, el que está más cerca.

	for (int i=f+1; i<adyacentesAlAGM.size(); ++i) {
		//AL AGARARRAR EL MAS CERCANO SOLO AGARRO EL QUE SEA CLIENTE
		if (adyacentesAlAGM[i].second == false && adyacentesAlAGM[i].first != -1) {

			if (nuevoNodo == -1) {
				nuevoNodo = i;
			} else {
				int candidatoACercano = matrizAdy[i][adyacentesAlAGM[i].first];
				int masCercanoActual = matrizAdy[nuevoNodo][adyacentesAlAGM[nuevoNodo].first];
				if (candidatoACercano < masCercanoActual) {
					nuevoNodo = i;
				}
			}


		}
	}


	return nuevoNodo;
}


// la consultora1 va a usar el algoritmo de prim para devolver el AGM de este grafo.
// lo devuelve en forma de listas de adyacencia para facilitarle el trabajo a la consultora 2.
tuple<vector<vector<int>>, int, vector<vector<int>>> consultora1(vector<vector<int>> & matrizAdy, int cantServidores, int f) {

	// el -1 significa que no encontré ningun nodo en mi AGM que sea adyacente al nodo i.
	// el int es el nodo del AGM vecino al nodo i, el bool es para marcar si i está en el AGM o todavía no
	// (i, adyacentesAlAGM[i].first) define la arista que estoy agregando al AGM
	vector<pair<int, bool>> adyacentesAlAGM(cantServidores+1, make_pair(-1, false));
	// acá voy a ir buscando el nodo a menor distancia (ie la arista más chica con un nodo
	// ya pintado y el otro no) para agregar en cada paso.


	// este es el arbol que devuelvo en forma de lista de adyacencias
	vector<vector<int>> listaVecinosAGM(cantServidores+1);

	// esta representación del arbol la devuelvo para que sea más facil coutear las aristas como las piden.
	// como es un arbol se que la cantidad de enlaces no es mayor a la cantidad de servidores
	vector<vector<int>> matrizIncidenciaAGM(cantServidores+1, vector<int>(cantServidores+1, 0));


	// agrego un nodo cualquiera y actualizo todos sus vecinos
	for (int j=1;j<=f;++j){
		adyacentesAlAGM[j] = make_pair(1, true);
		actualizoVecinos(j, cantServidores, matrizAdy, adyacentesAlAGM);
		//ACA PONGO COMO INICIALES A TODOS LOS QUE SON FABRICAS
	}

	int costoDelAGM = 0;

	// en la iteración i empiezo con i nodos y termino con i+1 nodos. En la iteración
	// cantServidores-1 tengo un arbol con todos los nodos de mi grafo original.
	for (int i=1; i<=cantServidores-f; ++i) {
		//REPITO TANTAS VECES COMO CLIENTES HAY
		int proximo = agarraElMasCercano(adyacentesAlAGM, matrizAdy,f);

		//assert(proximo != -1);
		
		adyacentesAlAGM[proximo].second = true; //al más cercano lo fijo.

		//agrego esa arista fijada (ie los dos nodos que une) al AGM que devuelvo
		listaVecinosAGM[proximo].push_back(adyacentesAlAGM[proximo].first);
		listaVecinosAGM[adyacentesAlAGM[proximo].first].push_back(proximo);

		costoDelAGM = costoDelAGM + matrizAdy[proximo][adyacentesAlAGM[proximo].first];

		matrizIncidenciaAGM[i][proximo] = 1;
		matrizIncidenciaAGM[i][adyacentesAlAGM[proximo].first] = 1;

		actualizoVecinos(proximo, cantServidores, matrizAdy, adyacentesAlAGM);
	}


	return make_tuple(listaVecinosAGM, costoDelAGM, matrizIncidenciaAGM);
}

//BASICAMENTE LO QUE HAGO ES MANDARLE EN VEZ DE CANT DE SERVIDORES F+C

int main(){
	int a=1;
	while( a!=0){
		int f,c,r;
		cin>>f>>c>>r;
		vector<vector<int>> matrizAdy(f+c+1, vector<int>(f+c+1, -1)); 
		// como se que los enlaces tienen pesos positivos,
		// si en la matriz hay un -1 es que no hay enlace entre esos servidores

		for (int i=1; i<=r; ++i) {
			int s1 = 0;
			int s2 = 0;
			int peso = 0;
			cin >> s1 >> s2 >> peso;

			matrizAdy[s1][s2] = peso;
			matrizAdy[s2][s1] = peso;
		}
		
		
		auto rtaConsultora1 = consultora1(matrizAdy,f+c, f);		
		auto listaVecinosG = get<0>(rtaConsultora1);
		auto costoDelG = get<1>(rtaConsultora1);
		auto matrizIncidenciaG = get<2>(rtaConsultora1);

		cout << costoDelG << " ";
		
		//ACA HICE ESTE FOR SUPER CABEZA, PARA CONTAR CUANTOS HAY EN EL CAMINO, COPIE LO DE ABAJO Y CONTE
		//Y DIVIDO POR DOS
		int cantaristas=0;
		for (int i=1; i<=f+c; ++i) {
			for (int j=1; j<=f+c; ++j) {
				if (matrizIncidenciaG[i][j] == 1) {
					cantaristas++;
				}
			}
		}
		cout << cantaristas/2 << " ";

		for (int i=1; i<=f+c; ++i) {
			for (int j=1; j<=f+c; ++j) {
				if (matrizIncidenciaG[i][j] == 1) {
					cout << j << " ";
				}
			}
		}
	}
	
}
