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
        backend/src/AIPlayer.cpp
        backend/src/ai/RandomMoveStrategy.cpp
        backend/src/HumanPlayer.cpp
        backend/src/Piece.cpp
        backend/src/Pawn.cpp
        backend/src/Player.cpp
        backend/src/PlayerFactory.cpp
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
        controllers/src/ChessController.cpp
        controllers/src/BoardModel.cpp
        controllers/src/GameClock.cpp
        controllers/src/MatchManager.cpp
        controllers/src/GameLogger.cpp

        # --- Tests y Main ---
        test/Test_jaqueMatePastor.cpp
        test/Test_AjedrezPorConsola.cpp
        main.cpp
    )

# 4. Lista tus archivos de cabecera (.hpp o .h) para que Qt Creator los muestre
set(HEADERS
    # --- Backend ---
        backend/include/Board.hpp
        backend/include/Game.hpp
        backend/include/GameState.hpp
        backend/include/AIPlayer.hpp
        backend/include/ai/IAIStrategy.hpp # Solo es una cabecera ya que es una interfaz pura (Un contrato)
        backend/include/ai/RandomMoveStrategy.hpp
        backend/include/HumanPlayer.hpp
        backend/include/Piece.hpp
        backend/include/Pawn.hpp
        backend/include/Player.hpp
        backend/include/PlayerFactory.hpp
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
        controllers/include/ChessController.hpp
        controllers/include/BoardModel.hpp
        controllers/include/GameClock.hpp
        controllers/include/MatchManager.hpp
        controllers/include/GameLogger.hpp
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
    ui/views/GameView.qml
    ui/views/MenuView.qml

    ui/components/game/ChessBoardUI.qml
    ui/components/game/ControlPanel.qml
    ui/components/game/DrawOfferPopup.qml
    ui/components/game/PromotionPopup.qml
    ui/components/game/GameSetupPopup.qml
    ui/components/game/SurrenderPopup.qml
    ui/components/shared/SidebarMenu.qml

    ui/assets/fonts/GravitasOne/GravitasOne-Regular.ttf
    ui/assets/fonts/OpenSans/OpenSans-VariableFont_wdth,wght.ttf
    ui/assets/fonts/Rajdhani/Rajdhani-Bold.ttf
    ui/assets/fonts/Rajdhani/Rajdhani-Medium.ttf
    ui/assets/fonts/Rajdhani/Rajdhani-Regular.ttf
    ui/assets/fonts/Rajdhani/Rajdhani-SemiBold.ttf
    ui/assets/pieces/b_bishop.svg
    ui/assets/pieces/w_bishop.svg
    ui/assets/pieces/b_king.svg
    ui/assets/pieces/w_king.svg
    ui/assets/pieces/b_knight.svg
    ui/assets/pieces/w_knight.svg
    ui/assets/pieces/b_queen.svg
    ui/assets/pieces/w_queen.svg
    ui/assets/pieces/b_pawn.svg
    ui/assets/pieces/w_pawn.svg
    ui/assets/pieces/b_rook.svg
    ui/assets/pieces/w_rook.svg
    ui/assets/icon/Logo_ChessAI.png


)
