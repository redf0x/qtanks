import QtQuick 2.0
import Tanks.Globals 0.1

Rectangle {
    color: "grey"

    Rectangle {
        signal heightChanged(int newheight)
        signal widthChanged(int newwidth)

        id: battleArea
        x: 32
        y: 32
        // height: Globals.fieldCellRows * Globals.defaultEntityHeight
        // width: Globals.fieldCellColumns * Globals.defaultEntityWidth
        color: "black"

        Repeater {
            model: battleField.bmap
            Cell { }
        }

        Repeater {
            model: battleField.playableItems
            Tank { }
        }

        Repeater {
            model: battleField.npcItems
            ActiveItem { }
        }

        Repeater {
            model: battleField.projectiles
            Projectile { }
        }

        onHeightChanged: {
            console.log("BF2: height " + height);
            battleField.heightChanged(height);
        }

        onWidthChanged: {
            console.log("BF2: width " + width);
            battleField.widthChanged(width);
        }

        function setDimensions(dim1, dim2) {
            var dim = Math.min(dim1, dim2) - x * 2;
            height = dim;
            width = dim;
            heightChanged(dim);
            widthChanged(dim);
        }

        Component.onCompleted: {
            heightChanged(height);
            widthChanged(width);
            console.log("field cols/rows " + Globals.fieldCellColumns + ", "
                        + Globals.fieldCellRows);
        }
    }

    HUD {
        anchors { left: battleArea.right; leftMargin: 16; }
    }

    onHeightChanged: {
        console.log("GS1: width x height: " + width + " x " + height);
        battleArea.setDimensions(width, height);
    }

    onWidthChanged: {
        console.log("GS1: width x height: " + width + " x " + height);
        battleArea.setDimensions(width, height);
    }
}
