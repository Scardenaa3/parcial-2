#include "ArbolBPlus.h"

ArbolBPlus::ArbolBPlus(int _grado, string _nombre_archivo)
    : raiz(nullptr), grado(_grado), nombre_archivo(_nombre_archivo) {}

ArbolBPlus::~ArbolBPlus() {
    liberarMemoria(raiz);
}

void ArbolBPlus::liberarMemoria(NodoBPlus* nodo) {
    if (!nodo) return;
    if (!nodo->es_hoja) {
        for (NodoBPlus* hijo : nodo->hijos) {
            liberarMemoria(hijo);
        }
    }
    delete nodo;
}

void ArbolBPlus::limpiar() {
    liberarMemoria(raiz);
    raiz = nullptr;
}

void ArbolBPlus::insertar(int clave, string datos) {
    Registro nuevoReg = {clave, datos};

    if (raiz == nullptr) {
        raiz = new NodoBPlus(true);
        raiz->registros.push_back(nuevoReg);
        raiz->claves.push_back(clave);
        return;
    }

    NodoBPlus* cursor = raiz;
    NodoBPlus* padre = nullptr;

    while (!cursor->es_hoja) {
        padre = cursor;
        for (size_t i = 0; i < cursor->claves.size(); i++) {
            if (clave < cursor->claves[i]) {
                cursor = cursor->hijos[i];
                break;
            }
            if (i == cursor->claves.size() - 1) {
                cursor = cursor->hijos[i + 1];
                break;
            }
        }
    }

    auto itReg = cursor->registros.begin();
    auto itClave = cursor->claves.begin();
    while (itReg != cursor->registros.end() && itReg->clave < clave) {
        itReg++;
        itClave++;
    }
    cursor->registros.insert(itReg, nuevoReg);
    cursor->claves.insert(itClave, clave);

    if (cursor->registros.size() >= (size_t)grado) {
        NodoBPlus* nuevaHoja = new NodoBPlus(true);
        size_t mitad = (grado + 1) / 2;

        nuevaHoja->registros.assign(cursor->registros.begin() + mitad, cursor->registros.end());
        nuevaHoja->claves.assign(cursor->claves.begin() + mitad, cursor->claves.end());

        cursor->registros.erase(cursor->registros.begin() + mitad, cursor->registros.end());
        cursor->claves.erase(cursor->claves.begin() + mitad, cursor->claves.end());

        nuevaHoja->siguiente_hoja = cursor->siguiente_hoja;
        cursor->siguiente_hoja = nuevaHoja;

        if (cursor == raiz) {
            NodoBPlus* nuevaRaiz = new NodoBPlus(false);
            nuevaRaiz->claves.push_back(nuevaHoja->claves[0]);
            nuevaRaiz->hijos.push_back(cursor);
            nuevaRaiz->hijos.push_back(nuevaHoja);
            raiz = nuevaRaiz;
        } else {
            insertarInterno(nuevaHoja->claves[0], padre, nuevaHoja);
        }
    }
}

void ArbolBPlus::insertarInterno(int clave, NodoBPlus* cursor, NodoBPlus* hijo) {
    auto itClave = cursor->claves.begin();
    auto itHijo = cursor->hijos.begin() + 1;

    while (itClave != cursor->claves.end() && *itClave < clave) {
        itClave++;
        itHijo++;
    }

    cursor->claves.insert(itClave, clave);
    cursor->hijos.insert(itHijo, hijo);

    if (cursor->claves.size() >= (size_t)grado) {
        NodoBPlus* nuevoInterno = new NodoBPlus(false);
        size_t mitad = cursor->claves.size() / 2;
        int clavePromovida = cursor->claves[mitad];

        nuevoInterno->claves.assign(cursor->claves.begin() + mitad + 1, cursor->claves.end());
        nuevoInterno->hijos.assign(cursor->hijos.begin() + mitad + 1, cursor->hijos.end());

        cursor->claves.erase(cursor->claves.begin() + mitad, cursor->claves.end());
        cursor->hijos.erase(cursor->hijos.begin() + mitad + 1, cursor->hijos.end());

        if (cursor == raiz) {
            NodoBPlus* nuevaRaiz = new NodoBPlus(false);
            nuevaRaiz->claves.push_back(clavePromovida);
            nuevaRaiz->hijos.push_back(cursor);
            nuevaRaiz->hijos.push_back(nuevoInterno);
            raiz = nuevaRaiz;
        } else {
            NodoBPlus* padre = buscarPadre(raiz, cursor);
            insertarInterno(clavePromovida, padre, nuevoInterno);
        }
    }
}

