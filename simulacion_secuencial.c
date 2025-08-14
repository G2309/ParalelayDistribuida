#include "common.h"

int main(int argc, char** argv) {
    // Configuración inicial 
    int num_vehiculos = 20;   // cantidad de vehiculos
    int num_semaforos = 4;    // cantidad de semaforos
    int iteraciones   = 10;   // Número de pasos de simulación

    // paso de parametros cuando se corre el ejecutable
    // argv[1] = num de vehiculos, argv[2] = num de semaforos, argv[3] = iteraciones
    if (argc >= 2) num_vehiculos = std::stoi(argv[1]);
    if (argc >= 3) num_semaforos = std::stoi(argv[2]);
    if (argc >= 4) iteraciones   = std::stoi(argv[3]);

	// inicializacion
    // crea la estructura con n vehiculos y m semaforos, con ciclos predefinidos.
    Interseccion inter = crear_interseccion(num_vehiculos, num_semaforos, 3, 1, 2);

    for (int t = 1; t <= iteraciones; ++t) {
        // actualizar semáforos, decrementar tiempo, cambiar de estado si llega a 0
        for (auto& s : inter.semaforos) {
            actualizar_semaforo_tick(s);
        }

        // mover vehiculos si el semaforo correspondiente está en verde
        for (auto& v : inter.vehiculos) {
            mover_vehiculo_tick(v, inter.semaforos);
        }

        // imprimir estado actual 
        imprimir_estado(t, inter.vehiculos, inter.semaforos);
    }

    return 0;
}

