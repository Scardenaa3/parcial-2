# 🗄️ Sistema Gestor de Base de Datos SQL (Motor Árbol B+)

Este proyecto es una implementación de un **Motor de Base de Datos y Analizador SQL en C++17** respaldado por la estructura de datos de un **Árbol B+ (Grado 3)** para almacenamiento indexado en memoria RAM y persistencia en disco.

---

## 🚀 Características Principales

* **Estructura Árbol B+ (Grado 3):** Búsqueda, inserción y eliminación eficiente con punteros entre nodos hoja para recorridos secuenciales rápidos.
* **Analizador SQL Interactivo:** Interfaz por consola CLI capaz de parsear e interpretar sentencias DDL, DQL y DML.
* **Persistencia de Datos:** Carga automática desde `base_datos.txt` al iniciar y guardado de estado al salir con el comando `EXIT`.
* **Validación Rigurosa de Errores y Memoria:**
  * Control de acceso seguro a punteros en C++ (prevención de `free(): invalid pointer`).
  * Validación de existencia de tabla (exige un `CREATE TABLE` antes de manipular registros).
  * Control de duplicados en clave primaria (`id`).
  * Mensajes informativos para tablas vacías o consultas sin coincidencia.

---

## 🛠️ Comandos SQL Soportados

### 1. Definición de Datos (DDL)
| Comando | Descripción |
| :--- | :--- |
| `CREATE TABLE usuarios (id INT, nombre STR)` | Habilita la tabla para operar en la sesión. |
| `CREATE INDEX idx_nombre ON usuarios (nombre)` | Instancia la estructura del índice secundario. |
| `DROP TABLE usuarios` | Limpia los registros en RAM y elimina el archivo `base_datos.txt`. |

### 2. Manipulación y Consulta (DML / DQL)
| Comando | Descripción |
| :--- | :--- |
| `INSERT INTO usuarios VALUES (id, 'nombre')` | Inserta un nuevo registro verificando que el `id` no esté duplicado. |
| `SELECT * FROM usuarios` | Muestra todos los registros guardados. |
| `SELECT * FROM usuarios WHERE id = X` | Búsqueda indexada directa por clave primaria `id`. |
| `DELETE FROM usuarios WHERE id = X` | Elimina un registro por `id`. |

### 3. Comandos Generales
| Comando | Descripción |
| :--- | :--- |
| `HELP` | Muestra la guía interactiva con la sintaxis de comandos. |
| `EXIT` / `QUIT` | Guarda las modificaciones en `base_datos.txt` y finaliza la ejecución. |

---

## 💻 Compilación y Ejecución (Linux / WSL)

Para compilar el proyecto utilizando `g++` en un entorno de **Linux** o **Windows Subsystem for Linux (WSL)**:

```bash
# Compilar todos los archivos fuentes en C++17
g++ -std=c++17 -Wall main.cpp ArbolBPlus.cpp AnalizadorSQL.cpp -o bd_sql

# Ejecutar el motor de base de datos
./bd_sql
