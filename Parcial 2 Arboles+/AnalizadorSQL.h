#ifndef ANALIZADOR_SQL_H
#define ANALIZADOR_SQL_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include "ArbolBPlus.h"

using namespace std;

class AnalizadorSQL {
private:
    ArbolBPlus* bd;
    ArbolBPlus* indiceSecundario;
    bool tablaCreada;

    string aMayusculas(string cadena);
    void analizarDDL(string consulta, string comando);
    void analizarDQL_DML(string consulta, string comando);

public:
    AnalizadorSQL(ArbolBPlus* base_datos);
    ~AnalizadorSQL();

    void ejecutarConsulta(string consulta);
    void mostrarAyuda();
};

#endif
