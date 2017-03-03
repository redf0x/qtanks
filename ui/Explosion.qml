import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property bool triggered: false

    AnimatedSprite {
        id: explosion
        x: parent.x; y: parent.y
        width: parent.width; height: parent.height
        anchors.centerIn: parent
        source: "qrc:/image/ui/res/explosion.png"
        frameCount: 5
        frameRate: 6
        loops: 1
        running: triggered
        visible: false
        onRunningChanged: {
            visible = running;

            if (!running)
                triggered = false;
        }
    }

    onTriggeredChanged: {
        if (triggered)
            explosion.start ();
    }

    function trigger() { triggered = true; }
}
