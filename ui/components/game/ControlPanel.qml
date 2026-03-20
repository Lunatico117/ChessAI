import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#11161d"

    Rectangle {
        width: 1; height: parent.height
        anchors.left: parent.left
        color: "#2a323c"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text { text: "⏱ Move History"; color: "#ffffff"; font.bold: true; font.pixelSize: 18 }

        Item { Layout.fillHeight: true; Layout.fillWidth: true }

        // ZONA DE BOTONES DINÁMICA
        Item {
            Layout.fillWidth: true
            // Calculamos la altura automáticamente para no cortar nada
            implicitHeight: chessController.match.isGameOver ? gameOverColumn.implicitHeight : activeColumn.implicitHeight

            // Botones de JUEGO ACTIVO
            Column {
                id: activeColumn
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10
                visible: !chessController.match.isGameOver

                Rectangle {
                    width: parent.width; height: 45; radius: 8; color: "#222d3a"
                    Text { text: "↩ Deshacer"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true }
                    MouseArea {
                            anchors.fill: parent;
                            onClicked: chessController.requestUndo()
                        }
                }
                RowLayout {
                    width: parent.width; spacing: 10
                    Rectangle {
                        Layout.fillWidth: true; height: 45; radius: 8; color: "#222d3a"
                        Text { text: "🤝 Tablas"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true }
                        MouseArea { anchors.fill: parent; onClicked: chessController.offerDraw() }
                    }
                    Rectangle {
                        Layout.fillWidth: true; height: 45; radius: 8; color: "#3a2222"
                        Text { text: "🏳 Rendirse"; color: "#ff6b6b"; anchors.centerIn: parent; font.bold: true }
                        MouseArea { anchors.fill: parent; onClicked: chessController.surrender() }
                    }
                }
            }

            // Botones de FIN DE JUEGO
            Column {
                id: gameOverColumn
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10
                visible: chessController.match.isGameOver

                Rectangle {
                    width: parent.width; height: 45; radius: 8;
                    color: chessController.isAnalysisMode ? "#1a2a3a" : "#222d3a"
                    border.color: chessController.isAnalysisMode ? "#208ce8" : "transparent"
                    border.width: chessController.isAnalysisMode ? 2 : 0

                    Text {
                        text: chessController.isAnalysisMode ? "👁 Ver Resultado" : "🔍 Analizar Juego"
                        color: chessController.isAnalysisMode ? "#208ce8" : "#ffffff"
                        anchors.centerIn: parent
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: chessController.toggleAnalysisMode()
                    }
                }

                Rectangle {
                    width: parent.width; height: 45; radius: 8; color: "#208ce8"
                    Text { text: "🔄 Volver a jugar"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: chessController.restartGame()
                    }
                }

                Rectangle {
                    width: parent.width; height: 45; radius: 8; color: "#222d3a"
                    Text { text: "🏠 Menú"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true }
                    MouseArea { anchors.fill: parent; onClicked: console.log("Volver al menú") }
                }
            }
        }
    }
}
