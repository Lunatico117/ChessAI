import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: rootPopup
    width: 340
    height: 190
    modal: true
    focus: true
    // Cerrar al hacer clic fuera o con Escape
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // Centrar en el padre (igual que en PromotionPopup)
    anchors.centerIn: parent

    // Señal para notificar a la vista principal
    signal accepted()

    // Fondo idéntico al de Coronación
    background: Rectangle {
        color: "#1a222b"
        radius: 14
        border.color: "#2e3b4e"
        border.width: 1
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Título Principal
        Text {
            text: "Rendición"
            color: "#ff6b6b" // Color de advertencia en lugar del azul
            font.family: "Rajdhani"
            font.weight: Font.SemiBold
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Subtítulo
        Text {
            text: "¿Estás seguro de que deseas rendirte?"
            color: "#cfd8e3"
            font.family: "Open Sans"
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Fila de botones
        RowLayout {
            width: parent.width
            spacing: 14

            // Usamos un pequeño margen superior para separar los botones del texto
            anchors.topMargin: 5

            // Botón Cancelar (Estilo similar a las piezas inactivas)
            Rectangle {
                Layout.fillWidth: true
                height: 45
                radius: 10
                color: cancelMouse.containsMouse ? "#2e3b4e" : "#222d3a"
                border.color: cancelMouse.containsMouse ? "#cfd8e3" : "transparent"
                border.width: cancelMouse.containsMouse ? 1 : 0

                Behavior on color { ColorAnimation { duration: 120 } }
                scale: cancelMouse.pressed ? 0.95 : 1.0

                Text {
                    text: "Cancelar"
                    color: "#cfd8e3"
                    anchors.centerIn: parent
                    font.family: "Open Sans"
                    font.pixelSize: 14
                }

                MouseArea {
                    id: cancelMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: rootPopup.close()
                }
            }

            // Botón Confirmar (Estilo de alerta/acción)
            Rectangle {
                Layout.fillWidth: true
                height: 45
                radius: 10
                color: confirmMouse.containsMouse ? "#ff4d4d" : "#ff6b6b"
                // Un borde rojo un poco más claro al hacer hover
                border.color: confirmMouse.containsMouse ? "#ff8080" : "transparent"
                border.width: confirmMouse.containsMouse ? 1 : 0

                Behavior on color { ColorAnimation { duration: 120 } }
                scale: confirmMouse.pressed ? 0.95 : 1.0

                Text {
                    text: "Sí, rendirme"
                    color: "#ffffff"
                    anchors.centerIn: parent
                    font.family: "Open Sans"
                    font.bold: true
                    font.pixelSize: 14
                }

                MouseArea {
                    id: confirmMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        rootPopup.accepted()
                        rootPopup.close()
                    }
                }
            }
        }
    }
}
