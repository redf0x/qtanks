import QtQuick 2.0

Item {
    id: cell
    x: modelData.x * width
    y: modelData.y * height
    z: modelData.zlevel
    width: battleArea.width / battleField.columns
    height: battleArea.height / battleField.rows
    objectName: "linked_" + modelData.objectId

    Component.onCompleted: modelData.linkObject(cell);

    Image {
        id: cellTexture
        anchors.fill: parent
        source: modelData.texture
        rotation: modelData.rotation
    }
}
