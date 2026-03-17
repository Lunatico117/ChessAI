import QtQuick
import QtQuick.Controls

Popup {
    id: root
    width: 320
    height: 140
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    property string prefix: chessController.currentTurn === "white" ? "w" : "b"

    background: Rectangle {
        color: "#1a2a3a"; radius: 12; border.color: "#208ce8"; border.width: 2
    }

    Column {
        anchors.centerIn: parent
        spacing: 15

        Text { text: "¡Peón coronado! Elige una pieza:"; color: "white"; font.pixelSize: 16; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter }

        Row {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            // Dama
            Rectangle {
                width: 50; height: 50; radius: 8; color: "#222d3a"
                border.color: hoverAreaQ.containsMouse ? "#208ce8" : "transparent"
                Image { source: "qrc:/ui/assets/" + root.prefix + "_queen.svg"; width: 40; height: 40; anchors.centerIn: parent; fillMode: Image.PreserveAspectFit }
                MouseArea { id: hoverAreaQ; anchors.fill: parent; hoverEnabled: true; onClicked: { chessController.promotePendingPawn("queen"); root.close() } }
            }
            // Torre
            Rectangle {
                width: 50; height: 50; radius: 8; color: "#222d3a"
                border.color: hoverAreaR.containsMouse ? "#208ce8" : "transparent"
                Image { source: "qrc:/ui/assets/" + root.prefix + "_rook.svg"; width: 40; height: 40; anchors.centerIn: parent; fillMode: Image.PreserveAspectFit }
                MouseArea { id: hoverAreaR; anchors.fill: parent; hoverEnabled: true; onClicked: { chessController.promotePendingPawn("rook"); root.close() } }
            }
            // Alfil
            Rectangle {
                width: 50; height: 50; radius: 8; color: "#222d3a"
                border.color: hoverAreaB.containsMouse ? "#208ce8" : "transparent"
                Image { source: "qrc:/ui/assets/" + root.prefix + "_bishop.svg"; width: 40; height: 40; anchors.centerIn: parent; fillMode: Image.PreserveAspectFit }
                MouseArea { id: hoverAreaB; anchors.fill: parent; hoverEnabled: true; onClicked: { chessController.promotePendingPawn("bishop"); root.close() } }
            }
            // Caballo
            Rectangle {
                width: 50; height: 50; radius: 8; color: "#222d3a"
                border.color: hoverAreaN.containsMouse ? "#208ce8" : "transparent"
                Image { source: "qrc:/ui/assets/" + root.prefix + "_knight.svg"; width: 40; height: 40; anchors.centerIn: parent; fillMode: Image.PreserveAspectFit }
                MouseArea { id: hoverAreaN; anchors.fill: parent; hoverEnabled: true; onClicked: { chessController.promotePendingPawn("knight"); root.close() } }
            }
        }
    }
}
