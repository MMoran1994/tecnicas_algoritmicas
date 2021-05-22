#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <set>
#include <map>
#include <limits.h>
using namespace std;

int INFTY = INT_MAX; // Valor para indicar que no hubo solución.

// Información de la instancia a resolver.
int n, R;

// r: Vector de resistencias.
// w: Vector de pesos.
vector<int> r;
vector<int> w;

// rt: Resistencia del sistema restante.
// n: cantidad de total de productos.
// i: Producto actual a considerar.
int FB(int rt, int i)
{

	if (i == n){
		// Verificamos que no se haya superado la resistencia del tubo.
        return (rt<0) ? -INFTY : 0;
	}

	int agrego = 1 + FB(min(rt-w[i], r[i]), i+1);
	int no_agrego = FB(rt, i+1);

	return max(agrego, no_agrego);
}


bool poda_factibilidad;
bool poda_optimalidad;
int mejor_solucion = -INFTY;

int optimo;

void BT(int rt, int i, int souli_parcial) {
    
    if (poda_factibilidad and rt<0) return;
    if(poda_optimalidad){
        int mejor_prox = 0;
        for (int k = i+1; k < n; ++k){
            //cout << rt << endl;
            if (w[k] <= rt){ // Entonces puedo sumar 1 a la solución
                mejor_prox++;
            }
        }
        
        // mejor_prox = // la suma de los posibles que podes agregar en lo que quede de elementos

        if (souli_parcial + mejor_prox <= optimo) {
            return;
        }
    }

    if (i == n) {

        // Verificamos que no se haya superado la resistencia del tubo.
        if (rt < 0) return;
        if (optimo < souli_parcial) optimo = souli_parcial; 
        //cout << optimo << endl; 
        return;
    }

    BT(min(rt - w[i], r[i]), i + 1, souli_parcial + 1);
    BT(rt, i + 1, souli_parcial);  

}

// Estructura de memoizacion. Matriz de (n+1)*(R+1). Se inicializa todo en UNDEFINED.
vector<vector<int> > Mem;
#define UNDEFINED -1

// rt: resistencia total restante a utilizar
// i: indice del producto actual a considerar

int PD(int rt, int i) {
    if (rt < 0) return -INFTY;
    if (i == n) return 0;
    if (Mem[i][rt] == UNDEFINED) {
        Mem[i][rt] = max(1 + PD(min(rt - w[i], r[i]), i + 1),
                             PD(rt, i + 1));
    }
    return Mem[i][rt];
}


int main(int argc, char const *argv[]) {
    // Leemos el parametro que indica el algoritmo a ejecutar.
    map<string, string> algoritmos_implementados = {
            {"FB",   "Fuerza Bruta"},
            {"BT",   "Backtracking con podas"},
            {"BT-F", "Backtracking con poda por factibilidad"},
            {"BT-O", "Backtracking con poda por optimalidad"},
            {"DP",   "Programacion dinámica"}
    };

    // Verificar que el algoritmo pedido exista.
    if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end()) {
        cerr << "Algoritmo no encontrado: " << argv[1] << endl;
        cerr << "Los algoritmos existentes son: " << endl;
        for (auto &alg_desc: algoritmos_implementados)
            cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
        return 0;
    }
    string algoritmo = argv[1];

    // Leemos el input.
    cin >> n >> R;
    w.assign(n, 0);
    r.assign(n, 0);
    for (int i = 0; i < n; ++i) cin >> w[i] >> r[i];

    cout << n << " " << R << endl;
    for (int i = 0; i < n; ++i) cout << w[i] << " " << r[i] << endl;

    // Ejecutamos el algoritmo y obtenemos su tiempo de ejecución.
    int optimum;
    optimum = INFTY;
    auto start = chrono::steady_clock::now();
    if (algoritmo == "FB") {
        optimum = FB(R, 0);
    }
    if (algoritmo == "BT")
	{
		optimo = -INFTY;
		poda_optimalidad = poda_factibilidad = true;
		BT(R, 0, 0);
        optimum = optimo;
	}
	if (algoritmo == "BT-F")
	{
		optimo = -INFTY;
		poda_optimalidad = false;
		poda_factibilidad = true;
		BT(R, 0, 0);
        optimum = optimo;
	}
	if (algoritmo == "BT-O")
	{
		optimo = -INFTY;
		poda_optimalidad = true;
		poda_factibilidad = false;
		BT(R, 0, 0);
        optimum = optimo;
	}
    if (algoritmo == "DP") {
        Mem = vector<vector<int> >(n + 1, vector<int>(R + 1, UNDEFINED));
        optimum = PD(R, 0);
    }
    auto end = chrono::steady_clock::now();
    double total_time = chrono::duration<double, milli>(end - start).count();

    // Imprimimos el tiempo de ejecución por stderr.
    clog << total_time << endl;

    // Imprimimos el resultado por stdout.
    cout << (optimum == INFTY ? -1 : optimum) << endl;
    return 0;
}
