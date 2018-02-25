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

        Item {
            id: listMenu
            x: 15
            y: 15

            Rectangle {
                id: listMenuBg
                width: 100
                height: 100
                radius: 15
                color: "black"
                opacity: 0.3
            }

            Image {
                id: name
                x: 10
                y: 10
                width: 80
                height: 80
                source: "assets/images/list.png"
            }
        }

        Item {
            id: playlist
            x: 15
            y: 130

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
                leftMargin: 10
                topMargin: 10
                rightMargin: 10
                bottomMargin: 10

                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                interactive: true
                ScrollBar.vertical: ScrollBar {}

                model: _tracksService.tracks

                delegate: TrackItem {
                    title: modelData.title
                    duration: modelData.duration
                }
            }
        }

    }

    function isActiveTrack() {
        return _tracksService.active !== null && _tracksService.active !== undefined;
    }

    function startTrack() {
        if (!isActiveTrack()) {
            _tracksController.next();
            window.playing = true;
        }
    }

    function changeTrack(track) {
        player.source = track.streamUrl;
        cover.source = track.artworkUrl;
        cassette.imageSource = track.artworkUrl;
        player.play();
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
        _tracksService.activeChanged.connect(window.changeTrack);
        _api.loaded.connect(startTrack);
        _api.load();
    }
}
