#include "AnalizadorSQL.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;

AnalizadorSQL::AnalizadorSQL(ArbolBPlus* base_datos) 
    : bd(base_datos), indiceSecundario(nullptr) {}

AnalizadorSQL::~AnalizadorSQL() {
    if (indiceSecundario) delete indiceSecundario;
}

string AnalizadorSQL::aMayusculas(string cadena) {
    string cadenaMayus = "";
    for (char c : cadena) cadenaMayus += toupper(c);
    return cadenaMayus;
}

void AnalizadorSQL::ejecutarConsulta(string consulta) {
    if (consulta.empty()) return;

    stringstream ss(consulta);
    string comando;
    ss >> comando;
    string comandoUpper = aMayusculas(comando);

    // --- MANEJO INTERACTIVO POR NÚMEROS (1-9) ---
    if (comando == "1") {
        string nombreTabla;
        cout << "\n[Crear Tabla] Ingrese el nombre de la tabla: ";
        cin >> nombreTabla;
        analizarDDL("CREATE TABLE " + nombreTabla, "CREATE");
        return;
    }
    else if (comando == "2") {
        string nombreIndice, nombreTabla, campo;
        cout << "\n[Crear Índice] Nombre del índice: ";
        cin >> nombreIndice;
        cout << "Nombre de la tabla objetivo: ";
        cin >> nombreTabla;
        cout << "Campo a indexar: ";
        cin >> campo;
        analizarDDL("CREATE INDEX " + nombreIndice + " ON " + nombreTabla + " (" + campo + ")", "CREATE");
        return;
    }
    else if (comando == "3") {
        int id;
        string datos;
        cout << "\n[Insertar Registro] Ingrese ID (número entero): ";
        cin >> id;
        cout << "Ingrese los datos/nombre del registro: ";
        cin.ignore();
        getline(cin, datos);
        
        string consultaSintetizada = "INSERT INTO usuarios VALUES (" + to_string(id) + ", " + datos + ")";
        analizarDQL_DML(consultaSintetizada, "INSERT");
        return;
    }
    else if (comando == "4") {
        int id;
        cout << "\n[Consultar por ID] Ingrese el ID a buscar: ";
        cin >> id;
        analizarDQL_DML("SELECT * FROM usuarios WHERE id = " + to_string(id), "SELECT");
        return;
    }
    else if (comando == "5") {
        analizarDQL_DML("SELECT * FROM usuarios", "SELECT");
        return;
    }
    else if (comando == "6") {
        int id;
        cout << "\n[Eliminar Registro] Ingrese el ID a borrar: ";
        cin >> id;
        analizarDQL_DML("DELETE FROM usuarios WHERE id = " + to_string(id), "DELETE");
        return;
    }
    else if (comando == "7") {
        string nombreTabla;
        cout << "\n[Eliminar Tabla] Nombre de la tabla a borrar: ";
        cin >> nombreTabla;
        analizarDDL("DROP TABLE " + nombreTabla, "DROP");
        return;
    }
    else if (comando == "8" || comandoUpper == "HELP") {
        mostrarAyuda();
        return;
    }
    else if (comando == "9" || comandoUpper == "EXIT") {
        cout << "Guardando datos y cerrando el programa...\n";
        return;
    }

    // --- MANEJO POR TEXTO COMPLETO SQL ---
    if (comandoUpper == "CREATE" || comandoUpper == "DROP") {
        analizarDDL(consulta, comandoUpper);
    } else if (comandoUpper == "SELECT" || comandoUpper == "INSERT" || comandoUpper == "DELETE") {
        analizarDQL_DML(consulta, comandoUpper);
    } else {
        cout << "Error: Comando u opción no reconocida. Escriba HELP o seleccione de 1 a 9.\n";
    }
}

void AnalizadorSQL::analizarDDL(string consulta, string comando) {
    string consultaUpper = aMayusculas(consulta);

    if (comando == "CREATE") {
        if (consultaUpper.find("INDEX") != string::npos) {
            if (!indiceSecundario) {
                // Instancia el indice dentro de la carpeta Parcial 2 Arboles+
                indiceSecundario = new ArbolBPlus(3, "Parcial 2 Arboles+/indice_secundario.txt");
            }
            cout << "[OK] Indice secundario instanciado en memoria (Parcial 2 Arboles+).\n";
        } else if (consultaUpper.find("TABLE") != string::npos) {
            cout << "[OK] Tabla lista para operar.\n";
        }
    } else if (comando == "DROP") {
        bd->limpiar();
        remove("Parcial 2 Arboles+/base_datos.txt");
        cout << "[OK] Tabla y datos eliminados en disco y RAM.\n";
    }
}

