# Makefile
CXX := g++
CXXFLAGS := -O2 -std=c++17
OMPFLAGS := -fopenmp

all: secuencial paralelo

secuencial: simulacion_secuencial.cpp common.h
	$(CXX) $(CXXFLAGS) -o simulacion_secuencial simulacion_secuencial.cpp

paralelo: simulacion_paralela.cpp common.h
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -o simulacion_paralela simulacion_paralela.cpp

clean:
	rm -f simulacion_secuencial simulacion_paralela

