import QtQuick
import QtQuick.Window

Window {
    width: 1400
    height: 900
    visible: true
    title: "ChessAI"
    color: "#11161d"

    Loader {
        id: mainLoader
        anchors.fill: parent

        // 1. Iniciamos en el Menú
        source: "qrc:/ui/views/MenuView.qml"

        // 2. Escuchamos las señales del archivo que esté cargado actualmente
        Connections {
            target: mainLoader.item

            // Esta función se activa automáticamente cuando WelcomeScreen emite playClicked()
            function onPlayClicked() {
                // Cambiamos la vista al juego
                mainLoader.source = "views/GameView.qml"
            }
            function onMenuClicked() {
                // Cambiamos la vista al juego
                mainLoader.source = "views/MenuView.qml"
            }
        }
    }
}
