cmake_minimum_required(VERSION 3.16)

# Nombre del proyecto
project(ChessAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configuraciones mágicas de Qt (Necesarias para la interfaz gráfica)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 2. Buscar las librerías gráficas de Qt (Widgets es para botones, ventanas, etc.)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# 3. AQUÍ ES DONDE APARECERÁN TUS ARCHIVOS
# Lista todos tus archivos .cpp aquí 
set(SOURCES
    src/Game.cpp
    src/Board.cpp
    src/GameState.cpp
    src/AIPlayer.cpp
    src/HumanPlayer.cpp
    src/Piece.cpp
    src/Pawn.cpp
    src/Player.cpp
    src/Rook.cpp
    src/Bishop.cpp
    src/Knight.cpp
    src/Queen.cpp
    src/King.cpp
    src/Move.cpp
    src/Position.cpp
    src/MoveGenerator.cpp
    src/RuleValidator.cpp
    test/Test_jaqueMatePastor.cpp
    main.cpp

)

# 4. Lista tus archivos de cabecera (.hpp o .h) para que Qt Creator los muestre
set(HEADERS
    include/Game.hpp
    include/Board.hpp
    include/GameState.hpp
    include/AIPlayer.hpp
    include/HumanPlayer.hpp
    include/Piece.hpp
    include/Pawn.hpp
    include/Player.hpp
    include/Rook.hpp
    include/Bishop.hpp
    include/Knight.hpp
    include/Queen.hpp
    include/King.hpp
    include/Move.hpp
    include/Position.hpp
    include/MoveGenerator.hpp
    include/RuleValidator.hpp
)

# 5. Crear el ejecutable con la interfaz gráfica (WIN32 evita que salga la consola negra en Windows)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}
    test/Test_AjedrezPorConsola.cpp)

# 6. Vincular tu código con las herramientas gráficas de Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
