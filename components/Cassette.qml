import QtQuick 2.9

Item {
    id: cassette

    property bool playing: false
    property string imageSource: ""

    width: 480
    height: 320
    anchors.horizontalCenter: parent.horizontalCenter

    onPlayingChanged: {
        if (playing) {
            leftCogwheelRotation.start();
            rightCogwheelRotation.start();
        } else {
            leftCogwheelRotation.stop();
            rightCogwheelRotation.stop();
        }
    }

    Image {
        id: cassetteCover
        source: cassette.imageSource === "" ? "../assets/images/cover.jpg" : cassette.imageSource
        smooth: true
        fillMode: Image.PreserveAspectCrop
        width: 460
        height: 250
        anchors.verticalCenterOffset: -15
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Image {
        id: cassetteBody
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        source: "../assets/images/cassette-body.png"
        smooth: true
        fillMode: Image.PreserveAspectFit
    }

    Row {
        id: row
        spacing: 144
        anchors.verticalCenterOffset: -20
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        layoutDirection: Qt.LeftToRight

        Image {
            id: leftCogwheel
            width: 60
            height: 60
            sourceSize.width: 0
            source: "../assets/images/cogwheel.png"

            RotationAnimator {
                id: leftCogwheelRotation
                target: leftCogwheel
                from: 0
                to: -360
                duration: 4000
                loops: RotationAnimator.Infinite

            }
        }

        Image {
            id: rightCogwheel
            width: 60
            height: 60
            source: "../assets/images/cogwheel.png"

            RotationAnimator {
                id: rightCogwheelRotation
                target: rightCogwheel
                from: 0
                to: -360
                duration: 2000
                loops: RotationAnimator.Infinite
            }
        }
    }
}
