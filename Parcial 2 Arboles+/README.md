# Parcial 2: Motor de Base de Datos SQL (Árboles B+)

Este repositorio contiene la especificación, arquitectura y código fuente completo del **Motor de Base de Datos SQL** desarrollado en **C++17 / C++11**. El sistema utiliza un **Árbol B+ (Grado 3)** para la indexación eficiente de datos en memoria RAM y cuenta con persistencia de archivos en disco.

---

## 1. Resumen de la Implementación

Se completó el desarrollo del motor de base de datos en C++, integrando la capa de parsing SQL directo con la estructura de datos persistente basada en Árboles B+.

### Aspectos Clave Desarrollados:
* **Estructura del Árbol B+ (`ArbolBPlus`):**
  * Implementación manual de nodos internos y hojas mediante punteros C++.
  * Algoritmo de inserción ordenada con división (*split*) de nodos y promoción ascendente de claves hacia los nodos padres.
  * Conexión secuencial entre nodos hoja (`siguiente_hoja`) para soporte eficiente de consultas de rango e inspección completa (*Full Table Scan*).
  * Búsqueda logarítmica descendente $O(\log N)$ desde la raíz hacia las hojas.
* **Analizador Léxico y Sintáctico (`AnalizadorSQL`):**
  * Procesamiento sintáctico de comandos DDL (`CREATE TABLE`, `CREATE INDEX`, `DROP TABLE`).
  * Procesamiento de comandos DML y DQL (`INSERT INTO`, `SELECT *`, `SELECT WHERE`, `DELETE`).
  * Sincronización de índices secundarios mediante la instanciación de un segundo Árbol B+ mapeado a la clave primaria.
* **Persistencia de Datos:**
  * Métodos de serialización y deserialización para guardar y recuperar el estado de los datos en el archivo físico `base_datos.txt`.
* **Manejo de Errores y Gestión de Memoria:**
  * **Solución de corrupción de memoria (`free(): invalid pointer`):** Aseguramiento de la integridad de cadenas y punteros al cerrar la aplicación.
  * **Validación de tabla:** Exige la creación explícita de la tabla antes de realizar operaciones DML/DQL.
  * **Validación de duplicados y estado:** Bloqueo de inserciones con `id` existente y notificaciones informativas ante tablas vacías.

---

## 2. Arquitectura del Sistema

El proyecto está diseñado bajo una separación de responsabilidades en tres capas principales:

```mermaid
flowchart TD
    A[Interfaz CLI - main.cpp] -->|Comando SQL| B(Parser SQL - AnalizadorSQL)
    B -->|DDL: CREATE/DROP| C{Controlador B+}
    B -->|DML: INSERT/DELETE| C
    B -->|DQL: SELECT| C
    C <-->|Manejo de Nodos| D[(Estructura B+ en RAM - ArbolBPlus)]
    D <-->|Serialización / Deserialización| E[Persistencia: base_datos.txt]
