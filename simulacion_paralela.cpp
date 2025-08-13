#include <omp.h>
#include <algorithm>
#include "common.h"

static int estimar_hilos_por_carga(int activos, int max_hilos) {
    if (activos <= 0) return 1;
    // 1 hilo por cada 8 elementos activos, hasta max_hilos
    int h = std::max(1, (activos + 7) / 8);
    return std::min(h, max_hilos);
}

int main(int argc, char** argv) {
    int num_vehiculos = 20;
    int num_semaforos = 4;
    int iteraciones   = 10;

    if (argc >= 2) num_vehiculos = std::stoi(argv[1]);
    if (argc >= 3) num_semaforos = std::stoi(argv[2]);
    if (argc >= 4) iteraciones   = std::stoi(argv[3]);

    Interseccion inter = crear_interseccion(num_vehiculos, num_semaforos, 3, 1, 2);

    // ajuste dinámico de hilos
    omp_set_dynamic(1);
    // paralelismo anidado 
    omp_set_nested(1);

    const int max_hilos = omp_get_max_threads();

    for (int t = 1; t <= iteraciones; ++t) {

        // contar potenciales activos: semaforos + vehiculos
        int activos_semaforos = static_cast<int>(inter.semaforos.size());
        int activos_vehiculos = static_cast<int>(inter.vehiculos.size());
        int hilos_semaforos   = estimar_hilos_por_carga(activos_semaforos, max_hilos);
        int hilos_vehiculos   = estimar_hilos_por_carga(activos_vehiculos, max_hilos);

        // secciones paralelas, una para semaforos, otra para vehiculos
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                omp_set_num_threads(hilos_semaforos);
                #pragma omp parallel for schedule(static)
                for (int i = 0; i < (int)inter.semaforos.size(); ++i) {
                    actualizar_semaforo_tick(inter.semaforos[i]);
                }
            }

            #pragma omp section
            {
                omp_set_num_threads(hilos_vehiculos);
                #pragma omp parallel for schedule(dynamic, 16)
                for (int i = 0; i < (int)inter.vehiculos.size(); ++i) {
                    mover_vehiculo_tick(inter.vehiculos[i], inter.semaforos);
                }
            }
        } 

        imprimir_estado(t, inter.vehiculos, inter.semaforos);
    }

    return 0;
}

