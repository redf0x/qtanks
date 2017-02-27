import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    id: mainWindow
    visible: true;
    width: 720
    height: 653

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
