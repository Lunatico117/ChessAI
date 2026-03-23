import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#11161d"

    Rectangle {
        width: 1; height: parent.height
        anchors.right: parent.right
        color: "#2a323c"
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Text {
            text: "♖ ChessAI"
            color: "#208ce8"
            font.pixelSize: 45
            font.family: "Rajdhani"
            font.weight: Font.Bold
        }



    }
}
