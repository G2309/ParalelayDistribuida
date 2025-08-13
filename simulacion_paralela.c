#include <omp.h>
#include <algorithm>
#include "common.h"

// Función auxiliar: estima la cantidad de hilos a usar según el tamaño de la carga
static int estimar_hilos_por_carga(int activos, int max_hilos) {
    if (activos <= 0) return 1;
    // Heurística: 1 hilo por cada 8 elementos activos, hasta max_hilos
    int h = std::max(1, (activos + 7) / 8);
    return std::min(h, max_hilos);
}

int main(int argc, char** argv) {
	// conf inicial
    int num_vehiculos = 20;
    int num_semaforos = 4;
    int iteraciones   = 10;

	// cambiar la configuracion si hay parametros de entrada
    if (argc >= 2) num_vehiculos = std::stoi(argv[1]);
    if (argc >= 3) num_semaforos = std::stoi(argv[2]);
    if (argc >= 4) iteraciones   = std::stoi(argv[3]);

    // inicializacion de la interseccion
    Interseccion inter = crear_interseccion(num_vehiculos, num_semaforos, 3, 1, 2);

    omp_set_dynamic(1);  // ajuste dinamico de threads 
    omp_set_nested(1);   // paralelismo anidado  

    const int max_hilos = omp_get_max_threads(); 

    // Bucle principal, este no esta paralelizado debido a que cada iteracion 
	// depende de la anterior y podria haber problemas como calcular antes un tick 2 que un tick 1
    for (int t = 1; t <= iteraciones; ++t) {
        // calcular cuantos hilos usar para cada carga
        int activos_semaforos = static_cast<int>(inter.semaforos.size());
        int activos_vehiculos = static_cast<int>(inter.vehiculos.size());
        int hilos_semaforos   = estimar_hilos_por_carga(activos_semaforos, max_hilos);
        int hilos_vehiculos   = estimar_hilos_por_carga(activos_vehiculos, max_hilos);

        // Se usaron las parallel sections, donde una 
        // seccion se utiliza para actualizar semaforos 
		// la y otra para mover vehiculos.
		// Estas secciones se ejecutan en paralelo
        #pragma omp parallel sections
        {
			// seccion para actualizar semaforos
            #pragma omp section
            {
                // actualizar cada semaforo en paralelo
                omp_set_num_threads(hilos_semaforos);
                #pragma omp parallel for schedule(static)
                for (int i = 0; i < (int)inter.semaforos.size(); ++i) {
                    actualizar_semaforo_tick(inter.semaforos[i]);
                }
            }

            // seccion para mover los vehiculos 
            #pragma omp section
            {
                // 
                // Se usa schedule(dynamic,16) porque el tiempo de cada iteración puede variar.
                omp_set_num_threads(hilos_vehiculos);
                #pragma omp parallel for schedule(dynamic, 16)
                for (int i = 0; i < (int)inter.vehiculos.size(); ++i) {
                    mover_vehiculo_tick(inter.vehiculos[i], inter.semaforos);
                }
            }

        imprimir_estado(t, inter.vehiculos, inter.semaforos);
    }

    return 0;
}

