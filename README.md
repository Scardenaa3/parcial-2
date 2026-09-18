# Especificación del Parcial 2: Motor de Base de Datos SQL (Árboles B+)

Este documento establece los requerimientos, la arquitectura y los detalles de implementación del Motor de Base de Datos SQL basado en Árboles B+.

---

## 1. Resumen de la Implementación

Se completó el desarrollo del motor de base de datos en C++, integrando la capa de parsing SQL con la estructura de datos persistente basada en Árboles B+.

### Aspectos Clave Desarrollados:
* **Estructura del Árbol B+ (`ArbolBPlus`):**
  * Implementación manual de nodos internos y hojas mediante punteros C++.
  * Algoritmo de inserción ordenada con división (*split*) de nodos y promoción ascendente de claves hacia los nodos padres.
  * Conexión secuencial entre nodos hoja (`siguiente_hoja`) para soporte eficiente de consultas de rango e inspección completa (*Full Table Scan*).
  * Búsqueda logarítmica descendente $O(\log N)$ desde la raíz hacia las hojas.
* **Analizador Léxico y Sintáctico (`AnalizadorSQL`):**
  * Procesamiento de comandos DDL (`CREATE TABLE`, `CREATE INDEX`, `DROP TABLE`).
  * Procesamiento de comandos DML y DQL (`INSERT INTO`, `SELECT *`, `SELECT WHERE`).
  * Sincronización de índices secundarios mediante la instanciación de un segundo Árbol B+ mapeado a la clave primaria.
* **Persistencia de Datos:**
  * Métodos de serialización y deserialización para guardar y recuperar el estado de los datos en el archivo físico `base_datos.txt`.

---

## 2. Arquitectura del Sistema

El proyecto está diseñado bajo una separación de responsabilidades en tres capas principales:

```mermaid
flowchart TD
    A[Interfaz CLI - main.cpp] -->|Instrucción en texto| B(Parser SQL - AnalizadorSQL)
    B -->|DDL: CREATE/DROP| C{Controlador B+}
    B -->|DML: INSERT/DELETE| C
    B -->|DQL: SELECT| C
    C <-->|Manejo de Nodos| D[(Estructura B+ en RAM - ArbolBPlus)]
    D <-->|Serialización / Deserialización| E[Persistencia: base_datos.txt]
