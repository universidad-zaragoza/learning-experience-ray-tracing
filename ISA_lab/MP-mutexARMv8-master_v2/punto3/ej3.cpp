//Ruben Gran
#include <iostream>
#include <vector>
#include <thread>
//#include <mutex>

using namespace std;
extern "C" unsigned int mi_fetch_and_add(unsigned int *x, int add);
extern "C" void mi_mutex_lock(int *x);
extern "C" void mi_mutex_unlock(int *x);

int mi_mutex=0; // mutex para utilizar con las funciones definidas en ensamblador
int mi_mutex2=0;
const unsigned int n_elementos = 1000000;
const unsigned int n_threads = 10;
const unsigned int trozos = 100; // de cuanto son los trozo de trabajo que cojo. trozo -> #elementos de v_elems

std::vector<std::thread> v_threads; //vector donde salvaguardo los threads que voy creando
//std::mutex mutex_biblioteca1; // mutex para actualizar coger trozo de trabajo con garantias
//std::mutex mutex_biblioteca2; // mutex para actualizar sumatorio_concurrente con garantias
unsigned int indice_trabajo = 0; //me indica el punto de procesamiento -> ultimo elemento asignado a thread

int v_elems[n_elementos]; //vector que almacena los elementos que deben ser sumados

long long sumatorio_concurrente = 0; //variable compartida con el sumatorio de los valores de v_elems

int funcion(int th){
    long long mi_suma = 0;

	unsigned int mi_indice_trabajo_ini = 0;

	while (mi_indice_trabajo_ini < n_elementos){


		mi_mutex_lock(&mi_mutex);

		mi_indice_trabajo_ini = indice_trabajo;

		indice_trabajo = indice_trabajo + trozos;

		unsigned int mi_indice_trabajo_fin = indice_trabajo;	

		mi_mutex_unlock(&mi_mutex);

		for (unsigned int i = mi_indice_trabajo_ini; ((i < mi_indice_trabajo_fin)&&(i < n_elementos)); i++){

			mi_suma = mi_suma + v_elems[i];
		}

		//cout << "Soy el thread: " << th << " acabo de realizar el trozo: " << mi_indice_trabajo_ini ;
		//cout << " - " << mi_indice_trabajo_fin << endl; 
	}

	mi_mutex_lock(&mi_mutex2);
	sumatorio_concurrente = sumatorio_concurrente + mi_suma;
	mi_mutex_unlock(&mi_mutex2);

	return 0;

}

int main(){

	long long sumatorio = 0;

	//inicialización del vector
	for (int i = 0; i < n_elementos; i ++){
		v_elems[i] = i;
	}

	//calculo del sumatorio
	for (int i = 0; i < n_elementos; i ++){
		sumatorio = sumatorio + v_elems[i];
	}
	
	cout << "El valor de la componente 200000 es: " << v_elems[200000] << endl;
	cout << "El resultado de la operación sumatorio es : " << to_string(sumatorio) << endl;


	for (int i = 0; i < n_threads; i++){
		std::thread th(funcion, i);
		v_threads.push_back(std::move(th));
	}

	for (std::thread & th : v_threads){

		if (th.joinable())
			th.join();
	}

	cout << "El valor de la componente 200000 es: " << v_elems[200000] << endl;
	cout << "El resultado de la operación sumatorio es : " << to_string(sumatorio) << endl;
	cout << "El resu de la oper sumatorio concurrente es : " << to_string(sumatorio_concurrente) << endl;
	
	return 0;

}
