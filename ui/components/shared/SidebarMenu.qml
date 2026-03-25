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
        anchors.margins: 10
        spacing: 5

        // Agrupamos el icono y el título en una fila (Row)
        Row {

            Image {
                source: "qrc:/ui/assets/icon/Logo_ChessAI.png"

                // Ajustamos el tamaño para que coincida con el texto
                width: 80
                height: 80
                fillMode: Image.PreserveAspectFit
            }

            Text {
                text: "ChessAI" // Ya sin el emoji
                color: "#208ce8"
                font.pixelSize: 45
                font.family: "Rajdhani"
                font.weight: Font.Bold

                // Centramos verticalmente el texto con respecto a la imagen
                anchors.verticalCenter: parent.verticalCenter
            }
        }

    }
}
