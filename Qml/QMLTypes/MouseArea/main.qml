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
    title: qsTr("Mouse Area")

    Rectangle {
        width: 100
        height: 100
        color: "green"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.color = 'red'
            }
        }
    }

    Rectangle {
        y: 120
        width: 100
        height: 100
        color: "green"

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: mouse => {
                           if (mouse.button === Qt.RightButton)
                           parent.color = 'blue'
                           else
                           parent.color = 'red'
                       }
        }
    }

    Rectangle {
        x: 120
        id: container
        width: 600
        height: 200

        Rectangle {
            id: rect
            width: 50
            height: 50
            color: "red"
            opacity: (600.0 - rect.x) / 600

            MouseArea {
                anchors.fill: parent
                drag.target: rect
                drag.axis: Drag.XAxis
                drag.minimumX: 0
                drag.maximumX: container.width - rect.width
            }
        }
    }

    Rectangle {
        x: 160
        y: 80
        width: 300
        height: 240
        color: "lightsteelblue"

        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: "XAxis"
            drag.minimumX: 30
            drag.maximumX: 150
            drag.filterChildren: true

            Rectangle {
                color: "yellow"
                x: 50
                y: 50
                width: 100
                height: 100
                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Clicked")
                }
            }
        }
    }

    Rectangle {
        x: 10
        y: 240
        width: 80
        height: 20

        border.width: 1

        Text {
            text: mouseArea.pressedButtons & Qt.RightButton ? "right" : ""
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
        }
    }

    Rectangle {
        x: 10
        y: 340
        width: 100
        height: 100
        border.width: 1

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            hoverEnabled: true
        }

        MouseArea {
            id: mouseArea2
            width: 50
            height: 50
            anchors.centerIn: parent
            hoverEnabled: true

            onEntered:{
                parent.color = "red"
            }

            onExited:{
            parent.color = "lightsteelblue"
            }
        }
    }
    Component.onCompleted: {

    }
}
