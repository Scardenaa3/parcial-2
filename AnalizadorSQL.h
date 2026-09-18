#ifndef ANALIZADOR_SQL_H
#define ANALIZADOR_SQL_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "ArbolBPlus.h"

using namespace std;

/**
 * @class AnalizadorSQL
 * Procesa las cadenas de texto SQL e invoca los metodos del Arbol B+.
 */
class AnalizadorSQL {
private:
    ArbolBPlus* bd;
    ArbolBPlus* indiceSecundario;

    string aMayusculas(string cadena);

public:
    AnalizadorSQL(ArbolBPlus* base_datos);
    ~AnalizadorSQL();

    void ejecutarConsulta(string consulta);
    void analizarDDL(string consulta, string comando);
    void analizarDQL_DML(string consulta, string comando);
    void mostrarAyuda();
};

#endif // ANALIZADOR_SQL_H
