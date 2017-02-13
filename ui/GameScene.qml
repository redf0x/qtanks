import QtQuick 2.0

Rectangle {
    color: "grey"

    Rectangle {
        id: battleArea
        x: 32
        y: 32
        height: 13 * 50
        width: 13 * 50
        color: "black"

        Repeater {
            model: battleField.bmap
            Cell { }
        }

        Repeater {
            model: battleField.playableItems
            Tank { }
        }
    }
}
