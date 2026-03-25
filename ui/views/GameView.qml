import QtQuick
import QtQuick.Layouts

import "../components/shared"
import "../components/game"

Item {
    id: rootView
    signal menuClicked()
    anchors.fill: parent

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 2. INSTANCIAMOS LOS COMPONENTES DE 'shared' Y 'game'
        SidebarMenu {
            Layout.preferredWidth: 280
            Layout.fillHeight: true
        }

        ChessBoardUI {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ControlPanel {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
        }
    }

    GameSetupPopup {
            id: setupPopup
            anchors.centerIn: parent

            onSetupComplete: function(mode, difficultyLevel) {
                console.log("Iniciando partida en modo:", mode, "- Nivel:", difficultyLevel)

                // Aquí conectamos directamente con tu backend en C++
                if (mode === "PvE") {
                    chessController.startGameVsAI(difficultyLevel)
                } else if (mode === "PvP") {
                    chessController.startLocalGame()
                }
            }
        }

        // Esto hace que el PopUp se abra apenas cargue el GameView
        Component.onCompleted: {
            setupPopup.open()
        }

    // 3. CONECTAMOS EL C++ Y LOS POPUPS
    Connections {
        target: chessController
        function onPromotionRequested() { promotionPopup.open()
            }

    }
    Connections {
        target: chessController.match

        function onDrawOffered() { drawOfferPopup.open()
        }
    }
    PromotionPopup {
        id: promotionPopup
        anchors.centerIn: parent
    }

    DrawOfferPopup {
        id: drawOfferPopup
        anchors.centerIn: parent
    }

}
