import QtQuick
import QtQuick.Window

Window {
    width: 1280
    height: 800
    visible: true
    title: "ChessAI"
    color: "#11161d" // El color oscuro de fondo general

    // Carga la vista del juego que crearemos a continuación
    Loader {
        anchors.fill: parent
        source: "views/GameView.qml"
    }
}
