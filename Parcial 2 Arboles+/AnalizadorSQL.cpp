#include "AnalizadorSQL.h"

AnalizadorSQL::AnalizadorSQL(ArbolBPlus* base_datos)
    : bd(base_datos), indiceSecundario(nullptr), tablaCreada(false) {
  // Si al cargar hay datos guardados previamente, asumimos la tabla disponible
  if (!bd->obtenerTodos().empty()) {
    tablaCreada = true;
  }
}

AnalizadorSQL::~AnalizadorSQL() {
  if (indiceSecundario) {
    delete indiceSecundario;
    indiceSecundario = nullptr;
  }
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
  } else if (comando == "SELECT" || comando == "INSERT" ||
             comando == "DELETE") {
    analizarDQL_DML(consulta, comando);
  } else if (comando == "HELP") {
    mostrarAyuda();
  } else {
    cout << "Error: Comando SQL no reconocido. Escriba HELP para mas "
            "informacion.\n";
  }
}

void AnalizadorSQL::analizarDDL(string consulta, string comando) {
  string consultaUpper = aMayusculas(consulta);

  if (comando == "CREATE") {
    if (consultaUpper.find("INDEX") != string::npos) {
      if (!indiceSecundario) {
        indiceSecundario = new ArbolBPlus(3, "indice_secundario.txt");
      }
      cout << "[OK] Indice secundario instanciado en memoria.\n";
    } else if (consultaUpper.find("TABLE") != string::npos) {
      tablaCreada = true;
      cout << "[OK] Tabla lista para operar.\n";
    }
  } else if (comando == "DROP") {
    bd->limpiar();
    remove("base_datos.txt");
    tablaCreada = false;
    cout << "[OK] Tabla y datos eliminados en disco y RAM.\n";
  }
}

void AnalizadorSQL::analizarDQL_DML(string consulta, string comando) {
  string consultaUpper = aMayusculas(consulta);

  if (comando == "INSERT") {
    if (!tablaCreada) {
      cout << "Error: No ha sido creada una tabla.\n";
      return;
    }

    size_t posValues = consultaUpper.find("VALUES");
    if (posValues != string::npos) {
      size_t parentesisIzq = consulta.find('(', posValues);
      size_t parentesisDer = consulta.find(')', parentesisIzq);
      if (parentesisIzq != string::npos && parentesisDer != string::npos) {
        string contenido = consulta.substr(parentesisIzq + 1,
                                           parentesisDer - parentesisIzq - 1);
        stringstream ss(contenido);
        string strId, datos;
        if (getline(ss, strId, ',')) {
          getline(ss, datos);
          try {
            int id = stoi(strId);

            // VALIDACIÓN DE DUPLICADOS
            if (!bd->buscar(id).empty()) {
              cout << "Error: El registro con ID " << id
                   << " ya existe en la base de datos.\n";
              return;
            }

            // Limpieza de comillas y espacios
            size_t inicio = datos.find_first_not_of(" '\"");
            size_t fin = datos.find_last_not_of(" '\"");
            if (inicio != string::npos && fin != string::npos) {
              datos = datos.substr(inicio, fin - inicio + 1);
            }

            bd->insertar(id, datos);
            cout << "[OK] Registro con ID " << id
                 << " insertado correctamente.\n";
          } catch (...) {
            cout << "Error: El ID debe ser un numero entero valido.\n";
          }
        }
      }
    }
  } else if (comando == "SELECT") {
    if (!tablaCreada) {
      cout << "Error: No ha sido creada una tabla.\n";
      return;
    }

    vector<Registro> todos = bd->obtenerTodos();
    if (todos.empty()) {
      cout << "-> No hay ningun dato registrado en la tabla.\n";
      return;
    }

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
      cout << "\n--- REGISTROS EN BASE DE DATOS ---\n";
      for (const auto& reg : todos) {
        cout << "[" << reg.clave << "] : " << reg.datos << "\n";
      }
      cout << "-----------------------------------\n";
    }
  } else if (comando == "DELETE") {
    if (!tablaCreada) {
      cout << "Error: No ha sido creada una tabla.\n";
      return;
    }

    vector<Registro> todos = bd->obtenerTodos();
    if (todos.empty()) {
      cout << "Error: No hay ningun dato registrado para eliminar.\n";
      return;
    }

    size_t posIgual = consulta.find('=');
    if (posIgual != string::npos) {
      string strId = consulta.substr(posIgual + 1);
      try {
        int id = stoi(strId);
        if (bd->buscar(id).empty()) {
          cout << "Error: El registro con ID " << id << " no existe.\n";
          return;
        }
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
  const string BOLD_WHITE = "\033[1;37m";

  cout << BOLD_YELLOW
       << "\n=== Sistema Gestor SQL basado en Arboles B+ ===" << RESET << "\n";
  cout << BOLD_WHITE << "Comandos Soportados:" << RESET << "\n";
  cout << BOLD_YELLOW << "  [DDL - Lenguaje de Definicion de Datos]" << RESET
       << "\n";
  cout << BOLD_CYAN << "    CREATE TABLE usuarios (id INT, nombre STR)" << RESET
       << "\n";
  cout << BOLD_CYAN << "    CREATE INDEX idx_nombre ON usuarios (nombre)"
       << RESET << "\n";
  cout << BOLD_CYAN << "    DROP TABLE usuarios" << RESET << "\n\n";
  cout << BOLD_YELLOW << "  [DQL / DML - Manipulacion y Consulta]" << RESET
       << "\n";
  cout << BOLD_CYAN << "    INSERT INTO usuarios VALUES (10, 'Juan Perez')"
       << RESET << "\n";
  cout << BOLD_CYAN << "    SELECT * FROM usuarios" << RESET << "\n";
  cout << BOLD_CYAN << "    SELECT * FROM usuarios WHERE id = 10" << RESET
       << "\n";
  cout << BOLD_CYAN << "    DELETE FROM usuarios WHERE id = 10" << RESET
       << "\n\n";
  cout << BOLD_YELLOW << "  [Otros Comandos]" << RESET << "\n";
  cout << BOLD_CYAN << "    HELP  - Muestra este menu" << RESET << "\n";
  cout << BOLD_CYAN << "    EXIT  - Guarda los datos y sale del programa"
       << RESET << "\n";
  cout << BOLD_YELLOW
       << "================================================" << RESET << "\n\n";
}
