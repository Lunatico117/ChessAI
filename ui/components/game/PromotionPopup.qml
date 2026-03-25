import QtQuick
import QtQuick.Controls

Popup {
    id: root
    width: 340
    height: 190
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    property string prefix: chessController.currentTurn === "white" ? "w" : "b"

    anchors.centerIn: parent

    background: Rectangle {
        color: "#1a222b"
        radius: 14
        border.color: "#2e3b4e"
        border.width: 1
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Text {
            text: "Coronación"
            color: "#208ce8"
            font.family: "Rajdhani"
            font.weight: Font.SemiBold
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: "Elige la pieza"
            color: "#cfd8e3"
            font.family: "Open Sans"
            font.pixelSize: 14
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            spacing: 14
            anchors.horizontalCenter: parent.horizontalCenter

            // 🔹 Dama
            Rectangle {
                id: queenBtn
                width: 55; height: 55; radius: 10
                color: queenMouse.containsMouse ? "#2e3b4e" : "#222d3a"
                border.color: queenMouse.containsMouse ? "#208ce8" : "transparent"
                border.width: queenMouse.containsMouse ? 1 : 0

                Behavior on color { ColorAnimation { duration: 120 } }
                scale: queenMouse.pressed ? 0.95 : 1.0

                Image {
                    source: "qrc:/ui/assets/pieces/" + root.prefix + "_queen.svg"
                    width: 38; height: 38
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: queenMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        chessController.promotePendingPawn("queen")
                        root.close()
                    }
                }
            }

            // 🔹 Torre
            Rectangle {
                id: rookBtn
                width: 55; height: 55; radius: 10
                color: rookMouse.containsMouse ? "#2e3b4e" : "#222d3a"
                border.color: rookMouse.containsMouse ? "#208ce8" : "transparent"

                Behavior on color { ColorAnimation { duration: 120 } }
                scale: rookMouse.pressed ? 0.95 : 1.0

                Image {
                    source: "qrc:/ui/assets/pieces/" + root.prefix + "_rook.svg"
                    width: 38; height: 38
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: rookMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        chessController.promotePendingPawn("rook")
                        root.close()
                    }
                }
            }

            // 🔹 Alfil
            Rectangle {
                id: bishopBtn
                width: 55; height: 55; radius: 10
                color: bishopMouse.containsMouse ? "#2e3b4e" : "#222d3a"
                border.color: bishopMouse.containsMouse ? "#208ce8" : "transparent"

                Behavior on color { ColorAnimation { duration: 120 } }
                scale: bishopMouse.pressed ? 0.95 : 1.0

                Image {
                    source: "qrc:/ui/assets/pieces/" + root.prefix + "_bishop.svg"
                    width: 38; height: 38
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: bishopMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        chessController.promotePendingPawn("bishop")
                        root.close()
                    }
                }
            }

            // 🔹 Caballo
            Rectangle {
                id: knightBtn
                width: 55; height: 55; radius: 10
                color: knightMouse.containsMouse ? "#2e3b4e" : "#222d3a"
                border.color: knightMouse.containsMouse ? "#208ce8" : "transparent"

                Behavior on color { ColorAnimation { duration: 120 } }
                scale: knightMouse.pressed ? 0.95 : 1.0

                Image {
                    source: "qrc:/ui/assets/pieces/" + root.prefix + "_knight.svg"
                    width: 38; height: 38
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: knightMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        chessController.promotePendingPawn("knight")
                        root.close()
                    }
                }
            }
        }
    }
}
