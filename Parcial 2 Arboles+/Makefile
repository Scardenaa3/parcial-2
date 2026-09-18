CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = bd_sql
SRCS = main.cpp ArbolBPlus.cpp AnalizadorSQL.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) "Parcial 2 Arboles+/base_datos.txt" "Parcial 2 Arboles+/indice_secundario.txt" base_datos.txt