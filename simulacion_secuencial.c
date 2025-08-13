#include <thread>
#include <chrono>
#include "common.h"


int main(int argc, char** argv) {
	//conf inicial
    int num_vehiculos = 20;
    int num_semaforos = 4;
    int iteraciones   = 10;

    if (argc >= 2) num_vehiculos = std::stoi(argv[1]);
    if (argc >= 3) num_semaforos = std::stoi(argv[2]);
    if (argc >= 4) iteraciones   = std::stoi(argv[3]);

    // inicializacion determinista
    Interseccion inter = crear_interseccion(num_vehiculos, num_semaforos, 3, 1, 2);

    for (int t = 1; t <= iteraciones; ++t) {
        // actualizar semaforos
        for (auto& s : inter.semaforos) {
            actualizar_semaforo_tick(s);
        }
        // mover vehiculos segun su semaforo
        for (auto& v : inter.vehiculos) {
            mover_vehiculo_tick(v, inter.semaforos);
        }
        // print estado
        imprimir_estado(t, inter.vehiculos, inter.semaforos);

        // un sleep para que se vea mejor 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

