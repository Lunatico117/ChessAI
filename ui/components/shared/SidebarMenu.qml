import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#11161d"

    property string titleFont: ""

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
            font.pixelSize: 24
            font.family: root.titleFont
            font.bold: true
        }

        Rectangle {
            width: parent.width; height: 40
            color: "#1a2a3a"
            radius: 8
            Text {
                anchors.centerIn: parent
                text: "🎮 Play"
                color: "#208ce8"
                font.pixelSize: 16
                font.bold: true
            }
        }
    }
}
