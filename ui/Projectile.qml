import QtQuick 2.0

Item {
    id: projectile

    x: modelData.x
    y: modelData.y
    z: modelData.z
    width: modelData.width
    height: modelData.height
    focus: false
    objectName: "linked_" + modelData.objectId

    Rectangle {
        Image {
            id: projectileTexture
            anchors.fill: parent
            source: modelData.texture
            rotation: modelData.rotation
        }

        anchors.fill: parent;
//        border.color: "blue"
//        border.width: 2
        color: "transparent"
    }
}
