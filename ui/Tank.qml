import QtQuick 2.0
import Tanks.ActiveItem 0.1

Item {
    id: player

    x: (modelData.x * width) / 2
    y: (modelData.y * height) / 2
    width: (battleArea.width / battleField.columns) * 2
    height: (battleArea.height / battleField.rows) * 2
    focus: true

    onStateChanged: {
        console.log("x " + x + "y");
    }

    Image {
        id: tankTexture
        anchors.fill: parent
        source: modelData.texture
        rotation: modelData.rotation
    }

    function move(direction) {
        var scale = (battleArea.width / battleField.columns) / 4;

        switch(direction) {
            case ActiveItem.SOUTH:
                y = y + scale;
                break;
            case ActiveItem.NORTH:
                y = y - scale;
                break;
            case ActiveItem.EAST:
                x = x + scale;
                break;
            case ActiveItem.WEST:
                x = x - scale;
        }
        console.log("new coordinates " + x + ", " + y);
    }

    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Up:
                move(ActiveItem.NORTH);
                break;
            case Qt.Key_Down:
                move(ActiveItem.SOUTH);
                break;
            case Qt.Key_Right:
                move(ActiveItem.EAST);
                break;
            case Qt.Key_Left:
                move(ActiveItem.WEST);
        }

        event.accepted = true;
    }
}
