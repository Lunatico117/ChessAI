import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    width: 340
    height: 190
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    anchors.centerIn: parent

    background: Rectangle {
        color: "#1a222b"
        radius: 14

        border.color: "#2e3b4e"
        border.width: 1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 18

        Text {
            text: "Oferta de Tablas"
            color: "#208ce8"
            font.family: "Rajdhani"
            font.weight: Font.SemiBold
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }


        Text {
            text: "¿Aceptas el empate?"
            color: "#cfd8e3"
            font.family: "Open Sans"
            font.pixelSize: 15
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            spacing: 14
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                id: acceptBtn
                width: 110
                height: 40
                radius: 8
                color: acceptMouse.containsMouse ? "#3aa0ff" : "#208ce8"

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }

                Text {
                    text: "✓ Aceptar"
                    color: "white"
                    anchors.centerIn: parent
                    font.family: "Rajdhani"
                    font.weight: Font.Medium
                    font.pixelSize: 15
                }

                MouseArea {
                    id: acceptMouse
                    anchors.fill: parent
                    hoverEnabled: true

                    onPressed: acceptBtn.scale = 0.97
                    onReleased: acceptBtn.scale = 1.0

                    onClicked: {
                        chessController.acceptDraw()
                        root.close()
                    }
                }
            }

            Rectangle {
                id: declineBtn
                width: 110
                height: 40
                radius: 8
                color: declineMouse.containsMouse ? "#4a2a2a" : "#3a2222"

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }

                Text {
                    text: "✗ Declinar"
                    color: "#ff6b6b"
                    anchors.centerIn: parent
                    font.family: "Rajdhani"
                    font.weight: Font.Medium
                    font.pixelSize: 15
                }

                MouseArea {
                    id: declineMouse
                    anchors.fill: parent
                    hoverEnabled: true

                    onPressed: declineBtn.scale = 0.97
                    onReleased: declineBtn.scale = 1.0

                    onClicked: {
                        chessController.declineDraw()
                        root.close()
                    }
                }
            }
        }
    }
}
