import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import "main.js" as MainJS

Window {
    id: window
    visible: true
    width: 360
    height: 48
    title: qsTr("File Attachment Downloader")

    property alias download: download
    property alias downloading: downloading

    ProgressBar {
        id: downloading

        anchors.centerIn: parent
        width: 320
        value: 0.1

        MouseArea {
            id: download
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        MainJS.init(window, core)
        MainJS.launch(window, core)
    }
}
