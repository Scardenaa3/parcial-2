#include <iostream>
#include <string>
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

    cout << "Inicializando Base de Datos con Arbol B+ (Grado 3) en 'Parcial 2 Arboles+'...\n";
    
    // Asigna la ruta directa a la carpeta del examen
    ArbolBPlus arbolBD(3, "Parcial 2 Arboles+/base_datos.txt");
    
    arbolBD.cargarDesdeArchivo();

    AnalizadorSQL analizador(&arbolBD);

    string consulta;
    cout << "\n[Consola SQL Arbol B+ - PARCIAL 2]\n";
    cout << "Escriba 'HELP' para ver los comandos, 'EXIT' para salir.\n";

    while (true) {
        cout << "sql> ";
        getline(cin, consulta);

        if (consulta.empty()) continue;

        if (consulta == "EXIT" || consulta == "exit" || consulta == "quit") {
            cout << "Guardando cambios en 'Parcial 2 Arboles+/base_datos.txt' y saliendo...\n";
            arbolBD.guardarEnArchivo();
            break;
        }

        analizador.ejecutarConsulta(consulta);
    }

    return 0;
}