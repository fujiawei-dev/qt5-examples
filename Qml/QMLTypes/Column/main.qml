import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Column")

    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 5

        Rectangle {
            color: "red"
            width: 50
            height: 50
        }

        Rectangle {
            id: greenRect
            color: "green"
            width: 20
            height: 50
        }

        Rectangle {
            color: "blue"
            width: 50
            height: 20
        }

        move: Transition {
            NumberAnimation {
                properties: "x,y"
                duration: 1000
            }
        }

        focus: true
        Keys.onSpacePressed: greenRect.visible = !greenRect.visible
    }

    Component.onCompleted: {

    }
}
