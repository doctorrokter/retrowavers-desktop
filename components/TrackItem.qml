import QtQuick 2.9

Item {
    id: track

    property string title: ""
    property int duration: 0
    width: parent.width - 10
    height: 50

    Text {
        text: track.title
        anchors.left: parent.left
        anchors.leftMargin: 0
        verticalAlignment: Text.AlignVCenter
        font.family: newtownFont.name
        color: "white"
        font.pixelSize: 20
        leftPadding: 5
    }

    Text {
        text: getMediaTime(track.duration)
//        text: "00:00"
        anchors.right: parent.right
        anchors.rightMargin: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.family: newtownFont.name
        color: "white"
        font.pixelSize: 20
        rightPadding: 5
    }

    RetroFont {
        id: newtownFont
    }

    function getMediaTime(time) {
        var seconds = Math.round(time / 1000);
        var h = Math.floor(seconds / 3600) < 10 ? '0' + Math.floor(seconds / 3600) : Math.floor(seconds / 3600);
        var m = Math.floor((seconds / 60) - (h * 60)) < 10 ? '0' + Math.floor((seconds / 60) - (h * 60)) : Math.floor((seconds / 60) - (h * 60));
        var s = Math.floor(seconds - (m * 60) - (h * 3600)) < 10 ? '0' + Math.floor(seconds - (m * 60) - (h * 3600)) : Math.floor(seconds - (m * 60) - (h * 3600));
        return m + ':' + s;
    }
}
