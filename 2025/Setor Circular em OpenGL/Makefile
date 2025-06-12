# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -std=c++11

# Bibliotecas do OpenGL e GLUT
LDLIBS = -lglut -lGL -lGLU -lm

# Nome do executável
TARGET = app

# Arquivos fonte (adicione outros .cpp se necessário)
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)  # Gera main.o a partir de main.cpp

# Regra padrão: compila o executável
all: $(TARGET)

# Compila o executável a partir dos objetos
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compila cada .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove o executável e objetos
clean:
	rm -f $(TARGET) $(OBJ)

# Executa o programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
