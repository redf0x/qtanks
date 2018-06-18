import QtQuick 2.0
import QtQuick.Window 2.0
import Tanks.Globals 0.1

Window {
    id: mainWindow
    visible: true;
    width: Globals.defaultWindowWidth
    height: Globals.defaultWindowHeight

    property var currentContent : null

    Loader {
        id: contentLoader
        anchors.fill: parent
        asynchronous: false
        focus: true
        onLoaded: { currentContent = item; }
    }

    function contentRequest(contentSrc) {
        contentLoader.source = contentSrc;
    }
}
