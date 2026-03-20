# Contexto del Proyecto: ChessAI (Actualizado)

## 1. Resumen Ejecutivo
ChessAI es un motor de ajedrez escrito desde cero en C++. El objetivo del proyecto es construir un backend lógico robusto, altamente optimizado y con validación estricta de reglas, que posteriormente servirá de cerebro para una Inteligencia Artificial basada en el algoritmo Minimax. La interfaz gráfica se desarrolla a medida utilizando QML, conectada al motor mediante una arquitectura híbrida de **Patrón Fachada y MVVM (Model-View-ViewModel)** de alto rendimiento.

## 2. Stack Tecnológico y Entorno
- **Lenguaje:** C++ (Estándar C++17 o superior).
- **Sistema de Compilación:** CMake.
- **Interfaz Gráfica:** Qt / QML (compilado Ahead-of-Time mediante `qt_add_qml_module`).
- **Patrón de Interfaz:** MVVM y CQRS. Se usa `QAbstractListModel` para evitar recargas completas del DOM en QML.
- **Testing:** Actualmente pruebas de estrés manuales en `main.cpp`. En el futuro se planea integrar un framework profesional como Google Test.

## 3. Arquitectura y Estructura Principal
El proyecto se divide estrictamente entre la lógica pura (Backend) y la capa de presentación (Frontend/Adaptadores). Ambos módulos siguen la estructura profesional de separar declaraciones e implementaciones en carpetas `/include` y `/src`.

### Capa de Dominio (Backend Puro)
- **Board:** Gestiona la matriz física de 8x8 y la memoria de las piezas.
- **GameState:** Almacena el turno actual, el historial de movimientos (`std::stack`) y el estado de reglas especiales (enroques, captura al paso).
- **Piece (y derivadas):** Clases polimórficas que calculan direcciones pseudo-legales y se identifican mediante `getTypeName()`.
- **Game:** Controlador principal del dominio (Máquina de estados). Orquesta los turnos y actúa como filtro de datos (DTOs) hacia la UI.
- **Player / RuleValidator / Move:** Cálculo de amenazas sin instanciación (Reverse Raycasting) y encapsulamiento estricto de movimientos en memoria mediante `unique_ptr` para `undoState`.

### Capa Adaptadora y Presentación (Frontend C++/Qt)
- **ChessController (Fachada):** Orquestador principal de la UI. Recibe inputs (clics) desde QML, orquesta la coronación y coordina a los submódulos. Delega todo el trabajo pesado, manteniendo un bajo acoplamiento.
- **BoardModel (MVVM - ViewModel):** Hereda de `QAbstractListModel`. Traduce el estado de `Board` a "Roles" (`pieceColor`, `isValidMove`, `isInCheck`). Resuelve dinámicamente las rutas de los archivos `.svg` y gestiona actualizaciones parciales (`dataChanged()`).
- **MatchManager (Árbitro - CQRS):** Separa lógicamente la gestión de final de partida. Maneja acciones que mutan el estado (rendirse, ofrecer tablas) y expone propiedades reactivas pasivas (`isGameOver`, `gameOverReason`) a QML.
- **GameClock:** Gestiona la asincronía del tiempo de juego utilizando `QTimer`, exponiendo textos formateados a la UI sin bloquear el hilo principal.
- **Vistas QML (`GameView.qml`, `ChessBoardUI.qml`):** Vistas puramente declarativas ("tontas"). No poseen estado lógico ni variables de selección; obedecen 100% a las propiedades expuestas por los controladores de C++.

## 4. Reglas, Convenciones y Patrones (ESTRICTO)

- **Gestión de Memoria:** Prohibido el uso de `new/delete` desnudos. La propiedad de las piezas la maneja exclusivamente `std::unique_ptr<Piece>`. Para observar se usan raw pointers (`Piece*`).
- **Move Semantics:** La clase `Move` NO es copiable (`= delete` en el constructor de copia). Todo movimiento de captura se hace con `std::move`.
- **Bajo Acoplamiento UI-Backend (DTOs):** La interfaz gráfica jamás interactúa directamente con objetos complejos del dominio (como la clase `Move`). El backend debe filtrar la información y entregar tipos primitivos o estructuras simples (ej. un `std::vector<Position>`) para evitar fugas de memoria y dependencias fuertes.
- **Operación Inversa (Optimización de IA):** Se utiliza `updateState(move)` y `undoState(move)` en lugar de clonar tableros.
- **Principio CQRS en UI:** Las acciones que alteran el estado del juego (Commands) deben estar separadas de las consultas visuales del estado (Queries). QML lee el estado desde los sub-modelos (`chessController.match.isGameOver`).
- **Principio de Responsabilidad Única (SRP):** QML tiene estrictamente prohibido llevar el control de turnos o estado de selecciones. C++ es la única fuente de la verdad.

## 5. Lógica de Dominio

- **Sistema de Coordenadas:** Matriz bidimensional de 8x8 (`grid[row][col]`).  
  Fila 0 (Negras / top), Fila 7 (Blancas / bottom).
- **Generación de movimientos:** Arreglos estáticos y Reverse Raycasting para optimización de IA.
- **Anotación y Registro:** El `GameLogger` deberá traducir los objetos `Move` a Notación Algebraica Estándar para el historial en la UI. Se implementará lectura/escritura de FEN (Forsyth-Edwards Notation).

## 6. Estado Actual y Siguiente Objetivo

### Completado
1. MVP del motor funcional en consola con validación de todas las reglas.  
2. Integración de CMake con Qt y estructuración de carpetas (`controllers/include`, `controllers/src`, `backend/...`).
3. Arquitectura MVVM implementada: `BoardModel` (`QAbstractListModel`) gestiona el renderizado parcial en C++ eliminando recargas del DOM.
4. Desacoplamiento de UI completado: El `ChessController` funciona como Fachada, apoyado por `MatchManager` (CQRS) y `GameClock`, logrando que QML sea un cliente declarativo 100% pasivo.

### Próximos pasos inmediatos
- **GameLogger:** Crear el GameLogger para tener el registro de los movimientos en la partida.
- **Deshacer:** Conectar el undo con el botón del QML, y hacerlo usable en el análisis de la partida, también crear un botón contrario a este para desplazarse en el historial de movimientos 
- **Historial de Movimientos:** Conectar el panel lateral derecho de QML al registro de Notación Algebraica Estándar (`GameLogger`) del backend.
- **Fase de IA (Minimax):** Estructurar el árbol de decisiones implementando el patrón Strategy en la clase `AIPlayer`.
- **Evaluación Heurística:** Programar las métricas de evaluación de tablero (seguridad del rey, valor de piezas, control del centro) requeridas para la IA.
- **Poda Alfa-Beta:** Optimizar la IA para alcanzar de forma eficiente los niveles de dificultad más altos.