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

                // ==========================================
                // EL TABLERO FÍSICO (REFACTORIZADO)
                // ==========================================
                Rectangle {
                    id: boardUI
                    anchors.fill: parent
                    radius: 10
                    clip: true
                    border.color: "#1f2a36"
                    border.width: 4

                    // ¡ADÍOS A LAS VARIABLES DE ESTADO EN QML!
                    // QML ya no necesita recordar 'selectedRow' o 'selectedCol'

                    Grid {
                        id: boardGrid
                        anchors.fill: parent
                        rows: 8; columns: 8

                        Repeater {
                            // Cambiamos a tu nuevo QAbstractListModel expuesto desde C++
                            // Asegúrate de que tu Q_PROPERTY apunte a este modelo
                            model: chessController.boardModel

                            Rectangle {
                                width: boardUI.width / 8
                                height: boardUI.height / 8

                                property int rowIdx: Math.floor(index / 8)
                                property int colIdx: index % 8

                                // Color base de la casilla (Patrón de ajedrez)
                                property color baseColor: ((rowIdx + colIdx) % 2 === 0) ? "#ffffff" : "#1b548b"

                                // 1. SELECCIÓN DICTADA POR C++
                                // Usamos el rol 'model.isSelected' que definiremos en C++
                                // Prioridad: 1. Jaque (Rojo) -> 2. Seleccionada -> 3. Normal
                                color: {
                                    if (model.isInCheck) { // <--- Cambiado a isInCheck
                                        return "#9FC5E8";
                                    } else if (model.isSelected) {
                                        return Qt.darker(baseColor, 1.25);
                                    } else {
                                        return baseColor;
                                    }
                                }

                                // 2. INDICADOR DE MOVIMIENTO VÁLIDO (Bolita verde)
                                // Si C++ dice que es legal y la casilla está vacía

                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 20; height: 20
                                    radius: 10
                                    color: "#073763" // Verde material design
                                    opacity: 0.8
                                    visible: model.isValidMove && model.pieceType === "empty"
                                }

                                // 3. INDICADOR DE CAPTURA (Borde rojo/verde si hay pieza enemiga)
                                Rectangle {
                                    anchors.fill: parent
                                    color: "transparent"
                                    border.color: "#4CAF50" // O "#ff6b6b" si prefieres rojo para capturas
                                    border.width: 4
                                    visible: model.isValidMove && model.pieceType !== "empty"
                                }

                                Image {
                                    anchors.fill: parent
                                    anchors.margins: 5
                                    fillMode: Image.PreserveAspectFit
                                    source: chessController.getPieceIcon(rowIdx, colIdx)

                                    opacity: source.toString() !== "" ? 1.0 : 0.0
                                    scale: source.toString() !== "" ? 1.0 : 0.5

                                    Behavior on opacity { NumberAnimation { duration: 150 } }
                                    Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutBack } }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        // 4. DELEGACIÓN TOTAL A C++
                                        // Literalmente le decimos al controlador: "Tocaron esta coordenada. Haz tu magia".
                                        chessController.handleSquareClick(rowIdx, colIdx)
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
                            Layout.fillWidth: true; height: 45; radius: 8; color: "#3a2222"
                            Text { text: "🏳 Rendirse"; color: "#ff6b6b"; anchors.centerIn: parent; font.bold: true }
                        }
                    }
                }
            }
        }
    }
}
