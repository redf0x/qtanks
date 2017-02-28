import QtQuick 2.0
import Tanks.Globals 0.1

Rectangle {
    color: "black"
    signal userAction(var c)

    Image {
        y: parent.height / 4
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/image/ui/res/title.png"
    }

    GameMenu {
        id: menu
        width: parent.width / 2 - 65
        height: parent.height / 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        anchors.horizontalCenter: parent.horizontalCenter

        model: ListModel {
            ListElement { itemText: "LETS ROCK!"; itemTextColor: "white"; itemTextSize: 28; itemId: Globals.START }
            /* not yet implemented */
            /* ListElement { itemText: "SETTINGS"; itemTextColor: "white"; itemTextSize: 28; itemId: 1 } */
            /* ListElement { itemText: "HALL OF FAME"; itemTextColor: "white"; itemTextSize: 28; itemId: 2 } */
            ListElement { itemText: "EXIT"; itemTextColor: "white"; itemTextSize: 28; itemId: Globals.EXIT }
        }

        onUserChoice: { gameLdr.userAction(choice); }
    }
}
