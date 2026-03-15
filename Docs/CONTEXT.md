\# Contexto del Proyecto: ChessAI



\## 1. Resumen Ejecutivo

\*\*ChessAI\*\* es un motor de ajedrez escrito desde cero en C++. El objetivo del proyecto es construir un backend lógico robusto, altamente optimizado y con validación estricta de reglas, que posteriormente servirá de cerebro para una Inteligencia Artificial basada en el algoritmo Minimax. La interfaz gráfica será desarrollada a medida utilizando QML.



\## 2. Stack Tecnológico y Entorno

\* \*\*Lenguaje:\*\* C++ (Estándar C++17 o superior).

\* \*\*Sistema de Compilación:\*\* CMake.

\* \*\*Interfaz Gráfica (Futuro):\*\* Qt / QML (compilado Ahead-of-Time mediante `qt\\\_add\\\_qml\\\_module`).

\* \*\*Testing:\*\* Actualmente pruebas de estrés manuales en `main.cpp`. En el futuro se planea integrar un framework profesional como Google Test.



\## 3. Arquitectura y Estructura Principal

El backend separa la lógica del estado, las reglas y la generación de jugadas:

\* `Board`: Gestiona la matriz física de 8x8 y la memoria de las piezas.

\* `GameState`: Almacena el turno actual, el historial de movimientos (`std::stack`) y el estado de reglas especiales (enroques, captura al paso).

\* `Piece` (y sus clases derivadas como `Pawn`, `King`...): Clases que calculan las direcciones puramente geométricas (movimientos pseudo-legales).

\* Game: Controlador principal (Máquina de estados). Orquesta los turnos, valida condiciones de victoria/empate y actúa como fachada para la interfaz externa, sin involucrarse en la lógica matemática.

\* Player (y HumanPlayer / AIPlayer): Interfaz polimórfica (DIP) que delega la toma de decisiones. Permite al motor solicitar jugadas sin conocer si interactúa con la consola, una GUI o un algoritmo.

\* RuleValidator: Clase de utilidad estática (Stateless). Calcula las "líneas de visión" mediante escaneo inverso (Reverse Raycasting) para detectar amenazas, sin instanciar objetos en memoria (optimización crítica para la IA).

\* `Move`: Encapsula el origen, destino y retiene la memoria de las piezas capturadas.



"Estructura de carpetas: Las declaraciones están en /include y las implementaciones en /src. Los archivos .cpp siempre deben incluir sus cabeceras usando rutas relativas como #include "../include/NombreClase.hpp"."



\## 4. Reglas, Convenciones y Patrones (ESTRICTO)

\* \*\*Gestión de Memoria:\*\* Prohibido el uso de `new`/`delete` desnudos. La propiedad de las piezas la maneja exclusivamente `std::unique\\\_ptr<Piece>`. Para observar o leer piezas, se usan raw pointers (`Piece\\\*`).

\* \*\*Move Semantics:\*\* La clase `Move` contiene un `unique\\\_ptr` de la pieza capturada, por lo que \*\*NO es copiable\*\* (`= delete` en el constructor de copia). Todo movimiento entre vectores debe hacerse con `std::move`.

\* \*\*Operación Inversa (Optimización de IA):\*\* Para simular jugadas (y a futuro en el árbol Minimax), no se clona el tablero. Se utiliza `updateState(move)` para avanzar en el tiempo, y `undoState(move)` para devolver las piezas a su origen y restaurar la memoria de la pieza capturada directamente desde el objeto `Move`.

\* \*\*Inyección de Dependencias: El motor (Game) no construye a los jugadores. Estos se inyectan como punteros (Player*) en su constructor. La memoria de la partida (GameState, MoveGenerator) se gestiona por valor (creación automática en el stack/objeto padre) para evitar sobrecarga del heap.



\## 5. Lógica de Dominio

\* \*\*Sistema de Coordenadas:\*\* Matriz bidimensional de 8x8 (`grid\\\[row]\\\[col]`).

&#x20;   \* \*\*Fila 0:\*\* Parte superior (Piezas Negras / equivalentes a la fila 8 del tablero real).

&#x20;   \* \*\*Fila 7:\*\* Parte inferior (Piezas Blancas / equivalentes a la fila 1 del tablero real).

\* \*\*Generación de movimientos:\*\* El validador usa arreglos de direcciones estáticas (ej. `{{1, 1}, {1, -1}...}`) para que las piezas calculen sus jugadas mediante ciclos tipo "radar" (raycasting), deteniéndose al chocar con piezas amigas o enemigas.



"Anotación y Registro: El GameLogger deberá traducir los objetos Move a Notación Algebraica Estándar para el registro de partidas. A futuro, el tablero debería ser capaz de leer e imprimir cadenas FEN (Forsyth-Edwards Notation) para inicializar posiciones específicas."



\## 6. Estado Actual y Siguiente Objetivo

\* \*\*Completado:\*\* Un MVP completado, un juego de ajedrez con todas sus reglas y lógicas que se puede jugar a través de movimientos como "e2 e4" mediante la consola, también se integro un tablero a través de la terminal.

\* \*\*Próximos pasos inmediatos:\*\*

1. Configuración del Entorno: Modificar CMakeLists.txt para enlazar las librerías de Qt y habilitar AUTOMOC (necesario para las macros de C++).

2.Desarrollo del Puente (Backend-Frontend): Crear la clase ChessController heredando de QObject, utilizando Q_PROPERTY y Q_INVOKABLE para exponer el estado del tablero (getBoard) y recibir inputs de la interfaz.

3.Diseño Visual (Frontend): Construir la cuadrícula 8x8 y las piezas de forma declarativa utilizando QML, conectando los eventos de clic al controlador.

4.Fase de IA: Una vez la GUI sea estable, comenzar la estructura del árbol de decisiones Minimax (con poda Alfa-Beta) implementando el patrón Strategy en AIPlayer.


