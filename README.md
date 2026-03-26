<div align="center">
  <img src="https://github.com/user-attachments/assets/3f39eeb3-d724-4718-b62f-0af907fd9720"alt="ChessAI Logo" width="150"/>


  # ChessAI
  
  **Un motor de ajedrez de alto rendimiento escrito en C++ desde cero, diseñado bajo una arquitectura limpia (MVVM) en Qt/QML para la integración de Inteligencia Artificial.**

  [![C++](https://img.shields.io/badge/C++-17%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)]()
  [![Qt](https://img.shields.io/badge/Qt_6-41CD52?style=for-the-badge&logo=qt&logoColor=white)]()
  [![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)]()
  [![License](https://img.shields.io/badge/Licencia-MIT-green?style=for-the-badge)]()
</div>

---

## 📖 Acerca del Proyecto

**ChessAI** es un motor de ajedrez desarrollado de manera nativa en C++. Su núcleo es un backend lógico altamente optimizado, estructurado con estricta validación de reglas y manejo de memoria avanzado, diseñado para servir como el "cerebro" de una futura Inteligencia Artificial basada en el algoritmo Minimax.

La interfaz gráfica ha sido construida a medida utilizando **QML** y conectada al motor mediante una arquitectura híbrida de **Patrón Fachada y MVVM**, garantizando un desacoplamiento absoluto entre la representación visual y la lógica matemática pura.

---

## 🤔 Motivación y Problema

El desarrollo de motores de ajedrez tradicionales suele sufrir de un alto acoplamiento entre las reglas del juego y la interfaz visual, lo que dificulta la escalabilidad al integrar algoritmos complejos de toma de decisiones. 

**El problema a resolver:** Crear una plataforma donde la interfaz gráfica sea un ente 100% pasivo (reactivo) y la lógica matemática ofrezca operaciones transaccionales eficientes (como la reversión temporal en $O(1)$ para permitir que una futura IA simule millones de nodos por segundo sin colapsar la memoria ni el hilo principal de la aplicación.

---

## 🎯 Objetivos

### Objetivo General
Desarrollar un motor de ajedrez robusto y escalable en C++, con una interfaz gráfica desacoplada, sentando las bases estructurales y algorítmicas para implementar una Inteligencia Artificial escalable con múltiples niveles de dificultad.

### Objetivos Específicos
- Implementar la lógica completa del ajedrez (movimientos pseudo-legales, capturas al paso, enroques, promociones) con validación estricta y prevención de amenazas al rey (*Reverse Raycasting*).
- Establecer una arquitectura **MVVM y CQRS** para separar las responsabilidades del frontend (QML) y backend (C++).
- Construir un sistema de historiador temporal (*Undo/Redo* con doble pila) para el análisis de partidas y la traducción a Notación Algebraica Estándar (SAN).
- Optimizar la gestión de memoria evitando el uso de punteros desnudos, priorizando `std::unique_ptr` y semánticas de movimiento (`std::move`).

---

## 🏗️ Arquitectura y Diseño

El proyecto está diseñado bajo los principios **SOLID** y se divide modularmente en capas lógicas:

1. **Capa de Dominio (Backend Puro):** Contiene la lógica matemática (`Board`, `GameState`, `Piece`, `RuleValidator`). Maneja la máquina de estados y las transacciones espaciales.
2. **Capa Adaptadora (Fachada y MVVM):** El `ChessController` orquesta la UI aislando al motor. El `BoardModel` (hereda de `QAbstractListModel`) traduce la topología del tablero a datos reactivos. `MatchManager` aplica el principio CQRS.
3. **Capa Visual (Frontend):** Vistas declarativas en QML (`GameView.qml`, popups modales) que solo reaccionan a las señales de C++.

---

## 🎨 Diseño UI/UX

La interfaz fue conceptualizada y bocetada inicialmente utilizando **Google Stitch**, priorizando la ergonomía cognitiva para partidas largas.

* **Modo Oscuro Nativo:** Tonos `#11161d` y `#1a222b` para mitigar la fatiga visual, generando un alto contraste pasivo para resaltar las piezas y elementos interactivos.
* **Acentos Funcionales:**
    * 🔵 **Azul Cian (`#208ce8`):** Acción principal y flujo de progresión natural (ej. "Jugar", "Aceptar").
    * 🔴 **Rojo Advertencia (`#ff6b6b`):** Acciones destructivas (ej. "Rendición"), previniendo clics accidentales.
* **Jerarquía Tipográfica:**
    * *Rajdhani:* Para métricas, relojes y títulos (transmite un carácter moderno, analítico y técnico).
    * *Open Sans:* Para notación algebraica y lectura prolongada (máxima legibilidad).

---

## 🛠️ Tecnologías y Lenguajes

| Categoría | Tecnologías |
| :--- | :--- |
| **Lenguaje Core** | C++ (Estándar C++17) |
| **Frontend / UI** | Qt 6, QML |
| **Compilación** | CMake (`qt_add_qml_module` para AOT) |
| **Diseño UX/UI** | Google Stitch, Figma |

---

## 📂 Estructura del Proyecto

```text
ChessAI/
├── CMakeLists.txt
├── src/
│   ├── backend/          # Lógica pura: Board, GameState, Pieces, RuleValidator
│   ├── controllers/      # Adaptadores: ChessController, BoardModel, MatchManager
│   └── main.cpp          # Punto de entrada y configuración de Qt
├── include/              # Cabeceras (.hpp) de backend y controladores
├── ui/                   # Capa Visual
│   ├── assets/           # Iconos, piezas (SVG) y fuentes (TTF)
│   ├── components/       # Componentes reusables de QML (ChessBoardUI, ControlPanel)
│   └── views/            # Vistas principales (MenuView.qml, GameView.qml)
└── docs/                 # Documentación técnica, diagramas UML
```
## 🚀 **Instalación y Uso**
Requisitos Previos
- Compilador C++ compatible con C++17 (GCC, Clang, o MSVC).
- CMake (v3.16 o superior).
- Qt 6 instalado (incluyendo los módulos QtQml y QtQuick).

### **Pasos de Instalación**

1. **Clonar el repositorio:**
   ```bash
   git clone [https://github.com/tu-usuario/ChessAI.git](https://github.com/tu-usuario/ChessAI.git)
   cd ChessAI
   ```
2. **Crear directorio de compilación:**
   ```bash
   mkdir build && cd build
   ```
3. **Generar archivos de compilación y construir:**
   ```bash
   cmake ..
   cmake --build .
   ```
4. **Ejecutar el motor:**
   ```bash
   ./ChessAI
   ```
   
## 📸 Ejemplos y Resultados
Aquí puedes observar el sistema en funcionamiento, destacando el sistema de historial temporal y la fluidez de la interfaz.

| Menú Principal | Partida Activa |
| :---: | :---: |
| <img src= "https://github.com/user-attachments/assets/768d87ad-5bc0-4542-8dbe-962574d1f3e5"  width="400"/> | <img src= "https://github.com/user-attachments/assets/7a0f1a9b-a37d-4cc7-8cf7-8531c69300e8"  width="400"/> |

| Historial y Análisis Temporal | Promoción / Popups Modales |
| :---: | :---: |
| <img src= "https://github.com/user-attachments/assets/85b42714-422c-400c-a61f-d68c5b53a418" width="400"/> | <img src= "https://github.com/user-attachments/assets/af6e2421-2cf0-454b-81e0-ac63174f2988"  width="400"/> |


## 🔮 Limitaciones y Trabajo Futuro (Fase IA)

Actualmente, el proyecto ha completado su fase **MVP como motor funcional de ajedrez**. El trabajo futuro se enfoca directamente en la construcción de la **Inteligencia Artificial**:

* 🧠 **Algoritmo Minimax:** Implementar el árbol de búsqueda recursivo explotando las funciones `updateState`/`undoState` para operar sin *Memory Leaks*.
* ⚡ **Poda Alfa-Beta:** Integración de la mitigación de ramas subóptimas para permitir una mayor profundidad de búsqueda sin afectar la CPU.
* 📊 **Evaluación Heurística:** Programar el análisis estático matemático (`evaluateBoard`), ponderando material, seguridad del rey, y control del centro.
* 🧩 **Patrón Strategy:** Consolidar la jerarquía en `AIPlayer` según la dificultad seleccionada por el usuario (Principiante a Gran Maestro).
* *Opcional:* Integración de tablas de transposición para *memoización* de estados.

---

## 📚 Referencias

* Documentación oficial de Qt 6 / QML.
* Patrones de Diseño (GoF) y Principios SOLID aplicados a C++.
* Lógica algorítmica de Motores de Ajedrez Clásicos (Shannon, 1950).

---

## 👨‍💻 Autor

**Samuel Thomas Vásquez Gerena**
* LinkedIn: [Mi LinkedIn](https://www.linkedin.com/in/samuel-thomas-vasquez-gerena-5bbb553b2)

<br>

















   
