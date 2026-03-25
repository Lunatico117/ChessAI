import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    signal playClicked()
    anchors.fill: parent
    color: "#11161d" // Tu color de fondo oscuro

    // ==========================================
    // BOTÓN SUPERIOR DERECHO: Iniciar Sesión
    // ==========================================
    Rectangle {
        id: loginBtn
        width: 140
        height: 40
        radius: 20
        color: loginMouseArea.containsMouse ? "#1a70ba" : "#208ce8" // Efecto hover
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 30

        Text {
            anchors.centerIn: parent
            text: "Iniciar Sesión"
            color: "white"
            font.pixelSize: 16
            font.family: "Rajdhani"
            font.weight: Font.Bold
        }

        MouseArea {
            id: loginMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                console.log("Botón Iniciar Sesión presionado")
                // Aquí irá la lógica para abrir el login
            }
        }
    }

    // ==========================================
    // CONTENIDO CENTRAL (Logo, Título y Jugar)
    // ==========================================
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 25

        // 1. Contenedor del Icono (El cuadro oscuro redondeado)
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 160
            height: 200
            radius: 40
            color: "#1a222c" // Un tono ligeramente más claro que el fondo

            Image {
                anchors.centerIn: parent
                width: 150
                height: 200

                source: "qrc:/ui/assets/icon/Logo_ChessAI.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        // 2. Título Principal
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "ChessAI"
            color: "white"
            font.pixelSize: 85
            font.family: "Rajdhani"
            font.weight: Font.Bold
        }

        // 3. Subtítulo
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Domina el tablero con la inteligencia artificial más\navanzada del mundo."
            color: "#8a9ba8" // Color gris/azulado para contraste
            font.pixelSize: 22
            font.family: "Rajdhani"
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.2
        }

        // Espaciador invisible para separar el texto del botón
        Item { Layout.preferredHeight: 20 }

        // 4. Botón Principal "Jugar"
        Rectangle {
            id: playBtn
            Layout.alignment: Qt.AlignHCenter
            width: 300
            height: 60
            radius: 30
            color: playMouseArea.containsMouse ? "#1a70ba" : "#208ce8" // Efecto hover

            // Sombra/Brillo sutil simulado con un borde
            border.color: playMouseArea.containsMouse ? "#208ce8" : "transparent"
            border.width: playMouseArea.containsMouse ? 2 : 0

            Text {
                anchors.centerIn: parent
                text: "Jugar"
                color: "white"
                font.pixelSize: 24
                font.family: "Rajdhani"
                font.weight: Font.Bold
            }

            MouseArea {
                id: playMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.playClicked()
                }
            }
        }
    }

    // ==========================================
    // PIE DE PÁGINA (Footer)
    // ==========================================
    ColumnLayout {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 40
        spacing: 15

        // Enlaces
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 35

            Text { text: "Reglas"; color: "#8a9ba8"; font.pixelSize: 16; font.family: "Rajdhani"; font.weight: Font.Medium }
            Text { text: "Rankings"; color: "#8a9ba8"; font.pixelSize: 16; font.family: "Rajdhani"; font.weight: Font.Medium }
            Text { text: "Soporte"; color: "#8a9ba8"; font.pixelSize: 16; font.family: "Rajdhani"; font.weight: Font.Medium }
        }

        // Copyright
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "© 2026 ChessAI."
            color: "#465563" // Gris más oscuro para que no robe atención
            font.pixelSize: 14
            font.family: "Rajdhani"
        }
    }
}
