# Especificación del Parcial 2: Motor de Base de Datos SQL (Árboles B+)

Este documento establece los requerimientos, la arquitectura esperada y la rúbrica de evaluación para el desarrollo del Motor de Base de Datos SQL basado en Árboles B+.

> [!IMPORTANT]
> **Fecha límite de entrega:** Viernes 18 de septiembre a las 12:00 M (Mediodía).
> **Plataforma:** Buzón de entrega de EAFIT Interactiva.

---

## 1. Descripción del Problema

El objetivo fundamental de la práctica es consolidar los conceptos de **Estructuras de Datos y Algoritmos**. Para ello, se requiere completar un esqueleto de código en `C++` que conecta un analizador léxico/sintáctico de comandos SQL (Parser) con una estructura de almacenamiento persistente basada en **Árboles B+**.

El sistema procesará instrucciones en formato de texto (DDL y DML/DQL), interactuará de manera eficiente con el árbol B+ en memoria principal y garantizará la persistencia de los datos en un archivo físico (`base_datos.txt`).

---

## 2. Arquitectura del Sistema

El proyecto está diseñado bajo una separación de responsabilidades en tres capas principales:

```mermaid
flowchart TD
    A[Interfaz CLI] -->|Instrucción en texto| B(Parser SQL)
    B -->|DDL: CREATE/DROP| C{Controlador B+}
    B -->|DML: INSERT/DELETE| C
    B -->|DQL: SELECT| C
    C <-->|Manejo de Nodos| D[(Estructura B+ en RAM)]
    D <-->|Serialización / Deserialización| E[Persistencia: base_datos.txt]