import QtQuick 2.0
import QtQuick.Controls 2.1

ListView {
    id: container
    focus: true
    highlightFollowsCurrentItem: false
    keyNavigationWraps: true
    interactive: false
    property int choice: 0
    signal userChoice(int value)

    FontLoader {
        id: fontLoader
        source: "qrc:/font/ui/res/arcadeclassic.ttf"
    }

    delegate: Rectangle {
        color: "transparent"
        width: container.width
        height: container.height / container.count
        anchors.horizontalCenter: parent.horizontalCenter
        property variant myData: model

        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: fontLoader.name
            font.pointSize: itemTextSize
            text: itemText
            color: itemTextColor
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { container.currentIndex = index; choice = container.currentIndex; }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Up) {
                container.decrementCurrentIndex ();
            } else if (event.key === Qt.Key_Down) {
                container.incrementCurrentIndex ();
            }

            event.accepted = true;
        }

        Keys.onReturnPressed: container.userChoice(choice)
    }

    highlight: Component {
        Image {
            y: container.currentItem.y + container.currentItem.height / 2 - (height / 2)
            Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
            source: "qrc:/image/ui/res/item_selector.png"
        }
    }

    onCurrentItemChanged: {
        choice = currentItem.myData.itemId
    }
}
