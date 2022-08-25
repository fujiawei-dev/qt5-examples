import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.4

Window {
    id: window
    visible: true
    width: 480
    height: 320
    title: qsTr("Qt Multimedia")

    Rectangle {
        id: cameraController

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15

        width: 320
        height: 180

        border.color: "#000000"
        border.width: 0

        Camera {
            id: camera
            deviceId: QtMultimedia.defaultCamera.deviceId

            captureMode: Camera.CaptureStillImage

            imageCapture {
                onCaptureFailed: {
                    console.log("Capture failed, " + message)
                }

                onImageSaved: {
                    console.log("Image saved to " + path)
                }
            }
        }

        VideoOutput {
            source: camera
            anchors.fill: parent
            focus: visible // to receive focus and capture key events when visible

            MouseArea {
                anchors.fill: parent
                onClicked: camera.imageCapture.capture()
            }
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.left: cameraController.left
        anchors.top: cameraController.bottom
        anchors.topMargin: 15

        spacing: 15

        ComboBox {
            id: camerasList
            displayText: camera.displayName

            model: QtMultimedia.availableCameras
            delegate: ItemDelegate {
                contentItem: Text {
                    text: modelData.displayName
                    anchors.fill: parent
                    anchors.margins: 5
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    font.pixelSize: 14
                }

                onClicked: camera.deviceId = modelData.deviceId

                MouseArea {
                    onClicked: comboBox.popup.close()
                }
            }
        }

        Button {
            text: qsTr("Refresh Cameras")
            onClicked: camerasList.model = QtMultimedia.availableCameras
        }
    }

    Timer {
        id: timer
        running: false
        interval: 1000
        repeat: true
        onTriggered: {
            let cameras = QtMultimedia.availableCameras
            console.log("Available cameras:" + cameras.length)
            let displayNames = []
            for (var i = 0; i < cameras.length; i++) {
                displayNames.push(cameras[i].displayName)
            }
            console.log(displayNames.join(","))
        }
    }

    Component.onCompleted: {
        console.log("defaultCamera.displayName is " + QtMultimedia.defaultCamera.displayName)
        console.log("defaultCamera.deviceId is " + QtMultimedia.defaultCamera.deviceId)
    }
}
