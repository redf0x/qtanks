import QtQuick 2.0

Item {
    id: ai
    property int blink: 0

    x: modelData.x
    y: modelData.y
    z: modelData.z
    width: modelData.width
    height: modelData.height
    objectName: modelData.objectId
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
        visible: modelData.alive
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

    Explosion {
        id: explosion
    }

    Connections {
        target: modelData
        onAliveChanged: {
            if (alive) {
                aiTexture.visible = true;
                timer.start ();
            } else {
                console.log(modelData.objectId + " exploded @ " + x + "," + y);
                explosion.trigger(x, y);
                aiTexture.visible = false;
            }
        }
    }
}
