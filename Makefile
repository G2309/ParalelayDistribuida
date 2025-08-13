# Makefile
CXX := g++
CXXFLAGS := -O2 -std=c++17
OMPFLAGS := -fopenmp

all: secuencial paralelo

secuencial: simulacion_secuencial.c common.h
	$(CXX) $(CXXFLAGS) -o simulacion_secuencial simulacion_secuencial.c

paralelo: simulacion_paralela.c common.h
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -o simulacion_paralela simulacion_paralela.c

clean:
	rm -f simulacion_secuencial simulacion_paralela

