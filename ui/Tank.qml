import QtQuick 2.0
import Tanks.ActiveItem 0.1

Item {
    id: player

    x: (modelData.x * width) / 2
    y: (modelData.y * height) / 2
    width: (battleArea.width / battleField.columns) * 2
    height: (battleArea.height / battleField.rows) * 2
    focus: true
    objectName: "linked_" + modelData.objectId

    Component.onCompleted: modelData.linkObject(player);

    Rectangle {
        Image {
            id: tankTexture
            anchors.fill: parent
            source: modelData.texture
            rotation: modelData.rotation
        }

        anchors.fill: parent;
        border.color: "blue"
        border.width: 2
        color: "transparent"
    }

    function rotateCCW() {
        modelData.rotation += -90;
        tankTexture.rotation = modelData.rotation;

        if (modelData.direction - 1 < ActiveItem.NORTH) {
            modelData.direction = ActiveItem.WEST;
        } else {
            modelData.direction--;
        }
    }

    function rotateCW() {
        modelData.rotation += 90;
        tankTexture.rotation = modelData.rotation;

        if (modelData.direction + 1 > ActiveItem.WEST) {
            modelData.direction = ActiveItem.NORTH;
        } else {
            modelData.direction++;
        }
    }

    function turn(direction) {
        switch (direction) {
            case ActiveItem.NORTH:
                switch (modelData.direction) {
                    case ActiveItem.EAST:
                        rotateCCW();
                        break;

                    case ActiveItem.WEST:
                        rotateCW();
                }

                return;

            case ActiveItem.SOUTH:
                switch (modelData.direction) {
                    case ActiveItem.EAST:
                        rotateCW();
                        break;

                    case ActiveItem.WEST:
                        rotateCCW();
                }

                return;

            case ActiveItem.EAST:
                switch (modelData.direction) {
                    case ActiveItem.NORTH:
                        rotateCW();
                        break;
                    case ActiveItem.SOUTH:
                        rotateCCW();
                }

                return;

            case ActiveItem.WEST:
                switch (modelData.direction) {
                    case ActiveItem.NORTH:
                        rotateCCW();
                        break;

                    case ActiveItem.SOUTH:
                        rotateCW();
                }
        }
    }

    function move(direction) {
        var scale = Math.floor((battleArea.width / battleField.columns) / 4);

        if (modelData.distance == 0)
            return;

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

        modelData.distance = modelData.distance - scale;
    }

    Keys.onPressed: {
        if ((event.key == controller.keyWest || event.key == controller.keyEast ||
                event.key == controller.keySouth || event.key == controller.keyNorth ||
                event.key == controller.keyFire) && battleField.frozen)
            return;

        switch (event.key) {
            case controller.keyWest:
                (modelData.direction == ActiveItem.WEST) ? move(ActiveItem.WEST) : turn(ActiveItem.WEST);
                break;

            case controller.keyEast:
                (modelData.direction == ActiveItem.EAST) ? move(ActiveItem.EAST) : turn(ActiveItem.EAST);
                break;

            case controller.keySouth:
                (modelData.direction == ActiveItem.SOUTH) ? move(ActiveItem.SOUTH) : turn(ActiveItem.SOUTH);
                break;

            case controller.keyNorth:
                (modelData.direction == ActiveItem.NORTH) ? move(ActiveItem.NORTH) : turn(ActiveItem.NORTH);
                break;

            case controller.keyPause:
                battleField.frozen = !battleField.frozen;
        }

        event.accepted = true;
    }
}
