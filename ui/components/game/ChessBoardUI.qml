import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#161e27"


    Item {
        width: 600
        height: 600
        anchors.centerIn: parent

        // Header Oponente
        RowLayout {
            id: headerTop
            anchors.bottom: boardUI.top
            anchors.bottomMargin: 15
            width: parent.width
            Text {
                text: "Player 2";
                color: "#ffffff";
                font.family: "Rajdhani";
                font.weight: chessController.currentTurn === "black" ? Font.Bold : Font.Medium;
                opacity: chessController.currentTurn === "black" ? 1.0 : 0.5
                font.pixelSize: 25;
                Layout.fillWidth: true }

            Rectangle {
                width: 80; height: 35; radius: 17; color: "#1f2a36"
                Text {
                    text: chessController.clock.blackTimeText;
                    color: "#ffffff";
                    font.family: "Rajdhani";
                    font.weight: chessController.currentTurn === "black" ? Font.Bold : Font.Medium;
                    opacity: chessController.currentTurn === "black" ? 1.0 : 0.5
                    anchors.centerIn: parent;
                    font.pixelSize: 20 }
            }
        }

        // TABLERO
        Rectangle {
            id: boardUI
            anchors.fill: parent
            radius: 10
            clip: true
            border.color: "#1f2a36"
            border.width: 4

            Grid {
                id: boardGrid
                anchors.fill: parent
                rows: 8; columns: 8

                Repeater {
                    model: chessController.boardModel

                    Rectangle {
                        width: boardUI.width / 8
                        height: boardUI.height / 8
                        property int rowIdx: Math.floor(index / 8)
                        property int colIdx: index % 8
                        property color baseColor: ((rowIdx + colIdx) % 2 === 0) ? "#ffffff" : "#1b548b"

                        color: model.isInCheck ? "#9FC5E8" : (model.isSelected ? Qt.darker(baseColor, 1.25) : baseColor)

                        Rectangle {
                            anchors.centerIn: parent; width: 20; height: 20; radius: 10; color: "#073763"; opacity: 0.8
                            visible: model.isValidMove && model.pieceType === "empty"
                        }
                        Rectangle {
                            anchors.fill: parent; color: "transparent"; border.color: "#073763"; border.width: 4
                            visible: model.isValidMove && model.pieceType !== "empty"
                        }

                        Image {
                            anchors.fill: parent; anchors.margins: 5; fillMode: Image.PreserveAspectFit
                            source: chessController.boardModel.getPieceIcon(rowIdx, colIdx)
                            opacity: source.toString() !== "" ? 1.0 : 0.0
                            scale: source.toString() !== "" ? 1.0 : 0.5
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                            Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutBack } }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: if (!chessController.match.isGameOver) chessController.handleSquareClick(rowIdx, colIdx)
                        }
                    }
                }
            }

            // AVISO DE FIN DE JUEGO (CAPA OSCURA SOBRE EL TABLERO)
            Rectangle {
                anchors.fill: parent
                color: "#B3000000" // Fondo oscuro semi-transparente
                visible: chessController.match.isGameOver && !chessController.isAnalysisMode
                z: 10 // Lo pone por encima de las fichas
                radius: 6 // Para que coincida con los bordes internos del tablero

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: chessController.match.gameOverReason === "Tablas" ? "🤝" : "👑";
                        color: "white";
                        font.pixelSize: 45
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Text {
                        text: chessController.match.gameOverReason.toUpperCase();
                        color: "#208ce8";
                        font.pixelSize: 45;
                        font.family: "Gravitas One";
                        font.weight: Font.Bold;
                        font.letterSpacing: 2
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text { text: chessController.match.gameOverWinner;
                        color: "white";
                        font.pixelSize: 35;
                        font.family: "Rajdhani";
                        font.weight: Font.SemiBold;
                        Layout.alignment: Qt.AlignHCenter }
                }
            }
        }

        // Header Jugador
        RowLayout {
            anchors.top: boardUI.bottom
            anchors.topMargin: 15
            width: parent.width
            Text {
                text: "Player 1";
                color: "#ffffff";
                font.family: "Rajdhani";
                font.weight: chessController.currentTurn === "white" ? Font.Bold : Font.Medium;
                opacity: chessController.currentTurn === "white" ? 1.0 : 0.5
                font.pixelSize: 25;
                Layout.fillWidth: true }
            Rectangle {
                width: 80; height: 35; radius: 17; color: "#208ce8"
                Text {
                    text: chessController.clock.whiteTimeText;
                    color: "#ffffff";
                    font.family: "Rajdhani";
                    font.weight: chessController.currentTurn === "white" ? Font.Bold : Font.Medium;
                    opacity: chessController.currentTurn === "white" ? 1.0 : 0.5
                    anchors.centerIn: parent;
                    font.pixelSize: 20 }
            }
        }
    }
}
