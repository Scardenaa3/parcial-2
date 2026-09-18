#include "AnalizadorSQL.h"

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
    comando = aMayusculas(comando);

    if (comando == "CREATE" || comando == "DROP") {
        analizarDDL(consulta, comando);
    } else if (comando == "SELECT" || comando == "INSERT" || comando == "DELETE") {
        analizarDQL_DML(consulta, comando);
    } else if (comando == "HELP") {
        mostrarAyuda();
    } else {
        cout << "Error: Comando SQL no reconocido. Escriba HELP para mas informacion.\n";
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

    cout << BOLD_YELLOW << "\n=== Sistema Gestor SQL basado en Arboles B+ ===" << RESET << "\n";
    cout << BOLD_WHITE << "Comandos Soportados:" << RESET << "\n";
    cout << BOLD_YELLOW << "  [DDL - Lenguaje de Definicion de Datos]" << RESET << "\n";
    cout << BOLD_CYAN << "    CREATE TABLE usuarios (id INT, nombre STR)" << RESET << "\n";
    cout << BOLD_CYAN << "    CREATE INDEX idx_nombre ON usuarios (nombre)" << RESET << "\n";
    cout << BOLD_CYAN << "    DROP TABLE usuarios" << RESET << "\n\n";
    cout << BOLD_YELLOW << "  [DQL / DML - Manipulacion y Consulta]" << RESET << "\n";
    cout << BOLD_CYAN << "    INSERT INTO usuarios VALUES (10, 'Juan Perez')" << RESET << "\n";
    cout << BOLD_CYAN << "    SELECT * FROM usuarios" << RESET << "\n";
    cout << BOLD_CYAN << "    SELECT * FROM usuarios WHERE id = 10" << RESET << "\n";
    cout << BOLD_CYAN << "    DELETE FROM usuarios WHERE id = 10" << RESET << "\n\n";
    cout << BOLD_YELLOW << "  [Otros Comandos]" << RESET << "\n";
    cout << BOLD_CYAN << "    HELP  - Muestra este menu" << RESET << "\n";
    cout << BOLD_CYAN << "    EXIT  - Guarda los datos y sale del programa" << RESET << "\n";
    cout << BOLD_YELLOW << "================================================" << RESET << "\n\n";
}