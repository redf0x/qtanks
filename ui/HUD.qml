import QtQuick 2.0

Item {
    Grid {
        id: enemyCounter
        y: 32
        columns: 2
        rows: 9
        spacing: 2

        Repeater {
            model: battleField.enemyCounter

            Rectangle {
                width: 32
                height: 32
                color: "transparent"

                Image {
                    anchors.fill: parent
                    anchors.centerIn: parent
                    source: "qrc:/image/ui/res/enemy_counter.png"
                }
            }
        }
    }

    Item {
        id: playerCounter
        anchors.top: enemyCounter.bottom
        anchors.topMargin: 32

        FontLoader {
            id: fontLoader
            source: "qrc:/font/ui/res/arcadeclassic.ttf"
        }

        Text {
            id: ctrLabel
            text: "IP"
            font { family: fontLoader.name; pointSize: 48 }
        }

        Image {
            id: ctrImage
            anchors.top: ctrLabel.bottom
            height: 32
            width: 32
            source: "qrc:/image/ui/res/life.png"
        }

        Text {
            anchors { top: ctrLabel.bottom; left: ctrImage.right }
            text: battleField.playerTanks
            font { family: fontLoader.name; pointSize: 42 }
        }

        Image {
            id: stageImage
            anchors { top: ctrImage.bottom; topMargin: 16 }
            width: ctrLabel.width
            height: ctrLabel.width
            source: "qrc:/image/ui/res/flag.png"
        }

        Text {
            anchors.top: stageImage.bottom
            text: battleField.stage
            font { family: fontLoader.name; pointSize: 42 }
        }
    }
}
