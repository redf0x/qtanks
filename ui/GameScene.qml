import QtQuick 2.0
import Tanks.Globals 0.1

Rectangle {
    color: "grey"

    Rectangle {
        id: battleArea
        x: 32
        y: 32
        height: 13 * 45
        width: 13 * 45
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
    }

    HUD {
        anchors { left: battleArea.right; leftMargin: 16; }
    }
}