NodoBPlus* ArbolBPlus::buscarPadre(NodoBPlus* cursor, NodoBPlus* hijo) {
    if (cursor->es_hoja || cursor->hijos[0]->es_hoja) return nullptr;

    for (size_t i = 0; i < cursor->hijos.size(); i++) {
        if (cursor->hijos[i] == hijo) return cursor;
        NodoBPlus* padre = buscarPadre(cursor->hijos[i], hijo);
        if (padre != nullptr) return padre;
    }
    return nullptr;
}

string ArbolBPlus::buscar(int clave) {
    if (raiz == nullptr) return "";

    NodoBPlus* cursor = raiz;
    while (!cursor->es_hoja) {
        for (size_t i = 0; i < cursor->claves.size(); i++) {
            if (clave < cursor->claves[i]) {
                cursor = cursor->hijos[i];
                break;
            }
            if (i == cursor->claves.size() - 1) {
                cursor = cursor->hijos[i + 1];
                break;
            }
        }
    }

    for (const auto& reg : cursor->registros) {
        if (reg.clave == clave) return reg.datos;
    }
    return "";
}

void ArbolBPlus::eliminar(int clave) {
    if (!raiz) return;

    NodoBPlus* cursor = raiz;
    while (!cursor->es_hoja) {
        for (size_t i = 0; i < cursor->claves.size(); i++) {
            if (clave < cursor->claves[i]) {
                cursor = cursor->hijos[i];
                break;
            }
            if (i == cursor->claves.size() - 1) {
                cursor = cursor->hijos[i + 1];
                break;
            }
        }
    }

    for (auto it = cursor->registros.begin(); it != cursor->registros.end(); ++it) {
        if (it->clave == clave) {
            cursor->registros.erase(it);
            break;
        }
    }

    for (auto it = cursor->claves.begin(); it != cursor->claves.end(); ++it) {
        if (*it == clave) {
            cursor->claves.erase(it);
            break;
        }
    }
}

vector<Registro> ArbolBPlus::obtenerTodos() {
    vector<Registro> resultado;
    if (raiz == nullptr) return resultado;

    NodoBPlus* cursor = raiz;
    while (!cursor->es_hoja) {
        cursor = cursor->hijos[0];
    }

    while (cursor != nullptr) {
        for (const auto& reg : cursor->registros) {
            resultado.push_back(reg);
        }
        cursor = cursor->siguiente_hoja;
    }

    return resultado;
}

void ArbolBPlus::guardarEnArchivo() {
    ofstream archivo(nombre_archivo);
    if (!archivo.is_open()) return;

    vector<Registro> todos = obtenerTodos();
    for (const auto& reg : todos) {
        archivo << reg.clave << "," << reg.datos << "\n";
    }
    archivo.close();
}

void ArbolBPlus::cargarDesdeArchivo() {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) return;

    limpiar();
    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string tempClave, datos;
        if (getline(ss, tempClave, ',') && getline(ss, datos)) {
            try {
                int clave = stoi(tempClave);
                // Limpieza de espacios y comillas al cargar desde disco
                size_t inicio = datos.find_first_not_of(" '\"");
                size_t fin = datos.find_last_not_of(" '\"");
                if (inicio != string::npos && fin != string::npos) {
                    datos = datos.substr(inicio, fin - inicio + 1);
                }
                insertar(clave, datos);
            } catch (...) {}
        }
    }
    archivo.close();
}
