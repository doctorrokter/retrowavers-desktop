import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import QtMultimedia 5.9
import QtQml.Models 2.3
import "components"

ApplicationWindow {
    id: window

    property bool playing: false
    property alias cassette: cassette

    visible: true
    width: 1024
    height: 768
    title: qsTr("Retrowave Radio")

    color: "#323232"

    MediaPlayer {
        id: player

        onPositionChanged: {
            trackInfo.currentPosition = position;
            if (position !== 0 && position === player.duration) {
                window.next();
            }
        }
    }

    Image {
        id: palms
        fillMode: Image.PreserveAspectCrop
        source: "assets/images/palms-bg.png"
        anchors.fill: parent
    }

    Image {
        id: cover
        source: "assets/images/cover.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    GaussianBlur {
        source: cover
        anchors.fill: cover
        radius: 80
        samples: 100
        opacity: 0.5
    }

    Column {
        id: column
        spacing: 30

        anchors.centerIn: parent

        Image {
            id: logo
            source: "assets/images/logo.png"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Cassette {
            id: cassette
            playing: window.playing
        }

        Controls {
            id: controls

            onPlay: {
                window.playing = true;
            }

            onPause: {
                window.playing = false;
            }
        }

        TrackInfo {
            id: trackInfo
            anchors.horizontalCenter: parent.horizontalCenter
            title: !isActiveTrack() ? "" : _tracksService.active.title
            duration: !isActiveTrack() ? 0 : _tracksService.active.duration
        }
    }

    Column {
        x: 15
        y: 15
        spacing: 15

        Item {
            id: listMenu
            width: 100
            height: 100

            Rectangle {
                id: listMenuBg
                width: 100
                anchors.fill: parent
                color: "black"
                radius: 15
                opacity: 0.3
            }

            Image {
                id: name
                width: 80
                height: 80
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "assets/images/list.png"

                MouseArea {
                    width: parent.width
                    height: parent.height

                    onClicked: {
                        playlist.visible = !playlist.visible;
                    }
                }
            }
        }

        Item {
            id: playlist

            visible : false
            width: window.width / 2.5
            height: (window.height / 1.3) - listMenuBg.height

            Rectangle {
                anchors.fill: parent
                radius: 15
                color: "black"
                opacity: 0.3
            }

            ListView {
                id: listView
                width: parent.width - 20
                height: parent.height - 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                leftMargin: 10
                topMargin: 10
                rightMargin: 10
                bottomMargin: 10
                clip: true
                highlight: Rectangle {
                    color: "#FF3333"
                    height: 50
                    opacity: 0.5
                    radius: 15
                }

                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                interactive: true
                ScrollBar.vertical: ScrollBar {}

                currentIndex: _tracksController.index
                model: _tracksService.tracks

                delegate: TrackItem {
                    id: trackItem
                    title: modelData.title
                    duration: modelData.duration

                    MouseArea {
                        width: trackItem.width
                        height: trackItem.height

                        onClicked: {
                            _tracksController.play(modelData);
                        }
                    }
                }
            }
        }
    }

    function isActiveTrack() {
        return _tracksService.active !== null && _tracksService.active !== undefined;
    }

    function startTrack() {
            window.next();
            window.playing = true;
    }

    function changeTrack(track) {
        player.source = track.streamUrl;
        cover.source = track.artworkUrl;
        cassette.imageSource = track.artworkUrl;
        player.play();
    }

    function next() {
        if (!_tracksController.next()) {
            _api.load();
        }
    }

    onPlayingChanged: {
        controls.playing = playing;
        if (playing) {
            player.play();
        } else {
            player.pause();
        }
    }

    Component.onCompleted: {
        window.showMaximized();
        _tracksService.activeChanged.connect(window.changeTrack);
        _api.loaded.connect(startTrack);
        _api.load();
    }
}
