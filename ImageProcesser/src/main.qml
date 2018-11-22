import QtQuick 2.9
import QtQuick.Window 2.2
import com.st.ImageProcesser 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Window {
    visible: true;
    width: 640;
    height: 480;
    title: qsTr("ImageProcesser");

    BusyIndicator{
        id:busyIndicator;
        running: false;
        anchors.centerIn: parent;
        z:2;
    }

    Label{
        id: info;
        visible: false;
        anchors.centerIn: parent;
    }

    Image {
        id: imageviwer;
        asynchronous: true;
        anchors.fill: parent;
        fillMode: Image.PreserveAspectFit;

        onStatusChanged: {
            if(imageviwer.status == Image.Loading)
            {
                busyIndicator.running = true;
                info.visible = false;
            }
            else if(imageviwer.status == Image.Ready)
            {
                busyIndicator.running = false;
            }
            else if(imageviwer.status == Image.Error)
            {
                busyIndicator.running = false;
                info.visible = true;
                info.text = "ERROR";
            }
        }
    }
    ImageProcesser {
        id: imageprocesser;
        onFinished:{
            if (newFile.length == 0)
            {
                info.visible = true;
                info.text = "不支持的处理算法";
                imageviwer.source = "";
            }
            else
                imageviwer.source = "file:///" + newFile;
            busyIndicator.running = false;
        }
    }

    FileDialog {
        id: fileDialog;
        title: "打开图片";
        folder: shortcuts.desktop;
        nameFilters: ["Image files (*.jpg *.png)", "All files (*)"];
        onAccepted: {
            console.log(fileDialog.fileUrl);
            imageviwer.source = fileDialog.fileUrl;
        }
    }

    Component{
        id: btnstyle;
        ButtonStyle{
                background: Rectangle{
                radius: 6;
                implicitHeight: 30;
                implicitWidth: 60;

                border.width: control.pressed ? 2 : 1;
                border.color: (control.pressed || control.hoverd) ?
                                  "#00A060" : "#888888"
            }
        }
    }

    Button {
        id: openBtn;
        text: "打开"
        style: btnstyle;
        anchors.left: parent.left;
        anchors.leftMargin: 6;
        anchors.top: parent.top;
        anchors.topMargin: 6;
        z: 1;

        onClicked: {
            fileDialog.visible = true;
        }
    }

    Button {
        id: closeBtn;
        style: btnstyle;
        text: "退出";
        anchors.left: openBtn.right;
        anchors.leftMargin: 6;
        anchors.bottom: openBtn.bottom;
        z: 1;

        onClicked: {
            Qt.quit();
        }
    }

    Grid {
        id: group;
        rows: 2;
        columns: 3;
        anchors.left: parent.left;
        anchors.leftMargin: 6;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 6;
        rowSpacing: 4;
        columnSpacing: 4;
        z: 1;

        Button {
            text: "锐化";
            style: btnstyle;
            onClicked: {
                busyIndicator.running = true;
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.SHARPEN);
            }
        }

        Button {
            text: "灰度";
            style: btnstyle;
            onClicked: {
                busyIndicator.running = true;
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.GRAY);
            }
        }

        Button {
            text: "浮雕";
            style: btnstyle;
            onClicked: {
                busyIndicator.running = true;
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.EMBOSS);
            }
        }

        Button {
            text: "黑白";
            style: btnstyle;
            onClicked: {
                busyIndicator.running = true;
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.BINARIZE);
            }
        }


        Button {
            text: "底片";
            style: btnstyle;
            onClicked: {
                busyIndicator.running = true;
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.NEGATIVE);
            }
        }

        Button {
            text: "模糊";
            style: btnstyle;
            onClicked: {
                busyIndicator.running = true;
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.SOFTEN);
            }
        }
    }

}
