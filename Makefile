# Nombre del ejecutable
# (Hecho por chatgpt)
TARGET = juego

# Compilador
CXX = g++
CXXFLAGS = -g -std=c++17 -Wall

# Archivos fuente
SRCS = main.cpp player.cpp enemy.cpp groundlocation.cpp item.cpp skill.cpp

# Archivos objeto generados
OBJS = $(SRCS:.cpp=.o)

# Regla por defecto
all: $(TARGET)

# Cómo construir el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para limpiar archivos intermedios
clean:
	rm -f $(OBJS) $(TARGET)
