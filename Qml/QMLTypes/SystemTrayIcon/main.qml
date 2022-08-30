import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("System Tray Icon")

    Button {
        text: qsTr("Hide Window")
        onClicked: {
            window.visible = false
        }
    }

    SystemTrayIcon {
        id: systemTrayIcon
        visible: true
        tooltip: qsTr("This is a system tray icon")

        icon.mask: true
        icon.source: "qrc:tray-icon.png"

        onMessageClicked: console.log("Message clicked")

        Component.onCompleted: showMessage(
                                   "Message Title",
                                   "Something important came up. Click this to know more.")

        onActivated: {
            window.show()
            window.raise()
            window.requestActivate()
        }

        menu: Menu {
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }

    Component.onCompleted: {

    }
}
