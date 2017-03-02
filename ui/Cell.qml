import QtQuick 2.0

Item {
    id: cell
    x: modelData.x
    y: modelData.y
    z: modelData.z
    width: modelData.width
    height: modelData.height

    Image {
        id: cellTexture
        anchors.fill: parent
        source: modelData.texture
        rotation: modelData.rotation
    }
}
