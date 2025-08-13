#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

enum EstadoSemaforo { ROJO, VERDE, AMARILLO };

struct Semaforo {
    int id;
    EstadoSemaforo estado;
    int tiempo; // tiempo restante en el estado actual
};

struct Vehiculo {
    int id;
    float posicion;
    float velocidad;
};

struct Interseccion {
    vector<Semaforo> semaforos;
    vector<Vehiculo> vehiculos;
};

// Funcion para mostrar el estado del semaforo en texto
string estadoToString(EstadoSemaforo estado) {
    switch (estado) {
        case ROJO: return "ROJO";
        case VERDE: return "VERDE";
        case AMARILLO: return "AMARILLO";
        default: return "";
    }
}

// Funcion que actualiza el estado de un semaforo
void actualizarSemaforo(Semaforo &s) {
    s.tiempo--;
    if (s.tiempo <= 0) {
        if (s.estado == VERDE) {
            s.estado = AMARILLO;
            s.tiempo = 2; // duracion amarillo
        } else if (s.estado == AMARILLO) {
            s.estado = ROJO;
            s.tiempo = 5; // duracion rojo
        } else if (s.estado == ROJO) {
            s.estado = VERDE;
            s.tiempo = 5; // duracion verde
        }
    }
}

// Funcion que mueve los vehiculos segun el semaforo
void moverVehiculos(vector<Vehiculo> &vehiculos, Semaforo &s) {
    for (auto &v : vehiculos) {
        if (s.estado == VERDE) {
            v.posicion += v.velocidad;
        }
    }
}

int main() {
    Interseccion inter;

    // Inicializar semaforos
    Semaforo s1 = {1, VERDE, 5};
    Semaforo s2 = {2, ROJO, 5};
    inter.semaforos.push_back(s1);
    inter.semaforos.push_back(s2);

    // Inicializar vehiculos
    for (int i = 0; i < 10; i++) {
        Vehiculo v = {i, 0.0f, 1.0f};
        inter.vehiculos.push_back(v);
    }

    // Simulacion
    for (int t = 0; t < 20; t++) {
        cout << "Iteracion: " << t << endl;

        // Actualizar semaforos
        for (auto &s : inter.semaforos) {
            actualizarSemaforo(s);
            cout << "Semaforo " << s.id << " -> " << estadoToString(s.estado) << " (" << s.tiempo << ")" << endl;
        }

        // Mover vehiculos usando el primer semaforo (simplificacion)
        moverVehiculos(inter.vehiculos, inter.semaforos[0]);

        // Mostrar posiciones
        for (auto &v : inter.vehiculos) {
            cout << "Vehiculo " << v.id << " posicion: " << v.posicion << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}

