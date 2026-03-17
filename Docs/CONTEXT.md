# Contexto del Proyecto: ChessAI (Actualizado)

## 1. Resumen Ejecutivo
ChessAI es un motor de ajedrez escrito desde cero en C++. El objetivo del proyecto es construir un backend lógico robusto, altamente optimizado y con validación estricta de reglas, que posteriormente servirá de cerebro para una Inteligencia Artificial basada en el algoritmo Minimax. La interfaz gráfica se desarrolla a medida utilizando QML, conectada al motor mediante una arquitectura Model/View de alto rendimiento.

## 2. Stack Tecnológico y Entorno
- **Lenguaje:** C++ (Estándar C++17 o superior).
- **Sistema de Compilación:** CMake.
- **Interfaz Gráfica:** Qt / QML (compilado Ahead-of-Time mediante `qt_add_qml_module`).
- **Patrón de Interfaz:** Modelo-Vista-Adaptador usando `QAbstractListModel` para evitar recargas completas del DOM en QML.
- **Testing:** Actualmente pruebas de estrés manuales en `main.cpp`. En el futuro se planea integrar un framework profesional como Google Test.

## 3. Arquitectura y Estructura Principal
El proyecto se divide estrictamente entre la lógica pura (Backend) y la capa de presentación (Frontend/Adaptadores):

### Capa de Dominio (Backend Puro)
- **Board:** Gestiona la matriz física de 8x8 y la memoria de las piezas.
- **GameState:** Almacena el turno actual, el historial de movimientos (`std::stack`) y el estado de reglas especiales (enroques, captura al paso).
- **Piece (y derivadas):** Clases que calculan las direcciones puramente geométricas (movimientos pseudo-legales).
- **Game:** Controlador principal (Máquina de estados). Orquesta los turnos, valida condiciones de victoria/empate.
- **Player / RuleValidator / Move:** Polimorfismo de jugadores, cálculo de amenazas sin instanciación (Reverse Raycasting) y encapsulamiento estricto de movimientos para `undoState`.

### Capa Adaptadora y Presentación (Nuevo)
- **BoardModel (Hereda de `QAbstractListModel`):** Actúa como el ViewModel. Su única responsabilidad es traducir el estado de `Board` a "Roles" (`pieceColor`, `pieceIcon`, `isSelected`) que QML pueda entender. Gestiona actualizaciones parciales de casillas (ej. `dataChanged()`) para un rendimiento óptimo.

- **ChessController:** Orquestador de la UI. Recibe inputs (clics) desde QML, consulta al `Game` si el movimiento es legal, y ordena al `BoardModel` repintar la vista.

- **GameView.qml:** Vista puramente declarativa ("tonta"). No posee estado lógico ni variables de selección; obedece 100% a los roles expuestos por `BoardModel`.

- **Estructura de carpetas:** Las declaraciones están en `/include` y las implementaciones en `/src`. Los archivos `.cpp` siempre deben incluir sus cabeceras usando rutas relativas como  
  `#include "../include/NombreClase.hpp"`.

## 4. Reglas, Convenciones y Patrones (ESTRICTO)

- **Gestión de Memoria:** Prohibido el uso de `new/delete` desnudos. La propiedad de las piezas la maneja exclusivamente `std::unique_ptr<Piece>`. Para observar se usan raw pointers (`Piece*`).

- **Move Semantics:** La clase `Move` NO es copiable (`= delete` en el constructor de copia). Todo movimiento de captura se hace con `std::move`.

- **Operación Inversa (Optimización de IA):** Se utiliza `updateState(move)` y `undoState(move)` en lugar de clonar tableros.

- **Principio de Responsabilidad Única (SRP) en UI:** QML tiene estrictamente prohibido llevar el control de turnos o estado de selecciones. C++ es la única fuente de la verdad.

- **Inyección de Dependencias:** El motor (`Game`) no construye a los jugadores. Los jugadores se inyectan. La memoria se gestiona por valor.

## 5. Lógica de Dominio

- **Sistema de Coordenadas:** Matriz bidimensional de 8x8 (`grid[row][col]`).  
  Fila 0 (Negras / top), Fila 7 (Blancas / bottom).

- **Generación de movimientos:** Arreglos estáticos y Reverse Raycasting para optimización de IA.

- **Anotación y Registro:** El `GameLogger` deberá traducir los objetos `Move` a Notación Algebraica Estándar para el historial en la UI. Se implementará lectura/escritura de FEN (Forsyth-Edwards Notation).

## 6. Estado Actual y Siguiente Objetivo

### Completado
1. MVP del motor funcional en consola con validación de todas las reglas.  
2. Integración de CMake con Qt y AUTOMOC.  
3. Maquetación base de la UI en QML (tablero, paneles laterales, reloj).

### Próximos pasos inmediatos

- **Refactorización a `QAbstractListModel`:** Implementar `BoardModel` en C++ para reemplazar las listas estáticas (`QVariantList`). Esto solucionará los errores de destrucción de memoria en QML y habilitará animaciones fluidas.

- **Limpieza de QML:** Eliminar la lógica de estado en `GameView.qml` y atar las propiedades visuales directamente a los Roles de C++.

- **Historial de Movimientos:** Conectar el panel lateral derecho de QML al registro de Notación Algebraica Estándar (`GameLogger`).

- **Fase de IA:** Estructurar el árbol Minimax (con poda Alfa-Beta) implementando el patrón Strategy en `AIPlayer`.
