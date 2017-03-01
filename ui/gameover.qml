import QtQuick 2.0
import Tanks.Globals 0.1

Rectangle {
    focus: true
    color: "black"
    signal userAction(var c)

    Image {
        id: img
        y: parent.height / 4
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/image/ui/res/gameover.png"
    }

    FontLoader {
        id: fontLoader
        source: "qrc:/font/ui/res/arcadeclassic.ttf"
    }

    Text {
        anchors.top: img.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 42
        text: "HIT ANY KEY TO EXIT"
        font { family: fontLoader.name; pointSize: 48; }
        color: "white"
    }

    Keys.onPressed: {
        console.log("key hit");
        gameLdr.userAction(Globals.EXIT);
    }
}
