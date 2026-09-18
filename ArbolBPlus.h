#ifndef ARBOL_BPLUS_H
#define ARBOL_BPLUS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

/**
 * @struct Registro
 * Unidad minima de informacion (fila) almacenada en los nodos hoja.
 */
struct Registro {
    int clave;
    string datos;

    string serializar() const {
        return to_string(clave) + "," + datos;
    }
};

/**
 * @struct NodoBPlus
 * Representa un nodo dentro del Arbol B+.
 */
struct NodoBPlus {
    bool es_hoja;
    vector<int> claves;
    vector<NodoBPlus*> hijos;
    vector<Registro> registros;
    NodoBPlus* siguiente_hoja;

    NodoBPlus(bool hoja) : es_hoja(hoja), siguiente_hoja(nullptr) {}
};

/**
 * @class ArbolBPlus
 * Maneja la logica del Arbol B+ y la persistencia en disco.
 */
class ArbolBPlus {
private:
    NodoBPlus* raiz;
    int grado;
    string nombre_archivo;

    void insertarInterno(int clave, NodoBPlus* cursor, NodoBPlus* hijo);
    NodoBPlus* buscarPadre(NodoBPlus* cursor, NodoBPlus* hijo);
    void liberarMemoria(NodoBPlus* nodo);

public:
    ArbolBPlus(int _grado, string _nombre_archivo);
    ~ArbolBPlus();

    void insertar(int clave, string datos);
    string buscar(int clave);
    void eliminar(int clave);
    vector<Registro> obtenerTodos();

    void guardarEnArchivo();
    void cargarDesdeArchivo();
    void limpiar();
};

#endif // ARBOL_BPLUS_H