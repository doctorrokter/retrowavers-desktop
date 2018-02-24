import QtQuick 2.0

Row {
    id: controls

    property bool playing: false

    spacing: 250
    anchors.horizontalCenter: parent.horizontalCenter
    layoutDirection: Qt.LeftToRight

    signal play()
    signal pause()
    signal prev()
    signal next()

    onPlayingChanged: {
        if (playing) {
            playBtn.visible = false;
            pauseBtn.visible = true;
        } else {
            pauseBtn.visible = false;
            playBtn.visible = true;
        }
    }

    Image {
        id: prevBtn
        source: "../assets/images/ic_previous.png"

        MouseArea {
            anchors.fill: parent

            onClicked: {
                _tracksController.prev();
                controls.prev();
            }
        }
    }

    Image {
        id: playBtn
        visible: true
        source: "../assets/images/ic_play.png"

        MouseArea {
            anchors.fill: parent

            onClicked: {
                controls.play();
            }
        }
    }

    Image {
        id: pauseBtn
        visible: false
        source: "../assets/images/ic_pause.png"

        MouseArea {
            anchors.fill: parent

            onClicked: {
                controls.pause();
            }
        }
    }

    Image {
        id: nextBtn
        source: "../assets/images/ic_next.png"

        MouseArea {
            anchors.fill: parent

            onClicked: {
                _tracksController.next();
                controls.next();
            }
        }
    }


}
