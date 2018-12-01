import QtQuick 2.9

Rectangle {
    id: flatButton;

    property bool hovered: false;
    property alias iconSource: iconImage.source;
    property alias iconWidth: iconImage.width;
    property alias iconHeight: iconImage.height;
    property alias text: btnText.text;
    property alias textColor: btnText.color;
    property alias font: btnText.font;

    signal clicked;

    color: "transparent";
    implicitWidth: 120;
    implicitHeight: 50;
    radius: 6;
    border.color: "#949494";
    border.width: hovered ? 2 : 0;

    Image {
        id: iconImage;
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;
    }

    Text {
        id: btnText;
        anchors.left: iconImage.right;
        anchors.verticalCenter: iconImage.verticalCenter;
        anchors.margins: 4;
        color: mouseArea.pressed ? "blue": (parent.hovered ? "#0000a0" : "white")
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
        hoverEnabled: true;

        onEntered: {
            flatButton.hovered = true;
        }
        onExited: {
            flatButton.hovered = false;
        }

        onClicked: {
            flatButton.hovered = false;
            flatButton.clicked();
        }
    }
}
