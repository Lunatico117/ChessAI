import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 640
    visible: true
    title: "ChessAI"
    color: "#2b2b2b" // Fondo gris oscuro para los bordes

    // Contenedor principal del tablero
    Grid {
        id: boardGrid
        rows: 8
        columns: 8
        anchors.centerIn: parent
        width: 600
        height: 600

        // El Repeater es como un bucle 'for'. Creará 64 rectángulos automáticamente.
        Repeater {
            model: 64

            Rectangle {
                width: boardGrid.width / 8
                height: boardGrid.height / 8

                // Lógica matemática para alternar los colores de las casillas
                // index va de 0 a 63. Calculamos la fila y la columna actuales.
                property int row: Math.floor(index / 8)
                property int col: index % 8

                // Si la suma de fila + columna es par, es color claro. Si es impar, oscuro.
                color: (row + col) % 2 === 0 ? "#EEEED2" : "#769656"
            }
        }
    }
}
