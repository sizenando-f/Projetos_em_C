CXX = g++
CXXFLAGS = -Wall -g -static-libstdc++
EXECUTABLES = cliente servidor

all: $(EXECUTABLES)

cliente: cliente.o
	$(CXX) $(CXXFLAGS) -o cliente cliente.o

servidor: servidor.o
	$(CXX) $(CXXFLAGS) -o servidor servidor.o

cliente.o: cliente.cpp
	$(CXX) $(CXXFLAGS) -c cliente.cpp

servidor.o: servidor.cpp
	$(CXX) $(CXXFLAGS) -c servidor.cpp

clean:
	rm -f $(EXECUTABLES) *.o