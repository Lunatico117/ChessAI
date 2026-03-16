import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    anchors.fill: parent

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ==========================================
        // COLUMNA 1: MENÚ LATERAL IZQUIERDO
        // ==========================================
        Rectangle {
            Layout.preferredWidth: 240
            Layout.fillHeight: true
            color: "#11161d" // Gris muy oscuro

            // Separador visual de la columna
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
                    color: "#208ce8" // Azul brillante
                    font.pixelSize: 24
                    font.bold: true
                }

                // Aquí irían tus botones del menú (Play, Analyze, etc.)
                Rectangle {
                    width: parent.width; height: 40
                    color: "#1a2a3a" // Fondo del botón activo
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

        // ==========================================
        // COLUMNA 2: TABLERO CENTRAL
        // ==========================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#161e27" // Un poco más claro que los paneles laterales

            // Contenedor del tablero para mantenerlo siempre cuadrado
            Item {
                width: 600
                height: 600
                anchors.centerIn: parent

                // Header Top (Oponente)
                RowLayout {
                    anchors.bottom: boardUI.top
                    anchors.bottomMargin: 15
                    width: parent.width

                    Text { text: "🤖 CHESSAI BOT"; color: "#ffffff"; font.bold: true; Layout.fillWidth: true }
                    Rectangle {
                        width: 80; height: 35; radius: 17; color: "#1f2a36"
                        Text { text: "05:00"; color: "#ffffff"; anchors.centerIn: parent; font.pixelSize: 16 }
                    }
                }

                // EL TABLERO FÍSICO
                Rectangle {
                    id: boardUI
                    anchors.fill: parent
                    radius: 10
                    clip: true
                    border.color: "#1f2a36"
                    border.width: 4


                    // Variables para recordar la pieza seleccionada (-1 significa que no hay nada seleccionado)
                    property int selectedRow: -1
                    property int selectedCol: -1

                    Grid {
                        id: boardGrid // Le damos un nombre explícito
                        anchors.fill: parent
                        rows: 8; columns: 8


                        Repeater {
                            model: chessController.boardState

                            Rectangle {
                                width: boardUI.width / 8
                                height: boardUI.height / 8

                                // Usamos nombres diferentes (rowIdx) para evitar más choques
                                property int rowIdx: Math.floor(index / 8)
                                property int colIdx: index % 8

                                // 1. MANTENER COLOR Y OSCURECER:
                                            property color baseColor: ((rowIdx + colIdx) % 2 === 0) ? "#ffffff" : "#1b548b"
                                            property bool isSelected: (boardUI.selectedRow === rowIdx && boardUI.selectedCol === colIdx)

                                            // Qt.darker(color, 1.25) hace que el color original sea un 25% más oscuro
                                            color: isSelected ? Qt.darker(baseColor, 1.25) : baseColor

                                Image {
                                    anchors.fill: parent
                                    anchors.margins: 5
                                    fillMode: Image.PreserveAspectFit
                                    source: chessController.getPieceIcon(rowIdx, colIdx)

                                    // 1. Ocultamos la imagen si la ruta está vacía
                                    opacity: source.toString() !== "" ? 1.0 : 0.0
                                    // 2. Le damos un tamaño más pequeño si está vacía para el efecto "Pop"
                                    scale: source.toString() !== "" ? 1.0 : 0.5

                                    // 3. LA MAGIA: Animamos los cambios de opacidad y escala en 150 milisegundos
                                    Behavior on opacity {
                                        NumberAnimation { duration: 150 }
                                    }
                                    Behavior on scale {
                                        NumberAnimation { duration: 150; easing.type: Easing.OutBack }
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        // ESCUDO: Si la casilla se está destruyendo, ignoramos el clic
                                        if (!modelData) return;

                                        if (boardUI.selectedRow === -1) {
                                            if (modelData.pieceColor === chessController.currentTurn) {
                                                boardUI.selectedRow = rowIdx;
                                                boardUI.selectedCol = colIdx;
                                            }
                                        } else {
                                            if (modelData.pieceColor === chessController.currentTurn) {
                                                boardUI.selectedRow = rowIdx;
                                                boardUI.selectedCol = colIdx;
                                            } else {
                                                chessController.attemptMove(boardUI.selectedRow, boardUI.selectedCol, rowIdx, colIdx);
                                                boardUI.selectedRow = -1;
                                                boardUI.selectedCol = -1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Header Bottom (Jugador)
                RowLayout {
                    anchors.top: boardUI.bottom
                    anchors.topMargin: 15
                    width: parent.width

                    Text { text: "👤 YOU"; color: "#ffffff"; font.bold: true; Layout.fillWidth: true }
                    Rectangle {
                        width: 80; height: 35; radius: 17; color: "#208ce8"
                        Text { text: "04:42"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true; font.pixelSize: 16 }
                    }
                }
            }
        }

        // ==========================================
        // COLUMNA 3: PANEL DERECHO (HISTORIAL Y BOTONES)
        // ==========================================
        Rectangle {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            color: "#11161d"

            // Separador visual
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

                // Espacio reservado para la tabla de movimientos
                Item { Layout.fillHeight: true; Layout.fillWidth: true }

                // Botones de acción inferiores
                Column {
                    Layout.fillWidth: true
                    spacing: 10

                    Rectangle {
                        width: parent.width; height: 45; radius: 8; color: "#222d3a"
                        Text { text: "↩ Deshacer"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true }
                    }
                    RowLayout {
                        width: parent.width; spacing: 10
                        Rectangle {
                            Layout.fillWidth: true; height: 45; radius: 8; color: "#222d3a"
                            Text { text: "🤝 Tablas"; color: "#ffffff"; anchors.centerIn: parent; font.bold: true }
                        }
                        Rectangle {
                            Layout.fillWidth: true; height: 45; radius: 8; color: "#3a2222" // Rojizo
                            Text { text: "🏳 Rendirse"; color: "#ff6b6b"; anchors.centerIn: parent; font.bold: true }
                        }
                    }
                }
            }
        }
    }
}
