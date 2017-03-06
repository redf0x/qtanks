import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property bool triggered: false
    property real originx: 0
    property real originy: 0

    AnimatedSprite {
        id: explosion
        x: 0; y: 0
        width: parent.width; height: parent.height
        anchors.centerIn: parent
        source: "qrc:/image/ui/res/explosion.png"
        frameCount: 5
        frameRate: 6
        loops: 1
        running: triggered
        visible: false
        onRunningChanged: {
            visible = running;

            if (!running)
                triggered = false;
        }
    }

    Component.onCompleted: {
        originx = parent.x;
        originy = parent.y;
    }

    onTriggeredChanged: {
        if (triggered)
            explosion.start ();
    }

    function trigger(ex, ey) {
        var nx, ny;
        console.log(parent.objectName + 'parent = (' + ex + ', ' + ey + ')');
        console.log(parent.objectName + 'origin = (' + originx + ', ' + originy + ')');
        if (ex < originx)
            nx = ex - originx;
        else
            nx = originx + ex;
        if (ey < originy)
            ny = ey - originy;
        else
            ny = originy + ey;
        x = nx; y = ny; width = parent.width; height = parent.height;
        console.log(parent.objectName + 'local = (' + x + ', ' + y + ')');
        triggered = true;
    }
}
