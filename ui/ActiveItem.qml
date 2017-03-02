import QtQuick 2.0

Item {
    id: ai
    property int blink: 0

    x: modelData.x
    y: modelData.y
    z: modelData.z
    width: modelData.width
    height: modelData.height
    focus: false

    Rectangle {
        Image {
            id: aiTexture
            anchors.fill: parent
            source: modelData.texture
            rotation: modelData.rotation
        }

        anchors.fill: parent;
        /* border.color: "blue" */
        /* border.width: 2 */
        color: "transparent"
        visible: modelData.spawned
    }

    Timer {
        id: timer
        interval: 300
        repeat: true
        onTriggered: {
            if (blink < 6) {
                aiTexture.visible = !aiTexture.visible;
                blink++;
            } else {
                timer.stop ();
                blink = 0;
            }
        }
    }

    Connections {
        target: modelData
        onAliveChanged: {
            if (alive) {
                timer.start ();
            }
        }
    }
}
