import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import Qt.labs.platform 1.1

import "main.js" as MainJS

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Menu")

    Rectangle {
        width: 320
        height: 240
        color: "#FFFF00"

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton)
                    contextMenu.open()
            }

            onPressAndHold: {
                if (mouse.source === Qt.MouseEventNotSynthesized)
                    contextMenu.open()
            }

            Menu {
                id: contextMenu

                MenuItem {
                    text: "Cut"
                }

                MenuItem {
                    text: "Copy"
                }

                MenuItem {
                    text: "Paste"
                }
            }
        }
    }

    Button {
        id: fileButton
        width: 120
        height: 40
        anchors.bottom: parent.bottom

        text: "File"
        onClicked: menu.open()

        Menu {
            id: menu

            MenuItem {
                text: "New..."
            }
            MenuItem {
                text: "Open..."
            }

            MenuSeparator { }

            MenuItem {
                text: "Save"
            }
        }
    }

    Component.onCompleted: {
        MainJS.init(window, core)
        MainJS.launch(window, core)
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/