void AnalizadorSQL::analizarDQL_DML(string consulta, string comando) {
    string consultaUpper = aMayusculas(consulta);

    if (comando == "INSERT") {
        size_t posValues = consultaUpper.find("VALUES");
        if (posValues != string::npos) {
            size_t parentesisIzq = consulta.find('(', posValues);
            size_t parentesisDer = consulta.find(')', parentesisIzq);
            if (parentesisIzq != string::npos && parentesisDer != string::npos) {
                string contenido = consulta.substr(parentesisIzq + 1, parentesisDer - parentesisIzq - 1);
                stringstream ss(contenido);
                string strId, datos;
                if (getline(ss, strId, ',')) {
                    getline(ss, datos);
                    try {
                        int id = stoi(strId);
                        size_t posComa = datos.find_first_not_of(" ");
                        if (posComa != string::npos) datos = datos.substr(posComa);

                        bd->insertar(id, datos);
                        cout << "[OK] Registro con ID " << id << " insertado correctamente.\n";
                    } catch (...) {
                        cout << "Error: El ID debe ser un numero entero valido.\n";
                    }
                }
            }
        }
    } 
    else if (comando == "SELECT") {
        if (consultaUpper.find("WHERE") != string::npos) {
            size_t posIgual = consulta.find('=');
            if (posIgual != string::npos) {
                string strId = consulta.substr(posIgual + 1);
                try {
                    int id = stoi(strId);
                    string res = bd->buscar(id);
                    if (!res.empty()) {
                        cout << "-> [" << id << "] : " << res << "\n";
                    } else {
                        cout << "-> Registro con ID " << id << " no encontrado.\n";
                    }
                } catch (...) {
                    cout << "Error: ID de busqueda invalido.\n";
                }
            }
        } else {
            vector<Registro> todos = bd->obtenerTodos();
            cout << "\n--- REGISTROS EN BASE DE DATOS ---\n";
            for (const auto& reg : todos) {
                cout << "[" << reg.clave << "] : " << reg.datos << "\n";
            }
            cout << "-----------------------------------\n";
        }
    }
    else if (comando == "DELETE") {
        size_t posIgual = consulta.find('=');
        if (posIgual != string::npos) {
            string strId = consulta.substr(posIgual + 1);
            try {
                int id = stoi(strId);
                bd->eliminar(id);
                cout << "[OK] Registro " << id << " eliminado.\n";
            } catch (...) {
                cout << "Error: ID invalido para eliminacion.\n";
            }
        }
    }
}

void AnalizadorSQL::mostrarAyuda() {
    const string RESET = "\033[0m";
    const string BOLD_YELLOW = "\033[1;33m";
    const string BOLD_CYAN = "\033[1;36m";
    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_WHITE = "\033[1;37m";

    cout << BOLD_YELLOW << "\n======================================================================\n";
    cout << "                 MOTOR DE BASE DE DATOS (ÁRBOL B+)                   \n";
    cout << "======================================================================\n" << RESET;

    cout << BOLD_WHITE << " MENÚ DE OPCIONES RÁPIDAS (Ingresa el número o el comando SQL):\n" << RESET;
    cout << BOLD_YELLOW << " --------------------------------------------------------------------\n" << RESET;
    cout << BOLD_GREEN << "  [1]" << BOLD_CYAN << " Crear Tabla          - Define una nueva tabla en el sistema\n";
    cout << BOLD_GREEN << "  [2]" << BOLD_CYAN << " Crear Índice         - Optimiza búsquedas en un campo\n";
    cout << BOLD_GREEN << "  [3]" << BOLD_CYAN << " Insertar Registro    - Agrega un nuevo registro por ID\n";
    cout << BOLD_GREEN << "  [4]" << BOLD_CYAN << " Consultar por ID     - Busca un registro específico\n";
    cout << BOLD_GREEN << "  [5]" << BOLD_CYAN << " Ver Todos            - Muestra todos los datos almacenados\n";
    cout << BOLD_GREEN << "  [6]" << BOLD_CYAN << " Eliminar Registro    - Elimina un dato por su ID\n";
    cout << BOLD_GREEN << "  [7]" << BOLD_CYAN << " Eliminar Tabla       - Borra la tabla y sus archivos\n";
    cout << BOLD_GREEN << "  [8]" << BOLD_CYAN << " Ver Ayuda (HELP)     - Muestra este menú nuevamente\n";
    cout << BOLD_GREEN << "  [9]" << BOLD_CYAN << " Salir (EXIT)         - Guarda los datos y cierra el programa\n" << RESET;

    cout << BOLD_YELLOW << " --------------------------------------------------------------------\n" << RESET;
    cout << BOLD_WHITE << " EJEMPLOS DE SINTAXIS MANUAL DIRECTA:\n" << RESET;
    cout << BOLD_CYAN << "   • CREATE TABLE usuarios (id INT, nombre STR)\n";
    cout << BOLD_CYAN << "   • INSERT INTO usuarios VALUES (10, 'Juan Perez')\n";
    cout << BOLD_CYAN << "   • SELECT * FROM usuarios WHERE id = 10\n";
    cout << BOLD_CYAN << "   • DELETE FROM usuarios WHERE id = 10\n";
    cout << BOLD_YELLOW << "======================================================================\n\n" << RESET;
}
