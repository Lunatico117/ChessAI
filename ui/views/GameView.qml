import QtQuick
import QtQuick.Layouts

// 1. IMPORTAMOS LAS CARPETAS DE LOS COMPONENTES
// Los dos puntos "../" significan "sal de la carpeta 'views'"
import "../components/shared"
import "../components/game"

Item {
    id: rootView
    anchors.fill: parent

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 2. INSTANCIAMOS LOS COMPONENTES DE 'shared' Y 'game'
        SidebarMenu {
            Layout.preferredWidth: 240
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
