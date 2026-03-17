import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    width: 320
    height: 160
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    background: Rectangle {
        color: "#1a2a3a"; radius: 12; border.color: "#208ce8"; border.width: 2
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width

        Text { text: "🤝 Oferta de Tablas"; color: "#ffcc00"; font.pixelSize: 20; font.bold: true; Layout.alignment: Qt.AlignHCenter }
        Text { text: "¿Aceptas el empate?"; color: "white"; font.pixelSize: 16; Layout.alignment: Qt.AlignHCenter }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                width: 100; height: 40; radius: 8; color: "#208ce8"
                Text { text: "✓ Aceptar"; color: "white"; font.bold: true; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: { chessController.acceptDraw(); root.close() } }
            }

            Rectangle {
                width: 100; height: 40; radius: 8; color: "#3a2222"
                Text { text: "✗ Declinar"; color: "#ff6b6b"; font.bold: true; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: { chessController.declineDraw(); root.close() } }
            }
        }
    }
}
