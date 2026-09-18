#ifndef ARBOL_BPLUS_H
#define ARBOL_BPLUS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Registro {
    int clave;
    string datos;
};

struct NodoBPlus {
    bool es_hoja;
    vector<int> claves;
    vector<NodoBPlus*> hijos;
    vector<Registro> registros;
    NodoBPlus* siguiente_hoja;

    NodoBPlus(bool hoja) : es_hoja(hoja), siguiente_hoja(nullptr) {}
};

class ArbolBPlus {
private:
    NodoBPlus* raiz;
    int grado;
    string nombre_archivo;

    void liberarMemoria(NodoBPlus* nodo);
    void insertarInterno(int clave, NodoBPlus* cursor, NodoBPlus* hijo);
    NodoBPlus* buscarPadre(NodoBPlus* cursor, NodoBPlus* hijo);

public:
    ArbolBPlus(int _grado, string _nombre_archivo);
    ~ArbolBPlus();

    void insertar(int clave, string datos);
    string buscar(int clave);
    void eliminar(int clave);
    void limpiar();
    vector<Registro> obtenerTodos();
    
    void guardarEnArchivo();
    void cargarDesdeArchivo();
};

#endif
