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

        Text {
            text: "Move History";
            color: "#ffffff";
            font.family: "Rajdhani";
            font.weight: Font.Medium;
            font.pixelSize: 25
        }
        ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 0

                    // 1. Cabecera de la tabla
                    Rectangle {
                        Layout.fillWidth: true
                        height: 30
                        color: "transparent"
                        border.color: "#2a323c"
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10

                            Text {
                                text: "#";
                                color: "#6e7b8a";
                                font.pixelSize: 13;
                                font.family: "Open Sans"
                                Layout.preferredWidth: 30
                            }
                            Text {
                                text: "White";
                                color: "#6e7b8a";
                                font.pixelSize: 13;
                                font.family: "Open Sans"
                                Layout.fillWidth: true
                            }
                            Text {
                                text: "Black";
                                color: "#6e7b8a";
                                font.pixelSize: 13;
                                font.family: "Open Sans"
                                Layout.fillWidth: true
                            }
                        }
                    }

                    // 2. Lista de Movimientos (ListView)
                    ListView {
                        id: historyList
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true // Para que no se dibuje fuera del área al hacer scroll

                        // Leemos la lista desde tu propiedad en C++
                        model: chessController.logger.moveHistoryList

                        // TRUCO DE QML: Solo creamos un delegado cada 2 movimientos (índices pares)
                        delegate: Item {
                            width: ListView.view.width
                            height: 22
                            // Si el índice es impar, este Item se oculta y no ocupa espacio.
                            // Si es par, dibuja la fila completa (Turno: Blanco + Negro).
                            visible: index % 2 === 0

                            // Esta propiedad asegura que los items ocultos no ocupen altura
                            implicitHeight: visible ? 22 : 0

                            Rectangle {
                                anchors.fill: parent
                                // Fondo intercalado para mayor legibilidad (opcional, como en las tablas)
                                color: (index / 2) % 2 === 0 ? "transparent" : "#1a222c"

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 10
                                    anchors.rightMargin: 10

                                    // Número de turno (Ej: 0,1 -> 1; 2,3 -> 2; 4,5 -> 3)
                                    Text {
                                        text: Math.floor(index / 2) + 1
                                        color: "#6e7b8a"
                                        font.pixelSize: 13
                                        Layout.preferredWidth: 30
                                        font.bold: true;
                                    }

                                    // Movimiento Blanco (Es el elemento actual del delegate)
                                    Rectangle {
                                        Layout.fillWidth: true
                                        height: 25
                                        color: "transparent"
                                        // Resaltado azul (como en la imagen) si es la jugada actual
                                        radius: 4
                                        border.color: (index === chessController.logger.moveHistoryList.length - 1 || index === chessController.logger.moveHistoryList.length - 2) ? "#208ce8" : "transparent"

                                        Text {
                                            text: modelData // El string de la lista (ej. "e4")
                                            color: "#ffffff"
                                            font.pixelSize: 14
                                            font.family: "Open Sans"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            font.bold: true;
                                        }
                                    }

                                    // Movimiento Negro (Es el siguiente elemento en la lista, si existe)
                                    Rectangle {
                                        Layout.fillWidth: true
                                        height: 25
                                        color: "transparent"
                                        radius: 4

                                        Text {
                                            // Comprobamos si el movimiento negro existe en la lista
                                            text: (index + 1 < chessController.logger.moveHistoryList.length) ? chessController.logger.moveHistoryList[index + 1] : ""
                                            color: "#ffffff"
                                            font.pixelSize: 14
                                            font.family: "Open Sans"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            font.bold: true;
                                        }
                                    }
                                }
                            }
                        }

                        // Autoscroll: Cada vez que se añade un elemento, baja la lista hasta el final
                        onCountChanged: {
                            historyList.positionViewAtEnd()
                        }
                    }
                }

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
                    id: btnUndo
                    width: parent.width
                    height: 45
                    radius: 8
                    color: undoMouse.containsMouse ? "#2e3b4e" : "#222d3a"

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }

                    Text {
                        text: "↩ Deshacer"
                        color: "#ffffff"
                        anchors.centerIn: parent
                        font.family: "Rajdhani"
                        font.weight: Font.Regular
                        font.pixelSize: 16
                    }

                    MouseArea {
                        id: undoMouse
                        anchors.fill: parent
                        hoverEnabled: true

                        onPressed: btnUndo.scale = 0.97
                        onReleased: btnUndo.scale = 1.0
                        onClicked: chessController.requestUndo()
                    }
                }

                RowLayout {
                    width: parent.width
                    spacing: 10

                    Rectangle {
                        id: btnDraw
                        Layout.fillWidth: true
                        height: 45
                        radius: 8
                        color: drawMouse.containsMouse ? "#2e3b4e" : "#222d3a"

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }

                        Text {
                            text: "🤝 Tablas"
                            color: "#ffffff"
                            anchors.centerIn: parent
                            font.family: "Rajdhani"
                            font.weight: Font.Regular
                            font.pixelSize: 16
                        }

                        MouseArea {
                            id: drawMouse
                            anchors.fill: parent
                            hoverEnabled: true

                            onPressed: btnDraw.scale = 0.97
                            onReleased: btnDraw.scale = 1.0
                            onClicked: chessController.offerDraw()
                        }
                    }

                    Rectangle {
                        id: btnSurrender
                        Layout.fillWidth: true
                        height: 45
                        radius: 8
                        color: surrenderMouse.containsMouse ? "#4a2a2a" : "#3a2222"

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }

                        Text {
                            text: "🏳 Rendirse"
                            color: "#ff6b6b"
                            anchors.centerIn: parent
                            font.family: "Rajdhani"
                            font.weight: Font.Regular
                            font.pixelSize: 16
                        }

                        MouseArea {
                            id: surrenderMouse
                            anchors.fill: parent
                            hoverEnabled: true

                            onPressed: btnSurrender.scale = 0.97
                            onReleased: btnSurrender.scale = 1.0
                            onClicked: chessController.surrender()
                        }
                    }
                }
            }

            Column {
                id: gameOverColumn
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10
                visible: chessController.match.isGameOver

                Rectangle {
                    id: btnAnalysis
                    width: parent.width
                    height: 45
                    radius: 8

                    color: chessController.isAnalysisMode
                           ? (analysisMouse.containsMouse ? "#22364d" : "#1a2a3a")
                           : (analysisMouse.containsMouse ? "#2e3b4e" : "#222d3a")

                    border.color: chessController.isAnalysisMode ? "#208ce8" : "transparent"
                    border.width: chessController.isAnalysisMode ? 2 : 0

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }

                    Text {
                        text: chessController.isAnalysisMode ? "👁 Ver Resultado" : "🔍 Analizar Juego"
                        color: chessController.isAnalysisMode ? "#208ce8" : "#ffffff"
                        anchors.centerIn: parent
                        font.family: "Rajdhani"
                        font.weight: Font.Regular
                        font.pixelSize: 16
                    }

                    MouseArea {
                        id: analysisMouse
                        anchors.fill: parent
                        hoverEnabled: true

                        onPressed: btnAnalysis.scale = 0.97
                        onReleased: btnAnalysis.scale = 1.0
                        onClicked: chessController.toggleAnalysisMode()
                    }
                }

                Rectangle {
                    id: btnRestart
                    width: parent.width
                    height: 45
                    radius: 8
                    color: restartMouse.containsMouse ? "#3aa0ff" : "#208ce8"

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }

                    Text {
                        text: "🔄 Volver a jugar"
                        color: "#ffffff"
                        anchors.centerIn: parent
                        font.family: "Rajdhani"
                        font.weight: Font.Regular
                        font.pixelSize: 16
                    }

                    MouseArea {
                        id: restartMouse
                        anchors.fill: parent
                        hoverEnabled: true

                        onPressed: btnRestart.scale = 0.97
                        onReleased: btnRestart.scale = 1.0
                        onClicked: chessController.restartGame()
                    }
                }

                Rectangle {
                    id: btnMenu
                    width: parent.width
                    height: 45
                    radius: 8
                    color: menuMouse.containsMouse ? "#2e3b4e" : "#222d3a"

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }

                    Text {
                        text: "🏠 Menú"
                        color: "#ffffff"
                        anchors.centerIn: parent
                        font.family: "Rajdhani"
                        font.weight: Font.Regular
                        font.pixelSize: 16
                    }

                    MouseArea {
                        id: menuMouse
                        anchors.fill: parent
                        hoverEnabled: true

                        onPressed: btnMenu.scale = 0.97
                        onReleased: btnMenu.scale = 1.0
                        onClicked: console.log("Volver al menú")
                    }
                }
            }
        }
    }
}
