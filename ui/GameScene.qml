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
        height: /* Globals.fieldCellRows */ 26 * 22
        width: /* Globals.fieldCellColumns */ 26 * 22
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

        onHeightChanged: { battleField.heightChanged(height); }
        onWidthChanged: { battleField.widthChanged(width); }
        Component.onCompleted: { heightChanged(height); widthChanged(width);
        console.log("field cols/rows " + Globals.fieldCellRows); }
    }

    HUD {
        anchors { left: battleArea.right; leftMargin: 16; }
    }
}
