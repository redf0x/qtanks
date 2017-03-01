import QtQuick 2.0

Item {
    id: projectile

    x: modelData.x
    y: modelData.y
    z: 1
    width: (battleArea.width / battleField.columns) / 2
    height: (battleArea.height / battleField.rows) / 2
    focus: false
    objectName: "linked_" + modelData.objectId

    Component.onCompleted: { modelData.linkObject(projectile); }

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
