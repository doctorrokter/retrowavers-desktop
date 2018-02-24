import QtQuick 2.0

Column {
    id: trackInfo

    property string title: "Dance with the Dead - Threasher"
    property int duration: 0
    property int currentPosition: 0

    FontLoader {
        id: newtownFont
        source: "../assets/fonts/NEWTOW_I.ttf"
    }

    anchors.horizontalCenter: parent.horizontalCenter

    Text {
        id: trackTitle
        text: trackInfo.title
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: newtownFont.name
        color: "white"
        font.pixelSize: 42
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            text: getMediaTime(trackInfo.currentPosition)
            font.family: newtownFont.name
            color: "white"
            font.pixelSize: 28
        }

        Text {
            text: " / "
            font.family: newtownFont.name
            color: "white"
            font.pixelSize: 28
        }

        Text {
            text: getMediaTime(trackInfo.duration)
            font.family: newtownFont.name
            color: "#FF3333"
            font.pixelSize: 28
        }
    }

    function getMediaTime(time) {
        var seconds = Math.round(time / 1000);
        var h = Math.floor(seconds / 3600) < 10 ? '0' + Math.floor(seconds / 3600) : Math.floor(seconds / 3600);
        var m = Math.floor((seconds / 60) - (h * 60)) < 10 ? '0' + Math.floor((seconds / 60) - (h * 60)) : Math.floor((seconds / 60) - (h * 60));
        var s = Math.floor(seconds - (m * 60) - (h * 3600)) < 10 ? '0' + Math.floor(seconds - (m * 60) - (h * 3600)) : Math.floor(seconds - (m * 60) - (h * 3600));
        return m + ':' + s;
    }
}
