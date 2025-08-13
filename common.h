// common.h
#pragma once
#include <vector>
#include <cstdint>
#include <cstdio>
#include <string>

// estados, se imprimen como enteros (0,1,2)
enum EstadoSemaforo : int {
    ROJO = 0,
    VERDE = 1,
    AMARILLO = 2
};

struct Semaforo {
    int id;
    EstadoSemaforo estado;
    int tiempo_restante;
    // Duraciones del ciclo
    int dur_verde;
    int dur_amarillo;
    int dur_rojo;
};

struct Vehiculo {
    int id;
    int lane;         // a que semaforo obedece
    int posicion;     // posición discreta para imprimir entero
    int velocidad;    // pasos por tick si puede moverse 
};

struct Interseccion {
    std::vector<Semaforo> semaforos;
    std::vector<Vehiculo> vehiculos;
};

// transicion de estado del semaforo
// ciclo: verde -> amarillo -> rojo -> verde
// al cambiar estado se actualiza el tiempo_restante con la duracion
inline void avanzar_estado(Semaforo& s) {
    if (s.estado == VERDE) {
        s.estado = AMARILLO;
        s.tiempo_restante = s.dur_amarillo;
    } else if (s.estado == AMARILLO) {
        s.estado = ROJO;
        s.tiempo_restante = s.dur_rojo;
    } else { // ROJO
        s.estado = VERDE;
        s.tiempo_restante = s.dur_verde;
    }
}

// actualiza un semaforo por un tick
// decrementa tiempo restante y cambia de estado al llegar a 0
//
inline void actualizar_semaforo_tick(Semaforo& s) {
    s.tiempo_restante -= 1;
    if (s.tiempo_restante <= 0) {
        avanzar_estado(s);
    }
}

// Mueve un vehiculo si su carril con semaforo asociado esta en verde
// incrementa la posicion segun la velocidad del vehiculo
inline void mover_vehiculo_tick(Vehiculo& v, const std::vector<Semaforo>& semaforos) {
    const Semaforo& s = semaforos[v.lane];
    if (s.estado == VERDE) {
        v.posicion += v.velocidad;
    }
}

// inicializa interseccion con n vehiculos y M semaforos
inline Interseccion crear_interseccion(int num_vehiculos,
                                       int num_semaforos,
                                       // ciclo por defecto: verde 3, amarillo 1, rojo 2
                                       int dur_verde = 3, int dur_amarillo = 1, int dur_rojo = 2) {
	// Crear semaforos con estados diferentes y luego determna el estado inicial con base al indice de arriba
    Interseccion inter;
    inter.semaforos.reserve(num_semaforos);
    for (int i = 0; i < num_semaforos; ++i) {
        EstadoSemaforo estado_ini;
        if (i % 4 == 0 || i % 4 == 1) estado_ini = VERDE;
        else if (i % 4 == 2) estado_ini = ROJO;
        else estado_ini = AMARILLO;
		// establece el tiempo inicial correspondiente al estado
        int t0 = (estado_ini == VERDE) ? dur_verde
               : (estado_ini == AMARILLO) ? dur_amarillo
               : dur_rojo;

        inter.semaforos.push_back(Semaforo{
            .id = i,
            .estado = estado_ini,
            .tiempo_restante = t0,
            .dur_verde = dur_verde,
            .dur_amarillo = dur_amarillo,
            .dur_rojo = dur_rojo
        });
    }
	// crea vehiculos y los asigna a un carril
    inter.vehiculos.reserve(num_vehiculos);
    for (int i = 0; i < num_vehiculos; ++i) {
        int lane = i % num_semaforos; 
        inter.vehiculos.push_back(Vehiculo{
            .id = i,
            .lane = lane,
            .posicion = 0,
            .velocidad = 1
        });
    }
    return inter;
}

inline void imprimir_estado(int iteracion,
                            const std::vector<Vehiculo>& vehiculos,
                            const std::vector<Semaforo>& semaforos) {
    std::printf("iteracion %d:\n", iteracion);
    for (const auto& v : vehiculos) {
        std::printf("Vehículo %d - Posición: %d\n", v.id, v.posicion);
    }
    for (const auto& s : semaforos) {
        std::printf("Semáforo %d - Estado: %d\n", s.id, static_cast<int>(s.estado));
    }
    std::printf("\n");
}

