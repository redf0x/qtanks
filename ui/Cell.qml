import QtQuick 2.0

Item {
    id: cell
    x: modelData.x * width
    y: modelData.y * height
    width: battleArea.width / battleField.columns
    height: battleArea.height / battleField.rows

    Image {
        id: cellTexture
        anchors.fill: parent
        source: modelData.texture
        rotation: modelData.rotation
    }
}
