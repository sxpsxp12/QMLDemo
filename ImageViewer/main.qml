import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true;
    width: 640;
    height: 480;
    color: "black";
    title: qsTr("ImageViewer");

    Column {
        id: actionPanel;
        anchors.centerIn: parent;
        spacing: 4;

        FlatButton {
            id: local;
            iconSource: "qrc:///icons/icons/ic_archive.png";
        }

    }
}
