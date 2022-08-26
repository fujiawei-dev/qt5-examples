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
    title: qsTr("System Tray Icon")

    SystemTrayIcon {
        visible: true
icon.source: "qrc:/images/tray-icon.png"

        onActivated: {
        //    window.show()
             //      window.raise()
                //       window.requestActivate()
        }

        menu: Menu {
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }

    Component.onCompleted: {
        MainJS.init(window, core)
        MainJS.launch(window, core)
    }
}
