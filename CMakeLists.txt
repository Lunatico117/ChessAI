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
find_package(Qt6 COMPONENTS Core Gui Widgets Qml Quick REQUIRED)

# 3. AQUÍ ES DONDE APARECERÁN TUS ARCHIVOS
# Lista todos tus archivos .cpp aquí
set(SOURCES
    # --- Backend (Lógica del juego) ---
        backend/src/Board.cpp
        backend/src/Game.cpp
        backend/src/GameState.cpp
        backend/src/GameLogger.cpp
        backend/src/AIPlayer.cpp
        backend/src/HumanPlayer.cpp
        backend/src/Piece.cpp
        backend/src/Pawn.cpp
        backend/src/Player.cpp
        backend/src/Rook.cpp
        backend/src/Bishop.cpp
        backend/src/Knight.cpp
        backend/src/Queen.cpp
        backend/src/King.cpp
        backend/src/Move.cpp
        backend/src/Position.cpp
        backend/src/MoveGenerator.cpp
        backend/src/RuleValidator.cpp

    # --- Controladores (Puente C++ <-> QML) ---
        controllers/ChessController.cpp
        controllers/BoardModel.cpp

        # --- Tests y Main ---
        test/Test_jaqueMatePastor.cpp
        test/Test_AjedrezPorConsola.cpp
        main.cpp
    )

# 4. Lista tus archivos de cabecera (.hpp o .h) para que Qt Creator los muestre
set(HEADERS
    # --- Backend ---
        backend/include/Game.hpp
        backend/include/Board.hpp
        backend/include/GameState.hpp
        backend/include/GameLogger.hpp
        backend/include/AIPlayer.hpp
        backend/include/HumanPlayer.hpp
        backend/include/Piece.hpp
        backend/include/Pawn.hpp
        backend/include/Player.hpp
        backend/include/Rook.hpp
        backend/include/Bishop.hpp
        backend/include/Knight.hpp
        backend/include/Queen.hpp
        backend/include/King.hpp
        backend/include/Move.hpp
        backend/include/Position.hpp
        backend/include/MoveGenerator.hpp
        backend/include/RuleValidator.hpp

        # --- Controladores ---
        controllers/ChessController.hpp
        controllers/BoardModel.hpp
    )

# 5. Crear el ejecutable con la interfaz gráfica (WIN32 evita que salga la consola negra en Windows)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 6. Incluir directorios para que el compilador encuentre los .hpp más fácilmente
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/backend/include
    ${CMAKE_SOURCE_DIR}/controllers
    ${CMAKE_SOURCE_DIR}/ui
)


# 7. Vincular tu código con las herramientas gráficas de Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Qml
    Qt6::Quick
)

# 8. Empaquetamos la interfaz gráfica en el ejecutable
# Esto permite usar la ruta "qrc:/ui/main.qml" en tu main.cpp
qt_add_resources(${PROJECT_NAME} "qml_resources" PREFIX "/" FILES
    ui/main.qml
    ui/views/DificultyAIView.qml
    ui/views/GameOverView.qml
    ui/views/GameView.qml
    ui/views/MenuView.qml
    ui/components/game/ChessBoardUI.qml
    ui/components/game/ControlPanel.qml
    ui/components/game/DrawOfferPopup.qml
    ui/components/game/PromotionPopup.qml
    ui/components/shared/SidebarMenu.qml

    ui/assets/fonts/GravitasOne-Regular.ttf
    ui/assets/b_bishop.svg
    ui/assets/w_bishop.svg
    ui/assets/b_king.svg
    ui/assets/w_king.svg
    ui/assets/b_knight.svg
    ui/assets/w_knight.svg
    ui/assets/b_queen.svg
    ui/assets/w_queen.svg
    ui/assets/b_pawn.svg
    ui/assets/w_pawn.svg
    ui/assets/b_rook.svg
    ui/assets/w_rook.svg

)
