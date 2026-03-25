import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: setupPopup
    width: 450
    height: 550
    modal: true // Hace que el fondo se oscurezca y no se pueda hacer clic fuera
    focus: true
    closePolicy: Popup.NoAutoClose // Obliga al usuario a elegir una opción
    anchors.centerIn: parent

    // Señal que enviaremos a tu controlador de C++ cuando termine la configuración
    signal setupComplete(string mode, int difficultyLevel)

    // Variable para controlar qué pantalla mostrar (0 = Modo, 1 = Dificultad)
    property int currentStep: 0

    // Fondo oscuro que oscurece el tablero (Overlay)
    Overlay.modal: Rectangle {
        color: "#aa0a0f14" // Negro con transparencia
    }

    // Fondo del PopUp en sí
    background: Rectangle {
        color: "#161b22" // Color oscuro de tu diseño
        radius: 20
        border.color: "#2a323c"
        border.width: 1
    }

    // ==========================================
    // PASO 1: SELECCIÓN DE MODO (Local vs IA)
    // ==========================================
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20
        visible: setupPopup.currentStep === 0

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Modo de Juego"
            color: "white"
            font.pixelSize: 28
            font.family: "Rajdhani"
            font.weight: Font.Bold
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Selecciona contra quién jugarás"
            color: "#8a9ba8"
            font.pixelSize: 16
            font.family: "Rajdhani"
        }

        Item { Layout.fillHeight: true } // Espaciador

        // Botón PvP
        Rectangle {
            Layout.fillWidth: true
            height: 60
            radius: 15
            color: pvpMouseArea.containsMouse ? "#2a323c" : "#1e252e"

            Text {
                anchors.centerIn: parent
                text: "Jugador vs Jugador (Local)"
                color: "white"
                font.pixelSize: 18
                font.family: "Rajdhani"
                font.weight: Font.Bold
            }

            MouseArea {
                id: pvpMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    setupPopup.close()
                    setupComplete("PvP", -1) // -1 porque no hay dificultad
                }
            }
        }

        // Botón IA
        Rectangle {
            Layout.fillWidth: true
            height: 60
            radius: 15
            color: pveMouseArea.containsMouse ? "#1a70ba" : "#208ce8"

            Text {
                anchors.centerIn: parent
                text: "Jugador vs IA"
                color: "white"
                font.pixelSize: 18
                font.family: "Rajdhani"
                font.weight: Font.Bold
            }

            MouseArea {
                id: pveMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    //setupPopup.currentStep = 1 // Pasamos a la pantalla de dificultad
                }
            }
        }

        Item { Layout.fillHeight: true } // Espaciador
    }

    // ==========================================
    // PASO 2: SELECCIÓN DE DIFICULTAD (Tu imagen)
    // ==========================================
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 12
        visible: setupPopup.currentStep === 1

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Dificultad"
            color: "white"
            font.pixelSize: 28
            font.family: "Rajdhani"
            font.weight: Font.Bold
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Selecciona tu nivel de desafío para comenzar"
            color: "#8a9ba8"
            font.pixelSize: 14
            font.family: "Rajdhani"
        }

        Item { Layout.preferredHeight: 10 } // Espaciador

        // Función para crear los botones de dificultad
        Repeater {
            model: [
                { name: "Principiante", isPro: false, level: 0 },
                { name: "Aficionado", isPro: false, level: 1 },
                { name: "Experto", isPro: false, level: 2 },
                { name: "Maestro", isPro: false, level: 3 },
                { name: "GRAN MAESTRO", isPro: true, level: 4 }
            ]

            Rectangle {
                Layout.fillWidth: true
                height: 50
                radius: 15
                // Si es Gran Maestro usa el azul, sino usa el oscuro
                color: modelData.isPro ? (diffMouseArea.containsMouse ? "#1a70ba" : "#208ce8")
                                       : (diffMouseArea.containsMouse ? "#2a323c" : "#1e252e")

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20

                    Text {
                        text: modelData.isPro ? "🏅" : "⊿"
                        color: "white"
                        font.pixelSize: 16
                    }

                    Text {
                        Layout.fillWidth: true
                        text: modelData.name
                        color: "white"
                        font.pixelSize: 16
                        font.family: "Rajdhani"
                        font.weight: modelData.isPro ? Font.Bold : Font.Medium
                    }

                    Text {
                        visible: modelData.isPro
                        text: "★"
                        color: "white"
                        font.pixelSize: 16
                    }
                }

                MouseArea {
                    id: diffMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        setupPopup.close()
                        setupComplete("PvE", modelData.level)
                    }
                }
            }
        }

        Item { Layout.fillHeight: true } // Espaciador

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "ELIGE SABIAMENTE. LA IA NO TIENE PIEDAD."
            color: "#465563"
            font.pixelSize: 10
            font.family: "Rajdhani"
            font.letterSpacing: 1
        }
    }

    // Al abrir el popup, siempre empezamos en el paso 0
    onOpened: currentStep = 0
}
