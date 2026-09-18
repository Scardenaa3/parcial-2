#include <iostream>
#include <string>
#include <algorithm>
#include "ArbolBPlus.h"
#include "AnalizadorSQL.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        string argumento = argv[1];
        if (argumento == "--help" || argumento == "-h") {
            cout << "Uso de la aplicacion por CLI:\n";
            cout << "  ./bd_sql          : Inicia la consola interactiva SQL.\n";
            cout << "  ./bd_sql --help   : Muestra esta ayuda.\n";
            return 0;
        }
    }

    string rutaArchivo = "base_datos.txt";
    ArbolBPlus arbolBD(3, rutaArchivo);
    
    // Carga los registros del archivo al árbol en RAM
    arbolBD.cargarDesdeArchivo();

    AnalizadorSQL analizador(&arbolBD);

    string consulta;
    cout << "Inicializando Base de Datos con Arbol B+ (Grado 3)...\n";
    cout << "\n[Consola SQL Arbol B+ - PARCIAL 2]\n";
    cout << "Escriba 'HELP' para ver los comandos, 'EXIT' para salir.\n";

    while (true) {
        cout << "sql> ";
        if (!getline(cin, consulta)) break;

        if (consulta.empty()) continue;

        // Normalización para verificar salida insensible a mayúsculas/minúsculas
        string testExit = consulta;
        transform(testExit.begin(), testExit.end(), testExit.begin(), ::toupper);

        if (testExit == "EXIT" || testExit == "QUIT") {
            cout << "Guardando cambios en '" << rutaArchivo << "' y saliendo...\n";
            arbolBD.guardarEnArchivo();
            break;
        }

        analizador.ejecutarConsulta(consulta);
    }

    return 0;
}
