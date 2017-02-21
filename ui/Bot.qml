import QtQuick 2.0
import Tanks.ActiveItem 0.1

Item {
    id: npc

    x: (modelData.x * width) / 2
    y: (modelData.y * height) / 2
    width: (battleArea.width / battleField.columns) * 2
    height: (battleArea.height / battleField.rows) * 2
    focus: false
    objectName: "linked_" + modelData.objectId

    Component.onCompleted: { modelData.linkObject(npc); }

    Rectangle {
        Image {
            id: npcTexture
            anchors.fill: parent
            source: modelData.texture
            rotation: modelData.rotation
        }

        anchors.fill: parent;
        border.color: "blue"
        border.width: 2
        color: "transparent"
    }
}
